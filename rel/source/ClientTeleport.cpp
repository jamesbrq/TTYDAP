#include "ClientTeleport.h"
#include "OWR.h"
#include "visibility.h"
#include "ttyd/mapdata.h"
#include "ttyd/pmario_sound.h"
#include "ttyd/seqdrv.h"
#include "ttyd/swdrv.h"
#include "ttyd/countdown.h"

#include <cstring>

namespace mod::client_teleport
{
    namespace
    {
        // Follows the receive-feed sender ring (0x80004700..0x800047ff).
        struct Request
        {
            uint32_t magic;
            volatile uint32_t status; // 0 idle, 1 requested, 2 started, 3 unknown room, 4 busy
            char room[32];
            char entrance[32];
        };
        static_assert(sizeof(Request) == 72);
        Request *const request = reinterpret_cast<Request *>(0x80004800);
        struct BossRequest
        {
            uint32_t magic;
            volatile uint32_t status; // 0 idle, 1 requested, 2 started, 3 invalid boss, 4 busy, 5 disabled
            uint32_t enemy;
            uint32_t encounter; // Boss loadout encounter index
        };
        static_assert(sizeof(BossRequest) == 16);
        BossRequest *const bossRequest = reinterpret_cast<BossRequest *>(0x80004850);
        constexpr uint8_t kBossIds[] = {
            0x79, 0x17, 0x06, 0x07, 0x14, 0x4c, 0x4f, 0x4d,
            0xab, 0x87, 0x84, 0x90, 0x92, 0x94, 0x95, 0x22,
            0x63, 0x5d, 0x6b, 0x08, 0x41, 0x40, 0x3f, 0x21
        };
        struct BossTarget { uint32_t encounter; const char *room; const char *entrance; };
        constexpr BossTarget kBossTargets[] = {
            {1, "gon_11", nullptr},
            {4, "hei_10", "w_bero"},
            {19, "tik_02", "w_bero_1"},
            {23, "win_00", "e_bero"},
            {15, "mri_01", "e_bero"},
            {21, "tou_03", "s_bero_1"},
            {20, "tou_03", "s_bero_1"},
            {5, "jin_00", "w_bero"},
            {7, "jin_04", "s_bero"},
            {6, "jin_04", "s_bero"},
            {17, "muj_12", "w_bero"},
            {16, "muj_00", "e_bero"},
            {18, "rsh_06_a", "w_bero"},
            {0, "aji_14", "w_bero"},
            {9, "las_09", "majyorin_evt"},
            {10, "las_26", "w_bero"},
            {12, "las_28", "w_bero"},
            {11, "las_28", "koopa_evt"},
            {13, "las_29", "sekai_yami2"},
            {14, "las_29", "starstone"},
            {8, "jon_06", "dokan_2"},
        };
        char sRoom[32];
        char sEntrance[32];

        void PrepareBossRoom(const char *room, uint32_t encounter = 0)
        {
            if (std::strcmp(room, "jon_06") == 0 && encounter == 8)
            {
                ttyd::swdrv::swByteSet(1321, 99);
                ttyd::swdrv::swClear(5085); // Replay the boss entrance.
                ttyd::swdrv::swClear(5084); // Restore the normal chest reward sequence.
                return;
            }
            if (std::strcmp(room, "las_09") == 0 && encounter == 9)
            {
                ttyd::swdrv::swByteSet(1708, 8);
                return;
            }
            if (std::strcmp(room, "las_26") == 0 && encounter == 10)
            {
                ttyd::swdrv::swClear(6072);
                return;
            }
            if (std::strcmp(room, "las_28") == 0 && (encounter == 11 || encounter == 12))
            {
                ttyd::swdrv::swByteSet(1708, encounter == 12 ? 14 : 15);
                return;
            }
            if (std::strcmp(room, "las_29") == 0 && (encounter == 13 || encounter == 14))
            {
                ttyd::swdrv::swByteSet(1708, 16);
                return;
            }
            if (std::strcmp(room, "aji_14") == 0)
            {
                ttyd::swdrv::swByteSet(1707, 15);
                return;
            }
            if (std::strcmp(room, "rsh_06_a") == 0 && encounter == 18)
            {
                ttyd::swdrv::swByteSet(1706, 35);
                ttyd::swdrv::swByteSet(1720, 0); // Select the train story, not postgame travel.
                return;
            }
            if (std::strcmp(room, "muj_12") == 0 && encounter == 17)
            {
                ttyd::swdrv::swByteSet(1717, 9);
                return;
            }
            if (std::strcmp(room, "muj_00") == 0 && encounter == 16)
            {
                ttyd::swdrv::swByteSet(1717, 16);
                // Avoid the arrival cutscene taking precedence over the ship fight.
                if (ttyd::swdrv::swByteGet(1705) == 9)
                    ttyd::swdrv::swByteSet(1705, 10);
                return;
            }
            if (std::strcmp(room, "jin_04") == 0 && (encounter == 6 || encounter == 7))
            {
                ttyd::swdrv::swByteSet(1715, encounter == 7 ? 1 : 5);
                return;
            }
            if (std::strcmp(room, "jin_00") == 0 && encounter == 5)
            {
                ttyd::swdrv::swByteSet(1715, 3);
                ttyd::swdrv::swByteSet(1716, 2);
                ttyd::swdrv::swSet(6045); // Steeple entrance puzzle completed.
                ttyd::swdrv::swSet(6046); // Entrance key collected.
                ttyd::swdrv::swSet(6047); // Upper door unlocked.
                ttyd::swdrv::swClear(2226); // Enable the Boo swarm.
                ttyd::swdrv::swClear(2242); // Atomic Boo reward not collected.
                return;
            }
            if (std::strcmp(room, "tou_03") == 0 && (encounter == 20 || encounter == 21))
            {
                ttyd::swdrv::swClear(2383); // Skip the earlier championship confinement scene.
                ttyd::swdrv::swClear(2389); // Clear the previous match result.
                ttyd::swdrv::swClear(2450); // Keep the current partner for the fight.
                if (encounter == 20)
                {
                    ttyd::swdrv::swByteSet(1703, 19);
                    ttyd::swdrv::swClear(2388);
                }
                else
                {
                    ttyd::swdrv::swByteSet(1703, 12);
                    ttyd::swdrv::swByteSet(501, 0);
                    for (int rule = 502; rule <= 504; ++rule) ttyd::swdrv::swByteSet(rule, 0);
                    ttyd::swdrv::swSet(2388);
                    ttyd::swdrv::swClear(2399); // Replay Rawk Hawk's full entrance.
                    ttyd::swdrv::swClear(2400); // Allow the win event to run again.
                    ttyd::swdrv::swSet(2455);
                }
                return;
            }
            if (std::strcmp(room, "win_00") == 0)
            {
                ttyd::swdrv::swByteSet(1702, 13);
                ttyd::swdrv::swByteSet(1712, 1);
                return;
            }
            if (std::strcmp(room, "mri_01") == 0)
            {
                ttyd::swdrv::swByteSet(1713, 9);
                // Start from the bomb escape state rather than carrying an
                // expired timer or Bowser's intermission into the boss scene.
                ttyd::countdown::countDownEnd();
                ttyd::swdrv::swSet(2880);
                const int story = ttyd::swdrv::swByteGet(1703);
                if (story >= 25 && story <= 27) ttyd::swdrv::swByteSet(1703, 24);
                return;
            }
            if (std::strcmp(room, "hei_10") != 0) return;
            // Both stone pickups and unfinished Chapter 1 progress are required
            // by hei_10_init_evt before it starts Golden Fuzzy's scene.
            ttyd::swdrv::swSet(1774);
            ttyd::swdrv::swSet(1775);
            ttyd::swdrv::swByteSet(1701, 8);
        }
    }

    KEEP_FUNC void Init()
    {
        std::memset(bossRequest, 0, sizeof(BossRequest));
        bossRequest->magic = 0x54544254; // TTBT: boss target selection
        bossRequest->encounter = 1;
        std::memset(request, 0, sizeof(Request));
        request->magic = 0x54545750; // TTWP; client only writes to supporting builds.
    }

    KEEP_FUNC void Update()
    {
        if (bossRequest->status == 1)
        {
            bool valid = false;
            for (uint8_t id : kBossIds)
                if (bossRequest->enemy == id) valid = true;
            const BossTarget *target = nullptr;
            for (const auto &candidate : kBossTargets)
                if (candidate.encounter == bossRequest->encounter) target = &candidate;
            using ttyd::seqdrv::SeqIndex;
            if (!valid || !target)
                bossRequest->status = 3;
            else if (!owr::gState || !owr::gState->apSettings ||
                     !owr::gState->apSettings->bossRandomizer)
                bossRequest->status = 5;
            else if (request->status == 1 ||
                     ttyd::seqdrv::seqGetSeq() != SeqIndex::kGame ||
                     ttyd::seqdrv::seqGetNextSeq() != SeqIndex::kGame)
                bossRequest->status = 4;
            else
            {
                auto &loadout = owr::gState->bossLoadouts[bossRequest->encounter];
                loadout.enemyCount = 1;
                for (int i = 0; i < 5; ++i)
                {
                    loadout.enemyIds[i] = i == 0 ? bossRequest->enemy : 0;
                    loadout.originalKinds[i] = nullptr;
                }
                ttyd::swdrv::swByteSet(1696, ttyd::swdrv::swByteGet(1696) | 0x08);
                // Reload the area to rebuild the boss group and choose the
                // appropriate dragon/non-dragon scene patches.
                ttyd::pmario_sound::psndBGMOff(513);
                owr::gState->fastTraveling = true;
                bossRequest->status = 2;
                PrepareBossRoom(target->room, target->encounter);
                ttyd::seqdrv::seqSetSeq(SeqIndex::kMapChange, target->room, target->entrance);
            }
            return;
        }
        if (request->status != 1)
            return;
        std::memcpy(sRoom, request->room, sizeof(sRoom));
        std::memcpy(sEntrance, request->entrance, sizeof(sEntrance));
        sRoom[31] = '\0';
        sEntrance[31] = '\0';
        if (!sRoom[0] || !ttyd::mapdata::mapDataPtr(sRoom))
        {
            request->status = 3;
            return;
        }
        using ttyd::seqdrv::SeqIndex;
        if (ttyd::seqdrv::seqGetSeq() != SeqIndex::kGame ||
            ttyd::seqdrv::seqGetNextSeq() != SeqIndex::kGame)
        {
            request->status = 4;
            return;
        }
        // Mark this debug action using the same dirty-reason bit as client debug commands.
        ttyd::swdrv::swByteSet(1696, ttyd::swdrv::swByteGet(1696) | 0x08);
        ttyd::pmario_sound::psndBGMOff(513);
        owr::gState->fastTraveling = true;
        request->status = 2;
        PrepareBossRoom(sRoom);
        ttyd::seqdrv::seqSetSeq(SeqIndex::kMapChange, sRoom, sEntrance[0] ? sEntrance :
            (std::strcmp(sRoom, "tik_02") == 0 ? "w_bero_1" :
             (std::strcmp(sRoom, "hei_10") == 0 ? "w_bero" :
              (std::strcmp(sRoom, "win_00") == 0 || std::strcmp(sRoom, "mri_01") == 0 ? "e_bero" : nullptr))));
    }
}
