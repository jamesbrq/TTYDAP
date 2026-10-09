#include "BossPreview.h"
#include "OWR.h"
#include "patch.h"
#include "visibility.h"
#include "ttyd/animdrv.h"
#include "ttyd/npcdrv.h"
#include "ttyd/hitdrv.h"
#include "ttyd/seq_mapchange.h"
#include "ttyd/seqdrv.h"
#include "ttyd/evt_cam.h"
#include "ttyd/evtmgr_cmd.h"
#include "ttyd/evt_msg.h"
#include "ttyd/windowdrv.h"
#include "ttyd/swdrv.h"
#include "ttyd/mario.h"
#include "ttyd/mariost.h"
#include "gc/mtx.h"

#include <algorithm>
#include <cstring>
#include "ttyd/system.h"

extern "C" void *camGetPtr(int cameraId);

namespace mod::boss_preview
{
    namespace
    {
        constexpr int kHooktailEncounter = 1;
        constexpr int kBlooperEncounter = 19;
        constexpr int kGoldenFuzzyEncounter = 4;
        constexpr uint8_t kHooktail = 0x17;
        // NPC names in the game use Shift-JIS, regardless of source encoding.
        constexpr char kHooktailNpcName[] = "\x83\x53\x83\x93\x83\x6f\x83\x6f";
        constexpr char kBlooperNpcName[] = "\x83\x51\x83\x62\x83\x5c\x81\x5b";
        constexpr char kGoldenFuzzyNpcName[] = "\x83\x53\x81\x5b\x83\x8b\x83\x68\x83\x60\x83\x87\x83\x8d\x83\x7b\x83\x93";
        constexpr char kSirensNpcName[] = "\x83\x7d\x83\x57\x83\x87\x83\x8a\x83\x93";
        constexpr char kMarilynNpcName[] = "\x83\x7d\x83\x8a\x83\x8a\x83\x93";
        constexpr char kVivianNpcName[] = "\x83\x72\x83\x72\x83\x41\x83\x93";
        constexpr char kMagnusNpcName[] = "\x83\x7b\x83\x58\x83\x8d\x83\x7b\x83\x62\x83\x67";
        constexpr char kMagnusEmblemNpcName[] = "\x83\x7b\x83\x58\x83\x8d\x83\x7b\x83\x62\x83\x67\x83\x6f\x83\x63";
        constexpr char kMagnusCockpitNpcName[] = "\x83\x7b\x83\x58\x83\x8d\x83\x7b\x83\x62\x83\x67\x83\x7d\x83\x8b";
        constexpr char kRawkNpcName[] = "\x93\x47\x82\x50";
        constexpr char kGrubbaNpcName[] = "\x83\x7d\x83\x62\x83\x60\x83\x87\x83\x4b\x83\x93\x83\x58";
        constexpr char kNormalGrubbaNpcName[] = "\x83\x4b\x83\x93\x83\x58";
        constexpr char kDooplissNpcName[] = "\x83\x89\x83\x93\x83\x79\x83\x8b";
        constexpr char kFakeMarioNpcName[] = "\x82\xc9\x82\xb9\x83\x7d\x83\x8a\x83\x49";
        constexpr char kAtomicBooNpcName[] = "\x83\x41\x83\x67\x83\x7e\x83\x62\x83\x4e\x83\x65\x83\x8c\x83\x54";
        constexpr const char *kFakePartnerNames[] = {
            "\x83\x4e\x83\x8a\x83\x58\x83\x60\x81\x5b\x83\x6b", "\x83\x6d\x83\x52\x83\x5e\x83\x8d\x83\x45", "\x83\x4e\x83\x89\x83\x45\x83\x5f", "\x83\x88\x83\x62\x83\x56\x81\x5b",
        };
        constexpr char kCortezNpcName[] = "\x83\x52\x83\x8b\x83\x65\x83\x58";
        constexpr char kCrumpNpcName[] = "\x91\xe6\x8e\x4f\x90\xa8\x97\xcd\x8a\xb2\x95\x94";
        constexpr char kMagnus2NpcName[] = "\x83\x8d\x83\x7b\x83\x62\x83\x67";
        constexpr char kGloomtailNpcName[] = "\x83\x7b\x83\x93\x83\x6f\x83\x6f";
        constexpr char kGrodusNpcName[] = "\x83\x56\x83\x85\x83\x8a\x83\x87\x81\x5b";
        constexpr char kBowserNpcName[] = "\x83\x4e\x83\x62\x83\x70";
        constexpr char kKammyNpcName[] = "\x83\x6f\x83\x6f";
        constexpr char kBlackPeachNpcName[] = "\x83\x75\x83\x89\x83\x62\x83\x4e\x83\x73\x81\x5b\x83\x60";
        constexpr char kQueenNpcName[] = "\x89\x65\x82\xcc\x8f\x97\x89\xa4";
        constexpr char kFakeNpcName[] = "\x82\xc9\x82\xb9";
        struct Scene
        {
            const char *room;
            const char *npcName;
            int encounter;
            uint8_t originalEnemy;
        };
        constexpr Scene kScenes[] = {
            {"gon_11", kHooktailNpcName, kHooktailEncounter, kHooktail},
            {"tik_02", kBlooperNpcName, kBlooperEncounter, 0x08},
            {"hei_10", kGoldenFuzzyNpcName, kGoldenFuzzyEncounter, 0x14},
            {"win_00", kSirensNpcName, 23, 0x21},
            {"mri_01", kMagnusNpcName, 15, 0x22},
            {"tou_03", kRawkNpcName, 21, 0x40},
            {"tou_03", kGrubbaNpcName, 20, 0x41},
            {"jin_00", kAtomicBooNpcName, 5, 0x4c},
            {"jin_04", kDooplissNpcName, 7, 0x4d},
            {"jin_04", kFakeMarioNpcName, 6, 0x4f},
            {"muj_12", kCortezNpcName, 17, 0x5d},
            {"muj_00", kCrumpNpcName, 16, 0x63},
            {"rsh_06_a", "base", 18, 0x6b},
            {"aji_14", kMagnus2NpcName, 0, 0x79},
            {"las_09", kSirensNpcName, 9, 0x87},
            {"las_26", kGloomtailNpcName, 10, 0x84},
            {"las_28", kGrodusNpcName, 12, 0x92},
            {"las_28", kBowserNpcName, 11, 0x90},
            {"las_29", kBlackPeachNpcName, 13, 0x94},
            {"las_29", kBlackPeachNpcName, 14, 0x95},
            {"jon_06", "\x93\x47", 8, 0xab},
        };
        int sEncounter = -1;
        bool sSmorgFormed = false;
        bool sMagnus2Revealed = false;
        int sPalaceEncounter = -1;

        const Scene *FindScene(const char *room)
        {
            for (const auto &scene : kScenes)
                if (std::strcmp(room, scene.room) == 0)
                {
                    if (scene.encounter == 8)
                    {
                        if (ttyd::swdrv::swByteGet(1321) != 99 || ttyd::swdrv::swGet(5085)) continue;
                    }
                    else if (scene.encounter == 9)
                    {
                        if (ttyd::swdrv::swByteGet(1708) != 8) continue;
                    }
                    else if (scene.encounter == 10)
                    {
                        if (ttyd::swdrv::swGet(6072)) continue;
                    }
                    else if (scene.encounter >= 11 && scene.encounter <= 14)
                    {
                        if (sPalaceEncounter != scene.encounter) continue;
                    }
                    else if (scene.encounter == 0)
                    {
                        if (ttyd::swdrv::swByteGet(1707) >= 16) continue;
                    }
                    else if (scene.encounter == 18)
                    {
                        if (ttyd::swdrv::swByteGet(1706) != 35) continue;
                    }
                    else if (scene.encounter == 17)
                    {
                        const int story = ttyd::swdrv::swByteGet(1717);
                        if (story != 9 && story != 10) continue;
                    }
                    else if (scene.encounter == 16)
                    {
                        const int story = ttyd::swdrv::swByteGet(1717);
                        if (story != 16 && story != 17) continue;
                    }
                    else if (scene.encounter == 7)
                    {
                        if (ttyd::swdrv::swByteGet(1715) >= 2) continue;
                    }
                    else if (scene.encounter == 6)
                    {
                        if (ttyd::swdrv::swByteGet(1715) != 5) continue;
                    }
                    else if (scene.encounter == 5)
                    {
                        if (ttyd::swdrv::swByteGet(1716) < 2) continue;
                    }
                    else if (scene.encounter == 21)
                    {
                        // The previous match result can survive into Grubba's scene.
                        const int story = ttyd::swdrv::swByteGet(1703);
                        if (story == 19 || story == 20) continue;
                        if (ttyd::swdrv::swByteGet(501) != 0 ||
                            !(ttyd::swdrv::swGet(2388) || ttyd::swdrv::swGet(2389) || ttyd::swdrv::swGet(2383)))
                            continue;
                    }
                    else if (scene.encounter == 20)
                    {
                        const int story = ttyd::swdrv::swByteGet(1703);
                        if (story != 19 && story != 20) continue;
                    }
                    return &scene;
                }
            return nullptr;
        }

        bool InBlooperRoom()
        {
            return std::strcmp(ttyd::seq_mapchange::_next_map, "tik_02") == 0;
        }

        bool InGoldenFuzzyRoom()
        {
            return std::strcmp(ttyd::seq_mapchange::_next_map, "hei_10") == 0;
        }

        const Scene *CurrentScene()
        {
            return FindScene(ttyd::seq_mapchange::_next_map);
        }

        bool SceneReady(const Scene *scene)
        {
            // Keep the earlier necklace/story scenes intact. Replacement begins
            // when the Sirens are ready to fight and lasts through their retreat.
            if (scene && scene->encounter == 9)
            {
                // The opening line is spoken from offscreen, before Beldam
                // emerges. Keep its native camera and text anchor until then.
                const auto *npc = ::npcNameToPtr_NoAssert(scene->npcName);
                if (!npc || npc->position.y < -10.0f) return false;
            }
            return scene && (scene->encounter != 0 || sMagnus2Revealed) && (scene->encounter != 18 || sSmorgFormed) && (scene->encounter != 23 ||
                (ttyd::swdrv::swByteGet(1702) >= 13 && ttyd::swdrv::swByteGet(1712) <= 2));
        }

        bool FrontCameraRoom()
        {
            const auto *scene = CurrentScene();
            return scene && scene->encounter != kHooktailEncounter && scene->encounter != kBlooperEncounter;
        }

        NpcEntry *PreviewNpc()
        {
            const auto *scene = CurrentScene();
            if (!scene) return nullptr;
            return ::npcNameToPtr_NoAssert(scene->npcName);
        }
        int32_t sPreviewPose = -1;
        int32_t sSourcePose = -1;
        uint8_t sEnemyId = 0;
        float sVisualScale = 1.0f;
        char sModel[64] = {};
        char sIdle[64] = {};
        const char *sTalk = nullptr;
        char sSourceAnim[64] = {};
        bool sTalking = false;
        bool sWasTalking = false;
        bool sBlooperEmerged = false;
        float sHeight = 80.0f;
        struct ExtraVisual
        {
            int32_t pose = -1;
            char model[64] = {};
            char idle[64] = {};
            const char *talk = nullptr;
            int anchorParent = -1;
            int anchorGroup = -1;
            gc::vec3 offset = {0.0f, 0.0f, 0.0f};
            bool mirror = false;
            bool talking = false;
            float rowOffset = 0.0f;
            float scale = 1.0f;
        };
        ExtraVisual sExtras[5];
        int sExtraCount = 0;
        int sCockpitGroup = -1;
        struct SmorgChain { int start = -1; int end = -1; };
        SmorgChain sSmorgChains[47];


        constexpr const char *kSmorgChains[47][2] = {
            {"joint_00", "jointdai1_A01"},
            {"jointdai1_A01", "jointdai1_A02"},
            {"jointdai1_A02", "jointdai1_A03"},
            {"jointdai1_A03", "jointdai1_A04"},
            {"jointdai1_A04", "jointdai1_A05"},
            {"joint_00", "jointdai1_B01"},
            {"jointdai1_B01", "jointdai1_B02"},
            {"jointdai1_B02", "jointdai1_B03"},
            {"jointdai1_B03", "jointdai1_B04"},
            {"jointdai1_B04", "jointdai1_B05"},
            {"joint_00", "jointdai1_C01"},
            {"jointdai1_C01", "jointdai1_C02"},
            {"jointdai1_C02", "jointdai1_C03"},
            {"jointdai1_C03", "jointdai1_C04"},
            {"jointdai1_C04", "jointdai1_C05"},
            {"joint_00", "jointdai2_01"},
            {"jointdai2_01", "jointdai2_02"},
            {"jointdai2_02", "jointdai2_03"},
            {"jointdai2_03", "jointdai2_04"},
            {"jointdai2_04", "jointdai2_05"},
            {"jointdai2_05", "jointdai2_06"},
            {"jointdai2_06", "jointdai2_07"},
            {"jointdai2_07", "jointdai2_upper01"},
            {"jointdai2_upper01", "jointdai2_upper02"},
            {"jointdai2_upper02", "jointdai2_upper03"},
            {"jointdai2_upper03", "jointdai2_upper04"},
            {"jointdai2_07", "jointdai2_lower01"},
            {"jointdai2_lower01", "jointdai2_lower02"},
            {"jointdai2_lower02", "jointdai2_lower03"},
            {"jointdai2_lower03", "jointdai2_lower04"},
            {"joint_00", "jointasi_A01"},
            {"jointasi_A01", "jointasi_A02"},
            {"jointasi_A02", "jointasi_A03"},
            {"jointasi_A03", "jointasi_A04"},
            {"joint_00", "jointasi_B01"},
            {"jointasi_B01", "jointasi_B02"},
            {"jointasi_B02", "jointasi_B03"},
            {"jointasi_B03", "jointasi_B04"},
            {"joint_00", "jointasi_C01"},
            {"jointasi_C01", "jointasi_C02"},
            {"jointasi_C02", "jointasi_C03"},
            {"jointasi_C03", "jointasi_C04"},
            {"joint_00", "jointasi_D01"},
            {"jointasi_D01", "jointasi_D02"},
            {"jointasi_D02", "jointasi_D03"},
            {"jointasi_D03", "jointasi_D04"},
            {"joint_00", "joint_core"},
        };

        const char *IdlePose(const BattleUnitKindPart &part)
        {
            if (!part.pose_table) return nullptr;
            // Pose 1 is the sleep/status pose; 28 is the normal standing pose.
            for (auto *entry = part.pose_table; entry->name; ++entry)
                if (entry->id == 28) return entry->name;
            return nullptr;
        }

        const char *TalkPose(const char *model)
        {
            // Use the native model's talking pose, including its animation prefix.
            if (std::strcmp(model, "c_gesso") == 0) return nullptr;
            if (std::strncmp(model, "c_gonbaba", 9) == 0) return "GNB_T_1";
            if (std::strcmp(model, "c_vivian") == 0) return "PTR_T_1";
            if (std::strcmp(model, "c_majyorin") == 0) return "MJR_T_1";
            if (std::strcmp(model, "c_maririn") == 0) return "MRR_T_1";
            if (std::strcmp(model, "c_koopa") == 0) return "KPA_T_1";
            if (std::strcmp(model, "c_mario") == 0) return "EM_T_1";
            if (std::strcmp(model, "c_chorobon_k") == 0) return "CBN_T_1";
            if (std::strcmp(model, "c_korutesu3") == 0) return "KRT_T_2";
            if (std::strcmp(model, "c_b_peach_b") == 0 ||
                std::strcmp(model, "c_q_kage_f") == 0) return "KRT_T_1";
            if (std::strncmp(model, "c_mb_robo_", 10) == 0) return "MGN_T_1";
            if (std::strcmp(model, "c_kamek_bb") == 0) return "T_2";
            // Static attachments have no mouth/talking animation.
            if (std::strcmp(model, "c_q_kage_b") == 0 ||
                std::strcmp(model, "c_q_kage_h") == 0 ||
                std::strcmp(model, "c_b_hand") == 0 ||
                std::strcmp(model, "c_n_moa_a") == 0) return nullptr;
            return "T_1";
        }

        void SelectAnimation(int32_t pose, const char *animation, const char *idle)
        {
            ttyd::animdrv::animPoseSetAnim(pose, animation, 1);
            const char *selected = ttyd::animdrv::animPoseGetCurrentAnim(pose);
            if (!selected || std::strcmp(selected, animation) != 0)
                ttyd::animdrv::animPoseSetAnim(pose, idle, 1);
        }

        void ConfigureExtra(int index, const char *model, const char *idle, float x, float scale)
        {
            auto &extra = sExtras[index];
            if (!model || !idle || std::strlen(model) >= sizeof(extra.model) ||
                std::strlen(idle) >= sizeof(extra.idle)) return;
            std::strcpy(extra.model, model);
            std::strcpy(extra.idle, idle);
            extra.talk = TalkPose(model);
            extra.rowOffset = x;
            extra.scale = scale;
            sExtraCount = std::max(sExtraCount, index + 1);
        }

        void ConfigureCompanion(int index, uint8_t enemy, float side)
        {
            auto *kind = ttyd::battle_unit::GetUnitKindById(enemy);
            if (!kind || !kind->parts || kind->num_parts <= 0) return;
            const auto &part = kind->parts[0];
            const float scale = kind->height > 0 ? std::clamp(140.0f / kind->height, 0.25f, 1.0f) : 1.0f;
            auto *lead = ttyd::battle_unit::GetUnitKindById(sEnemyId);
            const float leadWidth = lead ? lead->width * sVisualScale : 40.0f;
            const float width = kind->width * scale;
            const float spacing = std::max(50.0f, (leadWidth + width) * 0.5f + 30.0f);
            ConfigureExtra(index, part.model_name, IdlePose(part), side * spacing, scale);
            sExtras[index].talk = nullptr;
        }

        // Native Magnus and Smorg rendering use the driver's evaluated group
        // positions (AnimPose + 0x15c/0x160) for their attached visuals.
        bool GroupPosition(int32_t pose, int group, gc::vec3 &position)
        {
            auto *raw = reinterpret_cast<uint8_t *>(ttyd::animdrv::animPoseGetAnimPosePtr(pose));
            if (!raw) return false;
            const int count = *reinterpret_cast<int32_t *>(raw + 0x160);
            auto **buffer = *reinterpret_cast<gc::vec3 ***>(raw + 0x15c);
            if (group < 0 || group >= count || !buffer || !*buffer) return false;
            position = (*buffer)[group];
            return __builtin_isfinite(position.x) && __builtin_isfinite(position.y) && __builtin_isfinite(position.z);
        }

        gc::vec3 QueenFloorPosition(gc::vec3 position)
        {
            const auto *scene = CurrentScene();
            if (!scene || scene->encounter != 13 || position.y < -100.0f)
                return position;
            ttyd::hitdrv::HitCheckQuery query = {};
            query.targetPosition = position;
            query.targetPosition.y += 30.0f;
            query.targetDirection.y = -1.0f;
            query.inOutTargetDistance = 150.0f;
            // Mario's normal ground filter excludes non-floor collision.
            auto filter = reinterpret_cast<ttyd::hitdrv::PFN_HitFilterFunction>(0x800915e4);
            if (ttyd::hitdrv::hitCheckVecFilter(&query, filter) && query.hitNormal.y > 0.5f)
                position.y = query.hitPosition.y;
            return position;
        }

        gc::vec3 RowPosition(gc::vec3 position, float offset)
        {
            // The intro camera looks along (-0.6, 0, 0.8). Space companions
            // along its perpendicular instead of changing their camera depth.
            const bool blooper = InBlooperRoom();
            position.x += offset * (FrontCameraRoom() ? 1.0f : (blooper ? 0.94f : 0.8f));
            position.z += offset * (FrontCameraRoom() ? 0.0f : (blooper ? 0.35f : 0.6f));
            // A companion can stand on a different step than the main model.
            return QueenFloorPosition(position);
        }

        bool HasGhostTail(const char *model)
        {
            return std::strcmp(model, "c_vivian") == 0 || std::strcmp(model, "c_majyorin") == 0 ||
                   std::strcmp(model, "c_maririn") == 0;
        }


        gc::vec3 VisualPosition(const NpcEntry *npc)
        {
            // Hooktail's script raises its root for dragon-specific animations.
            // Replacement characters stand on this room's floor instead. Keep
            // the script's hidden entrance position until it brings the NPC up.
            // Blooper replacements follow the scripted underwater root and
            // upward jump exactly; clamping would make them pop onto the surface.
            float y = CurrentScene() && CurrentScene()->encounter != kHooktailEncounter
                ? npc->position.y
                : (UsesOriginalDragonScene() || npc->position.y < 0.0f ? npc->position.y : 10.0f);
            // Smorg's rig root is below the roof on which the replacement stands.
            if (CurrentScene() && CurrentScene()->encounter == 18 && y >= 0.0f)
                y += 25.0f;
            // Ground non-dragon Gloomtail replacements. Queen phase 1 uses
            // the raised platform's collision; phase 2 retains Peach's floating
            // root and its scripted bobbing motion.
            if (CurrentScene() && CurrentScene()->encounter == 10 && !UsesOriginalDragonScene() && y >= 0.0f)
                y = 0.0f;
            // Beldam supplies the native movement; center the preview in the
            // original trio's row rather than on its rightmost member.
            const float x = npc->position.x - (CurrentScene() && CurrentScene()->encounter == 23 ? 40.0f : 0.0f);
            return QueenFloorPosition({x, y, npc->position.z});
        }

        bool Active()
        {
            return sPreviewPose >= 0 && owr::gState && owr::gState->apSettings &&
                   owr::gState->apSettings->bossRandomizer && SceneReady(CurrentScene()) &&
                   sEncounter == CurrentScene()->encounter && sEnemyId != CurrentScene()->originalEnemy;
        }

        void (*sDrawTrampoline)(int32_t, const gc::mat3x4 *, int32_t, float, float) = nullptr;

        bool InPreviewRoom()
        {
            return ttyd::seqdrv::seqGetSeq() == ttyd::seqdrv::SeqIndex::kGame &&
                   ttyd::seqdrv::seqGetNextSeq() == ttyd::seqdrv::SeqIndex::kGame &&
                   CurrentScene() != nullptr;
        }

        bool CanDrawFieldPreview()
        {
            // Both next_seq and now_seq change before the field fade finishes.
            // inBattle selects the NPC work actually being rendered; retain the
            // replacement until battle_init releases the field NPCs.
            return _globalWorkPtr && _globalWorkPtr->inBattle == 0 &&
                   FindScene(_globalWorkPtr->currentMapName) != nullptr;
        }

        void Reset()
        {
            for (auto &extra : sExtras) extra = ExtraVisual{};
            sExtraCount = 0;
            sCockpitGroup = -1;
            for (auto &chain : sSmorgChains) chain = SmorgChain{};
            sPreviewPose = -1;
            sSourcePose = -1;
            sEnemyId = 0;
            sEncounter = -1;
            sModel[0] = '\0';
            sIdle[0] = '\0';
            sTalk = nullptr;
            sSourceAnim[0] = '\0';
            sTalking = sWasTalking = false;
            sBlooperEmerged = false;
        }

        gc::mat3x4 FacingMatrix(const gc::vec3 &position, float scale)
        {
            gc::vec3 cameraPosition;
            std::memcpy(&cameraPosition, static_cast<uint8_t *>(camGetPtr(4)) + 0x0c, sizeof(cameraPosition));
            gc::vec3 facing = {cameraPosition.x - position.x, 0.0f, cameraPosition.z - position.z};
            if (facing.x == 0.0f && facing.z == 0.0f) facing.z = 1.0f;
            gc::mtx::PSVECNormalize(&facing, &facing);
            // The Sirens are approached from the right. The Queen's assembled
            // paper rig needs a horizontal reflection rather than a half turn,
            // which also reverses the hands' front/back offsets.
            const bool reverse = CurrentScene() && CurrentScene()->encounter == 23;
            const bool mirrorQueen = reverse && sEnemyId == 0x95;
            if (reverse && !mirrorQueen)
            {
                facing.x = -facing.x;
                facing.z = -facing.z;
            }
            gc::mat3x4 visual = {};
            visual.a[0] = facing.z * scale; visual.a[2] = facing.x * scale;
            visual.a[5] = scale;
            visual.a[8] = -facing.x * scale; visual.a[10] = facing.z * scale;
            if (mirrorQueen)
            {
                visual.a[0] = -visual.a[0];
                visual.a[8] = -visual.a[8];
            }
            visual.a[3] = position.x; visual.a[7] = position.y; visual.a[11] = position.z;
            return visual;
        }

        void DrawBattlePart(int pose, const gc::mat3x4 &matrix, float scale)
        {
            // Battle parts receive all three passes. Hooktail's NPC only submits
            // passes for its own materials, which can omit replacement translucency.
            for (int pass = 1; pass <= 3; ++pass)
                sDrawTrampoline(pose, &matrix, pass, 0.0f, scale);
        }

        void DrawSmorg(const gc::mat3x4 &matrix, float scale)
        {
            for (int i = 0; i < sExtraCount; ++i)
                if (sExtras[i].pose < 0) return;
            // Native Smorg has separate body, three tentacle and mouth rigs.
            // Evaluate each at the same origin before reading their chain joints.
            DrawBattlePart(sPreviewPose, matrix, scale);
            for (int i = 1; i < sExtraCount; ++i)
                DrawBattlePart(sExtras[i].pose, matrix, scale);
            for (int i = 0; i < 750; ++i)
            {
                const int chainIndex = i % 47;
                const auto &chain = sSmorgChains[chainIndex];
                const int rig = chainIndex < 15 ? sExtras[3 - chainIndex / 5].pose : sPreviewPose;
                gc::vec3 start, end;
                if (!GroupPosition(rig, chain.start, start) || !GroupPosition(rig, chain.end, end)) continue;
                // Match the battle's chain interpolation and +/-10 unit spread.
                // Deterministic samples avoid changing the game's random sequence.
                uint32_t sample = static_cast<uint32_t>(i + 1) * 2654435761u;
                const float t = (sample & 1023) / 1024.0f;
                gc::vec3 point = {start.x + (end.x - start.x) * t,
                                  start.y + (end.y - start.y) * t,
                                  start.z + (end.z - start.z) * t};
                sample = sample * 1664525u + 1013904223u;
                point.x += (static_cast<int>((sample >> 16) % 20) - 10) * sVisualScale;
                sample = sample * 1664525u + 1013904223u;
                point.y += (static_cast<int>((sample >> 16) % 20) - 10) * sVisualScale;
                sample = sample * 1664525u + 1013904223u;
                point.z += (static_cast<int>((sample >> 16) % 20) - 10) * sVisualScale;
                const auto particleMatrix = FacingMatrix(point, sVisualScale);
                DrawBattlePart(sExtras[0].pose, particleMatrix, scale);
            }
        }

        void DrawShadowQueen(const gc::vec3 &position, int stage, float scale)
        {
            for (int i = 0; i < sExtraCount; ++i)
            {
                const auto &extra = sExtras[i];
                if (extra.pose < 0) continue;
                auto point = position;
                if (extra.anchorParent >= 0)
                {
                    const auto &parent = sExtras[extra.anchorParent];
                    if (parent.pose < 0 || !GroupPosition(parent.pose, extra.anchorGroup, point)) continue;
                }
                auto matrix = FacingMatrix(point, extra.scale);
                gc::vec3 shifted;
                auto offset = extra.offset;
                gc::mtx::PSMTXMultVec(&matrix, &offset, &shifted);
                matrix.a[3] = shifted.x; matrix.a[7] = shifted.y; matrix.a[11] = shifted.z;
                if (extra.mirror)
                {
                    matrix.a[0] = -matrix.a[0]; matrix.a[4] = -matrix.a[4]; matrix.a[8] = -matrix.a[8];
                }
                sDrawTrampoline(extra.pose, &matrix, stage, 0.0f, scale);
            }
        }

        void DrawHook(int32_t pose, const gc::mat3x4 *matrix, int32_t stage, float rotation, float scale)
        {
            if (CanDrawFieldPreview() && owr::gState && owr::gState->apSettings &&
                owr::gState->apSettings->bossRandomizer &&
                owr::gState->bossLoadouts[FindScene(_globalWorkPtr->currentMapName)->encounter].enemyIds[0] !=
                    FindScene(_globalWorkPtr->currentMapName)->originalEnemy)
            {
                const auto *scene = FindScene(_globalWorkPtr->currentMapName);
                // Remove the native supporting models as a group. Their scripts
                // still run so movement, battle triggers and rewards stay intact.
                if (SceneReady(scene))
                {
                    const char *hidden[4] = {};
                    if (scene->encounter == 9)
                    {
                        hidden[0] = kMarilynNpcName;
                        hidden[1] = kDooplissNpcName;
                        hidden[2] = kFakeNpcName;
                    }
                    else if (scene->encounter == 11)
                    {
                        hidden[0] = kKammyNpcName;
                    }
                    else if (scene->encounter == 13 || scene->encounter == 14)
                    {
                        hidden[0] = kQueenNpcName;
                    }
                    else if (scene->encounter == 23)
                    {
                        hidden[0] = kMarilynNpcName;
                        hidden[1] = kVivianNpcName;
                    }
                    else if (scene->encounter == 15)
                    {
                        hidden[0] = kMagnusEmblemNpcName;
                        hidden[1] = kMagnusCockpitNpcName;
                    }
                    else if (scene->encounter == 18)
                    {
                        hidden[0] = "moa_a";
                        hidden[1] = "moa_b";
                    }
                    else if (scene->encounter == 6)
                    {
                        std::copy(kFakePartnerNames, kFakePartnerNames + 4, hidden);
                    }
                    else if (scene->encounter == 20)
                    {
                        // Keep normal Grubba during the machine scene, then
                        // hide him while the transformed replacement is visible.
                        auto *macho = ::npcNameToPtr_NoAssert(kGrubbaNpcName);
                        if (macho && macho->position.y >= 0.0f)
                            hidden[0] = kNormalGrubbaNpcName;
                    }
                    for (const char *name : hidden)
                        if (name)
                            if (auto *part = ::npcNameToPtr_NoAssert(name))
                                if (static_cast<int32_t>(part->poseId) == pose) return;
                }
                auto *npc = ::npcNameToPtr_NoAssert(scene->npcName);
                if (SceneReady(scene) && npc && static_cast<int32_t>(npc->poseId) == pose &&
                    npc->scale.x > 0.001f && npc->scale.y > 0.001f && npc->scale.z > 0.001f)
                {
                    // The visible arm belongs to Blooper's main model; the
                    // separate arm NPC is only an invisible collision target.
                    // Keep the native arm until the emergence replaces the body.
                    if (scene->encounter == kBlooperEncounter && !sBlooperEmerged)
                    {
                        sDrawTrampoline(pose, matrix, stage, rotation, scale);
                        return;
                    }
                    // Room entry can draw before the replacement asset is ready.
                    // Suppress the vanilla model until the replacement is ready.
                    if (sPreviewPose < 0 || sEncounter != scene->encounter)
                        return;
                    // Dragons share Hooktail's rig: retain its exact staging.
                    if (UsesOriginalDragonScene())
                    {
                        sDrawTrampoline(sPreviewPose, matrix, stage, rotation, scale);
                        return;
                    }
                    if (sEnemyId == 0x6b && stage != 1) return;
                    auto position = VisualPosition(npc);
                    if (sEnemyId == 0x6b)
                    {
                        const auto visual = FacingMatrix(position, sVisualScale);
                        DrawSmorg(visual, scale);
                        return;
                    }
                    if (sEnemyId == 0x95)
                    {
                        // Do not show Peach alone while the attached assets load.
                        for (int i = 0; i < sExtraCount; ++i)
                            if (sExtras[i].pose < 0) return;
                        // Keep translucent hair and hands in the NPC's late pass,
                        // after the room and opaque body have written their depth.
                        DrawShadowQueen(position, stage, scale);
                        // Native phase 2 suspends Peach 40 units above the body.
                        position.y += 42.0f * sVisualScale;
                    }
                    auto visual = FacingMatrix(position, sVisualScale);
                    if (sEnemyId == 0x22 || sEnemyId == 0x79)
                    {
                        // Both Magnus battle scripts use a -55 degree base yaw.
                        // Apply it to the complete robot before evaluating the
                        // cockpit locator; Crump keeps his separate paper facing.
                        gc::mat3x4 baseRotation, rotated;
                        gc::mtx::PSMTXRotTrig(&baseRotation, 'y', -0.81915204f, 0.57357644f);
                        gc::mtx::PSMTXConcat(&visual, &baseRotation, &rotated);
                        visual = rotated;
                    }
                    sDrawTrampoline(sPreviewPose, &visual, stage, 0.0f, scale);
                    if (sEnemyId != 0x95)
                        for (int i = 0; i < sExtraCount; ++i)
                        {
                            const auto &extra = sExtras[i];
                            if (extra.pose < 0) continue;
                            auto extraPosition = position;
                            if (sEnemyId == 0x22 || sEnemyId == 0x79)
                            {
                                if (!GroupPosition(sPreviewPose, sCockpitGroup, extraPosition)) continue;
                            }
                            else extraPosition = RowPosition(position, extra.rowOffset);
                            const auto extraMatrix = FacingMatrix(extraPosition, extra.scale);
                            sDrawTrampoline(extra.pose, &extraMatrix, stage, 0.0f, scale);
                        }
                    return;
                }
            }
            sDrawTrampoline(pose, matrix, stage, rotation, scale);
        }
    }

    KEEP_FUNC bool IsBossSceneNpc(const NpcEntry *npc)
    {
        if (!npc) return false;
        const char *room = ttyd::seq_mapchange::_next_map;
        if (!room || !room[0]) return false;
        const auto matches = [npc](const char *name) {
            return name && ::npcNameToPtr_NoAssert(name) == npc;
        };
        // Protect actors before scene activation, throughout transformations,
        // and after battle. Story flags and randomizer settings are irrelevant.
        for (const auto &scene : kScenes)
        {
            if (std::strcmp(room, scene.room) != 0) continue;
            if (matches(scene.npcName)) return true;
            switch (scene.encounter)
            {
            case 23:
            case 9:
                if (matches(kMarilynNpcName) || matches(kVivianNpcName) ||
                    matches(kDooplissNpcName) || matches(kFakeNpcName)) return true;
                break;
            case 15:
                if (matches(kMagnusEmblemNpcName) || matches(kMagnusCockpitNpcName) ||
                    matches(kCrumpNpcName)) return true;
                break;
            case 20:
                if (matches(kNormalGrubbaNpcName)) return true;
                break;
            case 6:
                for (const char *name : kFakePartnerNames)
                    if (matches(name)) return true;
                break;
            case 18:
                if (matches("moa_a") || matches("moa_b")) return true;
                break;
            case 11:
                if (matches(kKammyNpcName)) return true;
                break;
            case 13:
            case 14:
                if (matches(kQueenNpcName)) return true;
                break;
            }
        }
        return false;
    }

    KEEP_FUNC bool UsesOriginalDragonScene()
    {
        if (!owr::gState || !owr::gState->apSettings || !owr::gState->apSettings->bossRandomizer ||
            !CurrentScene() || (CurrentScene()->encounter != kHooktailEncounter && CurrentScene()->encounter != 10 && CurrentScene()->encounter != 8))
            return false;
        const uint8_t enemy = owr::gState->bossLoadouts[CurrentScene()->encounter].enemyIds[0];
        return enemy == 0x17 || enemy == 0x84 || enemy == 0xab;
    }

    KEEP_FUNC void InstallHooks()
    {
        sDrawTrampoline = patch::hookFunction(ttyd::animdrv::animPoseDrawMtx, DrawHook);
    }

    // Called by the shared animation cleanup hook before the engine frees poses.
    KEEP_FUNC void BeforeAutoRelease(int32_t group)
    {
        if (group == 0)
        {
            sSmorgFormed = false;
            sMagnus2Revealed = false;
            sPalaceEncounter = -1;
            Reset();
        }
    }

    KEEP_FUNC int32_t SceneCamera(ttyd::evtmgr::EvtEntry *evt, bool firstCall)
    {
        auto *npc = PreviewNpc();
        if (!Active() || UsesOriginalDragonScene() || !InPreviewRoom() || !npc ||
            (InBlooperRoom() && !sBlooperEmerged))
            return ttyd::evt_cam::evt_cam3d_evt_set(evt, firstCall);
        auto *original = evt->evtArguments;
        int32_t args[8];
        std::copy(original, original + 8, args);
        // Use the same world position as the renderer. Clamp only the camera's
        // target height while Hooktail's entrance script keeps the NPC hidden.
        const auto boss = VisualPosition(npc);
        const auto mario = ttyd::mario::marioGetPtr()->playerPosition;
        const float halfWidth = sEnemyId == 0x21 || sEnemyId == 0x87 || sEnemyId == 0x90 || sEnemyId == 0x95 ? 100.0f : 50.0f;
        const float left = std::min(mario.x - 60.0f, boss.x - halfWidth);
        const float right = std::max(mario.x + 50.0f, boss.x + halfWidth);
        const float x = (left + right) * 0.5f;
        const float y = std::max(InGoldenFuzzyRoom() ? -15.0f : (FrontCameraRoom() ? 0.0f : 10.0f), boss.y) + std::max(45.0f, sHeight * 0.6f);
        const float z = (mario.z + boss.z) * 0.5f;
        const float distance = std::max(FrontCameraRoom() ? 400.0f : 500.0f, (right - left + 100.0f) * 1.4f);
        // Look into the room from the entrance side, with a gentle downward tilt.
        const bool blooper = InBlooperRoom();
        args[0] = static_cast<int32_t>(x - distance * (FrontCameraRoom() ? 0.0f : (blooper ? 0.35f : 0.6f)));
        args[1] = static_cast<int32_t>(y + 85.0f);
        args[2] = static_cast<int32_t>(z + distance * (FrontCameraRoom() ? 1.0f : (blooper ? 0.94f : 0.8f)));
        args[3] = static_cast<int32_t>(x);
        args[4] = static_cast<int32_t>(y);
        args[5] = static_cast<int32_t>(z);
        evt->evtArguments = args;
        const int32_t result = ttyd::evt_cam::evt_cam3d_evt_set(evt, firstCall);
        evt->evtArguments = original;
        return result;
    }

    KEEP_FUNC bool HasSmorgReplacement()
    {
        const auto *scene = CurrentScene();
        return scene && scene->encounter == 18 && sSmorgFormed &&
            owr::gState && owr::gState->apSettings && owr::gState->apSettings->bossRandomizer &&
            owr::gState->bossLoadouts[18].enemyIds[0] != 0x6b;
    }

    KEEP_FUNC void SetSmorgFormed(bool formed)
    {
        sSmorgFormed = formed;
    }

    KEEP_FUNC int32_t SmorgCamera(ttyd::evtmgr::EvtEntry *evt, bool firstCall)
    {
        auto *npc = PreviewNpc();
        if (!Active() || !InPreviewRoom() || CurrentScene()->encounter != 18 || !npc)
            return ttyd::evt_cam::evt_cam3d_evt_set(evt, firstCall);
        auto *original = evt->evtArguments;
        int32_t args[8];
        std::copy(original, original + 8, args);
        // Retain the rooftop reveal's pans, tilt, durations and easing. Map its
        // leg/body/head framing to the replacement's height above the roof.
        const float targetY = ttyd::evtmgr_cmd::evtGetFloat(evt, original[4]);
        const float eyeY = ttyd::evtmgr_cmd::evtGetFloat(evt, original[1]);
        const float y = VisualPosition(npc).y + (targetY - npc->position.y) * sHeight / 240.0f;
        const float targetX = ttyd::evtmgr_cmd::evtGetFloat(evt, original[3]);
        const float targetZ = ttyd::evtmgr_cmd::evtGetFloat(evt, original[5]);
        const float eyeX = ttyd::evtmgr_cmd::evtGetFloat(evt, original[0]);
        const float eyeZ = ttyd::evtmgr_cmd::evtGetFloat(evt, original[2]);
        // Pull back along the viewing direction so the angle stays the same.
        args[0] = static_cast<int32_t>(targetX + (eyeX - targetX) * 1.15f);
        args[1] = static_cast<int32_t>(y + (eyeY - targetY) * 1.15f);
        args[2] = static_cast<int32_t>(targetZ + (eyeZ - targetZ) * 1.15f);
        args[4] = static_cast<int32_t>(y);
        if (sExtraCount > 0 && eyeZ > targetZ && (eyeZ - targetZ) * 1.15f < 450.0f)
            args[2] = static_cast<int32_t>(targetZ + 450.0f);
        evt->evtArguments = args;
        const int32_t result = ttyd::evt_cam::evt_cam3d_evt_set(evt, firstCall);
        evt->evtArguments = original;
        return result;
    }

    KEEP_FUNC int32_t SceneRelativeCamera(ttyd::evtmgr::EvtEntry *evt, bool firstCall)
    {
        if (!Active() || !InPreviewRoom())
            return ttyd::evt_cam::evt_cam3d_evt_set_rel(evt, firstCall);
        // The shared camera frames Mario and the replacement in world space,
        // retaining the original movement duration and easing arguments.
        return SceneCamera(evt, firstCall);
    }

    KEEP_FUNC int32_t ArenaCamera(ttyd::evtmgr::EvtEntry *evt, bool firstCall)
    {
        if (!Active() || !InPreviewRoom() ||
            (CurrentScene()->encounter != 20 && CurrentScene()->encounter != 21 && CurrentScene()->encounter != 16))
            return ttyd::evt_cam::evt_cam3d_evt_set(evt, firstCall);
        auto *original = evt->evtArguments;
        int32_t args[8];
        std::copy(original, original + 8, args);
        // Preserve the arena entrance and Crump's ship staging and pans. Adapt
        // close-up height without changing wide crowd or ship shots.
        const float targetY = ttyd::evtmgr_cmd::evtGetFloat(evt, original[4]);
        if (targetY < 200.0f)
        {
            const float shift = (sHeight - 80.0f) * 0.5f;
            args[1] = static_cast<int32_t>(ttyd::evtmgr_cmd::evtGetFloat(evt, original[1]) + shift);
            args[4] = static_cast<int32_t>(targetY + shift);
            if (sEnemyId == 0x21 || sEnemyId == 0x87 || sEnemyId == 0x90 || sEnemyId == 0x95)
            {
                const float eyeZ = ttyd::evtmgr_cmd::evtGetFloat(evt, original[2]);
                const float targetZ = ttyd::evtmgr_cmd::evtGetFloat(evt, original[5]);
                if (eyeZ > targetZ && eyeZ - targetZ < 450.0f)
                    args[2] = static_cast<int32_t>(targetZ + 450.0f);
            }
        }
        evt->evtArguments = args;
        const int32_t result = ttyd::evt_cam::evt_cam3d_evt_set(evt, firstCall);
        evt->evtArguments = original;
        return result;
    }

    KEEP_FUNC int32_t MagnusTransformCamera(ttyd::evtmgr::EvtEntry *evt, bool firstCall)
    {
        if (!Active() || !InPreviewRoom() || CurrentScene()->encounter != 15)
            return ttyd::evt_cam::evt_cam3d_evt_set(evt, firstCall);
        auto *original = evt->evtArguments;
        int32_t args[8];
        std::copy(original, original + 8, args);
        // Keep vanilla's reveal, cockpit zoom and landing pan. Lower the eye
        // and target together so the movement and viewing angle stay intact.
        // Decode event floats before adjusting them; some camera coordinates
        // use the engine's encoded float arguments rather than plain integers.
        args[1] = static_cast<int32_t>(ttyd::evtmgr_cmd::evtGetFloat(evt, original[1]) - 30.0f);
        args[4] = static_cast<int32_t>(ttyd::evtmgr_cmd::evtGetFloat(evt, original[4]) - 30.0f);
        evt->evtArguments = args;
        const int32_t result = ttyd::evt_cam::evt_cam3d_evt_set(evt, firstCall);
        evt->evtArguments = original;
        return result;
    }

    KEEP_FUNC void SetPalaceEncounter(int encounter)
    {
        sPalaceEncounter = encounter;
    }

    KEEP_FUNC void SetMagnus2Revealed()
    {
        sMagnus2Revealed = true;
    }

    KEEP_FUNC int32_t Magnus2Camera(ttyd::evtmgr::EvtEntry *evt, bool firstCall)
    {
        auto *npc = PreviewNpc();
        if (!Active() || !InPreviewRoom() || CurrentScene()->encounter != 0 || !npc)
            return ttyd::evt_cam::evt_cam3d_evt_set(evt, firstCall);
        auto *original = evt->evtArguments;
        int32_t args[8];
        std::copy(original, original + 8, args);
        // Preserve the cockpit zoom and landing pan, adapting their target
        // height to the replacement as it follows the robot's scripted root.
        const float targetY = ttyd::evtmgr_cmd::evtGetFloat(evt, original[4]);
        const float eyeY = ttyd::evtmgr_cmd::evtGetFloat(evt, original[1]);
        const float y = VisualPosition(npc).y + (targetY - npc->position.y) * sHeight / 180.0f;
        args[1] = static_cast<int32_t>(y + eyeY - targetY);
        args[4] = static_cast<int32_t>(y);
        if (sExtraCount > 0)
        {
            const float eyeZ = ttyd::evtmgr_cmd::evtGetFloat(evt, original[2]);
            const float targetZ = ttyd::evtmgr_cmd::evtGetFloat(evt, original[5]);
            if (eyeZ > targetZ && eyeZ - targetZ < 450.0f)
                args[2] = static_cast<int32_t>(targetZ + 450.0f);
        }
        evt->evtArguments = args;
        const int32_t result = ttyd::evt_cam::evt_cam3d_evt_set(evt, firstCall);
        evt->evtArguments = original;
        return result;
    }

    KEEP_FUNC int32_t SceneTextAnchor(ttyd::evtmgr::EvtEntry *evt, bool firstCall)
    {
        auto *npc = PreviewNpc();
        if (!Active() || UsesOriginalDragonScene() || !InPreviewRoom() || !npc)
            return ttyd::evt_msg::evt_msg_toge(evt, firstCall);
        auto *original = evt->evtArguments;
        const auto boss = VisualPosition(npc);
        int32_t args[] = {original[0], static_cast<int32_t>(boss.x),
                          static_cast<int32_t>(boss.y + sHeight + 15.0f),
                          static_cast<int32_t>(boss.z)};
        evt->evtArguments = args;
        const int32_t result = ttyd::evt_msg::evt_msg_toge(evt, firstCall);
        evt->evtArguments = original;
        return result;
    }

    KEEP_FUNC int32_t SceneDialogue(ttyd::evtmgr::EvtEntry *evt, bool firstCall)
    {
        if (firstCall && Active() && (FrontCameraRoom() || (InBlooperRoom() && sBlooperEmerged)))
        {
            if (auto *npc = PreviewNpc())
            {
                auto *original = evt->evtArguments;
                const auto position = VisualPosition(npc);
                int32_t anchor[] = {2, static_cast<int32_t>(position.x),
                    static_cast<int32_t>(position.y + sHeight + 15.0f), static_cast<int32_t>(position.z)};
                evt->evtArguments = anchor;
                ttyd::evt_msg::evt_msg_toge(evt, true);
                evt->evtArguments = original;
            }
        }
        const int32_t result = ttyd::evt_msg::evt_msg_print(evt, firstCall);
        if (Active() && InPreviewRoom())
        {
            sTalking = false;
            if (result != 2 && ttyd::windowdrv::windowCheckID(evt->wActiveMsgWindowId))
            {
                const auto *window = ttyd::windowdrv::windowGetPointer(evt->wActiveMsgWindowId);
                // State 1 reveals text; input waits and window transitions stay idle.
                sTalking = window && window->windowState == 1;
            }
        }
        return result;
    }

    KEEP_FUNC void Update()
    {
        if (!InPreviewRoom() || !owr::gState || !owr::gState->apSettings ||
            !owr::gState->apSettings->bossRandomizer)
            return;

        auto *npc = PreviewNpc();
        if (!npc)
            return;
        const auto *scene = CurrentScene();
        const uint8_t enemy = owr::gState->bossLoadouts[scene->encounter].enemyIds[0];
        if (!SceneReady(scene) || enemy == scene->originalEnemy)
            return;

        if (sEnemyId != enemy || sEncounter != scene->encounter)
        {
            if (sPreviewPose >= 0)
                ttyd::animdrv::animPoseRelease(sPreviewPose);
            for (auto &extra : sExtras)
                if (extra.pose >= 0) ttyd::animdrv::animPoseRelease(extra.pose);
            Reset();
            auto *kind = ttyd::battle_unit::GetUnitKindById(enemy);
            if (!kind || !kind->parts || kind->num_parts <= 0)
                return;
            const auto &part = kind->parts[0];
            const char *idle = IdlePose(part);
            if (!part.model_name || !idle || std::strlen(part.model_name) >= sizeof(sModel) ||
                std::strlen(idle) >= sizeof(sIdle))
                return;
            std::strcpy(sModel, part.model_name);
            std::strcpy(sIdle, idle);
            // Vivian's field standing pose differs from her battle neutral pose.
            if (std::strcmp(sModel, "c_vivian") == 0)
                std::strcpy(sIdle, "PTR_S_1");
            // Preserve small characters' natural size; only shrink large bosses.
            sVisualScale = kind->height > 0 ? std::clamp(140.0f / kind->height, 0.25f, 1.0f) : 1.0f;
            sHeight = kind->height > 0 ? kind->height * sVisualScale : 80.0f;
            sEnemyId = enemy;
            sEncounter = scene->encounter;
            if (enemy == 0x5d)
            {
                std::strcpy(sModel, "c_korutesu3");
                std::strcpy(sIdle, "KRT_S_1");
            }
            else if (enemy == 0x95)
            {
                // Assemble the battle's body, face, hair and hands around Peach.
                // The field c_q_kage asset does not include this battle arrangement.
                std::strcpy(sModel, "c_b_peach_b");
                std::strcpy(sIdle, "KRT_S_1");
                sVisualScale = 0.7f;
                sHeight = 180.0f * sVisualScale;
                ConfigureExtra(0, "c_q_kage_b", "KRT_Z_1", 0.0f, sVisualScale);
                ConfigureExtra(1, "c_q_kage_f", "KRT_S_1", 0.0f, sVisualScale);
                ConfigureExtra(2, "c_q_kage_h", "KRT_S_1", 0.0f, sVisualScale);
                sExtras[1].anchorParent = 0;
                sExtras[2].anchorParent = 1;
                ConfigureExtra(3, "c_b_hand", "KRT_S_1", 0.0f, sVisualScale);
                ConfigureExtra(4, "c_b_hand", "KRT_S_1", 0.0f, sVisualScale);
                // Native hand homes are (-135, 0, -50) and (10, 0, 50)
                // relative to the queen's body; the second hand is mirrored.
                sExtras[3].offset = {-135.0f, 0.0f, -50.0f};
                sExtras[4].offset = {10.0f, 0.0f, 50.0f};
                sExtras[4].mirror = true;
            }
            sTalk = TalkPose(sModel);
            if (enemy == 0x21 || enemy == 0x87)
            {
                ConfigureCompanion(0, enemy == 0x21 ? 0x1f : 0x85, -1.0f);
                ConfigureCompanion(1, enemy == 0x21 ? 0x20 : 0x86, 1.0f);
            }
            else if (enemy == 0x90)
                ConfigureCompanion(0, 0x91, 1.0f);
            else if (enemy == 0x22 || enemy == 0x79)
            {
                ConfigureExtra(0, sModel, "MGN_L_1", 0.0f, sVisualScale);
                // The main robot's native talking pose includes Crump's speech;
                // keep this cockpit-only attachment in its own pose.
                sExtras[0].talk = nullptr;
            }
            else if (enemy == 0x6b)
            {
                ConfigureExtra(0, "c_n_moa_a", "S_1", 0.0f, sVisualScale);
                ConfigureExtra(1, sModel, "S_1C_1", 0.0f, sVisualScale);
                ConfigureExtra(2, sModel, "S_1B_1", 0.0f, sVisualScale);
                ConfigureExtra(3, sModel, "S_1A_1", 0.0f, sVisualScale);
                ConfigureExtra(4, sModel, "S_2", 0.0f, sVisualScale);
                for (int i = 1; i <= 3; ++i) sExtras[i].talk = nullptr;
                sExtras[4].talk = "T_2";
            }
        }

        if (sPreviewPose < 0)
        {
            // Wait for the animation asset before creating a field pose.
            if (!ttyd::animdrv::animGroupBaseAsync(sModel, 0, nullptr))
                return;
            sPreviewPose = ttyd::animdrv::animPoseEntry(sModel, 0);
            if (sPreviewPose < 0)
                return;
            ttyd::animdrv::animPoseSetAnim(sPreviewPose, sIdle, 1);
            if (enemy == 0x22 || enemy == 0x79 || enemy == 0x6b)
                ttyd::animdrv::animPoseWorldPositionEvalOn(sPreviewPose);
            if (enemy == 0x22 || enemy == 0x79)
                sCockpitGroup = ttyd::animdrv::animPoseGetGroupIdx(sPreviewPose, "locator17");
            if (enemy == 0x6b)
                for (int i = 0; i < 47; ++i)
                {
                    sSmorgChains[i].start = ttyd::animdrv::animPoseGetGroupIdx(sPreviewPose, kSmorgChains[i][0]);
                    sSmorgChains[i].end = ttyd::animdrv::animPoseGetGroupIdx(sPreviewPose, kSmorgChains[i][1]);
                }
        }

        sSourcePose = static_cast<int32_t>(npc->poseId);
        const char *source = ttyd::animdrv::animPoseGetCurrentAnim(sSourcePose);
        if (InBlooperRoom() && source &&
            (std::strcmp(source, "GSO_O_1") == 0 || std::strcmp(source, "GSO_O_2") == 0))
            sBlooperEmerged = true;
        if ((source && std::strcmp(source, sSourceAnim) != 0) || sTalking != sWasTalking)
        {
            const char *animation = sTalking && sTalk ? sTalk : sIdle;
            if (UsesOriginalDragonScene() && source)
                animation = source;
            else if (!sTalking && std::strcmp(sModel, "c_vivian") == 0 && source &&
                     std::strncmp(source, "GNB_H_", 6) == 0)
                animation = std::strcmp(source, "GNB_H_1") == 0 ? "PTR_D_1" : "PTR_D_2";
            SelectAnimation(sPreviewPose, animation, sIdle);
            if (source)
            {
                std::strncpy(sSourceAnim, source, sizeof(sSourceAnim) - 1);
                sSourceAnim[sizeof(sSourceAnim) - 1] = '\0';
            }
            sWasTalking = sTalking;
        }
        if (HasGhostTail(sModel))
        {
            // Vivian's lower body is generated by the animation driver from a
            // world-space ground anchor, rather than ordinary model geometry.
            const auto ground = VisualPosition(npc);
            ttyd::animdrv::animPoseVivianMain(sPreviewPose, &ground);
        }
        ttyd::animdrv::animPoseMain(sPreviewPose);
        for (int i = 0; i < sExtraCount; ++i)
        {
            auto &extra = sExtras[i];
            if (extra.pose < 0)
            {
                if (!ttyd::animdrv::animGroupBaseAsync(extra.model, 0, nullptr)) continue;
                extra.pose = ttyd::animdrv::animPoseEntry(extra.model, 0);
                if (extra.pose < 0) continue;
                SelectAnimation(extra.pose, sTalking && extra.talk ? extra.talk : extra.idle, extra.idle);
                extra.talking = sTalking;
                if ((enemy == 0x95 && i <= 2) || (enemy == 0x6b && i > 0))
                {
                    ttyd::animdrv::animPosePeraOff(extra.pose);
                    ttyd::animdrv::animPoseWorldPositionEvalOn(extra.pose);
                }
            }
            if (enemy == 0x95 && extra.anchorParent >= 0 && extra.anchorGroup < 0)
            {
                const auto &parent = sExtras[extra.anchorParent];
                if (parent.pose >= 0)
                    extra.anchorGroup = ttyd::animdrv::animPoseGetGroupIdx(parent.pose, "locator12");
            }
            if (extra.talking != sTalking)
            {
                SelectAnimation(extra.pose, sTalking && extra.talk ? extra.talk : extra.idle, extra.idle);
                extra.talking = sTalking;
            }
            if (HasGhostTail(extra.model))
            {
                const auto ground = RowPosition(VisualPosition(npc), extra.rowOffset);
                ttyd::animdrv::animPoseVivianMain(extra.pose, &ground);
            }
            ttyd::animdrv::animPoseMain(extra.pose);
        }
    }
}
