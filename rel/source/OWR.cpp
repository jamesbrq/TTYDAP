#include "relmgr.h"
#include "visibility.h"
#include <AP/rel_patch_definitions.h>
#include <gc/OSModule.h>
#include <gc/pad.h>
#include <mod.h>
#include "util.h"
#include "customWarp.h"
#include <ttyd/common_types.h>
#include <ttyd/countdown.h>
#include <ttyd/evt_bero.h>
#include <ttyd/evt_mario.h>
#include <ttyd/evt_memcard.h>
#include <ttyd/evt_msg.h>
#include <ttyd/evt_party.h>
#include <ttyd/evt_pouch.h>
#include <ttyd/evt_snd.h>
#include <ttyd/evt_window.h>
#include <ttyd/evt_lecture.h>
#include <ttyd/evtmgr.h>
#include <ttyd/evtmgr_cmd.h>
#include <ttyd/icondrv.h>
#include <ttyd/mario.h>
#include <ttyd/mario_motion.h>
#include <ttyd/mario_party.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/mariost.h>
#include <ttyd/msgdrv.h>
#include <ttyd/party.h>
#include <ttyd/pmario_sound.h>
#include <ttyd/seq_mapchange.h>
#include <ttyd/seqdrv.h>
#include <ttyd/statuswindow.h>
#include <ttyd/string.h>
#include <ttyd/swdrv.h>
#include <ttyd/win_log.h>
#include <ttyd/win_main.h>
#include <ttyd/win_root.h>
#include <ttyd/fontmgr.h>

#include "common.h"
#include "OWR.h"
#include "patch.h"

#include <cstdint>
#include <cstring>
#include <cstdio>

using gc::pad::PadInput;
using ttyd::common::ItemData;
using ttyd::seq_mapchange::_next_area;
using ttyd::seq_mapchange::RelId;
using ttyd::seqdrv::SeqIndex;
using namespace mod::util;
using namespace ttyd::common;
using namespace ttyd::mario;
using namespace ttyd::mario_pouch;
using namespace ttyd::icondrv;
using namespace ttyd::statuswindow;
using namespace ttyd::evt_mario;
using namespace ttyd::evt_msg;
using namespace ttyd::evt_pouch;
using namespace ttyd::evt_lecture;
using namespace ttyd::seqdrv;
using namespace mod::custom_warp;

char warpTextBuffer[64];

const uint16_t GSWF_ARR[] = {
    // Any of these being enabled will disable them
    // Shop Tutorial
    0,

    // Email Explanations
    37,
    38,

    // Save block and heart block tutorials
    233,
    234,

    // Zess T. blocking gate enable
    1187,

    // Contact Lens Ordered and in shop
    1189,
    1191,

    // Skip Zess T. Explain Dialogue
    1197,

    // Merlon Up Arrow
    1203,

    // Podley speach
    1214,

    // Pianta shop back door
    1230,

    // Attempt to enter ch.4 pipe early
    1341,

    // Goombella telling you about tattling
    1805,

    // Ch.4 talk to shopkeep once
    1925,

    // Ch.3 jolene hallway cutscene
    2445,

    // Ch.3 poison cake
    2451,

    // Post ch.3 guard
    2496,

    // Returned from ch.2
    2884,

    // Goldbob Approval
    3874,

    // Spawn General white
    3880,
    
    // Visited Rogueport
    6300};
constexpr int32_t GSWF_ARR_SIZE = sizeof(GSWF_ARR) / sizeof(GSWF_ARR[0]);

namespace mod::owr
{
    KEEP_VAR OWR *gSelf = nullptr;
    KEEP_VAR StateManager *gState = nullptr;

    KEEP_VAR bool (*g_OSLink_trampoline)(OSModuleInfo *, void *) = nullptr;
    KEEP_VAR void (*gTrampoline_seq_logoMain)(SeqInfo *info) = nullptr;
    KEEP_VAR void (*g_seqSetSeq_trampoline)(SeqIndex seq, const char *map, const char *bero) = nullptr;
    KEEP_VAR uint32_t (*g_pouchGetItem_trampoline)(int32_t) = nullptr;
    KEEP_VAR void (*g_partySetForceMove_trampoline)(ttyd::party::PartyEntry *ptr, float x, float z, float speed) = nullptr;
    KEEP_VAR int32_t (*g_evt_mario_set_pose_trampoline)(ttyd::evtmgr::EvtEntry *evt, bool firstCall) = nullptr;
    KEEP_VAR const char *(*g_msgSearch_trampoline)(const char *) = nullptr;
    KEEP_VAR void (*g_statusWinDisp_trampoline)(void) = nullptr;
    KEEP_VAR void (*g_pouchGetStarstone_trampoline)(int32_t) = nullptr;
    KEEP_VAR int32_t (*g_winItemMain_trampoline)(ttyd::win_root::WinPauseMenu *menu) = nullptr;
    KEEP_VAR int32_t (*g_winLogMain_trampoline)(ttyd::win_root::WinPauseMenu *menu) = nullptr;

    void OWR::SequenceInit()
    {
        SeqIndex Seq = seqGetNextSeq();

        if (Seq != SeqIndex::kMapChange)
            return;
        const OSModuleInfo *relPtr = _globalWorkPtr->relocationBase;
        if (!relPtr)
            return;
        if (relPtr->id == RelId::DMO)
            return;

        uint32_t SequencePosition = ttyd::swdrv::swByteGet(1700);

        if (SequencePosition != 0)
            return;

        ttyd::swdrv::swByteSet(1700, 16);
        ttyd::swdrv::swByteSet(1701, 3);
        ttyd::swdrv::swByteSet(1703, 1);
        ttyd::swdrv::swByteSet(1704, 1);
        ttyd::swdrv::swByteSet(1712, 1);
        ttyd::mario_pouch::pouchGetStarStone(0);

        // Give Return Pipe.
        ttyd::mario_pouch::pouchGetItem(ItemId::INVALID_ITEM_PAPER_0054);

        const bool apEnabled = static_cast<bool>(gState->apSettings->apEnabled);
        if (apEnabled)
        {
            ttyd::mario_pouch::pouchSetPartyColor(static_cast<uint8_t>(ttyd::party::PartyMembers::kYoshi),
                                                  gState->apSettings->yoshiColor);

            ttyd::mario_pouch::pouchSetYoshiName(gState->apSettings->yoshiName);

            ttyd::mario_party::partyJoin(gState->apSettings->startingPartner);
            ttyd::mario_party::marioPartyHello(gState->apSettings->startingPartner);

            if (gState->apSettings->openWestside)
                ttyd::swdrv::swSet(1188);
        }

        int32_t size = GSWF_ARR_SIZE;
        for (int i = 0; i < size; i++)
        {
            ttyd::swdrv::swSet(GSWF_ARR[i]);
        }

        // Perform a full recovery for Mario and the partners
        pouchReviseMarioParam();
        evt_pouch_mario_recovery(nullptr, false); // Parameters are unused for this function

        // Must call pouchRevisePartyParam to properly set each partner's stats, otherwise they will each have a maximum of 10
        // HP
        pouchRevisePartyParam();
        evt_pouch_all_party_recovery(nullptr, false); // Parameters are unused for this function

        if (apEnabled)
        {
            // Starting Stats
            PouchData *pouch = ttyd::mario_pouch::pouchGetPtr();
            pouch->base_max_hp = gState->apSettings->startingHP;
            pouch->max_hp = gState->apSettings->startingHP;
            pouch->current_hp = gState->apSettings->startingHP;
            pouch->base_max_fp = gState->apSettings->startingFP;
            pouch->max_fp = gState->apSettings->startingFP;
            pouch->current_fp = gState->apSettings->startingFP;
            pouch->total_bp = gState->apSettings->startingBP;
            pouch->unallocated_bp = gState->apSettings->startingBP;
        }
    }

    bool checkIfInGame()
    {
        const ttyd::seqdrv::SeqIndex nextSeq = seqGetNextSeq();
        if ((nextSeq != SeqIndex::kGame) && (nextSeq != SeqIndex::kBattle))
        {
            return false;
        }

        const ttyd::seqdrv::SeqIndex currentSeq = seqGetSeq();
        if ((currentSeq != SeqIndex::kGame) && (currentSeq != SeqIndex::kBattle))
        {
            return false;
        }

        const OSModuleInfo *relPtr = _globalWorkPtr->relocationBase;
        if (!relPtr)
        {
            return false;
        }

        return (relPtr->id != RelId::DMO);
    }

    void OWR::RecieveItems()
    {
        if (!checkIfInGame())
            return;

        uintptr_t length_pointer = 0x80000FFC;
        uintptr_t item_pointer = 0x80001000;

        uint32_t length = *reinterpret_cast<uint32_t *>(length_pointer);
        int16_t *items = reinterpret_cast<int16_t *>(item_pointer);

        if (length > 0)
        {
            for (uint32_t i = 0; i < length; i++)
            {
                // Try to give the item
                if (!pouchGetItem(items[i]))
                {
                    // Couldn't give the item, so try to send it to storage
                    pouchAddKeepItem(items[i]);
                }
                items[i] = 0;
            }
            memset(reinterpret_cast<void *>(length_pointer), 0, sizeof(uint32_t));
        }
    }

    KEEP_FUNC bool OSLinkHook(OSModuleInfo *new_module, void *bss)
    {
        bool result = g_OSLink_trampoline(new_module, bss);
        if (new_module != nullptr && result)
        {
            gSelf->OnModuleLoaded(new_module);
        }
        return result;
    }

    void setFirstVisitSW(const char* map)
    {
        if (strncmp(map, "gor", 3) == 0)
            ttyd::swdrv::swSet(6300);
        else if (strncmp(map, "tik", 3) == 0)
            ttyd::swdrv::swSet(6301);
        else if (strncmp(map, "hei", 3) == 0)
            ttyd::swdrv::swSet(6302);
        else if (strncmp(map, "nok", 3) == 0)
            ttyd::swdrv::swSet(6303);
        else if (strncmp(map, "gon", 3) == 0)
            ttyd::swdrv::swSet(6304);
        else if (strcmp(map, "win_06") == 0)
            ttyd::swdrv::swSet(6305);
        else if (strncmp(map, "mri", 3) == 0)
            ttyd::swdrv::swSet(6306);
        else if (strcmp(map, "win_04") == 0)
            ttyd::swdrv::swSet(6307);
        else if (strncmp(map, "tou", 3) == 0)
            ttyd::swdrv::swSet(6308);
        else if (strncmp(map, "usu", 3) == 0)
            ttyd::swdrv::swSet(6309);
        else if (strcmp(map, "gra_00") == 0)
            ttyd::swdrv::swSet(6310);
        else if (strcmp(map, "gra_06") == 0)
            ttyd::swdrv::swSet(6311);
        else if (strncmp(map, "muj", 3) == 0)
            ttyd::swdrv::swSet(6312);
        else if (strncmp(map, "dou", 3) == 0)
            ttyd::swdrv::swSet(6313);
        else if (strncmp(map, "hom", 3) == 0)
            ttyd::swdrv::swSet(6314);
        else if (strcmp(map, "pik_00") == 0 || strcmp(map, "pik_01") == 0)
            ttyd::swdrv::swSet(6315);
        else if (strcmp(map, "pik_02") == 0)
            ttyd::swdrv::swSet(6316);
        else if (strncmp(map, "bom", 3) == 0)
            ttyd::swdrv::swSet(6317);
        else if (strncmp(map, "moo", 3) == 0)
            ttyd::swdrv::swSet(6318);
        else if (strncmp(map, "aji", 3) == 0)
            ttyd::swdrv::swSet(6319);
        else if (strncmp(map, "las", 3) == 0)
            ttyd::swdrv::swSet(6320);
    }

    KEEP_FUNC void seqSetSeqHook(SeqIndex seq, const char *map, const char *bero)
    {
        // Make sure the map is valid
        if (!map)
        {
            return g_seqSetSeq_trampoline(seq, map, bero);
        }

        // Check if map is equal to 1 so we dont call a strcmp with an invalid pointer
        if (map == reinterpret_cast<const char *>(1))
        {
            return g_seqSetSeq_trampoline(seq, map, bero);
        }

        setFirstVisitSW(map); // Set GSWF flag on first visit to a map

        // Give Zess T. the conctact lens upon entering westside
        if (strcmp(map, "gor_03") == 0)
        {
            ttyd::swdrv::swSet(1188);
        }

        // Set cutscene flag for Don Pianta if player leaves westside
        if (ttyd::swdrv::swByteGet(1709) >= 3)
            ttyd::swdrv::swSet(6000);

        // Advance Boggly Woods sequence if the great tree is opened
        if (ttyd::swdrv::swByteGet(1713) >= 1 && ttyd::swdrv::swByteGet(1702) < 6)
            ttyd::swdrv::swByteSet(1702, 6);

        if (strcmp(map, "rsh_01_a") == 0)
        {
            uint8_t value = ttyd::swdrv::swByteGet(1706);

            if ((value >= 8 && value < 14) || (value >= 29 && value < 30))
                map = "rsh_01_b";
            else if ((value >= 14 && value < 22) || (value >= 30 && value < 31))
                map = "rsh_01_c";
        }
        else if (strcmp(map, "rsh_02_a") == 0)
        {
            uint8_t value = ttyd::swdrv::swByteGet(1706);

            if ((value >= 8 && value < 14) || (value >= 29 && value < 30))
                map = "rsh_02_b";
            else if ((value >= 14 && value < 22) || (value >= 30 && value < 31))
                map = "rsh_02_c";
        }
        else if (strcmp(map, "aaa_00") == 0)
        {
            if (ttyd::swdrv::swByteGet(1708) < 17)
            {
                uint32_t namePtr = 0x802c0298;
                const char *mapName = reinterpret_cast<char *>(namePtr);
                return g_seqSetSeq_trampoline(seq, mapName, bero);
            }
        }
        else if (strncmp(map, "rsh", 3) == 0)
        {
            if (ttyd::swdrv::swByteGet(1706) < 43)
            {
                if (bero && (strcmp(bero, "nidome") == 0 || strcmp(bero, "nidome_start") == 0))
                {
                    return g_seqSetSeq_trampoline(seq, map, 0);
                }
            }
        }

        if (gState->apSettings->palaceSkip != 0)
        {
            if (strcmp(map, "las_00") == 0)
            {
                if (ttyd::swdrv::swByteGet(1708) < 14)
                    ttyd::swdrv::swByteSet(1708, 14);
                uint32_t namePtr = 0x802c0a94; // las_27
                const char *mapName = reinterpret_cast<char *>(namePtr);
                uint32_t beroPtr = 0x802e7de4; // w_bero
                const char *beroName = reinterpret_cast<char *>(beroPtr);
                return g_seqSetSeq_trampoline(seq, mapName, beroName);
            }
            else if (strcmp(map, "las_25") == 0)
            {
                uint32_t namePtr = 0x802c02f8; // tik_05
                const char *mapName = reinterpret_cast<char *>(namePtr);
                uint32_t beroPtr = 0x802e8bd8; // n_bero
                const char *beroName = reinterpret_cast<char *>(beroPtr);
                return g_seqSetSeq_trampoline(seq, mapName, beroName);
            }
        }

        return g_seqSetSeq_trampoline(seq, map, bero);
    }

    KEEP_FUNC const char *msgSearchHook(const char *msgKey)
    {
        if (!strcmp(msgKey, "jolene_fukidashi"))
        {
            return "Oh my!<wait 100> Excuse me.\n<k>";
        }
        if (!strcmp(msgKey, "jolene_fukidashi_end"))
        {
            return "Well then.<wait 100> Shall we\nget going?\n<k>";
        }
        if (!strcmp(msgKey, "madam_abort"))
        {
            return "<p>Oh silly me, I already have\nmy ring.<k>";
        }
        if (!strcmp(msgKey, "keelhaul_return"))
        {
            return "Would you like to go back to\nKeelhaul Key?\n<o>";
        }
        if (!strcmp(msgKey, "keelhaul_return_yn"))
        {
            return "<select 0 0 0 40>Yes\nNo";
        }
        if (!strcmp(msgKey, "keelhaul_return_yes"))
        {
            return "<p>Let's get going then.<k>";
        }
        if (!strcmp(msgKey, "keelhaul_return_no"))
        {
            return "<p>I'll be here if you ever\nneed to get back.<k>";
        }
        if (!strcmp(msgKey, "goldbob_guide"))
        {
            return "I'm going to need to see\nthe Goldbob Guide if you are going\nto use the cannon.\n<k>";
        }
        if (!strcmp(msgKey, "no_goldbob_guide"))
        {
            return "Sorry pal, no guide\nno cannon.\n<k>";
        }
        if (!strcmp(msgKey, "give_goldbob_guide"))
        {
            return "Thanks, let's begin then.\n<k>";
        }
        if (!strncmp(msgKey, "stg4_jin_19", 11) && strcmp(msgKey, "stg4_jin_19_select"))
        {
            return g_msgSearch_trampoline("stg4_jin_19_viv");
        }
        if (!strncmp(msgKey, "stg4_jin_33", 11))
        {
            return g_msgSearch_trampoline("stg4_jin_33_viv");
        }
        if (!strncmp(msgKey, "stg4_jin_34", 11))
        {
            return g_msgSearch_trampoline("stg4_jin_34_viv");
        }
        if (!strncmp(msgKey, "stg4_jin_36", 11))
        {
            return g_msgSearch_trampoline("stg4_jin_36_viv");
        }
        if (!strncmp(msgKey, "stg4_jin_38", 11))
        {
            return g_msgSearch_trampoline("stg4_jin_38_viv");
        }
        if (!strcmp(msgKey, "no_diary"))
        {
            return "<p><col 000000a0><shake>I need my diary.\n</shake></col>\n<k>";
        }
        if (!strcmp(msgKey, "no_paper"))
        {
            return "<p>Gonna need that paper.\n<k>";
        }
        if (!strcmp(msgKey, "no_blanket"))
        {
            return "<p>I really need that blanket.\n<k>";
        }
        if (!strcmp(msgKey, "stg6_rsh_diary_01"))
        {
            // Change the text asking if you want to read the diary
            // Only needs to change when not on the train
            if (strcmp(_next_area, "rsh"))
            {
                return "An unseen force prevents\nyou from opening the "
                       "diary.<o>";
            }
        }
        if (strcmp(msgKey, "stg6_rsh_diary_01_yn") == 0)
        {
            // Change the yes/no text answers for the diary
            // Only needs to change when not on the train
            if (strcmp(_next_area, "rsh") != 0)
            {
#ifdef TTYD_JP
                const char *message = "<select 0 0 300 40>Dang\nShoot";
#else
                const char *message = "<select 0 0 0 40>Dang\nShoot";
#endif
                return message;
            }
        }
        if (!strcmp(msgKey, "raise_text"))
        {
            return "<system>Would you like to raise the\nfloor level of the "
                   "hallway?\n<o>";
        }
        if (!strcmp(msgKey, "raise_text2"))
        {
            return "<system><p>The floor was raised.\n"
                   "<k>";
        }
        if (!strcmp(msgKey, "raise_text_yn"))
        {
            return "<select 0 1 0 40>Yes\nNo";
        }
        if (!strcmp(msgKey, "lower_text"))
        {
            return "<system>Would you like to lower the\nfloor level of the "
                   "hallway?\n<o>";
        }
        if (!strcmp(msgKey, "lower_text2"))
        {
            return "<system><p>The floor was lowered.\n"
                   "<k>";
        }
        if (!strcmp(msgKey, "lower_text_yn"))
        {
            return "<select 0 1 0 40>Yes\nNo";
        }
        if (!strcmp(msgKey, "goombella"))
        {
            return "Goombella";
        }
        if (!strcmp(msgKey, "goombella_desc"))
        {
            return "Goombella<k>";
        }
        if (!strcmp(msgKey, "koops"))
        {
            return "Koops";
        }
        if (!strcmp(msgKey, "koops_desc"))
        {
            return "Koops<k>";
        }
        if (!strcmp(msgKey, "flurrie"))
        {
            return "Flurrie";
        }
        if (!strcmp(msgKey, "flurrie_desc"))
        {
            return "Flurrie<k>";
        }
        if (!strcmp(msgKey, "yoshi"))
        {
            return "Yoshi";
        }
        if (!strcmp(msgKey, "yoshi_desc"))
        {
            return "Yoshi<k>";
        }
        if (!strcmp(msgKey, "vivian"))
        {
            return "Vivian";
        }
        if (!strcmp(msgKey, "vivian_desc"))
        {
            return "Vivian<k>";
        }
        if (!strcmp(msgKey, "bobbery"))
        {
            return "Bobbery";
        }
        if (!strcmp(msgKey, "bobbery_desc"))
        {
            return "Bobbery<k>";
        }
        if (!strcmp(msgKey, "mowz"))
        {
            return "Ms.Mowz";
        }
        if (!strcmp(msgKey, "mowz_desc"))
        {
            return "Ms.Mowz<k>";
        }
        if (!strcmp(msgKey, "ap_item"))
        {
            return "AP Item<k>";
        }
        if (!strcmp(msgKey, "return_pipe"))
        {
            return "Return Pipe<k>";
        }
        if (!strcmp(msgKey, "return_pipe_description"))
        {
            return "This pipe can return you to\nRogueport.<k>";
        }
        if (!strcmp(msgKey, "10_coins"))
        {
            return "10 Coins<k>";
        }
        if (!strcmp(msgKey, "plane_mode"))
        {
            return "Plane Mode<k>";
        }
        if (!strcmp(msgKey, "paper_mode"))
        {
            return "Paper Mode<k>";
        }
        if (!strcmp(msgKey, "tube_mode"))
        {
            return "Tube Mode<k>";
        }
        if (!strcmp(msgKey, "boat_mode"))
        {
            return "Boat Mode<k>";
        }
        return g_msgSearch_trampoline(msgKey);
    }

    inline void pouchReAddReturnPipe()
    {
        // Remove the return pipe from the inventory, and then re-add it
        // Only re-add it if it was previously in the inventory, as it won't be when initially starting a new file
        if (pouchRemoveItem(ItemId::INVALID_ITEM_PAPER_0054))
        {
            pouchGetItem(ItemId::INVALID_ITEM_PAPER_0054);
        }
    }

    KEEP_FUNC uint32_t pouchGetItemHook(int32_t item)
    {
        switch (item)
        {
            case ItemId::SUPER_LUIGI:
            {
                ttyd::mario_party::partyJoin(PartyMembers::kGoombella);
                return 1;
            }
            case ItemId::SUPER_LUIGI_2:
            {
                ttyd::mario_party::partyJoin(PartyMembers::kKoops);
                return 1;
            }
            case ItemId::SUPER_LUIGI_3:
            {
                ttyd::mario_party::partyJoin(PartyMembers::kFlurrie);
                return 1;
            }
            case ItemId::SUPER_LUIGI_4:
            {
                ttyd::mario_party::partyJoin(PartyMembers::kYoshi);
                return 1;
            }
            case ItemId::SUPER_LUIGI_5:
            {
                ttyd::mario_party::partyJoin(PartyMembers::kVivian);
                return 1;
            }
            case ItemId::INVALID_ITEM_006F:
            {
                ttyd::mario_party::partyJoin(PartyMembers::kBobbery);
                return 1;
            }
            case ItemId::INVALID_ITEM_0070:
            {
                ttyd::mario_party::partyJoin(PartyMembers::kMsMowz);
                return 1;
            }
            case ItemId::INVALID_ITEM_0071:
            {
                return 1;
            }
            case ItemId::INVALID_ITEM_PAPER_0054:
            {
                // Give the return pipe without running pouchReAddReturnPipe
                return g_pouchGetItem_trampoline(item);
            }
            case ItemId::INVALID_ITEM_PAPER_0053:
            {
                pouchAddCoin(10);
                return 1;
            }
            case ItemId::BOOTS:
            {
                uint32_t ret;

                switch (pouchGetJumpLv())
                {
                    case PouchJumpLevel::JUMP_LEVEL_NONE:
                    {
                        ret = g_pouchGetItem_trampoline(ItemId::BOOTS);
                        break;
                    }
                    case PouchJumpLevel::JUMP_LEVEL_NORMAL:
                    {
                        ret = g_pouchGetItem_trampoline(ItemId::SUPER_BOOTS);
                        break;
                    }
                    case PouchJumpLevel::JUMP_LEVEL_SUPER:
                    default:
                    {
                        ret = g_pouchGetItem_trampoline(ItemId::ULTRA_BOOTS);
                        break;
                    }
                }

                // Regive the return pipe to make sure its always at the top of the inventory
                if (ret)
                {
                    pouchReAddReturnPipe();
                }
                return ret;
            }
            case ItemId::HAMMER:
            {
                uint32_t ret;

                switch (pouchGetHammerLv())
                {
                    case PouchHammerLevel::HAMMER_LEVEL_NONE:
                    {
                        ret = g_pouchGetItem_trampoline(ItemId::HAMMER);
                        break;
                    }
                    case PouchHammerLevel::HAMMER_LEVEL_NORMAL:
                    {
                        ret = g_pouchGetItem_trampoline(ItemId::SUPER_HAMMER);
                        break;
                    }
                    case PouchHammerLevel::HAMMER_LEVEL_SUPER:
                    default:
                    {
                        ret = g_pouchGetItem_trampoline(ItemId::ULTRA_HAMMER);
                        break;
                    }
                }

                // Regive the return pipe to make sure its always at the top of the inventory
                if (ret)
                {
                    pouchReAddReturnPipe();
                }
                return ret;
            }
            case ItemId::COCONUT:
            {
                // If the player has already given the coconut to Flavio, then just give the coconut normally
                if (ttyd::swdrv::swByteGet(1719) >= 4)
                {
                    return g_pouchGetItem_trampoline(item);
                }

                // Loop through all of the important items until either the coconut or an empty slot is found
                constexpr uint32_t loopCount = sizeof(PouchData::key_items) / sizeof(int16_t);
                int16_t *keyItemsPtr = &pouchGetPtr()->key_items[0];

                for (uint32_t i = 0; i < loopCount; i++)
                {
                    const int32_t currentItem = keyItemsPtr[i];
                    if (currentItem == ItemId::COCONUT)
                    {
                        // The player already has the coconut in their important items, so just give it normally
                        return g_pouchGetItem_trampoline(item);
                    }
                    else if (currentItem == ItemId::INVALID_NONE)
                    {
                        // The player does not have the coconut and an empty slot was found, so the coconut can be added
                        // Move all of the important items down one slot
                        memmove(&keyItemsPtr[1], &keyItemsPtr[0], (loopCount - 1) * sizeof(int16_t));

                        // Place the coconut in the first slot
                        keyItemsPtr[0] = ItemId::COCONUT;

                        // Regive the return pipe to make sure its always at the top of the inventory
                        pouchReAddReturnPipe();
                        return 1;
                    }
                }

                // If this is reached, then the important items part of the inventory is somehow full, so do some failsafe or
                // something
                return 0;
            }
            default:
            {
                // Give the item
                const uint32_t ret = g_pouchGetItem_trampoline(item);

                // If the given item is an important item, then regive the return pipe to make sure its always at the top of the
                // inventory
                if (ret && (item <= ItemId::CRYSTAL_STAR))
                {
                    pouchReAddReturnPipe();
                }

                return ret;
            }
        }
    }

    KEEP_FUNC void partySetForceMoveHook(ttyd::party::PartyEntry *ptr, float x, float z, float speed)
    {
        const Player *marioPtr = marioGetPtr();
        if (marioPtr->characterId == MarioCharacters::kMario)
        {
            if (marioPtr->currentMotionId == ttyd::mario_motion::MarioMotion::kYoshi)
                ttyd::mario_motion::marioChgMot(ttyd::mario_motion::MarioMotion::kStay);
        }
        return g_partySetForceMove_trampoline(ptr, x, z, speed);
    }

    KEEP_FUNC int32_t evtMarioSetPoseHook(ttyd::evtmgr::EvtEntry *evt, bool firstCall)
    {
        const Player *marioPtr = marioGetPtr();
        if (marioPtr->characterId == MarioCharacters::kMario)
        {
            if (marioPtr->currentMotionId == ttyd::mario_motion::MarioMotion::kYoshi)
                ttyd::mario_motion::marioChgMot(ttyd::mario_motion::MarioMotion::kStay);
        }
        return g_evt_mario_set_pose_trampoline(evt, firstCall);
    }

    // Skip logo
    KEEP_FUNC void logoSkip(SeqInfo *info)
    {
#ifdef TTYD_JP
        // No H&S screen so states are different
        // Skip from first logo fadeout wait directly
        // TODO: More efficient logo skip
        if (info->state == 3)
        {
            info->state = 9;
        }
#else
        // Skip states from H&S fadeout wait directly to demo fadeout start
        // TODO: Skip H&S
        if (info->state == 8)
        {
            info->state = 17;
        }
#endif
        return gTrampoline_seq_logoMain(info);
    }

    KEEP_FUNC void DisplayStarPowerOrbs(float x, float y, int32_t star_power)
    {
        using namespace ttyd::icondrv;
        using namespace ttyd::statuswindow;

        gc::vec3 pos;
        pos.y = y;
        pos.z = 0.f;

        int32_t max_star_power = pouchGetMaxAP();

        if (max_star_power > 800)
            max_star_power = 800;

        if (star_power > max_star_power)
            star_power = max_star_power;

        if (star_power < 0)
            star_power = 0;

        const int32_t full_orbs = star_power / 100;
        const int32_t remainder = star_power % 100;
        int32_t part_frame = remainder * 15 / 99;

        if (remainder > 0 && star_power > 0 && part_frame == 0)
            part_frame = 1;

        if (part_frame != 0)
        {
            pos.x = x + 32.f * intToFloat(full_orbs);
            // pos.y = y;
            // pos.z = 0.f;

            iconDispGx(1.f, &pos, 0x10, gauge_wakka[part_frame]);
        }

        // Draw grey orbs up to the max amount of SP / 100 (rounded up, max of 8).
        const uint16_t *gaugeBackPtr = &gauge_back[0];
        pos.y += 12.f;

        for (int32_t i = 0; i < (max_star_power + 99) / 100; ++i)
        {
            pos.x = x + 32.f * intToFloat(i);
            // pos.y = posY;
            // pos.z = 0.f;

            const uint16_t icon = i < full_orbs ? static_cast<IconType::e>(gaugeBackPtr[i]) : IconType::e::SP_ORB_EMPTY;
            iconDispGx(1.f, &pos, 0x10, icon);
        }
    }

    // Displays the Star Power in 0.01 units numerically below the status window.
    KEEP_FUNC void DisplayStarPowerNumber()
    {
        // Don't display SP if no Star Powers have been unlocked yet.
        if (pouchGetMaxAP() <= 0)
            return;

        // Don't try to display SP if the status bar is not on-screen.
        float menu_height =
            *reinterpret_cast<float *>(reinterpret_cast<uintptr_t>(ttyd::statuswindow::g_StatusWindowWork) + 0x24);

        if (menu_height < 100.f || menu_height > 330.f)
            return;

        gc::mtx34 matrix;
        gc::mtx::PSMTXTrans(&matrix, 192.f, menu_height - 100.f, 0.f);

        const int32_t current_AP = pouchGetAP();
        uint32_t color = ~0U;

        ttyd::icondrv::iconNumberDispGx(&matrix, current_AP, /* is_small = */ 1, &color);
        return g_statusWinDisp_trampoline();
    }

    // Make sure that the new MaxAP is greater than or equal to the current AP
    KEEP_FUNC void SetMaxSP(int star)
    {
        PouchData *pouchData = pouchGetPtr();
        const int32_t maxSP = pouchData->max_sp;

        g_pouchGetStarstone_trampoline(star);

        const int32_t newMaxSP = star * 100;
        if (newMaxSP < maxSP)
        {
            pouchData->max_sp = static_cast<int16_t>(maxSP);
        }
    }

    EVT_DECLARE_USER_FUNC(handleWarpConfirmResponse, 2)
    EVT_DEFINE_USER_FUNC(handleWarpConfirmResponse)
    {
        (void)isFirstCall;

        if (ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]) == 0)
        {
            ttyd::countdown::countDownEnd();
            ttyd::mario_motion::marioChgMot(ttyd::mario_motion::MarioMotion::kStay);

            // We clear the flag for being registered for a match in ch.3 so we can re-register later
            ttyd::swdrv::swClear(2388);

            // Also clear the flags for the champion match
            ttyd::swdrv::swClear(2383); // Match reservation flag
            ttyd::swdrv::swClear(2381); // Hallway escort flag

            // Set current pit floor to 0
            ttyd::swdrv::swByteSet(1321, 0);

            // Stop current music from certain areas
            ttyd::pmario_sound::psndBGMOff(513);

            uint32_t namePtr = 0x802c0298;
            const char *mapName = reinterpret_cast<char *>(namePtr);
            const char *bero = nullptr;

            WarpType warpType = static_cast<WarpType>(ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[1]));
            if (warpType == WarpType::FAST_TRAVEL)
            {
                const FastTravelPair fastTravelPair = FastTravelPair::getFastTravelPair();
                mapName = fastTravelPair.map_name;
                bero = fastTravelPair.bero;
            }

            ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, mapName, bero);
        }

        return 2;
    }

    EVT_DECLARE_USER_FUNC(checkValidWarpSequence, 1)
    EVT_DEFINE_USER_FUNC(checkValidWarpSequence)
    {
        (void)isFirstCall;

        const int ch5 = ttyd::swdrv::swByteGet(1717);
        if (10 <= ch5 && ch5 <= 18)
        {
            if (!strcmp(_next_area, "muj"))
            {
                ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 1);
            }
        }

        const int ch6 = ttyd::swdrv::swByteGet(1706);
        if (42 <= ch6 && ch6 <= 43)
        {
            ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 1);
        }

        return 2;
    }

    EVT_DECLARE_USER_FUNC(getWarpUnavailableText, 2)
    EVT_DEFINE_USER_FUNC(getWarpUnavailableText)
    {
        (void)isFirstCall;

        const WarpType warpType = static_cast<WarpType>(ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]));
        const char *text;

        if (warpType == WarpType::RETURN_PIPE)
        {
            text = "The Return Pipe";
        }
        else
        {
            text = "Fast travel";
        }

        snprintf(warpTextBuffer, sizeof(warpTextBuffer), "<system>\n<p>\n%s is currently\nunavailable.<k>", text);

        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], reinterpret_cast<uint32_t>(&warpTextBuffer[0]));
        return 2;
    }

    EVT_DECLARE_USER_FUNC(getWarpConfirmText, 2)
    EVT_DEFINE_USER_FUNC(getWarpConfirmText)
    {
        (void)isFirstCall;
        const WarpType warpType = static_cast<WarpType>(ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]));

        if (warpType == WarpType::RETURN_PIPE)
        {
            // Use snprintf to make sure that the string does not cause a buffer overflow and that it is properly null
            // terminated. Mainly only doing this in the event that the buffer and/or string are changed later on.
            snprintf(warpTextBuffer, sizeof(warpTextBuffer), "<system>\n<p>\nWarp home now?\n<o>");
        }
        else
        {
            // memory location points to key used to find the title of the location on the journal map
            const char *location_name = ttyd::msgdrv::msgSearch(ttyd::win_log::main_win_log_name);
            snprintf(warpTextBuffer, sizeof(warpTextBuffer), "<system>\n<p>\nFast travel to\n%s?\n<o>", location_name);
        }

        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], reinterpret_cast<uint32_t>(&warpTextBuffer[0]));
        return 2;
    }

    // clang-format off
    EVT_BEGIN(custom_warp_evt)
        USER_FUNC(lect_set_systemlevel, 1)
        USER_FUNC(evt_mario_key_onoff, 0)
        USER_FUNC(checkValidWarpSequence, LW(0))
        IF_EQUAL(LW(0), 1)
            USER_FUNC(getWarpUnavailableText, LW(10), LW(0))
            USER_FUNC(evt_msg_print, 1, LW(0), 0, 0)
            USER_FUNC(evt_mario_key_onoff, 1)
            USER_FUNC(lect_set_systemlevel, 0)
            RETURN()
        END_IF()
        USER_FUNC(getWarpConfirmText, LW(10), LW(0))
        USER_FUNC(evt_msg_print, 1, LW(0), 0, 0)
        USER_FUNC(evt_msg_select, 1, PTR("<select 0 1 0 40>\nYes\nNo"))
        USER_FUNC(evt_msg_continue)
        USER_FUNC(handleWarpConfirmResponse, LW(0), LW(10))
        IF_EQUAL(LW(0), 0)
            USER_FUNC(evt_mario_normalize)
        ELSE()
            USER_FUNC(evt_mario_key_onoff, 1)
        END_IF()
        USER_FUNC(lect_set_systemlevel, 0)
        RETURN()
    EVT_END()

    EVT_BEGIN(confirm_pipe_evt)
        SET(LW(10), static_cast<int32_t>(WarpType::RETURN_PIPE))
        RUN_CHILD_EVT(custom_warp_evt)
        RETURN()
    EVT_END()

    EVT_BEGIN(confirm_travel_evt)
        SET(LW(10), static_cast<int32_t>(WarpType::FAST_TRAVEL))
        RUN_CHILD_EVT(custom_warp_evt)
        RETURN()
    EVT_END()
    // clang-format on

    // Hook item menu update function to handle interactions with added key items.
    KEEP_FUNC int32_t WinItemMainHook(ttyd::win_root::WinPauseMenu *menu)
    {
        if (menu->itemMenuState == 10)
        {
            if ((menu->buttonsPressed & gc::pad::PadInput::PAD_A) && (menu->itemSubmenuId == 1) &&
                (menu->keyItemIds[menu->itemsCursorIdx[1]] == ItemId::INVALID_ITEM_PAPER_0054) &&
                (marioGetPtr()->characterId == MarioCharacters::kMario))
            {
                // Params taken from `evtEntryType` call in `mobjRunEvent` for running `mobj_save_blk_sysevt`, as using
                // `evtEntry` causes message selection boxes to not show up when the system level is raised, and certain `types`
                // cause the script to only run once the pause menu is fully closed
                ttyd::evtmgr::evtEntryType(const_cast<int32_t *>(confirm_pipe_evt), 30, 0, 26);
                return -2;
            }
        }

        return g_winItemMain_trampoline(menu);
    }

    // Hook journal menu to fast travel from the map
    KEEP_FUNC int32_t WinLogMainHook(ttyd::win_root::WinPauseMenu *menu)
    {
        if (menu->logMenuState == 10) // map open
        {
            if (!gState->apSettings->fastTravel)
                return g_winLogMain_trampoline(menu);

            if (menu->mapCursorIdx < 0) // no location selected
                return g_winLogMain_trampoline(menu);

            if ((menu->buttonsPressed & gc::pad::PadInput::PAD_A) && (marioGetPtr()->characterId == MarioCharacters::kMario))
            {
                // Params taken from `evtEntryType` call in `mobjRunEvent` for running `mobj_save_blk_sysevt`, as using
                // `evtEntry` causes message selection boxes to not show up when the system level is raised, and certain `types`
                // cause the script to only run once the pause menu is fully closed
                ttyd::evtmgr::evtEntryType(const_cast<int32_t *>(confirm_travel_evt), 30, 0, 26);
                return -2;
            }
        }

        return g_winLogMain_trampoline(menu);
    }

    void OWR::Update()
    {
        gState->apSettings->inGame = static_cast<uint8_t>(checkIfInGame());
        SequenceInit();
        RecieveItems();
    }

    void OWR::OnModuleLoaded(OSModuleInfo *module_info)
    {
        RelMgr *relMgrPtr = &relMgr;

        // The vanilla rel is unlinked every time you go through a loading zone, so our custom one must be relinked
        // If the game's vanilla tou2.rel was just linked, then force our custom tou2.rel to be loaded
        bool inNewArea;
        if (module_info->id == RelId::TOU2)
        {
            inNewArea = true;
            relMgrPtr->setPrevArea("tou2");
        }
        else
        {
            // If going into a new area, then load the new rel
            const char *nextArea = ttyd::seq_mapchange::_next_area;
            inNewArea = relMgrPtr->inNewArea(nextArea);
            relMgrPtr->setPrevArea(nextArea);
        }

        // Unlinking the rel now uses less instructions than doing so before checking for tou2
        const bool unlinked = relMgrPtr->unlinkRel();

        if (inNewArea || !unlinked)
        {
            relMgrPtr->unloadRel();

            if (!relMgrPtr->loadRel(relMgrPtr->getPrevArea()))
            {
                // Assume the desired rel is not included in this project
                return;
            }
        }

        // Relink the rel to reapply its patches
        relMgrPtr->linkRel();
    }

    void OWR::DrawString(const char *data, float x, float y, uint32_t color, float scale)
    {
        if (!data)
            return;

        // Init text drawing
        ttyd::fontmgr::FontDrawStart();
        ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
        ttyd::fontmgr::FontDrawEdge();
        ttyd::fontmgr::FontDrawScale(scale);

        // Draw each line
        char buf[128];
        const float lineDecrement = 32.f * scale;
        constexpr int32_t maxLength = sizeof(buf) - 1;

        while (1)
        {
            // Find the end of the current line
            const char *newline = strchr(data, '\n');

            // If a newline is not found, then currently at the last line
            if (!newline)
            {
                break;
            }

            // Copy this line to the temporary buffer and append a null byte
            int32_t lineLength = newline - data;

            // Make sure the current line won't be an empty string
            if (lineLength > 0)
            {
                // Prevent a buffer overflow
                if (lineLength > maxLength)
                {
                    lineLength = maxLength;
                }

                char *tempBuf = strncpy(buf, data, lineLength);
                tempBuf[lineLength] = '\0';

                ttyd::fontmgr::FontDrawString(x, y, tempBuf);
            }

            // Advance to the next line
            data = newline + 1;
            y -= lineDecrement;
        }

        // Draw the rest of the text
        ttyd::fontmgr::FontDrawString(x, y, data);
    }
} // namespace mod::owr