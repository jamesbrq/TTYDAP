#include "customWarp.h"
#include "MirrorMode.h"
#include "tracker.h"
#include "relmgr.h"
#include "util.h"
#include "visibility.h"
#include "GhostPeers.h"
#include <AP/rel_patch_definitions.h>
#include <gc/gx.h>
#include <gc/OSModule.h>
#include <gc/pad.h>
#include <mod.h>
#include "vm_prefetch.h"
#include "vm_ttyd.h"
#include <ttyd/common_types.h>
#include <ttyd/countdown.h>
#include <ttyd/evt_bero.h>
#include <ttyd/evt_fade.h>
#include <ttyd/evt_lecture.h>
#include <ttyd/evt_mario.h>
#include <ttyd/evt_memcard.h>
#include <ttyd/evt_mobj.h>
#include <ttyd/evt_msg.h>
#include <ttyd/evt_party.h>
#include <ttyd/evt_pouch.h>
#include <ttyd/evt_seq.h>
#include <ttyd/evt_snd.h>
#include <ttyd/evt_window.h>
#include <ttyd/evtmgr.h>
#include <ttyd/evtmgr_cmd.h>
#include <ttyd/fontmgr.h>
#include <ttyd/icondrv.h>
#include <ttyd/item_data.h>
#include <ttyd/itemdrv.h>
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
#include <ttyd/system.h>
#include <ttyd/win_log.h>
#include <ttyd/win_main.h>
#include <ttyd/win_root.h>
#include <ttyd/windowdrv.h>
#include <ttyd/animdrv.h>
#include <ttyd/battle.h>

#include "common.h"
#include "OWR.h"
#include "patch.h"
#include "boss_script.h"

#include <algorithm>
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstring>

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
using namespace ttyd::system;
using namespace ttyd::msgdrv;
using namespace ttyd::fontmgr;
using namespace ttyd::battle_unit;
using namespace ttyd::battle_database_common;
using namespace mod::custom_warp;

char warpTextBuffer[64];

const uint16_t GSWF_ARR[] = {
    // Any of these being enabled will disable them
    // Shop Tutorial
    0,

    // Email Explanations
    37,
    38,

    // Save block, heart block, and item tutorials
    233,
    234,
    235,

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

    // Sir Swoop cutscene
    // 2413,

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

namespace mod::ap_cooking
{
    KEEP_VAR int32_t gCookGiveItem = -1;
} // namespace mod::ap_cooking

namespace mod::owr
{
    KEEP_VAR OWR *gSelf = nullptr;
    KEEP_VAR StateManager *gState = nullptr;
    KEEP_VAR NumericInputData g_numericInput;

    KEEP_VAR const char *kChampStageGlobalDir = "bti_01";
    KEEP_VAR const char *kChampStageCurrentDir = "stg_03_0";
    KEEP_VAR BattleStageObjectData kChampStageProps[] = {
        {"gamen", 2, 0, {0.0f, 0.0f, -50.0f}, 0, 60, {0, 0}},
        {"etc", 2, 0, {0.0f, 0.0f, -54.0f}, 10, 50, {0, 0}},
        {"side__L", 2, 0, {-137.0f, 0.0f, -40.0f}, 20, 40, {0, 0}},
        {"side__R", 2, 0, {137.0f, 0.0f, -40.0f}, 30, 30, {0, 0}},
        {"B", 2, 2, {0.0f, 0.0f, 0.0f}, 0, 60, {0, 0}},
        {"C", 2, 3, {0.0f, 0.0f, 0.0f}, 0, 60, {0, 0}},
        {"if_wire", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0, 0}},
        {"if_body", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0, 0}},
    };

    KEEP_VAR const char *kZakoStageGlobalDir = "bti_01";
    KEEP_VAR const char *kZakoStageCurrentDir = "stg_05_0";
    KEEP_VAR BattleStageObjectData kZakoStageProps[] = {
        {"A1", 2, 0, {107.0f, 0.0f, -53.0f}, 0, 60, {0, 0}},
        {"S_mae_l", 2, 1, {-90.0f, 0.0f, 48.0f}, 0, 60, {0, 0}},
        {"S_mae_r", 2, 1, {84.0f, 0.0f, 48.0f}, 0, 60, {0, 0}},
        {"S_oku_l", 2, 1, {-35.0f, 0.0f, -42.0f}, 0, 60, {0, 0}},
        {"S_oku_r", 2, 1, {35.0f, 0.0f, -42.0f}, 0, 60, {0, 0}},
        {"B", 2, 2, {0.0f, 0.0f, 0.0f}, 0, 60, {0, 0}},
        {"C", 2, 3, {0.0f, 0.0f, 0.0f}, 0, 60, {0, 0}},
        {"if_wire", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0, 0}},
        {"if_body", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0, 0}},
    };

    KEEP_VAR BattleUnitSetup gKanbuPartyUnits[2];
    KEEP_VAR BattleGroupSetup gKanbuGroup;

    KEEP_VAR BattleStageData gChampStageData;
    KEEP_VAR BattleStageData gZakoStageData;

    KEEP_VAR bool (*g_OSLink_trampoline)(OSModuleInfo *, void *) = nullptr;
    KEEP_VAR void (*g_seq_logoMain_trampoline)(SeqInfo *info) = nullptr;
    KEEP_VAR void (*g_seq_gameInit_trampoline)(SeqInfo *info) = nullptr;
    KEEP_VAR void (*g_seqSetSeq_trampoline)(SeqIndex seq, const char *map, const char *bero) = nullptr;
    KEEP_VAR uint32_t (*g_pouchGetItem_trampoline)(int32_t) = nullptr;
    KEEP_VAR void (*g_partySetForceMove_trampoline)(ttyd::party::PartyEntry *ptr, float x, float z, float speed) = nullptr;
    KEEP_VAR int32_t (*g_evt_mario_set_pose_trampoline)(ttyd::evtmgr::EvtEntry *evt, bool firstCall) = nullptr;
    KEEP_VAR int32_t (*g_evt_party_jump_pos_trampoline)(ttyd::evtmgr::EvtEntry *evt, bool firstCall) = nullptr;
    KEEP_VAR const char *(*g_msgSearch_trampoline)(const char *) = nullptr;
    KEEP_VAR void (*g_statusWinDisp_trampoline)(void) = nullptr;
    KEEP_VAR void (*g_pouchGetStarstone_trampoline)(int32_t) = nullptr;
    KEEP_VAR int32_t (*g_winItemMain_trampoline)(ttyd::win_root::WinPauseMenu *menu) = nullptr;
    KEEP_VAR void (*g_marioEntry_trampoline)() = nullptr;
    KEEP_VAR int32_t (*g_winLogMain_trampoline)(ttyd::win_root::WinPauseMenu *menu) = nullptr;
    KEEP_VAR void (*g_msgAnalize_trampoline)(ttyd::memory::SmartAllocationData *smartAlloc, const char *text) = nullptr;
    KEEP_VAR int (*g_msgWindow_Entry_trampoline)(const char *message, int unk1, int windowType) = nullptr;
    KEEP_VAR void (*g__load_trampoline)(const char *mapName, const char *entranceName, const char *beroName) = nullptr;
    KEEP_VAR ttyd::battle_unit::BattleWorkUnit *(*g_BtlUnit_Entry_trampoline)(BattleUnitSetup *) = nullptr;
    KEEP_VAR void (*g_ExecAllUnitBattleEndEvent_trampoline)() = nullptr;
    KEEP_VAR int (*g_main__psndSFXOn_trampoline)(int, int, int, int, const void *, int, int, int) = nullptr;
    KEEP_VAR int (*g_psndSFXOff_trampoline)(int) = nullptr;
    KEEP_VAR void (*g_npcSetupBattleInfo_trampoline)(::NpcEntry *, void *) = nullptr;
    KEEP_VAR int32_t (*g_pouchRemoveItem_trampoline)(int32_t) = nullptr;
    KEEP_VAR int32_t (*g_pouchCheckItem_trampoline)(int32_t) = nullptr;
    KEEP_VAR int32_t (*g_sandersBombHitPosition_trampoline)(ttyd::evtmgr::EvtEntry *, bool) = nullptr;
    KEEP_VAR void (*g_swSet_trampoline)(int) = nullptr;
    KEEP_VAR int32_t (*g_BattleCalculateDamage_trampoline)(BattleWorkUnit *,
                                                           BattleWorkUnit *,
                                                           BattleWorkUnitPart *,
                                                           BattleWeapon *,
                                                           uint32_t *,
                                                           uint32_t) = nullptr;
    KEEP_VAR int32_t (*g_InterruptStop_trampoline)(ttyd::evtmgr::EvtEntry *, bool) = nullptr;
    KEEP_VAR int32_t (*g_BattleCheckConcluded_trampoline)(void *) = nullptr;
    KEEP_VAR void (*g_btlseqFirstAct_trampoline)(void *) = nullptr;
    KEEP_VAR ttyd::dvdmgr::DvdMgrFile *(*g_DVDMgrOpen_trampoline)(const char *, int, uint16_t) = nullptr;
    KEEP_VAR int32_t (*g_psndBGMOn_f_d_trampoline)(uint32_t, const char *, uint32_t, uint32_t, uint32_t) = nullptr;

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

        // Shuffle chapter stats before anything else
        if (gState->apSettings->shuffleChapterStats == 1)
            ShuffleBattleStats();

        ttyd::swdrv::swByteSet(1700, 16);
        ttyd::swdrv::swByteSet(1701, 3);
        ttyd::swdrv::swByteSet(1703, 1);
        ttyd::swdrv::swByteSet(1704, 1);
        ttyd::swdrv::swByteSet(1712, 1);
        ttyd::mario_pouch::pouchGetStarStone(0);

        // Store settings in GSW's so they are easier to access in scripts
        ttyd::swdrv::swByteSet(1727, gState->apSettings->grubbaBribeDirection);
        ttyd::swdrv::swByteSet(1728, gState->apSettings->grubbaBribeCost);

        if (gState->apSettings->cutsceneSkip)
        {
            ttyd::swdrv::swByteSet(1701, 5);
            ttyd::swdrv::swByteSet(1702, 11);
            ttyd::swdrv::swByteSet(1703, 4);
            ttyd::swdrv::swByteSet(1704, 7);
            ttyd::swdrv::swByteSet(1707, 2);
            ttyd::swdrv::swByteSet(1715, 1);
            ttyd::swdrv::swByteSet(1718, 2);
            ttyd::swdrv::swByteSet(1719, 1);
            ttyd::swdrv::swSet(1193); // Rougeport Pre-Old Letter Check Part 1
            ttyd::swdrv::swSet(1194); // Rougeport Pre-Old Letter Check Part 2
            ttyd::swdrv::swSet(1204); // Rougeport Talking to Merlon Outside
            ttyd::swdrv::swSet(1212); // Rougeport Gus Introduction
            ttyd::swdrv::swSet(1236); // Rougeport Don Pianta Pre-Blimp Ticket
            ttyd::swdrv::swSet(1237); // Rougeport Lucky Lottery Tutorial
            ttyd::swdrv::swSet(1325); // Rougeport Sewers Dazzle Introduction
            ttyd::swdrv::swSet(1342); // Rougeport Sewers Rip Cheato Introduction 1
            ttyd::swdrv::swSet(1353); // Rougeport Sewers Black Chest Cutscene
            ttyd::swdrv::swSet(1371); // Rougeport Sewers Rip Cheato Introduction 2
            ttyd::swdrv::swSet(1492); // Hooktail's Castle Black Chest Cutscene
            ttyd::swdrv::swSet(1781); // Petal Meadows Pre-Koops
            ttyd::swdrv::swSet(1782); // Petal Meadows Post Koops
            ttyd::swdrv::swSet(1932); // Twilight Town Black Chest Cutscene
            ttyd::swdrv::swSet(2392); // Glitz Pit Minor League Crew Introductions
            ttyd::swdrv::swSet(2395); // Glitz Pit Goomba Bros. Intro
            ttyd::swdrv::swSet(2397); // Glitz Pit Armored Harriers Intro
            ttyd::swdrv::swSet(2398); // Glitz Pit Armored Harriers 1st Win
            ttyd::swdrv::swSet(2399); // Glitz Pit Rawk Hawk Intro
            ttyd::swdrv::swSet(2401); // Glitz Pit Entered Promoter's Office from Air Vent
            ttyd::swdrv::swSet(2413); // Glitz Pit Sir Swoop Registers
            ttyd::swdrv::swSet(2513); // Glitz Pit Rawk Hawk Banter
            ttyd::swdrv::swSet(2514); // Glitz Pit Rawk Hawk Half HP
            ttyd::swdrv::swSet(2515); // Glitz Pit Rawk Hawk Desperation Phase
            ttyd::swdrv::swSet(2516); // Glitz Pit Rawk Hawk Hanging from Ceiling
            ttyd::swdrv::swSet(2847); // The Great Tree 10 Jabbi Introduction
            ttyd::swdrv::swSet(2855); // The Great Tree 100 Jabbi Introduction
            ttyd::swdrv::swSet(2982); // Pirate's Grotto Saw Toads on Boat
            ttyd::swdrv::swSet(3129); // Keelhaul Key Talked to Flavio about Bobbery
            ttyd::swdrv::swSet(3131); // Post Keelhaul Key Bridge Cutscene
            ttyd::swdrv::swSet(3136); // Pre-Keelhaul Key Embers at Pirate's Grotto Entrance
            ttyd::swdrv::swSet(3429); // Toodles Asks for Help
            ttyd::swdrv::swSet(3430); // Agreed to Help Toodles
            ttyd::swdrv::swSet(3431); // Excess Express Waitress Asks for Help
            ttyd::swdrv::swSet(3432); // Agreed to Help the Excess Express Waitress
            ttyd::swdrv::swSet(3438); // Excess Express Talked to Conductor on First Night
            ttyd::swdrv::swSet(3574); // Riverside Station Stationmaster Toad Cutscene
            ttyd::swdrv::swSet(3884); // Path to Fahr Outpost Intro
            ttyd::swdrv::swSet(4330); // Riddle Tower 3rd Floor 1st Entry
            ttyd::swdrv::swSet(6016); // Hooktail's Castle Meeting Ms. Mowz
            ttyd::swdrv::swSet(6023); // The Great Tree Zig-Zag Room Jabbi Running Away
            ttyd::swdrv::swSet(6035); // Storage Room Ms.Mowz Cutscene
        }

        if (gState->apSettings->enemyRandomizer != 0)
            ttyd::swdrv::swSet(2509); // Ch.3 first strike flag

        // Give Return Pipe.
        ttyd::mario_pouch::pouchGetItem(ItemId::INVALID_ITEM_PAPER_0054);

        ttyd::mario_pouch::pouchGetItem(ItemId::TRIPLE_DIP);

        const bool apEnabled = static_cast<bool>(gState->apSettings->apEnabled);
        if (apEnabled)
        {
            ttyd::mario_pouch::pouchSetPartyColor(static_cast<uint8_t>(ttyd::party::PartyMembers::kYoshi),
                                                  gState->apSettings->yoshiColor);

            ttyd::mario_pouch::pouchSetYoshiName(gState->apSettings->yoshiName);

            ttyd::mario_party::partyJoin(gState->apSettings->startingPartner);
            ttyd::mario_party::marioPartyHello(gState->apSettings->startingPartner);

            if (gState->apSettings->openWestside)
                ttyd::swdrv::swSet(6124); // Zess T. guards her kitchen instead of the west gate
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
            pouch->level = gState->apSettings->startingLevel;
            pouch->rank = getStageRankFromLevel(gState->apSettings->startingLevel);
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

    bool checkIfInGameNotBattle()
    {
        const ttyd::seqdrv::SeqIndex nextSeq = seqGetNextSeq();
        if (nextSeq != SeqIndex::kGame)
        {
            return false;
        }

        const ttyd::seqdrv::SeqIndex currentSeq = seqGetSeq();
        if (currentSeq != SeqIndex::kGame)
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

    namespace
    {
        constexpr uintptr_t kRunDirtyFlagAddr = 0x803DB190 + 1696; // GSW 1696
        constexpr uint8_t kDirtyCrossSeedSave = 0x01;   // save stamped with a different seed
        constexpr uint8_t kDirtyWallClockJump = 0x02;   // savestate load / emulator pause
        constexpr uint8_t kDirtyClockBeforeGen = 0x04;  // RTC predates seed generation
        constexpr uint8_t kDirtyExternalFlags = 0x08;   // flags written from outside the game
        constexpr uint8_t kDirtyExternalItems = 0x10;   // items delivered from outside the game
    } // namespace

    // Received-item feed interface; definitions live with the feed below
    void queueReceivedItemPopup(int16_t item, uint8_t cls, uint8_t senderSlot);
    bool recvFeedHasRoom(int32_t announcements);

    void OWR::RecieveItems()
    {
        if (!checkIfInGame())
            return;

        uintptr_t length_pointer = 0x80000FFC;
        uintptr_t item_pointer = 0x80001000;
        uintptr_t index_pointer = 0x803DB890;

        uint32_t length = *reinterpret_cast<uint32_t *>(length_pointer);
        if (length == 0)
            return;
        if (length > 255) // guard a clobbered/garbage length
        {
            *reinterpret_cast<uint32_t *>(length_pointer) = 0;
            return;
        }

        uint16_t *items = reinterpret_cast<uint16_t *>(item_pointer);

        uint32_t announced = 0;
        for (uint32_t i = 0; i < length; i++)
        {
            if (items[i] & 0x2000)
                announced++;
        }
        if (!recvFeedHasRoom(static_cast<int32_t>(announced)))
            return; // try again next frame once the feed has drained a little

        for (uint32_t i = 0; i < length; i++)
        {
            const uint16_t packed = items[i];
            const int16_t item = static_cast<int16_t>(packed & 0x1FF);
            // Try to give the item
            if (!pouchGetItem(item))
            {
                // Couldn't give the item, so try to send it to storage
                pouchAddKeepItem(item);
            }

            if (packed & 0x2000)
                queueReceivedItemPopup(item, static_cast<uint8_t>((packed >> 14) & 3),
                                       static_cast<uint8_t>((packed >> 9) & 0xF));

            // Crystal-star goal is handled in pouchGetItemHook via
            // checkCrystalStarGoal, which fires for stars from any source
            // (including the pouchGetItem call above).
            items[i] = 0;
        }

        *reinterpret_cast<uint32_t *>(index_pointer) += length;
        *reinterpret_cast<uint32_t *>(length_pointer) = 0; // release last; producer gates on this

        // Anything arriving through this channel came from outside the game
        *reinterpret_cast<uint8_t *>(kRunDirtyFlagAddr) |= kDirtyExternalItems;
    }

    KEEP_FUNC void replaceMultipleCharacters(ttyd::memory::SmartAllocationData *smartData, uint32_t startIndex, int value)
    {
        char newChars[4];
        snprintf(newChars, sizeof(newChars), "%3d", value); // Always 3 characters

        MessageData *msgData = reinterpret_cast<MessageData *>(smartData->pMemory);
        const uint32_t commandCount = msgData->command_count;
        TextCommand *commandsPtr = &msgData->commands[0];

        // Replace exactly 3 characters starting from startIndex
        for (int i = 0; i < 3 && (startIndex + i) < commandCount; i++)
        {
            TextCommand *cmd = &commandsPtr[startIndex + i];
            if (cmd->type >= 0xFFF4)
                continue;

            if (i == 0)
            {
                cmd->char_or_param1 = 0;
                cmd->param2 = -10;
            }

            char c = newChars[i];
            if (c == ' ')
            {
                cmd->type = 0x0000;
                cmd->flags &= ~(0x08 | 0x02);
            }
            else if (c >= '0' && c <= '9')
            {
                cmd->type = 0x0053 + (c - '0');
                cmd->flags = 0x08;
            }
        }
    }

    KEEP_FUNC int msgWindow_Entry_Hook(const char *message, int unk1, int windowType)
    {
        // Call original function first
        int windowId = g_msgWindow_Entry_trampoline(message, unk1, windowType);

        // Get the created window
        ttyd::windowdrv::Window *window = ttyd::windowdrv::windowGetPointer(windowId);

        // Make sure the window was properly created
        if (!window)
        {
            return windowId;
        }

        ttyd::memory::SmartAllocationData *msgDataPtr = window->msgData;
        if (!msgDataPtr)
        {
            return windowId;
        }

        void *memoryPtr = msgDataPtr->pMemory;
        if (!memoryPtr)
        {
            return windowId;
        }

        // Additional safety check - verify the memory is properly initialized
        MessageData *msgData = reinterpret_cast<MessageData *>(memoryPtr);
        if (!msgData) // Sanity check
        {
            return windowId;
        }

        const uint32_t commandCount = msgData->command_count;
        if ((commandCount == 0) || (commandCount > 1000)) // Sanity check
        {
            return windowId;
        }

        TextCommand *commandsPtr = &msgData->commands[0];
        NumericInputData *numericInputPtr = &g_numericInput;

        for (uint32_t i = 0; i < commandCount; i++)
        {
            TextCommand *cmd = &commandsPtr[i];
            if (cmd->type != 0xFFF0)
                continue;

            // Setup numeric input state
            numericInputPtr->initialValue = cmd->char_or_param1;
            numericInputPtr->minValue = cmd->param2;
            numericInputPtr->maxValue = cmd->param3;
            numericInputPtr->stepSize = (cmd->flags & 0xFF) ? cmd->flags : 1;
            numericInputPtr->currentValue = numericInputPtr->initialValue;
            numericInputPtr->active = true;
            numericInputPtr->window_id = windowId;

            // Configure window
            window->mainFunc = reinterpret_cast<void *>(numericWindow_Main);
            window->param = 6;
            window->x = -100.0f;
            window->y = 0.0f;
            window->width = 200.0f;
            window->height = 80.0f;

            cmd->flags |= 0x80000000;
            break;
        }

        return windowId;
    }

    KEEP_FUNC void MsgAnalizeHook(ttyd::memory::SmartAllocationData *smartAlloc, const char *text)
    {
        if (!smartAlloc || !text)
        {
            return;
        }

        g_msgAnalize_trampoline(smartAlloc, text);

        if (!strstr(text, "<numselect"))
        {
            return;
        }

        MessageData *msg_data = reinterpret_cast<MessageData *>(smartAlloc->pMemory);
        if (!msg_data || (msg_data->command_count > 1000))
        {
            return;
        }

        if (!util::ptrIsValid(msg_data))
        {
            return;
        }

        TextCommand *commands = reinterpret_cast<TextCommand *>((reinterpret_cast<uintptr_t>(msg_data) + 0x3C));
        const char *pos = text;
        int32_t tags_processed = 0;

        while ((pos = strstr(pos, "<numselect")) && (tags_processed < 3))
        {
            const char *end = strchr(pos, '>');
            if (!end)
                break;

            int32_t min_val = 0, max_val = 99, initial_val = 0, step_val = 1;
            const char *params = pos + 10;

            const int32_t parsed =
                sscanf(params, " %" PRId32 " %" PRId32 " %" PRId32 " %" PRId32, &min_val, &max_val, &initial_val, &step_val);

            const int32_t initial = ttyd::swdrv::swByteGet(1724);
            if (initial > 0)
            {
                initial_val = ttyd::swdrv::swByteGet(1724);
                ttyd::swdrv::swByteSet(1724, 0);
            }

            if ((parsed < 3) || (min_val > max_val) || (step_val <= 0))
            {
                pos = end + 1;
                continue;
            }

            // Find insertion point before END command
            const uint32_t commandCount = msg_data->command_count;
            uint32_t insert_pos = commandCount;

            for (uint32_t i = 0; i < commandCount; i++)
            {
                if (commands[i].type == 0xFFFF)
                {
                    insert_pos = i;
                    break;
                }
            }

            if (insert_pos >= 999)
                break;

            // Insert new command
            if (insert_pos < commandCount)
            {
                memmove(&commands[insert_pos + 1], &commands[insert_pos], (commandCount - insert_pos) * sizeof(TextCommand));
            }

            TextCommand *newCmd = &commands[insert_pos];
            memset(newCmd, 0, sizeof(TextCommand));

            newCmd->flags = step_val;
            newCmd->type = 0xFFF0;
            newCmd->char_or_param1 = static_cast<int16_t>(initial_val);
            newCmd->param2 = static_cast<int16_t>(min_val);
            newCmd->param3 = static_cast<int16_t>(max_val);
            newCmd->scale = 1.0f;
            // newCmd->rotation = 0.0f;

            msg_data->command_count = commandCount + 1;
            tags_processed++;
            pos = end + 1;
        }
    }

    static void handleNumericInput()
    {
        const uint32_t btnTrg = keyGetButtonTrg(gc::pad::PadId::CONTROLLER_ONE);
        const uint32_t btnRep = keyGetButtonRep(gc::pad::PadId::CONTROLLER_ONE);
        const uint32_t dirTrg = keyGetDirTrg(gc::pad::PadId::CONTROLLER_ONE);
        const uint32_t dirRep = keyGetDirRep(gc::pad::PadId::CONTROLLER_ONE);

        const bool upPressed = ((btnTrg | btnRep) & 0x08) || ((dirTrg | dirRep) & 0x1000);
        const bool downPressed = ((btnTrg | btnRep) & 0x04) || ((dirTrg | dirRep) & 0x2000);

        NumericInputData *numericInputPtr = &g_numericInput;
        int32_t currentValue = numericInputPtr->currentValue;
        const int32_t oldValue = currentValue;
        const int32_t minValue = numericInputPtr->minValue;
        const int32_t maxValue = numericInputPtr->maxValue;
        const int32_t stepSize = static_cast<int32_t>(numericInputPtr->stepSize);

        if (upPressed)
        {
            if ((currentValue + stepSize) <= maxValue)
            {
                currentValue += stepSize;
            }
        }
        else if (downPressed)
        {
            if ((currentValue - stepSize) >= minValue)
            {
                currentValue -= stepSize;
            }
        }
        else
        {
            return;
        }

        if (currentValue < minValue)
        {
            currentValue = numericInputPtr->minValue;
        }
        else if (currentValue > maxValue)
        {
            currentValue = maxValue;
        }

        numericInputPtr->currentValue = static_cast<int16_t>(currentValue);

        if (oldValue != currentValue)
        {
            ttyd::pmario_sound::psndSFXOn(0x20005);
        }
    }

    KEEP_FUNC int numericWindow_Main(ttyd::windowdrv::Window *window)
    {
        msgMain(window->msgData);

        NumericInputData *numericInputPtr = &g_numericInput;
        int32_t windowAlpha = window->alpha;

        switch (window->windowState)
        {
            case 5: // Fade in
            {
                windowAlpha = std::min(255, windowAlpha + 25);
                window->alpha = static_cast<int16_t>(windowAlpha);

                if (windowAlpha >= 255)
                {
                    window->windowState = 1;
                }
                break;
            }
            case 1: // Active
            {
                handleNumericInput();

                const uint32_t btnTrg = keyGetButtonTrg(gc::pad::PadId::CONTROLLER_ONE);
                if (btnTrg & gc::pad::PadInput::PAD_A)
                {
                    numericInputPtr->selectedValue = numericInputPtr->currentValue;
                    window->windowState = 7;
                    ttyd::pmario_sound::psndSFXOn(0x20012);
                }
                else if (btnTrg & gc::pad::PadInput::PAD_B)
                {
                    numericInputPtr->selectedValue = -1;
                    window->windowState = 7;
                    ttyd::pmario_sound::psndSFXOn(0x20013);
                }
                break;
            }
            case 7: // Fade out
            {
                if (windowAlpha <= 0)
                {
                    window->windowState = 4;
                    window->flags &= ~2;
                    numericInputPtr->active = false;
                    return 1;
                }

                windowAlpha = std::max(0, windowAlpha - 25);
                window->alpha = static_cast<int16_t>(windowAlpha);
                break;
            }
        }

        const float priority = 400.0f - intToFloat(static_cast<int32_t>(windowAlpha));
        ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::k2d, 0, priority, numericWindow_Disp, window);
        return 0;
    }

    KEEP_FUNC void numericWindow_Disp(ttyd::dispdrv::CameraId cameraId, void *user)
    {
        (void)cameraId;

        gc::gx::GXColor fogColor(0x66, 0x06, 0x42, 0x80);
        gc::gx::GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fogColor);

        const ttyd::windowdrv::Window *window = reinterpret_cast<ttyd::windowdrv::Window *>(user);
        const uint8_t alpha = window->alpha & 0xFF;
        uint32_t frameColor = 0xFFFFFF00 | alpha;

        ttyd::windowdrv::windowDispGX_Waku_col(0, &frameColor, window->x, window->y, window->width, window->height, 30.0f);
        replaceMultipleCharacters(window->msgData, 0, g_numericInput.currentValue);

        const float widthHalf = window->width / 2.0f;
        const float textX = window->x + (widthHalf - 30.0f);
        const float textY = window->y - ((window->height / 4.0f));
        ttyd::msgdrv::msgDisp(window->msgData, textX, textY, alpha);

        // Draw arrows
        const float windowPosX = window->x + widthHalf;
        gc::vec3 upArrowPos(windowPosX, (window->y - 15.0f), 0.0f);
        gc::vec3 downArrowPos(windowPosX, ((window->y - window->height) - 20.0f), 0.0f);

        ttyd::icondrv::iconDispGxAlpha(0.8f, &upArrowPos, 0x10, IconType::MENU_UP_POINTER, alpha);
        ttyd::icondrv::iconDispGxAlpha(0.8f, &downArrowPos, 0x10, IconType::MENU_DOWN_POINTER, alpha);
    }

    // --- Ingredient unlock toast: on-screen window announcing a newly unlocked
    // cooking ingredient (queued from pouchGetItemHook, shown one at a time) ---
    namespace
    {
        constexpr int32_t kToastQueueSize = 16;
        constexpr int32_t kToastHoldFrames = 210; // ~3.5s
        constexpr int32_t kToastAlphaStep = 15;
        constexpr float kToastTextScale = 0.8f;
        constexpr float kToastLineHeight = 32.0f * kToastTextScale;
        constexpr float kToastTopY = 200.0f;

        int16_t sToastQueue[kToastQueueSize];
        int32_t sToastHead = 0;
        int32_t sToastCount = 0;

        char sToastText[80];
        int32_t sToastState = 0; // 0 = idle, 1 = fade in, 2 = hold, 3 = fade out
        int32_t sToastAlpha = 0;
        int32_t sToastTimer = 0;
        float sToastTextWidth = 0.0f;
    } // namespace

    void queueIngredientToast(int32_t item)
    {
        if (sToastCount >= kToastQueueSize)
            return; // drop rather than overflow; the unlock flag itself is already set

        sToastQueue[(sToastHead + sToastCount) % kToastQueueSize] = static_cast<int16_t>(item);
        sToastCount++;
    }

    static void ingredientToastDisp(ttyd::dispdrv::CameraId cameraId, void *user)
    {
        (void)cameraId;
        (void)user;

        // Disable fog so the window/text render with clean colors (numericWindow_Disp pattern)
        gc::gx::GXColor fogColor(0x66, 0x06, 0x42, 0x80);
        gc::gx::GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fogColor);

        const uint8_t alpha = static_cast<uint8_t>(sToastAlpha);
        const float width = sToastTextWidth + 40.0f;
        const float height = (2.0f * kToastLineHeight) + 28.0f;
        const float x = -(width / 2.0f);

        uint32_t plateColor = (static_cast<uint32_t>(alpha) * 3) / 5; // translucent black plate
        ttyd::windowdrv::windowDispGX_Waku_col(0, &plateColor, x, kToastTopY, width, height, 30.0f);

        const uint32_t textColor = 0xFFFFFF00 | alpha;
        gSelf->DrawString(sToastText, x + 20.0f, kToastTopY - 22.0f, textColor, kToastTextScale);
    }

    void updateIngredientToast()
    {
        if (sToastState == 0)
        {
            if (sToastCount == 0)
                return;

            const int32_t item = sToastQueue[sToastHead];
            sToastHead = (sToastHead + 1) % kToastQueueSize;
            sToastCount--;

            const char *name = "???";
            if (item >= 0 && item < static_cast<int32_t>(sizeof(ttyd::item_data::itemDataTable) /
                                                         sizeof(ttyd::item_data::itemDataTable[0])))
            {
                const char *nameKey = ttyd::item_data::itemDataTable[item].name;
                if (nameKey)
                {
                    const char *searched = ttyd::msgdrv::msgSearch(nameKey);
                    if (searched)
                        name = searched;
                }
            }

            constexpr const char *header = "New ingredient unlocked!";
            snprintf(sToastText, sizeof(sToastText), "%s\n%s", header, name);

            const uint16_t headerWidth = ttyd::fontmgr::FontGetMessageWidth(header);
            const uint16_t nameWidth = ttyd::fontmgr::FontGetMessageWidth(name);
            sToastTextWidth = static_cast<float>(headerWidth > nameWidth ? headerWidth : nameWidth) * kToastTextScale;

            sToastState = 1;
            sToastAlpha = 0;
            sToastTimer = 0;
            ttyd::pmario_sound::psndSFXOn(0x20012);
        }

        switch (sToastState)
        {
            case 1: // Fade in
            {
                sToastAlpha += kToastAlphaStep;
                if (sToastAlpha >= 255)
                {
                    sToastAlpha = 255;
                    sToastState = 2;
                }
                break;
            }
            case 2: // Hold
            {
                if (++sToastTimer >= kToastHoldFrames)
                    sToastState = 3;
                break;
            }
            case 3: // Fade out
            {
                sToastAlpha -= kToastAlphaStep;
                if (sToastAlpha <= 0)
                {
                    sToastAlpha = 0;
                    sToastState = 0;
                }
                break;
            }
        }

        if (sToastState != 0 && checkIfInGame())
        {
            ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::kDebug3d, 1, 150.0f, ingredientToastDisp, nullptr);
        }
    }

    namespace
    {
        constexpr int32_t kRecvFeedSize = 6;         // visible rows
        constexpr int32_t kRecvFeedFadeInFrames = 12;
        constexpr int32_t kRecvFeedHoldFrames = 300; // ~5s
        constexpr int32_t kRecvFeedFadeOutFrames = 30;
        constexpr int32_t kRecvFeedLifeFrames = kRecvFeedFadeInFrames + kRecvFeedHoldFrames + kRecvFeedFadeOutFrames;
        constexpr float kRecvFeedX = -260.0f;
        constexpr float kRecvFeedTopY = 96.0f; // below the top-left HUD elements
        constexpr float kRecvFeedTextScale = 0.65f;
        constexpr float kRecvFeedRowHeight = 26.0f;

        // AP classification colors (RGB): filler, useful, progression, trap
        constexpr uint32_t kRecvFeedColors[4] = {0x00EEEE00, 0x6D8BE800, 0xAF99EF00, 0xFA807200};

        constexpr uintptr_t kRecvFeedSenderRingAddr = 0x80004700;
        constexpr uint32_t kRecvFeedSenderLen = 16;

        struct RecvFeedEntry
        {
            int16_t item;
            uint8_t cls;
            int32_t age;
            char sender[kRecvFeedSenderLen + 1];
        };
        RecvFeedEntry sRecvFeed[kRecvFeedSize];
        int32_t sRecvFeedCount = 0;

        constexpr int32_t kRecvFeedPendingSize = 32;
        RecvFeedEntry sRecvPending[kRecvFeedPendingSize];
        int32_t sRecvPendingCount = 0;
    } // namespace

    bool recvFeedHasRoom(int32_t announcements)
    {
        return sRecvPendingCount + announcements <= kRecvFeedPendingSize;
    }

    void queueReceivedItemPopup(int16_t item, uint8_t cls, uint8_t senderSlot)
    {
        if (sRecvPendingCount >= kRecvFeedPendingSize)
            return; // unreachable: RecieveItems reserves room for the whole batch
        RecvFeedEntry &entry = sRecvPending[sRecvPendingCount];
        entry.item = item;
        entry.cls = cls & 3;
        entry.age = 0;

        if (senderSlot == 15)
        {
            entry.sender[0] = '\0';
            sRecvPendingCount++;
            return;
        }

        // Copy the sender name now: the client reuses ring slots on later batches
        const char *ringName =
            reinterpret_cast<const char *>(kRecvFeedSenderRingAddr + (senderSlot & 0xF) * kRecvFeedSenderLen);
        uint32_t n = 0;
        for (; n < kRecvFeedSenderLen; n++)
        {
            const char c = ringName[n];
            if (c == '\0')
                break;
            // The client sanitizes to printable ASCII; drop anything else defensively
            entry.sender[n] = (c >= 0x20 && c < 0x7F) ? c : '?';
        }
        entry.sender[n] = '\0';
        sRecvPendingCount++;
    }

    static void receivedItemFeedDisp(ttyd::dispdrv::CameraId cameraId, void *user)
    {
        (void)cameraId;
        (void)user;

        // Disable fog so the text renders with clean colors (numericWindow_Disp pattern)
        gc::gx::GXColor fogColor(0x66, 0x06, 0x42, 0x80);
        gc::gx::GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fogColor);

        for (int32_t i = 0; i < sRecvFeedCount; i++)
        {
            const RecvFeedEntry &entry = sRecvFeed[i];

            int32_t alpha = 255;
            if (entry.age < kRecvFeedFadeInFrames)
                alpha = (entry.age * 255) / kRecvFeedFadeInFrames;
            else if (entry.age > kRecvFeedFadeInFrames + kRecvFeedHoldFrames)
                alpha = ((kRecvFeedLifeFrames - entry.age) * 255) / kRecvFeedFadeOutFrames;
            if (alpha < 0)
                alpha = 0;
            else if (alpha > 255)
                alpha = 255;

            const char *name = "???";
            const uint32_t nameRgb = kRecvFeedColors[entry.cls];
            if (entry.item >= 0 && entry.item < static_cast<int32_t>(sizeof(ttyd::item_data::itemDataTable) /
                                                                     sizeof(ttyd::item_data::itemDataTable[0])))
            {
                const char *nameKey = ttyd::item_data::itemDataTable[entry.item].name;
                if (nameKey)
                {
                    const char *searched = ttyd::msgdrv::msgSearch(nameKey);
                    if (searched)
                        name = searched;
                }
            }

            const float y = kRecvFeedTopY - static_cast<float>(i) * kRecvFeedRowHeight;

            // "Received <Item> from <player>" (item in its classification color)
            const char *prefixText = "Received ";
            const float prefixWidth =
                static_cast<float>(ttyd::fontmgr::FontGetMessageWidth(prefixText)) * kRecvFeedTextScale;
            const float nameWidth = static_cast<float>(ttyd::fontmgr::FontGetMessageWidth(name)) * kRecvFeedTextScale;

            char senderText[kRecvFeedSenderLen + 8];
            float senderWidth = 0.0f;
            if (entry.sender[0] != '\0')
            {
                snprintf(senderText, sizeof(senderText), " from %s", entry.sender);
                senderWidth = static_cast<float>(ttyd::fontmgr::FontGetMessageWidth(senderText)) *
                              kRecvFeedTextScale;
            }

            uint32_t plateColor = (static_cast<uint32_t>(alpha) * 2) / 5; // translucent black plate
            ttyd::windowdrv::windowDispGX_Waku_col(0, &plateColor, kRecvFeedX - 8.0f, y + 4.0f,
                                                   prefixWidth + nameWidth + senderWidth + 16.0f, kRecvFeedRowHeight,
                                                   8.0f);

            const uint32_t textAlpha = static_cast<uint32_t>(alpha);
            gSelf->DrawString(prefixText, kRecvFeedX, y, 0xC8C8C800 | textAlpha, kRecvFeedTextScale);
            gSelf->DrawString(name, kRecvFeedX + prefixWidth, y, nameRgb | textAlpha, kRecvFeedTextScale);
            if (senderWidth != 0.0f)
                gSelf->DrawString(senderText, kRecvFeedX + prefixWidth + nameWidth, y, 0xC8C8C800 | textAlpha,
                                  kRecvFeedTextScale);
        }
    }

    void updateReceivedItemFeed()
    {
        if (sRecvFeedCount == 0 && sRecvPendingCount == 0)
            return;

        const int32_t ageStep = sRecvPendingCount > 0 ? 4 : 1;
        for (int32_t i = 0; i < sRecvFeedCount; i++)
            sRecvFeed[i].age += ageStep;

        // Entries are pushed in order, so the front is always the oldest
        while (sRecvFeedCount > 0 && sRecvFeed[0].age >= kRecvFeedLifeFrames)
        {
            for (int32_t i = 1; i < sRecvFeedCount; i++)
                sRecvFeed[i - 1] = sRecvFeed[i];
            sRecvFeedCount--;
        }

        // Pull queued announcements into freed rows
        while (sRecvFeedCount < kRecvFeedSize && sRecvPendingCount > 0)
        {
            sRecvFeed[sRecvFeedCount++] = sRecvPending[0];
            for (int32_t i = 1; i < sRecvPendingCount; i++)
                sRecvPending[i - 1] = sRecvPending[i];
            sRecvPendingCount--;
        }

        if (sRecvFeedCount != 0 && checkIfInGame())
        {
            ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::kDebug3d, 1, 150.0f, receivedItemFeedDisp, nullptr);
        }
    }

    // --- RTA timer + credits results display ---
    namespace
    {
        // Total-playtime frame counter, persisted in the save file: GSW bytes 1796-1799
        // (base 0x803DB190), the aligned u32 right after the received-item index u32 at
        // 0x803DB890 (GSW 1792-1795). Ticks every frame a file is active, menus included.
        constexpr uintptr_t kRtaFrameCounterAddr = 0x803DB894;

        // Rom.py stores the seed name XOR'd with this key at 0x80003210 (mod protocol >= 2)
        // so the seed can't be casually read out of RAM mid-run; the client verifies against
        // the same encoding. Keep in sync with SEED_OBFUSCATION_KEY in the apworld's Data.py.
        constexpr uintptr_t kSeedNameAddr = 0x80003210;
        constexpr uint8_t kSeedObfuscationKey[16] = {0xA5, 0x1C, 0x7E, 0x33, 0xC9, 0x58, 0xE2, 0x0F,
                                                     0x96, 0x41, 0xDB, 0x6A, 0x24, 0xB7, 0x5D, 0xF0};

        // Run-integrity state, persisted in the save file. The (still obfuscated) seed is
        // stamped into GSW bytes 1680-1695 the first frame a file is active, binding the file
        // to its seed; a stamp that no longer matches the ISO (save copied from another seed)
        // dirties the run. GSW 1696 is the dirty flag, also set by the client when debug
        // commands are used or a fresh file gets a multi-item replay from the server.
        constexpr uintptr_t kSeedStampAddr = 0x803DB190 + 1680; // GSW 1680-1695
        // (kRunDirtyFlagAddr and the kDirty* reason bits are defined above, near RecieveItems)

        // Wall-clock anchors (RTC seconds), also save-persisted:
        // GSW 1668-1671 = wall seconds elapsed at goal completion (0 = not finished yet);
        // GSW 1672-1675 = RTC value when the file was first bound to the seed (0 = unset).
        constexpr uintptr_t kWallAtFinishAddr = 0x803DB190 + 1668;
        constexpr uintptr_t kWallStartAddr = 0x803DB190 + 1672;

        // EXI primitives (main.dol, via ttyd.us.lst) for reading the RTC seconds counter.
        extern "C"
        {
            int32_t EXILock(int32_t chan, uint32_t dev, void *unlockedCallback); // 0x802bcc40
            int32_t EXIUnlock(int32_t chan);                                     // 0x802bcd34
            int32_t EXISelect(int32_t chan, uint32_t dev, uint32_t freq);        // 0x802bc480
            int32_t EXIDeselect(int32_t chan);                                   // 0x802bc5ac
            int32_t EXIImm(int32_t chan, void *buf, int32_t len, uint32_t type, void *cb); // 0x802bb918
            int32_t EXISync(int32_t chan);                                       // 0x802bbd00
        }

        // Reads the RTC seconds counter over EXI (channel 0, device 1, read command 0x20000000).
        // Returns false without touching *out when the bus is busy (memcard traffic) - callers
        // simply retry on a later frame. Dolphin backs this device with the HOST clock; even
        // with Custom RTC enabled it only shifts the base by a boot-time constant and keeps
        // ticking in real time (truly emulated time exists only in movie playback/netplay).
        // So unlike the time base and every value in RAM, it does not rewind with savestates.
        bool readRtcSeconds(uint32_t *out)
        {
            if (!EXILock(0, 1, nullptr))
                return false;
            if (!EXISelect(0, 1, 3))
            {
                EXIUnlock(0);
                return false;
            }

            uint32_t cmd = 0x20000000;
            uint32_t data = 0;
            bool ok = EXIImm(0, &cmd, 4, 1, nullptr) && EXISync(0) && // 1 = EXI_WRITE
                      EXIImm(0, &data, 4, 0, nullptr) && EXISync(0);  // 0 = EXI_READ

            EXIDeselect(0);
            EXIUnlock(0);

            // All-zero / all-one responses are bus glitches (memcards share channel 0,
            // so contention around saves can slip a corrupt read past the busy checks)
            if (data == 0 || data == 0xFFFFFFFF)
                ok = false;

            if (ok)
                *out = data;
            return ok;
        }

        bool inEndingArea()
        {
            const OSModuleInfo *relPtr = _globalWorkPtr->relocationBase;
            return relPtr && relPtr->id == RelId::END;
        }

        // True only while a save file is actually loaded. Latched on the first real in-game
        // frame (checkIfInGame excludes the title attract demo via its rel id) and released
        // on the logo/title/file-select screens, so map changes, battles, and game over keep
        // the run going while nothing before a file load ever displays or ticks the timer.
        bool sSaveFileActive = false;

        // Set from updateRtaTimer whenever the sequence changes (map loads, battle entry,
        // game over) so the wall-clock monitor knows a frame stall was a loading screen
        // rather than a savestate/pause.
        bool sSeqChangedSinceLastPoll = false;

        // Set when a save file becomes active (fresh load, or save-and-quit followed by a
        // reload in the same session): the wall monitor re-anchors instead of treating the
        // time spent on the title screen as a wall jump.
        bool sResetWallMonitor = false;

        constexpr int32_t kPaperModeSaveGsw = 1800; // 0 = normal, 1 = boat, 2 = tube, 3 = paper, 4 = plane
        bool sPaperModeRestorePending = false;

        // Set when an RTC read fails between polls. The memcard shares EXI channel 0, so
        // failures mean card traffic (saving the game) - the resulting frame stalls are not
        // savestates, and the first reading after contention gets an extra confirmation.
        bool sExiBusySinceLastPoll = false;

        // Set from DVDMgrOpenHook whenever the game opens a file off the disc. Mid-map
        // loads that never touch the sequence (the W/L Emblem badges reloading Mario's
        // model, NPC spawns, area tattles) stall the frame loop just like a loading
        // screen; the wall monitor must not read that stall as a savestate/pause.
        bool sDiscReadSinceLastPoll = false;

        uint32_t sGracePollsRemaining = 0;
        constexpr uint32_t kGracePolls = 8;
        constexpr int32_t kGraceMaxDriftSeconds = 10;

        void updateSaveFileActive()
        {
            const SeqIndex seq = seqGetSeq();
            if (seq == SeqIndex::kLogo || seq == SeqIndex::kTitle || seq == SeqIndex::kLoad)
            {
                sSaveFileActive = false;
                if (seq == SeqIndex::kLoad)
                    sPaperModeRestorePending = true; // arm: next marioEntry restores the saved mode
            }
            else if (checkIfInGame() && !sSaveFileActive)
            {
                sSaveFileActive = true;
                sResetWallMonitor = true; // (re)entered a file; re-anchor the wall monitor
            }
        }

        void maintainRunIntegrity()
        {
            uint8_t *stamp = reinterpret_cast<uint8_t *>(kSeedStampAddr);
            const uint8_t *isoSeed = reinterpret_cast<const uint8_t *>(kSeedNameAddr);

            bool empty = true;
            bool matches = true;
            for (uint32_t i = 0; i < 16; i++)
            {
                if (stamp[i] != 0)
                    empty = false;
                if (stamp[i] != isoSeed[i])
                    matches = false;
            }

            // The obfuscated seed always has nonzero bytes (ASCII XOR high key bytes), so an
            // all-zero stamp can only mean a file that has never been bound to a seed yet.
            if (empty)
            {
                for (uint32_t i = 0; i < 16; i++)
                    stamp[i] = isoSeed[i];
            }
            else if (!matches)
            {
                *reinterpret_cast<uint8_t *>(kRunDirtyFlagAddr) |= kDirtyCrossSeedSave;
            }
        }

        // Once a second, compare how far the RTC advanced against how many frames we ran,
        // and reconcile the timer UP to true wall time. Loading screens block the frame
        // loop while the RTC keeps ticking; the reconcile makes that time count (real RTA),
        // makes emulator pauses cost time instead of stopping the clock, and gives a
        // savestate reload its rewound wall time straight back. A wall jump outside any
        // sequence transition additionally dirties the run as savestate/pause manipulation.
        void monitorWallClock(uint32_t frames)
        {
            static uint32_t sNextPollFrames = 0;
            if (frames < sNextPollFrames)
                return;

            uint32_t rtc;
            if (!readRtcSeconds(&rtc))
            {
                sExiBusySinceLastPoll = true; // memcard traffic on the shared channel
                return;                       // retry next frame
            }

            static uint32_t sLastRtc = 0;
            static uint32_t sLastFrames = 0;
            static uint32_t sSessionRtc = 0;    // per-session anchor: offline time between
            static uint32_t sSessionFrames = 0; // sessions must not flow into the timer
            uint32_t *counter = reinterpret_cast<uint32_t *>(kRtaFrameCounterAddr);

            if (sResetWallMonitor)
            {
                sResetWallMonitor = false;
                sLastRtc = 0; // re-anchor below; title-screen time stays off the clock
            }

            // Any reading more than an hour away from the last one is either a genuine
            // clock event or a corrupted transfer (the memcard shares EXI channel 0, and
            // on real hardware contention around saves can produce garbage that passes
            // the transfer checks). Require a second, agreeing read before believing it -
            // and treat the first reading after observed contention the same way, since
            // post-save reads are exactly the ones that come back subtly wrong.
            if (sLastRtc != 0)
            {
                const uint32_t magnitude = rtc >= sLastRtc ? rtc - sLastRtc : sLastRtc - rtc;
                if (magnitude > 3600 || sExiBusySinceLastPoll)
                {
                    uint32_t confirm;
                    if (!readRtcSeconds(&confirm) || (confirm >= rtc ? confirm - rtc : rtc - confirm) > 2)
                    {
                        sExiBusySinceLastPoll = true;
                        return; // glitched read; drop it and poll again next frame
                    }
                }
            }

            if (sLastRtc != 0)
            {
                // The RTC ticks in whole seconds, so alignment jitter alone can make a
                // legitimate ~1s poll window read +1; anything beyond that means the wall
                // clock moved while the game didn't. During sequence transitions that is
                // just the loading screen blocking the frame loop; anywhere else it is a
                // savestate load, an emulator pause, or clock manipulation.
                const int32_t rtcDelta = static_cast<int32_t>(rtc - sLastRtc);
                const int32_t frameSeconds = static_cast<int32_t>((frames - sLastFrames) / 60);
                const int32_t drift = rtcDelta - frameSeconds;
                const bool driftForgiven = drift <= kGraceMaxDriftSeconds &&
                                           (sSeqChangedSinceLastPoll || sExiBusySinceLastPoll ||
                                            sDiscReadSinceLastPoll || sGracePollsRemaining != 0);
                if (rtcDelta < -1 || (drift > 1 && !driftForgiven))
                    *reinterpret_cast<uint8_t *>(kRunDirtyFlagAddr) |= kDirtyWallClockJump;

                // Reconcile against the absolute session anchor (not per-window deltas, so
                // second-boundary jitter can't accumulate): if fewer frames ran than wall
                // time elapsed this session, credit the difference to the timer. The span
                // and raise guards keep a bad anchor or clock event from ever exploding
                // the timer: anything beyond them re-anchors instead of crediting.
                const uint32_t wallDelta = rtc - sSessionRtc; // u32; backward clocks wrap huge
                constexpr uint32_t kMaxSessionSpan = 48 * 3600;   // seconds
                constexpr uint32_t kMaxSingleRaise = 300 * 60;    // frames (5 minutes)
                if (wallDelta > kMaxSessionSpan)
                {
                    sSessionRtc = rtc; // anomalous span (wrap/garbage): re-anchor, no credit
                    sSessionFrames = *counter;
                }
                else
                {
                    const uint32_t expected = sSessionFrames + wallDelta * 60;
                    if (*counter < expected)
                    {
                        if (expected - *counter > kMaxSingleRaise)
                        {
                            sSessionRtc = rtc; // beyond any load/stall; jump check above
                            sSessionFrames = *counter; // already dirtied it if illegitimate
                        }
                        else
                        {
                            *counter = expected;
                        }
                    }
                }
            }
            else
            {
                // First poll of the session: the anchors poison everything downstream if
                // they start from a corrupted transfer, so demand a second agreeing read.
                uint32_t confirm;
                if (!readRtcSeconds(&confirm) || (confirm >= rtc ? confirm - rtc : rtc - confirm) > 2)
                    return; // glitched read; try again next frame

                sSessionRtc = rtc;
                sSessionFrames = *counter;

                uint32_t *wallStart = reinterpret_cast<uint32_t *>(kWallStartAddr);
                if (*wallStart == 0)
                    *wallStart = rtc; // first bind: anchor the run's wall clock
            }

            // A clock reading from before the seed was even generated is impossible: catches
            // Custom RTC set to the past and host clock rollback. The 2-day slack covers
            // timezone differences (the emulated RTC ticks in local time) and SRAM bias.
            const uint32_t patchTime = gState->apSettings->patchTimeGC;
            if (patchTime != 0 && rtc + (2 * 86400) < patchTime)
                *reinterpret_cast<uint8_t *>(kRunDirtyFlagAddr) |= kDirtyClockBeforeGen;

            sLastRtc = rtc;
            sLastFrames = *counter;
            sNextPollFrames = *counter + 60;
            if (sSeqChangedSinceLastPoll)
                sGracePollsRemaining = kGracePolls;
            else if (sGracePollsRemaining != 0)
                sGracePollsRemaining--;
            sSeqChangedSinceLastPoll = false;
            sExiBusySinceLastPoll = false;
            sDiscReadSinceLastPoll = false;
        }

        // Latch the elapsed wall time the moment the ending is reached so the credits
        // display (and the verification code hashed from it) can no longer move.
        void finalizeWallClock()
        {
            uint32_t *finish = reinterpret_cast<uint32_t *>(kWallAtFinishAddr);
            if (*finish != 0)
                return;

            const uint32_t start = *reinterpret_cast<uint32_t *>(kWallStartAddr);
            uint32_t rtc;
            if (start == 0 || !readRtcSeconds(&rtc))
                return; // retry next frame

            const uint32_t elapsed = rtc - start;
            *finish = elapsed != 0 ? elapsed : 1;
        }
    } // namespace

    KEEP_FUNC int32_t psndBGMOn_f_d_Hook(uint32_t flags, const char *name, uint32_t a3, uint32_t a4, uint32_t a5)
    {
        if (gState && gState->apSettings && gState->apSettings->music == 1 && name)
        {
            const bool jingle = strncmp(name, "BGM_FF_", 7) == 0 || strcmp(name, "BGM_BATTLE_WIN1") == 0 ||
                                strcmp(name, "BGM_BATTLE_WIN2") == 0 || strcmp(name, "BGM_BATTLE_LOSE1") == 0;
            if (!jingle)
                return -1;
        }
        return g_psndBGMOn_f_d_trampoline(flags, name, a3, a4, a5);
    }

    // Every disc access - sync and async alike - funnels through DVDMgrOpen (fileAsync
    // and _fileAlloc both open through it), so this is the one place to learn that the
    // game is loading something regardless of whether the sequence changes.
    KEEP_FUNC ttyd::dvdmgr::DvdMgrFile *DVDMgrOpenHook(const char *path, int priority, uint16_t wZero)
    {
        sDiscReadSinceLastPoll = true;
        return g_DVDMgrOpen_trampoline(path, priority, wZero);
    }

    void updateRtaTimer()
    {
        updateSaveFileActive();
        if (!sSaveFileActive)
            return; // logo/title/file select/attract demo - no save loaded

        // Seed binding and cross-seed detection stay on even when the timer feature is
        // gated off; the client's dirty-reason bits rely on the same flag either way.
        maintainRunIntegrity();

        if (gState->apSettings->rtaTimer == 0)
            return; // timer/credits feature disabled for this seed (mod-side gate, not a yaml option)

        // Track sequence transitions so the wall-clock monitor can tell loading-screen
        // stalls (map changes, battle entry, game over) apart from savestates/pauses.
        // The next-seq request changes before the current seq does, and the DVD stall
        // can happen entirely inside that window, so watch both.
        static SeqIndex sPrevSeq = SeqIndex::kLogo;
        static SeqIndex sPrevNextSeq = SeqIndex::kLogo;
        const SeqIndex seq = seqGetSeq();
        const SeqIndex nextSeq = seqGetNextSeq();
        if (seq != sPrevSeq || nextSeq != sPrevNextSeq)
        {
            sSeqChangedSinceLastPoll = true;
            sPrevSeq = seq;
            sPrevNextSeq = nextSeq;
        }

        if (inEndingArea())
        {
            finalizeWallClock();
            return; // goal reached; hold the final time for the credits display
        }

        uint32_t *frames = reinterpret_cast<uint32_t *>(kRtaFrameCounterAddr);
        (*frames)++;
        monitorWallClock(*frames);
    }

    KEEP_FUNC void MarioEntryHook()
    {
        if (sPaperModeRestorePending)
        {
            sPaperModeRestorePending = false;

            ttyd::mario::Player *mario = marioGetPtr();
            if (mario->characterId == MarioCharacters::kMario)
            {
                using ttyd::mario_motion::MarioMotion;
                switch (ttyd::swdrv::swByteGet(kPaperModeSaveGsw))
                {
                    case 1:
                        mario->currentMotionId = MarioMotion::kShip;
                        break;
                    case 2:
                        mario->currentMotionId = MarioMotion::kRoll;
                        break;
                    case 3:
                        mario->currentMotionId = MarioMotion::kSlit;
                        break;
                    default:
                        break;
                }
            }
        }

        g_marioEntry_trampoline();
    }

    void updatePaperModePersistence()
    {
        using ttyd::mario_motion::MarioMotion;

        if (!checkIfInGame())
            return;

        // Interludes play as Peach/Bowser; the mirror doesn't apply
        if (marioGetPtr()->characterId != MarioCharacters::kMario)
            return;

        const MarioMotion motion = marioGetPtr()->currentMotionId;

        if (motion == MarioMotion::kShip)
            ttyd::swdrv::swByteSet(kPaperModeSaveGsw, 1);
        else if (motion == MarioMotion::kRoll)
            ttyd::swdrv::swByteSet(kPaperModeSaveGsw, 2);
        else if (motion == MarioMotion::kSlit)
            ttyd::swdrv::swByteSet(kPaperModeSaveGsw, 3);
        else if (motion == MarioMotion::kPlane)
            ttyd::swdrv::swByteSet(kPaperModeSaveGsw, 4);
        else if (motion == MarioMotion::kStay || motion == MarioMotion::kWalk || motion == MarioMotion::kDash ||
                 motion == MarioMotion::kLand)
        {
            ttyd::swdrv::swByteSet(kPaperModeSaveGsw, 0);
        }
    }

    // Always-on RTA timer, bottom-left corner while a file is active (the credits
    // results panel replaces it once the goal is reached). A dirtied run turns the
    // timer red and names the reasons on the spot, so runners don't waste an attempt
    // only to find out at the credits.
    static void rtaTimerDisp(ttyd::dispdrv::CameraId cameraId, void *user)
    {
        (void)cameraId;
        (void)user;

        gc::gx::GXColor fogColor(0x66, 0x06, 0x42, 0x80);
        gc::gx::GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fogColor);

        const uint32_t frames = *reinterpret_cast<uint32_t *>(kRtaFrameCounterAddr);
        const uint32_t seconds = frames / 60;
        const uint32_t centis = (frames % 60) * 100 / 60;
        const uint8_t dirtyMask = *reinterpret_cast<uint8_t *>(kRunDirtyFlagAddr);

        char text[32];
        snprintf(text, sizeof(text), "%u:%02u:%02u.%02u", static_cast<unsigned int>(seconds / 3600),
                 static_cast<unsigned int>((seconds / 60) % 60), static_cast<unsigned int>(seconds % 60),
                 static_cast<unsigned int>(centis));
        gSelf->DrawString(text, -272.0f, -196.0f, dirtyMask != 0 ? 0xFF5050FF : 0xFFFFFFC8, 0.8f);

        if (dirtyMask != 0)
        {
            static const struct
            {
                uint8_t bit;
                const char *label;
            } kDirtyLabels[] = {
                {0x01, "wrong seed"},     {0x02, "savestate/pause"}, {0x04, "clock"},
                {0x08, "external flags"}, {0x10, "external items"},
            };

            char reasons[96];
            uint32_t pos = snprintf(reasons, sizeof(reasons), "Run invalid: ");
            bool first = true;
            for (uint32_t i = 0; i < sizeof(kDirtyLabels) / sizeof(kDirtyLabels[0]); i++)
            {
                if ((dirtyMask & kDirtyLabels[i].bit) == 0)
                    continue;
                if (pos < sizeof(reasons))
                    pos += snprintf(reasons + pos, sizeof(reasons) - pos, "%s%s", first ? "" : ", ",
                                    kDirtyLabels[i].label);
                first = false;
            }
            gSelf->DrawString(reasons, -272.0f, -176.0f, 0xFF5050FF, 0.62f);
        }
    }

    // FNV-1a (32-bit) used for the credits verification code. Keep in sync with
    // the validator in the apworld's verification.py.
    static uint32_t fnv1a32(const void *data, uint32_t len, uint32_t hash)
    {
        const uint8_t *p = static_cast<const uint8_t *>(data);
        for (uint32_t i = 0; i < len; i++)
        {
            hash ^= p[i];
            hash *= 0x01000193u;
        }
        return hash;
    }

    static void creditsResultsDisp(ttyd::dispdrv::CameraId cameraId, void *user)
    {
        (void)cameraId;
        (void)user;

        // Disable fog so the text renders with clean colors (toast/numeric window pattern)
        gc::gx::GXColor fogColor(0x66, 0x06, 0x42, 0x80);
        gc::gx::GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fogColor);

        const uint32_t frames = *reinterpret_cast<uint32_t *>(kRtaFrameCounterAddr);
        const uint32_t seconds = frames / 60;
        const uint32_t centis = (frames % 60) * 100 / 60;

        // The seed is only ever decoded here, once the run is over, and only onto the stack
        char seed[17];
        const uint8_t *encoded = reinterpret_cast<const uint8_t *>(kSeedNameAddr);
        for (uint32_t i = 0; i < 16; i++)
            seed[i] = static_cast<char>(encoded[i] ^ kSeedObfuscationKey[i]);
        seed[16] = '\0';

        // A dirtied run shows the reason bitmask after the time (decoded by verification.py)
        const uint8_t dirtyMask = *reinterpret_cast<uint8_t *>(kRunDirtyFlagAddr);
        char dirtyMark[8] = "";
        if (dirtyMask != 0)
            snprintf(dirtyMark, sizeof(dirtyMark), " *%X", dirtyMask);

        char timeLine[40];
        char seedLine[40];
        snprintf(timeLine, sizeof(timeLine), "Time: %u:%02u:%02u.%02u%s", static_cast<unsigned int>(seconds / 3600),
                 static_cast<unsigned int>((seconds / 60) % 60), static_cast<unsigned int>(seconds % 60),
                 static_cast<unsigned int>(centis), dirtyMark);
        snprintf(seedLine, sizeof(seedLine), "Seed: %s", seed);

        // Verification code: the frame count and dirty mask are the payload, authenticated
        // by a keyed MAC over "payload|seed", then whitened by XORing with a keystream
        // derived from the MAC - so the code alone carries everything needed to verify a
        // run, while similar runs produce entirely unrelated codes (no visible structure).
        // Validated by verification.py / the website race leaderboards.
        const uint32_t payloadFrames = frames > 0xFFFFFF ? 0xFFFFFF : frames; // 6 hex digits, ~77h cap
        const uint32_t payload = (payloadFrames << 8) | dirtyMask;

        char scratch[32];
        snprintf(scratch, sizeof(scratch), "%08X|%s", static_cast<unsigned int>(payload), seed);
        uint32_t msgLen = 0;
        while (scratch[msgLen] != '\0')
            msgLen++;
        const uint32_t mac =
            fnv1a32(scratch, msgLen, fnv1a32(kSeedObfuscationKey, sizeof(kSeedObfuscationKey), 0x811C9DC5u));

        snprintf(scratch, sizeof(scratch), "%08X|%s", static_cast<unsigned int>(mac), seed);
        msgLen = 0;
        while (scratch[msgLen] != '\0')
            msgLen++;
        const uint32_t mask =
            fnv1a32(scratch, msgLen, fnv1a32(kSeedObfuscationKey, sizeof(kSeedObfuscationKey), 0xCBF29CE4u));
        const uint32_t whitened = payload ^ mask;

        char codeLine[40];
        snprintf(codeLine, sizeof(codeLine), "Code: %04X-%04X-%04X-%04X", static_cast<unsigned int>(whitened >> 16),
                 static_cast<unsigned int>(whitened & 0xFFFF), static_cast<unsigned int>(mac >> 16),
                 static_cast<unsigned int>(mac & 0xFFFF));

        gSelf->DrawString(timeLine, -272.0f, -128.0f, 0xFFFFFFFF, 0.9f);
        gSelf->DrawString(seedLine, -272.0f, -154.0f, 0xFFFFFFFF, 0.9f);
        gSelf->DrawString(codeLine, -272.0f, -180.0f, 0xFFFFFFFF, 0.9f);
    }

    KEEP_FUNC bool OSLinkHook(OSModuleInfo *new_module, void *bss)
    {
        bool result = g_OSLink_trampoline(new_module, bss);
        if (new_module != nullptr && result)
        {
            gSelf->OnModuleLoaded(new_module);
            mod::mirror::SyncCullingTable(new_module->id);
            mod::tracker::trackerClearPanelTints();
        }
        return result;
    }

    void setFirstVisitSW(const char *map)
    {
        if (strncmp(map, "gor", 3) == 0)
            ttyd::swdrv::swSet(6300);
        else if (strncmp(map, "tik", 3) == 0)
            ttyd::swdrv::swSet(6301);
        else if (strcmp(map, "hei_00") == 0)
            ttyd::swdrv::swSet(6302);
        else if (strncmp(map, "nok", 3) == 0)
            ttyd::swdrv::swSet(6303);
        else if (strncmp(map, "gon", 3) == 0)
            ttyd::swdrv::swSet(6304);
        else if (strncmp(map, "win", 3) == 0 && strcmp(map, "win_04") != 0 && strcmp(map, "win_05") != 0)
            ttyd::swdrv::swSet(6305);
        else if (strncmp(map, "mri", 3) == 0)
            ttyd::swdrv::swSet(6306);
        else if (!ttyd::mario::marioCtrlOffChk() && // marioCtrlOffChk because of flurries missing necklace cutscene
                 strcmp(map, "win_04") == 0)
            ttyd::swdrv::swSet(6307);
        else if (strncmp(map, "tou", 3) == 0)
            ttyd::swdrv::swSet(6308);
        else if (strncmp(map, "usu", 3) == 0)
            ttyd::swdrv::swSet(6309);
        else if (strcmp(map, "gra_00") == 0)
            ttyd::swdrv::swSet(6310);
        else if (strcmp(map, "gra_06") == 0)
            ttyd::swdrv::swSet(6311);
        else if (strcmp(map, "muj_05") == 0 || strncmp(map, "dou", 3) == 0)
            ttyd::swdrv::swSet(6313);
        else if (strncmp(map, "muj", 3) == 0)
            ttyd::swdrv::swSet(6312);
        else if (strncmp(map, "hom", 3) == 0)
            ttyd::swdrv::swSet(6314);
        else if (strcmp(map, "pik_00") == 0 || strcmp(map, "pik_01") == 0)
            ttyd::swdrv::swSet(6315);
        else if (strcmp(map, "pik_02") == 0)
            ttyd::swdrv::swSet(6316);
        else if (strcmp(map, "bom_01") == 0 || strcmp(map, "bom_02") == 0)
            ttyd::swdrv::swSet(6317);
        else if (strncmp(map, "moo", 3) == 0)
            ttyd::swdrv::swSet(6318);
        else if (strncmp(map, "aji", 3) == 0)
            ttyd::swdrv::swSet(6319);
        else if (strcmp(map, "las_09") == 0)
            ttyd::swdrv::swSet(6322);
        else if (strncmp(map, "las", 3) == 0)
            ttyd::swdrv::swSet(6320);
        else if (strncmp(map, "rsh", 3) == 0)
            ttyd::swdrv::swSet(6321);
    }

    // Get destination map and bero given source map and bero
    // Returns true if found, false if not found
    KEEP_FUNC bool getDestination(EntranceData *entranceData,
                                  size_t count,
                                  const char *srcMap,
                                  const char *srcBero,
                                  const char **outDestMap,
                                  const char **outDestBero)
    {
        if (!srcBero)
        {
            srcBero = "null";
        }

        if (!entranceData || !srcMap || !outDestMap || !outDestBero || gState->fastTraveling || !gState->apSettings->inGame)
        {
            return false;
        }

        // If srcMap starts with "rsh", only match on the first 6 characters
        char mapBuf[16];
        const char *mapToCompare = srcMap;
        if (strncmp(srcMap, "rsh", 3) == 0 && strlen(srcMap) > 6)
        {
            strncpy(mapBuf, srcMap, 6);
            mapBuf[6] = '\0';
            mapToCompare = mapBuf;
        }

        // Convert "null" to empty string for comparison
        const char *searchBero = (strcmp(srcBero, "null") == 0) ? "" : srcBero;

        for (size_t i = 0; i < count; ++i)
        {
            // Check if srcMap matches
            if (strcmp(entranceData[i].srcMap, mapToCompare) != 0)
                continue;

            // Check if srcBero matches (treating "null" as "")
            const char *entrySrcBero = (strcmp(entranceData[i].srcBero, "null") == 0) ? "" : entranceData[i].srcBero;
            if (strcmp(entrySrcBero, searchBero) != 0)
                continue;

            // Found a match
            *outDestMap = entranceData[i].destMap;

            // Convert "None" to empty string for destBero output
            if (strcmp(entranceData[i].destBero, "null") == 0)
            {
                *outDestBero = nullptr;
            }
            else
            {
                *outDestBero = entranceData[i].destBero;
            }

            return true;
        }
        return false;
    }

    KEEP_FUNC void seqSetSeqHook(SeqIndex seq, const char *map, const char *bero)
    {
        if (seq == SeqIndex::kGameOver && !gState->firstDeath)
        {
            gState->apSettings->deathLinkSent = 1;
            gState->firstDeath = true;
        }
        if (seq == SeqIndex::kTitle && gState->firstDeath)
            gState->firstDeath = false;

        // Clear starstone item pointers to avoid issues
        gState->starItemPtr = nullptr;
        gState->starstoneName = nullptr;
        gState->starstoneFunctionPtr = nullptr;

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

        const char *destMap = nullptr;
        const char *destBero = nullptr;
        if (getDestination(gState->entranceData, gState->entranceDataCount, map, bero, &destMap, &destBero))
        {
            map = destMap;
            bero = destBero;
        }

        gState->fastTraveling = false;

        // Once westside has been reached her gate block is redundant: Zess T. moves
        // to guard her kitchen door instead (gor_01 nancy patches). Cooking still
        // requires delivering the Contact Lens (GSWF 1188 stays untouched).
        if (strcmp(map, "gor_03") == 0)
        {
            ttyd::swdrv::swSet(6124);
        }

        // Set cutscene flag for Don Pianta if player leaves westside
        if (ttyd::swdrv::swByteGet(1709) >= 3)
            ttyd::swdrv::swSet(6000);

        // Advance Punio tik sequence if warped out of early
        if (ttyd::swdrv::swByteGet(1718) >= 3 && strncmp(map, "win", 3) == 0)
            ttyd::swdrv::swByteSet(1718, 5);

        // Enable post chapter 2 checks right when the chapter is beaten
        if (ttyd::swdrv::swByteGet(1713) >= 11 && strncmp(map, "mri", 3) == 0)
            ttyd::swdrv::swSet(2884);

        // Advance Ch.5 prologue if post-cortez
        if (ttyd::swdrv::swByteGet(1717) >= 10 && ttyd::swdrv::swByteGet(1705) < 7 && strncmp(map, "muj", 3) != 0 &&
            strncmp(map, "dou", 3) != 0)
            ttyd::swdrv::swByteSet(1705, 11);

        const ttyd::mario::Player *playerPtr = ttyd::mario::marioGetPtr();
        if (strncmp(map, "tou", 3) == 0 && ttyd::swdrv::swGet(6075) == 1 &&
            playerPtr->prevFollowerId == ttyd::party::PartyMembers::kEgg)
            ttyd::party::partyKill2(1);

        if (gState->apSettings->cutsceneSkip)
        {
            if (ttyd::swdrv::swByteGet(1711) == 1) // Hooktail's Castle Intro
                ttyd::swdrv::swByteSet(1711, 3);

            if (ttyd::swdrv::swByteGet(1718) == 3) // Rougeport Sewers Punio Post Passageway Opened
                ttyd::swdrv::swByteSet(1718, 5);

            if (ttyd::swdrv::swByteGet(1713) == 3) // The Great Tree Ms. Mowz Knocks Out X-Naut
                ttyd::swdrv::swByteSet(1713, 4);

            if (ttyd::swdrv::swGet(2400) == 1 && ttyd::swdrv::swByteGet(1703) == 12) // Glitz Pit Rawk Hawk Defeated
                ttyd::swdrv::swByteSet(1703, 14);

            if (ttyd::swdrv::swByteGet(1703) == 15) // Glitz Pit Air Vent (Champ's Room -> Grubba's Office)
                ttyd::swdrv::swByteSet(1703, 17);

            if (ttyd::swdrv::swByteGet(1703) == 18) // Glitz Pit Finding Grubba's Paper in Desk
                ttyd::swdrv::swByteSet(1703, 19);

            if (ttyd::swdrv::swByteGet(1714) == 5) // Creepy Steeple Intro
                ttyd::swdrv::swByteSet(1714, 6);

            if (ttyd::swdrv::swByteGet(1719) == 2) // Keelhaul Key After Saving Bobbery from Tree
                ttyd::swdrv::swByteSet(1719, 3);

            if (ttyd::swdrv::swByteGet(1717) == 1) // Pirate's Grotto Post Entry
                ttyd::swdrv::swByteSet(1717, 3);

            if (ttyd::swdrv::swByteGet(1717) == 7) // Pirate's Grotto Pre-Cortez
                ttyd::swdrv::swByteSet(1717, 9);

            if (ttyd::swdrv::swByteGet(1709) == 5) // Keelhaul Key Frankie's 100 I Love Yous
                ttyd::swdrv::swByteSet(1709, 6);

            if (ttyd::swdrv::swByteGet(1709) == 10) // Rougeport Westside Post Train Ticket Check
                ttyd::swdrv::swByteSet(1709, 11);

            if (ttyd::swdrv::swByteGet(1706) == 2) // Excess Express Long Intro
                ttyd::swdrv::swByteSet(1706, 6);

            if (ttyd::swdrv::swByteGet(1706) == 7) // Excess Express Talking to Pennington
                ttyd::swdrv::swByteSet(1706, 10);

            if (ttyd::swdrv::swByteGet(1706) == 11) // Excess Express Agreed to Help Bub the Bob-omb
                ttyd::swdrv::swByteSet(1706, 12);

            if (ttyd::swdrv::swByteGet(1706) == 15) // Excess Express Agreed to Help the Conductor
                ttyd::swdrv::swByteSet(1706, 18);

            if (ttyd::swdrv::swByteGet(1706) == 23) // Excess Express Talked to Pennington & Ratooey Businessman
                ttyd::swdrv::swByteSet(1706, 24);

            if (ttyd::swdrv::swByteGet(1706) == 26) // Excess Express Examined Paper in Cabin 005
                ttyd::swdrv::swByteSet(1706, 27);

            if (ttyd::swdrv::swByteGet(1720) == 4) // Riverside Station Seeing Smorgs
                ttyd::swdrv::swByteSet(1720, 5);

            if (ttyd::swdrv::swByteGet(1706) == 33) // Excess Express Pre-Smorg
                ttyd::swdrv::swByteSet(1706, 35);

            if (ttyd::swdrv::swByteGet(1706) == 38) // Poshley Heights Pre-Garnet Star
                ttyd::swdrv::swByteSet(1706, 40);

            if (ttyd::swdrv::swByteGet(1707) == 7) // X-Naut Fortress Entry
                ttyd::swdrv::swByteSet(1707, 8);

            if (ttyd::swdrv::swByteGet(1708) == 5) // Riddle Tower Intro
                ttyd::swdrv::swByteSet(1708, 6);

            if (ttyd::swdrv::swByteGet(1708) == 14) // Palace of Shadow Pre-Grodus
                ttyd::swdrv::swByteSet(1708, 15);
        }

        // Update the map name if entering the pit with a checkpoint
        if (strcmp(ttyd::seq_mapchange::_next_area, "tik") == 0 && strncmp(map, "jon", 3) == 0)
        {
            switch (ttyd::swdrv::swByteGet(1321))
            {
                case 0: // No checkpoint
                    break;
                case 9:
                    map = "jon_03";
                    break;
                case 19:
                    map = "jon_03";
                    break;
                case 29:
                    map = "jon_03";
                    break;
                case 39:
                    map = "jon_03";
                    break;
                case 49:
                    map = "jon_04";
                    break;
                case 59:
                    map = "jon_04";
                    break;
                case 69:
                    map = "jon_04";
                    break;
                case 79:
                    map = "jon_05";
                    break;
                case 89:
                    map = "jon_05";
                    break;
                case 99:
                    map = "jon_06";
                    break;
                default:
                    break;
            }
        }

        if (strncmp(map, "mri", 3) == 0)
        {
            PartyEntry *followerPtr = partyGetPtr(PartySlotId::kFollower);

            if (followerPtr)
            {
                if (followerPtr->currentMemberId != PartyMembers::kPunio)
                    spawnPartnerOrFollower(PartyMembers::kPunio);
            }
            else
            {
                spawnPartnerOrFollower(PartyMembers::kPunio);
            }
        }

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
        else if (strcmp(map, "rsh_03_a") == 0)
        {
            uint8_t value = ttyd::swdrv::swByteGet(1706);

            if ((value >= 8 && value < 14) || (value >= 29 && value < 30))
                map = "rsh_03_b";
            else if ((value >= 14 && value < 22) || (value >= 30 && value < 31))
                map = "rsh_03_c";
        }
        else if (strcmp(map, "rsh_04_a") == 0)
        {
            uint8_t value = ttyd::swdrv::swByteGet(1706);

            if ((value >= 8 && value < 14) || (value >= 29 && value < 30))
                map = "rsh_04_b";
            else if ((value >= 14 && value < 22) || (value >= 30 && value < 31))
                map = "rsh_04_c";
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

    // runs after map changes
    KEEP_FUNC void seq_gameInitHook(SeqInfo *info)
    {
        // only visit a location if you are mario
        if (marioGetPtr()->characterId == MarioCharacters::kMario)
        {
            setFirstVisitSW(ttyd::seq_mapchange::_next_map); // Set GSWF flag on first visit to a map
        }
        g_seq_gameInit_trampoline(info);
    }

    KEEP_FUNC void _load_Hook(const char *mapName, const char *entranceName, const char *beroName)
    {
        g__load_trampoline(mapName, entranceName, beroName);
        for (int i = 8; i < 16; i++) gState->state_msgWork[i] = 0;
        ttyd::msgdrv::msgLoad("mod", 2);
        ttyd::msgdrv::msgLoad("desc", 3);
    }

    static BattleUnitKind *g_endScriptKind[0x40];
    static BattleUnitKind *g_powOrigKind[0x40];
    static BattleUnitKind *g_currentBossOrigKind;

    struct OrigKindEntry
    {
        BattleUnitSetup *setup;
        BattleUnitKind *kind;
        int32_t maxHp;
        int32_t level;
        bool boss;
    };

    // Sized for the Pit (jon): 150 groups x up to 5 slots, plus boss arenas.
    static constexpr int32_t kOrigKindMapCapacity = 1024;
    static OrigKindEntry g_origKindMap[kOrigKindMapCapacity];
    static int32_t g_origKindMapCount;

    // Setup pointers live inside the area rel, so entries go stale (and could
    // alias another area's setups at the same address) once it unloads.
    static void ResetOriginalKindMap()
    {
        g_origKindMapCount = 0;
    }

    KEEP_FUNC void RegisterOriginalKind(BattleUnitSetup *setup, BattleUnitKind *orig, bool isBoss)
    {
        if (!setup || !orig)
            return;
        for (int32_t i = 0; i < g_origKindMapCount; i++)
            if (g_origKindMap[i].setup == setup)
            {
                g_origKindMap[i].kind = orig;
                g_origKindMap[i].maxHp = orig->max_hp;
                g_origKindMap[i].level = orig->level;
                g_origKindMap[i].boss = isBoss;
                return;
            }
        if (g_origKindMapCount < kOrigKindMapCapacity)
        {
            g_origKindMap[g_origKindMapCount].setup = setup;
            g_origKindMap[g_origKindMapCount].kind = orig;
            g_origKindMap[g_origKindMapCount].maxHp = orig->max_hp;
            g_origKindMap[g_origKindMapCount].level = orig->level;
            g_origKindMap[g_origKindMapCount].boss = isBoss;
            g_origKindMapCount++;
        }
    }

    static OrigKindEntry *LookupOriginalEntry(BattleUnitSetup *setup)
    {
        for (int32_t i = 0; i < g_origKindMapCount; i++)
            if (g_origKindMap[i].setup == setup)
                return &g_origKindMap[i];
        return nullptr;
    }

    static void *GetData_FromTable(DataTableEntry *table, int32_t id)
    {
        if (!table)
            return nullptr;
        for (; table->id != 0; table++)
            if (static_cast<int32_t>(table->id) == id)
                return table->data;
        return nullptr;
    }

    static bool IsBossHpScaleExcluded(int32_t unitType)
    {
        switch (unitType)
        {
            case 0x09: // gesso_left_arm
            case 0x0A: // gesso_right_arm
            case 0x23: // rocket_punch
            case 0x5F: // cortez_claw
            case 0x60: // cortez_rapier
            case 0x61: // cortez_sword
            case 0x62: // cortez_saber
            case 0x64: // gundan_zako
            case 0x65: // gundan_zako
            case 0x66: // gundan_zako
            case 0x6C: // moamoa_tentacle
            case 0x6D: // moamoa_tentacle
            case 0x6E: // moamoa_tentacle
            case 0x6F: // moamoa_mouth
            case 0x7A: // rocket_punch_mkII
            case 0x95: // SQ hand
            case 0x97: // SQ hand
            case 0x98: // SQ hand
                return true;
            default:
                return false;
        }
    }

    static bool IsBossDefScaleExcluded(int32_t unitType)
    {
        switch (unitType)
        {
            case 0x09: // gesso_left_arm
            case 0x0A: // gesso_right_arm
            case 0x23: // rocket_punch
            case 0x5F: // cortez_claw
            case 0x60: // cortez_rapier
            case 0x61: // cortez_sword
            case 0x62: // cortez_saber
            case 0x64: // gundan_zako
            case 0x65: // gundan_zako
            case 0x66: // gundan_zako
            case 0x6C: // moamoa_tentacle
            case 0x6D: // moamoa_tentacle
            case 0x6E: // moamoa_tentacle
            case 0x6F: // moamoa_mouth
            case 0x7A: // rocket_punch_mkII
            case 0x93: // batten_satellite
            case 0x95: // SQ hand
            case 0x97: // SQ hand
            case 0x98: // SQ hand
                return true;
            default:
                return false;
        }
    }

    static bool IsBossPowScaleExcluded(int32_t unitType)
    {
        switch (unitType)
        {
            default:
                return false;
        }
    }

    static constexpr int32_t kHpScaleBlacklist[] = {
        BattleUnitType::MINI_YUX,   // 0x1E
        BattleUnitType::MINI_Z_YUX, // 0x74
        BattleUnitType::MINI_X_YUX, // 0x76
    };

    static bool IsHpScaleBlacklisted(int32_t unitType)
    {
        constexpr int32_t count = sizeof(kHpScaleBlacklist) / sizeof(kHpScaleBlacklist[0]);
        for (int32_t i = 0; i < count; i++)
        {
            if (kHpScaleBlacklist[i] == unitType)
                return true;
        }
        return false;
    }

    static constexpr int32_t kEnemyPowScaleBlacklist[] = {
        0x46, // twinkling_pansy (Amazy Dayzee)
    };

    static bool IsEnemyPowScaleBlacklisted(int32_t unitType)
    {
        constexpr int32_t count = sizeof(kEnemyPowScaleBlacklist) / sizeof(kEnemyPowScaleBlacklist[0]);
        for (int32_t i = 0; i < count; i++)
        {
            if (kEnemyPowScaleBlacklist[i] == unitType)
                return true;
        }
        return false;
    }

    static void ApplyBossScriptPatches(int32_t unitType, int32_t scaledHp)
    {
        switch (unitType)
        {
            case 0x14: // gold_chorobon
                GoldChorobonPatches(scaledHp);
                break;
            case 0x17: // gonbaba (Hooktail)
                HooktailPatches(scaledHp);
                break;
            case 0x22: // magnum_battender
                MagnumBattenderPatches(scaledHp);
                break;
            case 0x4F: // faker_mario
                FakerMarioPatches(scaledHp);
                break;
            case 0x63: // kanbu3
                Kanbu3Patches(scaledHp);
                break;
            case 0x79: // magnum_battender_mkII
                MagnumBattenderMKIIPatches(scaledHp);
                break;
            default:
                break;
        }
    }

    KEEP_FUNC BattleWorkUnit *BtlUnit_Entry_Hook(BattleUnitSetup *setup)
    {
        BattleUnitKind *kind = setup->unit_kind_params;
        if (setup)
        {
            if (kind && kind->unit_type == BattleUnitType::SYSTEM)
            {
                mod::vm::VM_UnlockAll(); // SYSTEM is first unit each battle: drop last fight's pins
                for (auto &k : g_endScriptKind) k = nullptr;
                for (auto &k : g_powOrigKind) k = nullptr;
                g_currentBossOrigKind = nullptr;
            }
            mod::vm::VM_PrefetchForKind(reinterpret_cast<uint32_t>(kind), true);
        }

        OrigKindEntry *origEntry = LookupOriginalEntry(setup);
        BattleUnitKind *bossOrigKind = (origEntry && origEntry->boss) ? origEntry->kind : nullptr;
        BattleUnitKind *enemyOrigKind = (origEntry && !origEntry->boss) ? origEntry->kind : nullptr;
        if (bossOrigKind)
            g_currentBossOrigKind = bossOrigKind;
        else if (!origEntry && g_currentBossOrigKind && kind && kind->unit_type <= BattleUnitType::BONETAIL)
            bossOrigKind = g_currentBossOrigKind;

        // A unit standing in its own vanilla slot needs no scaling at all - skipping it
        // keeps its untouched vanilla stats regardless of what earlier fights may have
        // written into the shared kind structs.
        const bool vanillaPlacement = origEntry && origEntry->kind && setup && setup->unit_kind_params &&
                                      origEntry->kind->unit_type == setup->unit_kind_params->unit_type;

        const OSModuleInfo *relPtr = _globalWorkPtr->relocationBase;
        if (relPtr && setup && setup->unit_kind_params)
        {
            RelId currentRel = static_cast<RelId>(relPtr->id);
            if (bossOrigKind)
            {
                // Boss scaling only means anything when bosses can actually move: with the
                // boss randomizer off, every boss is vanilla and must keep vanilla stats.
                if (gState->apSettings->bossStatScaling && gState->apSettings->bossRandomizer && !vanillaPlacement)
                {
                    // Stats come from the registration-time snapshots when this slot has an
                    // entry (guaranteed-vanilla values); the pointer fallback only serves
                    // event-spawned extra units, which are HP/DEF-excluded below anyway.
                    const int32_t origHp = origEntry ? origEntry->maxHp : bossOrigKind->max_hp;
                    const int32_t origLevel = origEntry ? origEntry->level : bossOrigKind->level;

                    BattleUnitKind *newKind = setup->unit_kind_params;
                    if (!IsBossHpScaleExcluded(newKind->unit_type))
                    {
                        newKind->max_hp = origHp;
                        if (newKind->unit_type == 0x93) // batten_satellite
                        {
                            newKind->max_hp = 2; // Small nerf for early game beatability
                        }
                        else
                        {
                            newKind->level = origLevel;
                        }
                        if (gState->apSettings->bossScalingNerfs &&
                            (newKind->unit_type == 0x5D || newKind->unit_type == 0x5E) && // boss_cortez / boss_honeduka
                            !(bossOrigKind->unit_type >= 0x5D && bossOrigKind->unit_type <= 0x62))
                            newKind->max_hp = (origHp + 2) / 3;
                    }
                    if (!IsBossDefScaleExcluded(newKind->unit_type) && newKind->parts && bossOrigKind->parts)
                    {
                        int32_t partCount = newKind->num_parts < bossOrigKind->num_parts ? newKind->num_parts : bossOrigKind->num_parts;
                        for (int32_t i = 0; i < partCount; i++) newKind->parts[i].defense = bossOrigKind->parts[i].defense;
                    }
                    ApplyBossScriptPatches(newKind->unit_type, origHp);
                }
            }
            else if (enemyOrigKind && gState->apSettings->enemyRandomizer && gState->apSettings->enemyStatScaling &&
                     !IsHpScaleBlacklisted(setup->unit_kind_params->unit_type))
            {
                // Enemy scaling mirrors boss scaling: the replacement fights with the
                // vanilla HP, DEF and level of the enemy it replaced.
                BattleUnitKind *newKind = setup->unit_kind_params;
                newKind->max_hp = enemyOrigKind->max_hp;
                newKind->level = enemyOrigKind->level;
                if (newKind->parts && enemyOrigKind->parts)
                {
                    int32_t partCount =
                        newKind->num_parts < enemyOrigKind->num_parts ? newKind->num_parts : enemyOrigKind->num_parts;
                    for (int32_t i = 0; i < partCount; i++) newKind->parts[i].defense = enemyOrigKind->parts[i].defense;
                }
            }
            else
            {
                ScaleUnitStats(setup->unit_kind_params, currentRel);
            }
        }

        const char *mapName = GetBossMsgMap(kind->unit_type);
        if (mapName)
        {
            ttyd::msgdrv::msgLoad(mapName, 15);
        }

        BattleWorkUnit *entered = g_BtlUnit_Entry_trampoline(setup);
        if (entered)
        {
            int32_t slot = entered->unit_id & 0x3F;
            g_endScriptKind[slot] = (origEntry && origEntry->boss) ? origEntry->kind : nullptr;
            g_powOrigKind[slot] = nullptr;
            if (kind && !IsBossPowScaleExcluded(kind->unit_type))
            {
                if (bossOrigKind && gState->apSettings->bossStatScaling && gState->apSettings->bossRandomizer)
                    g_powOrigKind[slot] = bossOrigKind;
                else if (enemyOrigKind && gState->apSettings->enemyRandomizer && gState->apSettings->enemyStatScaling &&
                         !IsEnemyPowScaleBlacklisted(kind->unit_type))
                    g_powOrigKind[slot] = enemyOrigKind;
            }
        }
        return entered;
    }

    KEEP_FUNC void ExecAllUnitBattleEndEvent_Hook()
    {
        void *bw = ttyd::battle::_battleWorkPtr;
        for (int32_t i = 0; i < 0x40; i++)
        {
            BattleWorkUnit *u = reinterpret_cast<BattleWorkUnit *>(ttyd::battle::BattleGetUnitPtr(bw, i));
            if (!u)
                continue;
            BattleUnitKind *orig = g_endScriptKind[i];
            void *evt = orig ? GetData_FromTable(orig->data_table, 0x3F) : ttyd::battle_unit::BtlUnit_GetData(u, 0x3F);
            if (!evt)
                continue;
            ttyd::evtmgr::EvtEntry *th = ttyd::evtmgr::evtEntry(evt, 0xa, 0);
            *reinterpret_cast<uint32_t *>(reinterpret_cast<char *>(th) + 0x160) = u->unit_id;
        }
    }
    KEEP_FUNC void ScaleUnitStats(BattleUnitKind *unit, RelId rel)
    {
        if (!unit)
            return;
        if ((gState->apSettings->enemyRandomizer == 0 && gState->apSettings->shuffleChapterStats == 0) ||
            gState->apSettings->enemyStatScaling == 0)
            return;
        const BattleStatRelValues *statRelValues = GetBattleStats(rel);
        BattleUnitKind *unit_kind = GetUnitKindById(unit->unit_type);
        if (!unit_kind || !statRelValues || rel == RelId::JON)
            return;
        if (IsHpScaleBlacklisted(unit->unit_type))
            return;
        unit->max_hp = statRelValues->base_hp;
        unit->level = statRelValues->level;
    }

    KEEP_FUNC int32_t AlterDamageCalculation(BattleWorkUnit *attacker,
                                             BattleWorkUnit *target,
                                             BattleWorkUnitPart *target_part,
                                             BattleWeapon *weapon,
                                             uint32_t *unk0,
                                             uint32_t unk1)
    {
        int32_t base_atk = weapon ? static_cast<int32_t>(weapon->damage_function_params[0]) : 0;
        bool overrode = false;
        // g_powOrigKind slots are only populated when the matching stat-scaling
        // option (boss or enemy) is enabled, so no settings check is needed here.
        if (attacker && weapon && weapon->damage_function && !weapon->item_id &&
            !(weapon->target_property_flags & 0x100000) && base_atk > 0)
        {
            BattleUnitKind *origKind = g_powOrigKind[attacker->unit_id & 0x3F];
            if (origKind)
            {
                int32_t atk = GetBossAtk(origKind->unit_type);
                if (atk < 0)
                    atk = GetEnemyAtk(origKind->unit_type);
                if (atk >= 0)
                {
                    if (atk < 1)
                        atk = 1;
                    if (atk > 99)
                        atk = 99;
                    weapon->damage_function_params[0] = static_cast<uint32_t>(atk);
                    overrode = true;
                }
            }
        }

        int32_t damage = g_BattleCalculateDamage_trampoline(attacker, target, target_part, weapon, unk0, unk1);

        if (overrode)
            weapon->damage_function_params[0] = static_cast<uint32_t>(base_atk);
        return damage;
    }

    KEEP_FUNC int32_t InterruptStopHook(ttyd::evtmgr::EvtEntry *evt, bool isFirstCall)
    {
        if (isFirstCall && gState->apSettings->bossRandomizer && ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]) == 1)
        {
            const char *currentMap = mod::common::GetCurrentMap();
            if (!currentMap || strcmp(currentMap, "las_29") != 0)
            {
                void *bw = ttyd::battle::_battleWorkPtr;
                for (int32_t i = 0; i < 0x40; i++)
                {
                    BattleWorkUnit *u = reinterpret_cast<BattleWorkUnit *>(ttyd::battle::BattleGetUnitPtr(bw, i));
                    if (!u || u->current_kind > BattleUnitType::BONETAIL)
                        continue;
                    uint8_t *p = reinterpret_cast<uint8_t *>(u);
                    *reinterpret_cast<int16_t *>(p + 0x10c) = 0;
                    *reinterpret_cast<uint32_t *>(p + 0x104) |= 0x20000;
                }
                *reinterpret_cast<uint32_t *>(reinterpret_cast<uint8_t *>(bw) + 0xef4) |= 0x10;
                return 2;
            }
        }
        return g_InterruptStop_trampoline(evt, isFirstCall);
    }

    KEEP_FUNC int32_t BattleCheckConcludedHook(void *battleWork)
    {
        int32_t concluded = g_BattleCheckConcluded_trampoline(battleWork);
        if (concluded && gState->apSettings->bossRandomizer)
        {
            const char *currentMap = mod::common::GetCurrentMap();
            if (currentMap && strcmp(currentMap, "las_29") == 0)
            {
                bool enemyAlive = false;
                for (int32_t i = 0; i < 0x40; i++)
                {
                    BattleWorkUnit *u = reinterpret_cast<BattleWorkUnit *>(ttyd::battle::BattleGetUnitPtr(battleWork, i));
                    if (!u || u->current_kind > BattleUnitType::BONETAIL)
                        continue;
                    uint8_t *p = reinterpret_cast<uint8_t *>(u);
                    if ((*reinterpret_cast<uint32_t *>(p + 0x104) & 0x20000) == 0 &&
                        *reinterpret_cast<int16_t *>(p + 0x10c) > 0)
                    {
                        enemyAlive = true;
                        break;
                    }
                }
                if (!enemyAlive)
                    *reinterpret_cast<uint32_t *>(reinterpret_cast<uint8_t *>(battleWork) + 0xef4) |= 0x20;
            }
        }
        return concluded;
    }

    static constexpr int32_t kFirstStrikeUnsafe[] = {
        0x54, // Ember
        0x55, // Lava Bubble
        0x7C, // Phantom Ember
    };

    static bool IsFirstStrikeUnsafe(int32_t unitType)
    {
        constexpr int32_t count = sizeof(kFirstStrikeUnsafe) / sizeof(kFirstStrikeUnsafe[0]);
        for (int32_t i = 0; i < count; i++)
        {
            if (kFirstStrikeUnsafe[i] == unitType)
                return true;
        }
        return false;
    }

    KEEP_FUNC void btlseqFirstAct_Hook(void *battleWork)
    {
        if (gState->apSettings->enemyRandomizer)
        {
            uint8_t *info = *reinterpret_cast<uint8_t **>(reinterpret_cast<uint8_t *>(battleWork) + 0x2738);
            int32_t *firstAttackType = info ? reinterpret_cast<int32_t *>(info + 8) : nullptr;
            if (firstAttackType && *firstAttackType >= 9)
            {
                for (int32_t i = 0; i < 0x40; i++)
                {
                    BattleWorkUnit *u = reinterpret_cast<BattleWorkUnit *>(ttyd::battle::BattleGetUnitPtr(battleWork, i));
                    if (!u || *(reinterpret_cast<int8_t *>(u) + 0xC) != 1) // first enemy-alliance unit only
                        continue;
                    if (IsFirstStrikeUnsafe(u->current_kind))
                        *firstAttackType = 0;
                    break;
                }
            }
        }
        g_btlseqFirstAct_trampoline(battleWork);
    }

    KEEP_FUNC int main__psndSFXOnHook(int idOrName, int vol, int pan, int a4, const void *pos, int a6, int a7, int a8)
    {
        const int channel = g_main__psndSFXOn_trampoline(idOrName, vol, pan, a4, pos, a6, a7, a8);
        if (channel != -1)
        {
            const int slot = channel & 0xFF;
            const volatile uint32_t *entry = reinterpret_cast<volatile uint32_t *>(0x803DF988 + slot * 0x28);
            const uint32_t f0 = *entry;
            if (f0 != 0xFFFFFFFFu)
                ghosts::OnLocalSfxFired(static_cast<int>(f0 & 0x1FFF), pos != nullptr, channel);
        }
        return channel;
    }

    KEEP_FUNC int psndSFXOffHook(int channel)
    {
        ghosts::OnLocalSfxStopped(channel);
        return g_psndSFXOff_trampoline(channel);
    }

    // Suppress overworld battles for the duration of a hide-and-seek round.
    // Passing a null battle info leaves the NPC alone but gives it nothing to
    // start a fight with, so touching an enemy while hiding no longer drops you
    // into a battle you cannot escape from while the timer runs.
    //
    // Gated on selfGameRole rather than on "a match exists": the role is only
    // non-zero during HIDE and SEEK, so normal play and the gaps between rounds
    // keep their encounters.
    //
    // Note the single trampoline call. This was disabled by commenting out the
    // guard, which left the unconditional call above it — uncommenting as-is
    // would have called the original twice and set the battle up regardless.
    KEEP_FUNC void npcSetupBattleInfoHook(::NpcEntry *npc, void *info)
    {
        if (ghosts::g_ghostState != nullptr &&
            ghosts::g_ghostState->selfGameRole != ghosts::kGameRoleNone)
        {
            info = nullptr;
        }
        g_npcSetupBattleInfo_trampoline(npc, info);
    }

    static void applyShopFlagLive(int flag)
    {
        char *shopWork = *reinterpret_cast<char **>(0x8041EB60);
        if (shopWork == nullptr)
            return;

        int gswfBase = 6200;
        const char *nextMapPtr = &ttyd::seq_mapchange::_next_map[0];

        for (int i = 0; i < goodsCount; i++)
        {
            if (strncmp(nextMapPtr, goods[i], 6) != 0)
            {
                if (i == goodsCount - 1)
                    return;
                gswfBase += 6;
                continue;
            }
            break;
        }

        int index = flag - gswfBase;
        if (index < 0 || index >= 6)
            return;

        uint32_t *itemIds = *reinterpret_cast<uint32_t **>(shopWork + 0x08);
        uint32_t itemId = itemIds[index * 2];
        switch (gState->apSettings->shopPurchaseLimit)
        {
            case 0: // Infinite
                if (itemId > 125)
                    return;
                break;
            case 1: // Consumables Only
                if (itemId > 125 && itemId < 236)
                    return;
                break;
            case 2: // Badges Only
                if (itemId > 239)
                    return;
                break;
            case 3: // Limited
                break;
            default:
                break;
        }

        uint16_t *itemFlags = reinterpret_cast<uint16_t *>(shopWork + 0x14);
        itemFlags[index] |= 1;
    }

    void DeleteFieldItemForFlag(int flag)
    {
        if (flag <= 0)
            return;

        uint16_t kItemStateGetItem = 2;

        char *work = reinterpret_cast<char *>(0x803dc290);

        int count = *reinterpret_cast<int *>(work + 0x0);
        char *entry = *reinterpret_cast<char **>(work + 0x4);
        if (entry == nullptr)
            return;

        for (int i = 0; i < count; i++, entry += 0x98)
        {
            uint16_t status = *reinterpret_cast<uint16_t *>(entry + 0x0);
            if ((status & 0x1) == 0)
                continue;
            int32_t entryFlag = *reinterpret_cast<int32_t *>(entry + 0x8);
            if (entryFlag != flag)
                continue;
            uint16_t itemState = *reinterpret_cast<uint16_t *>(entry + 0x24);
            if (itemState == kItemStateGetItem)
                return; // local player is picking this up; leave it alone
            ttyd::itemdrv::itemDelete(entry + 0xC);
            return;
        }
    }

    void HandleMobjForFlag(int flag)
    {
        if (flag <= 0)
            return;

        char *header = reinterpret_cast<char *>(0x803D98A8);
        int count = *reinterpret_cast<int *>(header + 0x0);
        char *entry = *reinterpret_cast<char **>(header + 0x4);
        if (entry == nullptr)
            return;

        const int32_t encoded = -130000000 + flag;
        for (int i = 0; i < count; i++, entry += 0x23C)
        {
            if ((*reinterpret_cast<uint32_t *>(entry + 0x0) & 0x1) == 0)
                continue;
            if (*reinterpret_cast<int32_t *>(entry + 0x1E4) != encoded)
                continue;

            const char *model = entry + 0x15;

            if (strncmp(model, "MOBJ_Kururin", 12) == 0)
                return;

            // Shine Sprite box: delete by instance name.
            if (strcmp(model, "MOBJ_PowerUpBlock") == 0)
            {
                ttyd::evt_mobj::mobjDelete(entry + 0x5);
                return;
            }

            const bool isBlock = strstr(model, "Block") != nullptr;
            const int32_t blockState = isBlock ? 0x5A : 0x63;
            if (*reinterpret_cast<int32_t *>(entry + 0x1DC) != blockState)
                *reinterpret_cast<int32_t *>(entry + 0x1DC) = blockState;

            if (isBlock)
            {
                int32_t poseId = *reinterpret_cast<int32_t *>(entry + 0x70);
                char *cur = ttyd::animdrv::animPoseGetCurrentAnim(poseId);
                if (cur != nullptr)
                {
                    int len = strlen(cur);
                    if (len >= 2 && cur[len - 2] == '_' && cur[len - 1] == '1')
                    {
                        char emptyAnim[16];
                        strcpy(emptyAnim, cur);
                        emptyAnim[len - 1] = '2';
                        ttyd::animdrv::animPoseSetAnim(poseId, emptyAnim, 1);
                    }
                }
            }
            return;
        }
    }

    KEEP_FUNC void swSetHook(int gswf)
    {
        g_swSet_trampoline(gswf);
        if (gswf >= 6200 && gswf <= 6300)
            applyShopFlagLive(gswf);
    }

    KEEP_FUNC const char *msgSearchHook(const char *msgKey)
    {
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
        if (!strcmp(msgKey, "stg6_rsh_diary_01"))
        {
            // Change the text asking if you want to read the diary
            // Only needs to change when not on the train
            if (strcmp(_next_area, "rsh"))
            {
                return "An unseen force prevents\n"
                       "you from opening the diary.\n<o>";
            }
        }
        if (strcmp(msgKey, "stg6_rsh_diary_01_yn") == 0)
        {
            // Change the yes/no text answers for the diary
            // Only needs to change when not on the train
            if (strcmp(_next_area, "rsh") != 0)
            {
#ifdef TTYD_JP
                const char *message =
                    "<select 0 0 300 40>Dang\n"
                    "Shoot";
#else
                const char *message =
                    "<select 0 0 0 40>Dang\n"
                    "Shoot";
#endif
                return message;
            }
        }

        return g_msgSearch_trampoline(msgKey);
    }

    inline bool removeItemFromKeyItems(int16_t itemId);
    inline int32_t addItemToKeyItems(int16_t itemId);

    inline void pouchReAddReturnPipe()
    {
        if (removeItemFromKeyItems(ItemId::TRIPLE_DIP))
        {
            addItemToKeyItems(ItemId::TRIPLE_DIP);
        }

        // Remove the return pipe from the inventory, and then re-add it
        // Only re-add it if it was previously in the inventory, as it won't be when initially starting a new file
        if (pouchRemoveItem(ItemId::INVALID_ITEM_PAPER_0054))
        {
            pouchGetItem(ItemId::INVALID_ITEM_PAPER_0054);
        }
    }

    inline bool containsKeyItem(int16_t itemId)
    {
        constexpr uint32_t loopCount = sizeof(ttyd::mario_pouch::PouchData::key_items) / sizeof(int16_t);
        const int16_t *keyItemsPtr = &ttyd::mario_pouch::pouchGetPtr()->key_items[0];

        // Full scan: stay correct even if something leaves a hole in the array
        for (uint32_t i = 0; i < loopCount; i++)
        {
            if (keyItemsPtr[i] == itemId)
            {
                return true;
            }
        }

        return false;
    }

    inline int32_t addItemToKeyItems(int16_t itemId)
    {
        constexpr uint32_t loopCount = sizeof(ttyd::mario_pouch::PouchData::key_items) / sizeof(int16_t);
        int16_t *keyItemsPtr = &ttyd::mario_pouch::pouchGetPtr()->key_items[0];

        // Key items inventory is full
        if (keyItemsPtr[loopCount - 1] != ItemId::INVALID_NONE)
        {
            return 0;
        }

        // Prepend, matching vanilla pouchGetItem's key-item insert
        memmove(&keyItemsPtr[1], &keyItemsPtr[0], (loopCount - 1) * sizeof(int16_t));
        keyItemsPtr[0] = itemId;
        return 1;
    }

    inline bool removeItemFromKeyItems(int16_t itemId)
    {
        constexpr uint32_t loopCount = sizeof(ttyd::mario_pouch::PouchData::key_items) / sizeof(int16_t);
        int16_t *keyItemsPtr = &ttyd::mario_pouch::pouchGetPtr()->key_items[0];

        // Full scan: stay correct even if something leaves a hole in the array
        for (uint32_t i = 0; i < loopCount; i++)
        {
            if (keyItemsPtr[i] != itemId)
            {
                continue;
            }

            // Found the item, shift everything after it up one slot
            const uint32_t remainingSize = (loopCount - i - 1) * sizeof(int16_t);
            memmove(&keyItemsPtr[i], &keyItemsPtr[i + 1], remainingSize);

            // Clear the last slot in case the inventory was full
            keyItemsPtr[loopCount - 1] = ItemId::INVALID_NONE;
            return true;
        }

        return false;
    }

    static void checkCrystalStarGoal()
    {
        if (gState->apSettings->goal != 2 || ttyd::swdrv::swGet(6120) != 0)
            return;

        uint8_t count = 0;
        for (int i = 114; i <= 120; i++)
        {
            if (pouchCheckItem(i) > 0)
                count++;
        }
        if (count < gState->apSettings->goalStars)
            return;

        if (checkIfInGameNotBattle())
        {
            ttyd::swdrv::swSet(6120);
            ttyd::seqdrv::seqSetSeq(SeqIndex::kMapChange, "end_00", 0);
        }
        else
        {
            // Defer the sequence change until we are back in the game
            ttyd::swdrv::swSet(6121);
        }
    }

    KEEP_FUNC void checkRecipeGoal()
    {
        if (gState->apSettings->goal != 4 || ttyd::swdrv::swGet(6120) != 0)
            return;

        uint8_t count = 0;
        for (int32_t i = 0; i < 57; i++)
        {
            if (ttyd::swdrv::swGet(mod::ap_cooking::kRecipeFlagBase + i))
                count++;
        }
        if (count < gState->apSettings->goalRecipes)
            return;

        if (checkIfInGameNotBattle())
        {
            ttyd::swdrv::swSet(6120);
            ttyd::seqdrv::seqSetSeq(SeqIndex::kMapChange, "end_00", 0);
        }
        else
        {
            ttyd::swdrv::swSet(6121);
        }
    }

    static uint32_t pouchGetItemHookImpl(int32_t item)
    {
        const int32_t ingredientIdx = mod::ap_cooking::ingredientIndex(item);
        if (gState->apSettings->cooksanity && ingredientIdx >= 0 && item != mod::ap_cooking::gCookGiveItem &&
            !ttyd::swdrv::swGet(mod::ap_cooking::kIngredientFlagBase + ingredientIdx))
        {
            ttyd::swdrv::swSet(mod::ap_cooking::kIngredientFlagBase + ingredientIdx);
            queueIngredientToast(item);
        }

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
            case ItemId::SQUARE_DIAMOND_BADGE_P: // relocated Briefcase (badge-range id)
            case ItemId::TRIPLE_DIP:             // Save Block key item (badge-range id)
            {
                if (containsKeyItem(item))
                {
                    return 1;
                }
                if (!addItemToKeyItems(item))
                {
                    return 0; // key items inventory is full
                }
                pouchReAddReturnPipe();
                return 2;
            }
            case ItemId::COCONUT:
            {
                if (ttyd::swdrv::swByteGet(1719) >= 4)
                {
                    return g_pouchGetItem_trampoline(item);
                }

                if (!containsKeyItem(item))
                {
                    if (addItemToKeyItems(item))
                    {
                        pouchReAddReturnPipe();
                        return 2;
                    }
                    else
                        return 0; // Key items inventory is full, can't give the item
                }
                return g_pouchGetItem_trampoline(item);
            }
            case ItemId::LIFE_SHROOM:
            {
                if (ttyd::swdrv::swByteGet(1740) >= 2)
                {
                    return g_pouchGetItem_trampoline(item);
                }

                if (!containsKeyItem(item))
                {
                    if (addItemToKeyItems(item))
                    {
                        pouchReAddReturnPipe();
                        return 2;
                    }
                    else
                        return 0; // Key items inventory is full, can't give the item
                }
                return g_pouchGetItem_trampoline(item);
            }
            case ItemId::KEEL_MANGO:
            {
                if (ttyd::swdrv::swByteGet(1752) >= 2 && ttyd::swdrv::swByteGet(1759) >= 2)
                {
                    return g_pouchGetItem_trampoline(item);
                }

                if (!containsKeyItem(item))
                {
                    if (addItemToKeyItems(item))
                    {
                        pouchReAddReturnPipe();
                        return 2;
                    }
                    else
                        return 0; // Key items inventory is full, can't give the item
                }
                return g_pouchGetItem_trampoline(item);
            }
            case ItemId::MYSTIC_EGG:
            {
                if (ttyd::swdrv::swByteGet(1752) >= 2)
                {
                    return g_pouchGetItem_trampoline(item);
                }

                if (!containsKeyItem(item))
                {
                    if (addItemToKeyItems(item))
                    {
                        pouchReAddReturnPipe();
                        return 2;
                    }
                    else
                        return 0; // Key items inventory is full, can't give the item
                }
                return g_pouchGetItem_trampoline(item);
            }
            case ItemId::GOLDEN_LEAF:
            {
                if (ttyd::swdrv::swByteGet(1752) >= 2)
                {
                    return g_pouchGetItem_trampoline(item);
                }

                if (!containsKeyItem(item))
                {
                    if (addItemToKeyItems(item))
                    {
                        pouchReAddReturnPipe();
                        return 2;
                    }
                    else
                        return 0; // Key items inventory is full, can't give the item
                }
                return g_pouchGetItem_trampoline(item);
            }
            case ItemId::HONEY_CANDY:
            {
                if (ttyd::swdrv::swByteGet(1756) >= 2)
                {
                    return g_pouchGetItem_trampoline(item);
                }

                if (!containsKeyItem(item))
                {
                    if (addItemToKeyItems(item))
                    {
                        pouchReAddReturnPipe();
                        return 2;
                    }
                    else
                        return 0; // Key items inventory is full, can't give the item
                }
                return g_pouchGetItem_trampoline(item);
            }
            case ItemId::FRIGHT_MASK:
            {
                if (ttyd::swdrv::swByteGet(1759) >= 2)
                {
                    return g_pouchGetItem_trampoline(item);
                }

                if (!containsKeyItem(item))
                {
                    if (addItemToKeyItems(item))
                    {
                        pouchReAddReturnPipe();
                        return 2;
                    }
                    else
                        return 0; // Key items inventory is full, can't give the item
                }
                return g_pouchGetItem_trampoline(item);
            }
            case ItemId::UP_ARROW:
            {
                // If the player obtains Up Arrow, then automatically turn it in to Merlon if Cutscene Skip is on
                if (gState->apSettings->cutsceneSkip)
                {
                    ttyd::swdrv::swSet(1186);
                    return 1;
                }
                return g_pouchGetItem_trampoline(item);
            }
            default:
            {
                if (ItemId::DIAMOND_STAR <= item && item <= ItemId ::CRYSTAL_STAR)
                    pouchGetStarStone(item - 113);

                // Give the item
                const uint32_t ret = g_pouchGetItem_trampoline(item);

                // If the given item is an important item, then regive the return pipe to make sure its always at the top of the
                // inventory
                if (ret && (item <= ItemId::CRYSTAL_STAR))
                {
                    pouchReAddReturnPipe();
                }

                // A crystal star can arrive through this path from any source
                // (AP item, chest, shop, or a tattle reward via _get_present_item),
                // so re-evaluate the crystal-star goal here rather than relying on
                // RecieveItems, which only sees stars from the AP item array.
                if (ItemId::DIAMOND_STAR <= item && item <= ItemId::CRYSTAL_STAR)
                    checkCrystalStarGoal();

                return ret;
            }
        }
    }

    KEEP_FUNC uint32_t pouchGetItemHook(int32_t item)
    {
        const uint32_t ret = pouchGetItemHookImpl(item);
        if (ret != 0)
            mod::tracker::recordItemObtained(item);
        return ret;
    }

    KEEP_FUNC int32_t sandersBombHitPositionHook(ttyd::evtmgr::EvtEntry *evt, bool isFirstCall)
    {
        const int32_t ret = g_sandersBombHitPosition_trampoline(evt, isFirstCall);

        struct BombPosOverride
        {
            int32_t unitKind;
            float x;
        };
        static constexpr BombPosOverride kOverrides[] = {
            {BattleUnitType::LEFT_TENTACLE, 90.0f}, // unit parked offstage at x=320
        };

        const int32_t targetId = ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[1]);
        const int32_t unitIdx = ttyd::battle::BattleTransID(evt, targetId);
        const BattleWorkUnit *unit =
            reinterpret_cast<BattleWorkUnit *>(ttyd::battle::BattleGetUnitPtr(ttyd::battle::_battleWorkPtr, unitIdx));
        if (!unit)
            return ret;

        for (const BombPosOverride &fix : kOverrides)
        {
            if (unit->current_kind == fix.unitKind)
            {
                ttyd::evtmgr_cmd::evtSetFloat(evt, evt->evtArguments[3], fix.x);
                break;
            }
        }
        return ret;
    }

    KEEP_FUNC int32_t pouchCheckItemHook(int32_t item)
    {
        switch (item)
        {
            case ItemId::COCONUT:
            case ItemId::LIFE_SHROOM:
            case ItemId::KEEL_MANGO:
            case ItemId::MYSTIC_EGG:
            case ItemId::GOLDEN_LEAF:
            case ItemId::HONEY_CANDY:
            case ItemId::FRIGHT_MASK:
            case ItemId::SQUARE_DIAMOND_BADGE_P: // relocated Briefcase
            {
                // pouchGetItemHook parks the first copy of these in the key items
                // inventory while their trouble is active, but pouchCheckItem is
                // range-based and never scans key items for consumable ids — so
                // possession checks (trouble talk evts, item-select windows) missed
                // the protected copy. Only count it on the field: battle keeps the
                // vanilla count so in-battle effects (e.g. Life Shroom's auto-
                // revive) can't consume the trouble item.
                const int32_t count = g_pouchCheckItem_trampoline(item);
                if (checkIfInGameNotBattle() && containsKeyItem(item))
                    return count + 1;
                return count;
            }
            default:
            {
                return g_pouchCheckItem_trampoline(item);
            }
        }
    }

    KEEP_FUNC int32_t pouchRemoveItemHook(int32_t item)
    {
        switch (item)
        {
            case ItemId::COCONUT:
            case ItemId::LIFE_SHROOM:
            case ItemId::KEEL_MANGO:
            case ItemId::MYSTIC_EGG:
            case ItemId::GOLDEN_LEAF:
            case ItemId::HONEY_CANDY:
            case ItemId::FRIGHT_MASK:
            case ItemId::SQUARE_DIAMOND_BADGE_P: // relocated Briefcase
            {
                // These items are placed in the key items inventory via the hacky add function,
                // so check there first and remove from there if found
                if (removeItemFromKeyItems(item))
                {
                    return 1;
                }

                // Item wasn't in key items, so fall through to normal removal
                return g_pouchRemoveItem_trampoline(item);
            }
            default:
            {
                return g_pouchRemoveItem_trampoline(item);
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

    KEEP_FUNC int32_t evtPartyJumpPosHook(ttyd::evtmgr::EvtEntry *evt, bool firstCall)
    {
        // Vanilla null-checks the party NPC only on the first call; a party
        // member killed mid-jump crashes the unchecked continuation tick.
        if (!firstCall)
        {
            const auto slot = static_cast<ttyd::party::PartySlotId>(evt->sleepTimeMs);
            if (!ttyd::party::partyGetPtr(slot))
                return 2; // NPC is gone; report the jump as complete
        }
        return g_evt_party_jump_pos_trampoline(evt, firstCall);
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
        return g_seq_logoMain_trampoline(info);
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

            gState->fastTraveling = true;
            ttyd::seqdrv::seqSetSeq(SeqIndex::kMapChange, mapName, bero);
        }

        return 2;
    }

    EVT_DECLARE_USER_FUNC(checkValidWarpSequence, 1)
    EVT_DEFINE_USER_FUNC(checkValidWarpSequence)
    {
        (void)isFirstCall;

        const int ch2 = ttyd::swdrv::swByteGet(1713);
        if (7 <= ch2 && ch2 <= 11)
        {
            ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 1);
        }

        const int ch5 = ttyd::swdrv::swByteGet(1717);
        if (10 <= ch5 && ch5 <= 18)
        {
            if (!strcmp(_next_area, "muj") || !strcmp(_next_area, "dou"))
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
            const char *location_name = msgSearch(ttyd::win_log::main_win_log_name);
            snprintf(warpTextBuffer, sizeof(warpTextBuffer), "<system>\n<p>\nFast travel to\n%s?\n<o>", location_name);
        }

        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], reinterpret_cast<uint32_t>(&warpTextBuffer[0]));
        return 2;
    }

    // clang-format off
    // Vanilla save-block system event in the DOL (resolved via ttyd.us.lst)
    extern "C" int32_t main_mobj_save_blk_sysevt[];

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
        IF_EQUAL(LW(0), 0)
            USER_FUNC(evt_mario_normalize)
            WAIT_MSEC(500) // Wait 500 msec since the normalize is not instant in paper mode
        END_IF()
        USER_FUNC(handleWarpConfirmResponse, LW(0), LW(10))
        IF_EQUAL(LW(0), 1)
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

    EVT_BEGIN(confirm_save_evt)
        RUN_CHILD_EVT(main_mobj_save_blk_sysevt)
        RETURN()
    EVT_END()

    EVT_BEGIN(confirm_travel_evt)
        SET(LW(10), static_cast<int32_t>(WarpType::FAST_TRAVEL))
        RUN_CHILD_EVT(custom_warp_evt)
        RETURN()
    EVT_END()

    EVT_BEGIN(deathlink_evt)
        USER_FUNC(ttyd::evt_fade::evt_fade_set_mapchange_type, 1, -1, 600, -1, -1)
        USER_FUNC(ttyd::evt_seq::evt_seq_set_seq, 5, 0, 0)
        RETURN()
    EVT_END()
    // clang-format on

    extern "C" KEEP_FUNC void winItemBuildPartyList(uint32_t *list, int32_t currentId)
    {
        constexpr int32_t kEntrySize = 0x24;
        constexpr int32_t kEntryCount = 7;
        const PouchData *pouch = ttyd::mario_pouch::pouchGetPtr();

        int32_t count = 0;
        for (int32_t i = 0; i < kEntryCount; i++)
        {
            const uint8_t *entry = &winPartyDt[i * kEntrySize];
            if (*reinterpret_cast<const int32_t *>(entry) == currentId)
            {
                list[count++] = reinterpret_cast<uint32_t>(entry);
                break;
            }
        }
        for (int32_t i = 0; i < kEntryCount; i++)
        {
            const uint8_t *entry = &winPartyDt[i * kEntrySize];
            const int32_t id = *reinterpret_cast<const int32_t *>(entry);
            if (id == currentId || id < 1 || id > 7)
                continue;
            if (!(pouch->party_data[id].flags & 1))
                continue;
            list[count++] = reinterpret_cast<uint32_t>(entry);
        }

        const uint32_t pad = count > 0 ? list[count - 1] : reinterpret_cast<uint32_t>(&winPartyDt[0]);
        for (int32_t i = count; i < kEntryCount; i++)
            list[i] = pad;
    }

    // Hook item menu update function to handle interactions with added key items.
    KEEP_FUNC int32_t WinItemMainHook(ttyd::win_root::WinPauseMenu *menu)
    {
        if (menu->itemMenuState == 10)
        {
            if ((menu->buttonsPressed & gc::pad::PadInput::PAD_A) && (menu->itemSubmenuId == 1) &&
                (marioGetPtr()->characterId == MarioCharacters::kMario))
            {
                const int16_t hovered = menu->keyItemIds[menu->itemsCursorIdx[1]];
                // Params taken from `evtEntryType` call in `mobjRunEvent` for running `mobj_save_blk_sysevt`, as using
                // `evtEntry` causes message selection boxes to not show up when the system level is raised, and certain `types`
                // cause the script to only run once the pause menu is fully closed
                if (hovered == ItemId::INVALID_ITEM_PAPER_0054)
                {
                    ttyd::evtmgr::evtEntryType(const_cast<int32_t *>(confirm_pipe_evt), 30, 0, 26);
                    return -2;
                }
                if (hovered == ItemId::TRIPLE_DIP) // Save Block key item
                {
                    ttyd::evtmgr::evtEntryType(const_cast<int32_t *>(confirm_save_evt), 30, 0, 26);
                    return -2;
                }
            }
        }

        return g_winItemMain_trampoline(menu);
    }

    // Tracker screen (opened with X on a hovered map node)
    static bool sTrackerScreenOpen = false;
    static int32_t sTrackerScreenScroll = 0;
    static int32_t sTrackerScreenRows = 0;
    static bool sTrackerHintShowWarp = false;
    static bool sTrackerHintShowList = false;
    static char sTrackerScreenTitle[64];

    static constexpr int32_t kTrackerVisibleRows = 13;
    static constexpr uint32_t kTrackerRowColors[3] = {
        0x9A9A9A00, // LOC_CHECKED - gray
        0x66EE6600, // LOC_AVAILABLE - green
        0xFA807200, // LOC_OUT_OF_LOGIC - red
    };

    static void trackerHintDisp(ttyd::dispdrv::CameraId cameraId, void *user)
    {
        (void)cameraId;
        (void)user;

        gc::gx::GXColor fogColor(0x66, 0x06, 0x42, 0x80);
        gc::gx::GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fogColor);

        constexpr float kScale = 0.58f;
        constexpr float kIconScale = 0.55f;
        constexpr float kIconWidth = 26.0f; // footprint of a button icon in the row
        constexpr float kSegmentGap = 16.0f;
        constexpr float y = -196.0f;

        const char *warpText = "Warp";
        const char *listText = "Check List";
        const float warpTextWidth = static_cast<float>(ttyd::fontmgr::FontGetMessageWidth(warpText)) * kScale;
        const float listTextWidth = static_cast<float>(ttyd::fontmgr::FontGetMessageWidth(listText)) * kScale;

        float width = 0.0f;
        if (sTrackerHintShowWarp)
            width += kIconWidth + warpTextWidth;
        if (sTrackerHintShowList)
            width += kIconWidth + listTextWidth;
        if (sTrackerHintShowWarp && sTrackerHintShowList)
            width += kSegmentGap;

        float x = -width * 0.5f;
        uint32_t plateColor = 130; // translucent black plate
        ttyd::windowdrv::windowDispGX_Waku_col(0, &plateColor, x - 8.0f, y + 6.0f, width + 16.0f, 28.0f, 8.0f);

        if (sTrackerHintShowWarp)
        {
            gc::vec3 iconPos = {x + kIconWidth * 0.5f - 4.0f, y - 19.0f, 0.0f};
            ttyd::icondrv::iconDispGx(kIconScale, &iconPos, 0x18, IconType::A_BUTTON);
            gSelf->DrawString(warpText, x + kIconWidth, y, 0xFFFFFFFF, kScale);
            x += kIconWidth + warpTextWidth + kSegmentGap;
        }
        if (sTrackerHintShowList)
        {
            gc::vec3 iconPos = {x + kIconWidth * 0.5f - 4.0f, y - 19.0f, 0.0f};
            ttyd::icondrv::iconDispGx(kIconScale, &iconPos, 0x18, IconType::X_BUTTON);
            gSelf->DrawString(listText, x + kIconWidth, y, 0xFFFFFFFF, kScale);
        }
    }

    static void trackerScreenDisp(ttyd::dispdrv::CameraId cameraId, void *user)
    {
        (void)cameraId;
        (void)user;

        gc::gx::GXColor fogColor(0x66, 0x06, 0x42, 0x80);
        gc::gx::GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fogColor);

        constexpr float kLeft = -272.0f;
        constexpr float kTop = 176.0f;
        constexpr float kWidth = 544.0f;
        constexpr float kRowHeight = 21.0f;
        constexpr float kRowScale = 0.58f;
        constexpr float kMaxRowWidth = kWidth - 24.0f;

        uint32_t plateColor = 215; // near-opaque black plate over the list area
        ttyd::windowdrv::windowDispGX_Waku_col(0, &plateColor, kLeft - 8.0f, kTop + 8.0f, kWidth + 16.0f, 360.0f,
                                               10.0f);

        gSelf->DrawString(sTrackerScreenTitle, kLeft, kTop - 4.0f, 0xFFFFFFFF, 0.78f);

        // Summary line: tally live statuses over the whole list
        int32_t checkedCount = 0;
        int32_t availableCount = 0;
        for (int32_t i = 0; i < sTrackerScreenRows; i++)
        {
            const char *name;
            uint8_t status;
            if (!mod::tracker::trackerGetListRow(i, &name, &status))
                break;
            if (status == mod::tracker::LOC_CHECKED)
                checkedCount++;
            else if (status == mod::tracker::LOC_AVAILABLE)
                availableCount++;
        }
        char summary[64];
        snprintf(summary, sizeof(summary), "Checked: %d/%d    In logic: %d", static_cast<int>(checkedCount),
                 static_cast<int>(sTrackerScreenRows), static_cast<int>(availableCount));
        gSelf->DrawString(summary, kLeft, kTop - 28.0f, 0xFFFFFFFF, 0.55f);

        float y = kTop - 54.0f;
        for (int32_t i = 0; i < kTrackerVisibleRows; i++)
        {
            const char *name;
            uint8_t status;
            if (!mod::tracker::trackerGetListRow(sTrackerScreenScroll + i, &name, &status))
                break;

            float scale = kRowScale;
            const float width = static_cast<float>(ttyd::fontmgr::FontGetMessageWidth(name));
            if (width * scale > kMaxRowWidth)
                scale = kMaxRowWidth / width;

            gSelf->DrawString(name, kLeft, y, kTrackerRowColors[status] | 0xFF, scale);
            y -= kRowHeight;
        }

        char footer[96];
        const int32_t first = sTrackerScreenRows == 0 ? 0 : sTrackerScreenScroll + 1;
        int32_t last = sTrackerScreenScroll + kTrackerVisibleRows;
        if (last > sTrackerScreenRows)
            last = sTrackerScreenRows;
        const float footerY = kTop - 56.0f - kTrackerVisibleRows * kRowHeight;
        constexpr float kFooterScale = 0.52f;
        snprintf(footer, sizeof(footer), "%d-%d / %d   Up/Down: scroll   L/R: page", static_cast<int>(first),
                 static_cast<int>(last), static_cast<int>(sTrackerScreenRows));
        gSelf->DrawString(footer, kLeft, footerY, 0xC8C8C8FF, kFooterScale);

        // "[B] Close" with the real button sprite
        const float footerWidth = static_cast<float>(ttyd::fontmgr::FontGetMessageWidth(footer)) * kFooterScale;
        gc::vec3 bIconPos = {kLeft + footerWidth + 24.0f, footerY - 19.0f, 0.0f};
        ttyd::icondrv::iconDispGx(0.5f, &bIconPos, 0x18, IconType::B_BUTTON);
        gSelf->DrawString("Close", kLeft + footerWidth + 38.0f, footerY, 0xC8C8C8FF, kFooterScale);

        // Legend: the status words in their own colors, right-aligned on the title line
        constexpr float kLegendScale = 0.5f;
        constexpr float kLegendGap = 14.0f;
        const char *legendWords[3] = {"Available", "Out of logic", "Checked"};
        const uint8_t legendStatus[3] = {mod::tracker::LOC_AVAILABLE, mod::tracker::LOC_OUT_OF_LOGIC,
                                         mod::tracker::LOC_CHECKED};
        float legendWidths[3];
        float legendTotal = 0.0f;
        for (int32_t i = 0; i < 3; i++)
        {
            legendWidths[i] = static_cast<float>(ttyd::fontmgr::FontGetMessageWidth(legendWords[i])) * kLegendScale;
            legendTotal += legendWidths[i];
        }
        legendTotal += kLegendGap * 2.0f;
        float legendX = kLeft + kWidth - legendTotal;
        for (int32_t i = 0; i < 3; i++)
        {
            gSelf->DrawString(legendWords[i], legendX, kTop - 6.0f, kTrackerRowColors[legendStatus[i]] | 0xFF,
                              kLegendScale);
            legendX += legendWidths[i] + kLegendGap;
        }
    }

    // Hook journal menu: tracker hint/screen and fast travel from the map
    KEEP_FUNC int32_t WinLogMainHook(ttyd::win_root::WinPauseMenu *menu)
    {
        if (menu->logMenuState == 10) // map open
        {
            mod::tracker::trackerTick();

            if (sTrackerScreenOpen)
            {
                const uint32_t pressed = menu->buttonsPressed;
                const uint32_t repeated = menu->buttonsRepeated;

                if (pressed & (gc::pad::PadInput::PAD_B | gc::pad::PadInput::PAD_X))
                {
                    sTrackerScreenOpen = false;
                    ttyd::pmario_sound::psndSFXOn(0x20013);
                }
                else
                {
                    (void)repeated;
                    const int32_t maxScroll =
                        sTrackerScreenRows > kTrackerVisibleRows ? sTrackerScreenRows - kTrackerVisibleRows : 0;

                    static int32_t sTrackerScrollHoldFrames = 0;
                    const uint32_t heldButtons = keyGetButton(gc::pad::PadId::CONTROLLER_ONE);
                    const int8_t stickY =
                        static_cast<int8_t>(keyGetStickY(gc::pad::PadId::CONTROLLER_ONE) & 0xFF);
                    const bool holdUp = (heldButtons & gc::pad::PadInput::PAD_DPAD_UP) || stickY > 40;
                    const bool holdDown = (heldButtons & gc::pad::PadInput::PAD_DPAD_DOWN) || stickY < -40;
                    int32_t delta = 0;
                    if (pressed & gc::pad::PadInput::PAD_DPAD_UP)
                        delta--;
                    if (pressed & gc::pad::PadInput::PAD_DPAD_DOWN)
                        delta++;
                    if (holdUp || holdDown)
                    {
                        sTrackerScrollHoldFrames++;
                        if (sTrackerScrollHoldFrames == 1 && !(pressed & (gc::pad::PadInput::PAD_DPAD_UP |
                                                                          gc::pad::PadInput::PAD_DPAD_DOWN)))
                            delta += holdDown ? 1 : -1;
                        if (sTrackerScrollHoldFrames > 12 && (sTrackerScrollHoldFrames & 1) == 0)
                            delta += holdDown ? 1 : -1;
                    }
                    else
                    {
                        sTrackerScrollHoldFrames = 0;
                    }
                    if (pressed & gc::pad::PadInput::PAD_L)
                        delta -= kTrackerVisibleRows;
                    if (pressed & gc::pad::PadInput::PAD_R)
                        delta += kTrackerVisibleRows;

                    sTrackerScreenScroll += delta;
                    if (sTrackerScreenScroll > maxScroll)
                        sTrackerScreenScroll = maxScroll;
                    if (sTrackerScreenScroll < 0)
                        sTrackerScreenScroll = 0;

                    ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::kDebug3d, 1, 150.0f, trackerScreenDisp,
                                             nullptr);
                }

                return 0;
            }

            if (menu->mapCursorIdx < 0) // no location selected
                return g_winLogMain_trampoline(menu);

            // The extended marker table (93 vanilla + tattle/cook virtual nodes)
            const char *prefix =
                reinterpret_cast<const ttyd::win_log::MapMarker *>(ap_map_markers)[menu->mapCursorIdx].map_prefix;
            const bool virtualNode =
                prefix && (strncmp(prefix, "tattle", 7) == 0 || strncmp(prefix, "cook", 7) == 0);

            const bool fastTravel = gState->apSettings->fastTravel != 0 && !virtualNode &&
                                    ttyd::swdrv::swGet(0x189C + win_log_mapGX_arr[menu->mapCursorIdx]);
            const bool trackerReady = mod::tracker::trackerReady();
            if (trackerReady || fastTravel)
            {
                sTrackerHintShowWarp = fastTravel;
                sTrackerHintShowList = trackerReady;
                ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::kDebug3d, 1, 150.0f, trackerHintDisp, nullptr);
            }

            if ((menu->buttonsPressed & gc::pad::PadInput::PAD_X) && trackerReady)
            {
                sTrackerScreenRows = mod::tracker::trackerBuildNodeList(prefix);
                sTrackerScreenScroll = 0;
                // The hovered node's display name, as shown by the map page itself
                const char *title = msgSearch(ttyd::win_log::main_win_log_name);
                snprintf(sTrackerScreenTitle, sizeof(sTrackerScreenTitle), "%s", title ? title : "Checks");
                sTrackerScreenOpen = true;
                ttyd::pmario_sound::psndSFXOn(0x20012);
                return 0; // normal "stay" return (-1 = tab bar takes over, -2 = close menu)
            }

            if (!fastTravel)
                return g_winLogMain_trampoline(menu);

            if ((menu->buttonsPressed & gc::pad::PadInput::PAD_A) && (marioGetPtr()->characterId == MarioCharacters::kMario))
            {
                if (!ttyd::swdrv::swGet(0x189C + win_log_mapGX_arr[menu->mapCursorIdx]))
                {
                    ttyd::pmario_sound::psndSFXOn(0x20013);
                    return g_winLogMain_trampoline(menu);
                }

                // Params taken from `evtEntryType` call in `mobjRunEvent` for running `mobj_save_blk_sysevt`, as using
                // `evtEntry` causes message selection boxes to not show up when the system level is raised, and certain `types`
                // cause the script to only run once the pause menu is fully closed
                ttyd::evtmgr::evtEntryType(const_cast<int32_t *>(confirm_travel_evt), 30, 0, 26);
                return -2;
            }
        }
        else if (sTrackerScreenOpen)
        {
            // The map page closed some other way; don't leave the overlay armed
            sTrackerScreenOpen = false;
        }

        return g_winLogMain_trampoline(menu);
    }

    void DrainReceivedFlags()
    {
        uintptr_t kRecvFlagRingAddr = 0x80004600;
        int kRecvFlagCapacity = 64;

        volatile uint16_t *head = reinterpret_cast<volatile uint16_t *>(kRecvFlagRingAddr + 0x0);
        volatile uint16_t *tail = reinterpret_cast<volatile uint16_t *>(kRecvFlagRingAddr + 0x2);
        volatile uint16_t *ring = reinterpret_cast<volatile uint16_t *>(kRecvFlagRingAddr + 0x4);

        uint16_t h = *head;
        while (*tail != h)
        {
            uint16_t flag = ring[*tail % kRecvFlagCapacity];
            *tail = static_cast<uint16_t>(*tail + 1);

            if (flag == 0)
                continue; // 0 is not a valid AP location flag; skip

            ttyd::swdrv::swSet(flag);
            DeleteFieldItemForFlag(flag);
            HandleMobjForFlag(flag);

            // Anything arriving through the flag ring came from outside the game
            *reinterpret_cast<uint8_t *>(kRunDirtyFlagAddr) |= kDirtyExternalFlags;
        }
    }

    void OWR::Update()
    {
        updateRtaTimer();
        updatePaperModePersistence();

        if (checkIfInGameNotBattle() && ttyd::swdrv::swByteGet(1700) != 0 &&
            marioGetPtr()->characterId == MarioCharacters::kMario)
        {
            if (!containsKeyItem(ItemId::INVALID_ITEM_PAPER_0054))
                ttyd::mario_pouch::pouchGetItem(ItemId::INVALID_ITEM_PAPER_0054);
            if (!containsKeyItem(ItemId::TRIPLE_DIP))
                ttyd::mario_pouch::pouchGetItem(ItemId::TRIPLE_DIP);
        }

        APSettings *apSettingsPtr = gState->apSettings;
        apSettingsPtr->inGame = static_cast<uint8_t>(checkIfInGame());

        uint8_t count = 0;
        for (int i = 114; i <= 120; i++)
        {
            if (pouchCheckItem(i) > 0)
                count++;
        }
        apSettingsPtr->collectedStars = count;

        if (apSettingsPtr->collectedStars >= apSettingsPtr->palaceStars && ttyd::swdrv::swByteGet(1708) == 0)
            if (apSettingsPtr->cutsceneSkip)
                ttyd::swdrv::swByteSet(1708, 2);

        if (apSettingsPtr->deathLinkTriggered)
        {
            apSettingsPtr->deathLinkTriggered = 0;
            ttyd::evtmgr::evtEntryType(const_cast<int32_t *>(mod::owr::deathlink_evt), 30, 0, 26);
        }


        // Deferred goal teleport (goal completed during battle/cutscene): 6120 must be set
        // here too, or the check stays true inside end_00 and re-teleports every frame.
        if (checkIfInGameNotBattle() && ttyd::swdrv::swGet(6121) == 1 && ttyd::swdrv::swGet(6120) != 1)
        {
            ttyd::swdrv::swSet(6120);
            ttyd::swdrv::swClear(6121); // deferral consumed
            ttyd::seqdrv::seqSetSeq(SeqIndex::kMapChange, "end_00", 0);
        }

        // Advance Boggly Woods sequence if the great tree is opened
        if (ttyd::swdrv::swByteGet(1713) >= 1 && ttyd::swdrv::swByteGet(1702) < 8)
            ttyd::swdrv::swByteSet(1702, 8);

        SequenceInit();
        RecieveItems();
        DrainReceivedFlags();
        updateIngredientToast();
        updateReceivedItemFeed();

        // RTA timer in the corner while playing; final time + seed reveal over the credits
        if (sSaveFileActive && apSettingsPtr->rtaTimer != 0)
        {
            if (inEndingArea())
            {
                if (seqGetSeq() == SeqIndex::kGame)
                    ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::kDebug3d, 1, 160.0f, creditsResultsDisp, nullptr);
            }
            else
            {
                ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::kDebug3d, 1, 160.0f, rtaTimerDisp, nullptr);
            }
        }
    }

    void OWR::OnModuleLoaded(OSModuleInfo *module_info)
    {
        RelMgr *relMgrPtr = &relMgr;

        // Stat-scaling registrations point into the outgoing area rel; clear them
        // before the relinked subrel registers this area's own setups.
        ResetOriginalKindMap();

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