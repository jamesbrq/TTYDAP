#include "customWarp.h"
#include "relmgr.h"
#include "util.h"
#include "visibility.h"
#include <AP/rel_patch_definitions.h>
#include <gc/gx.h>
#include <gc/OSModule.h>
#include <gc/pad.h>
#include <mod.h>
#include <ttyd/common_types.h>
#include <ttyd/countdown.h>
#include <ttyd/evt_bero.h>
#include <ttyd/evt_fade.h>
#include <ttyd/evt_lecture.h>
#include <ttyd/evt_mario.h>
#include <ttyd/evt_memcard.h>
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

#include "common.h"
#include "OWR.h"
#include "patch.h"

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cinttypes>
#include <algorithm>

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

namespace mod::owr
{
    KEEP_VAR OWR *gSelf = nullptr;
    KEEP_VAR StateManager *gState = nullptr;
    KEEP_VAR NumericInputData g_numericInput;

    KEEP_VAR bool (*g_OSLink_trampoline)(OSModuleInfo *, void *) = nullptr;
    KEEP_VAR void (*g_seq_logoMain_trampoline)(SeqInfo *info) = nullptr;
    KEEP_VAR void (*g_seq_gameInit_trampoline)(SeqInfo *info) = nullptr;
    KEEP_VAR void (*g_seqSetSeq_trampoline)(SeqIndex seq, const char *map, const char *bero) = nullptr;
    KEEP_VAR uint32_t (*g_pouchGetItem_trampoline)(int32_t) = nullptr;
    KEEP_VAR void (*g_partySetForceMove_trampoline)(ttyd::party::PartyEntry *ptr, float x, float z, float speed) = nullptr;
    KEEP_VAR int32_t (*g_evt_mario_set_pose_trampoline)(ttyd::evtmgr::EvtEntry *evt, bool firstCall) = nullptr;
    KEEP_VAR const char *(*g_msgSearch_trampoline)(const char *) = nullptr;
    KEEP_VAR void (*g_statusWinDisp_trampoline)(void) = nullptr;
    KEEP_VAR void (*g_pouchGetStarstone_trampoline)(int32_t) = nullptr;
    KEEP_VAR int32_t (*g_winItemMain_trampoline)(ttyd::win_root::WinPauseMenu *menu) = nullptr;
    KEEP_VAR int32_t (*g_winLogMain_trampoline)(ttyd::win_root::WinPauseMenu *menu) = nullptr;
    KEEP_VAR void (*g_msgAnalize_trampoline)(ttyd::memory::SmartAllocationData *smartAlloc, const char *text) = nullptr;
    KEEP_VAR int (*g_msgWindow_Entry_trampoline)(const char *message, int unk1, int windowType) = nullptr;
    KEEP_VAR void (*g__load_trampoline)(const char *mapName, const char *entranceName, const char *beroName) = nullptr;

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
            ttyd::swdrv::swSet(1193); //Rougeport Pre-Old Letter Check Part 1
            ttyd::swdrv::swSet(1194); //Rougeport Pre-Old Letter Check Part 2
            ttyd::swdrv::swSet(1204); //Rougeport Talking to Merlon Outside
            ttyd::swdrv::swSet(1212); //Rougeport Gus Introduction
            ttyd::swdrv::swSet(1236); //Rougeport Don Pianta Pre-Blimp Ticket
            ttyd::swdrv::swSet(1237); //Rougeport Lucky Lottery Tutorial
            ttyd::swdrv::swSet(1325); //Rougeport Sewers Dazzle Introduction
            ttyd::swdrv::swSet(1342); //Rougeport Sewers Rip Cheato Introduction 1
            ttyd::swdrv::swSet(1353); //Rougeport Sewers Black Chest Cutscene
            ttyd::swdrv::swSet(1371); //Rougeport Sewers Rip Cheato Introduction 2
            ttyd::swdrv::swSet(1492); //Hooktail's Castle Black Chest Cutscene
            ttyd::swdrv::swSet(1781); //Petal Meadows Pre-Koops
            ttyd::swdrv::swSet(1782); //Petal Meadows Post Koops
            ttyd::swdrv::swSet(1932); //Twilight Town Black Chest Cutscene
            ttyd::swdrv::swSet(2392); //Glitz Pit Minor League Crew Introductions
            ttyd::swdrv::swSet(2395); //Glitz Pit Goomba Bros. Intro
            ttyd::swdrv::swSet(2397); //Glitz Pit Armored Harriers Intro
            ttyd::swdrv::swSet(2398); //Glitz Pit Armored Harriers 1st Win
            ttyd::swdrv::swSet(2399); //Glitz Pit Rawk Hawk Intro
            ttyd::swdrv::swSet(2400); //Glitz Pit Rawk Hawk 1st Win
            ttyd::swdrv::swSet(2401); //Glitz Pit Entered Promoter's Office from Air Vent
            ttyd::swdrv::swSet(2413); //Glitz Pit Sir Swoop Registers
            ttyd::swdrv::swSet(2513); //Glitz Pit Rawk Hawk Banter
            ttyd::swdrv::swSet(2514); //Glitz Pit Rawk Hawk Half HP
            ttyd::swdrv::swSet(2515); //Glitz Pit Rawk Hawk Desperation Phase
            ttyd::swdrv::swSet(2516); //Glitz Pit Rawk Hawk Hanging from Ceiling
            ttyd::swdrv::swSet(2847); //The Great Tree 10 Jabbi Introduction
            ttyd::swdrv::swSet(2855); //The Great Tree 100 Jabbi Introduction
            ttyd::swdrv::swSet(2982); //Pirate's Grotto Saw Toads on Boat
            ttyd::swdrv::swSet(3129); //Keelhaul Key Talked to Flavio about Bobbery
            ttyd::swdrv::swSet(3131); //Post Keelhaul Key Bridge Cutscene
            ttyd::swdrv::swSet(3136); //Pre-Keelhaul Key Embers at Pirate's Grotto Entrance
            ttyd::swdrv::swSet(3429); //Toodles Asks for Help
            ttyd::swdrv::swSet(3430); //Agreed to Help Toodles
            ttyd::swdrv::swSet(3431); //Excess Express Waitress Asks for Help
            ttyd::swdrv::swSet(3432); //Agreed to Help the Excess Express Waitress
            ttyd::swdrv::swSet(3438); //Excess Express Talked to Conductor on First Night
            ttyd::swdrv::swSet(3574); //Riverside Station Stationmaster Toad Cutscene
            ttyd::swdrv::swSet(3884); //Path to Fahr Outpost Intro
            ttyd::swdrv::swSet(4330); //Riddle Tower 3rd Floor 1st Entry
            ttyd::swdrv::swSet(6016); //Hooktail's Castle Meeting Ms. Mowz
            ttyd::swdrv::swSet(6023); //The Great Tree Zig-Zag Room Jabbi Running Away
        }

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

                if (items[i] >= 114 && items[i] <= 120)
                {
                    uint8_t count = 0;
                    for (int i = 114; i <= 120; i++)
                    {
                        if (ttyd::mario_pouch::pouchCheckItem(i) > 0)
                            count++;
                    }
                    if (gState->apSettings->goal == 2 && count >= gState->apSettings->goalStars &&
                        ttyd::swdrv::swGet(6120) == 0)
                    {
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
                }
                items[i] = 0;
            }
            memset(reinterpret_cast<void *>(length_pointer), 0, sizeof(uint32_t));
        }
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

    KEEP_FUNC bool OSLinkHook(OSModuleInfo *new_module, void *bss)
    {
        bool result = g_OSLink_trampoline(new_module, bss);
        if (new_module != nullptr && result)
        {
            gSelf->OnModuleLoaded(new_module);
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

        // Give Zess T. the contact lens upon entering westside
        if (strcmp(map, "gor_03") == 0)
        {
            ttyd::swdrv::swSet(1188);
        }

        // Set cutscene flag for Don Pianta if player leaves westside
        if (ttyd::swdrv::swByteGet(1709) >= 3)
            ttyd::swdrv::swSet(6000);

        // Advance Boggly Woods sequence if the great tree is opened
        if (ttyd::swdrv::swByteGet(1713) >= 1 && ttyd::swdrv::swByteGet(1702) < 8)
            ttyd::swdrv::swByteSet(1702, 8);

        // Advance Punio tik sequence if warped out of early
        if (ttyd::swdrv::swByteGet(1718) >= 3 && strncmp(map, "win", 3) == 0)
            ttyd::swdrv::swByteSet(1718, 5);

        // Enable post chapter 2 checks right when the chapter is beaten
        if (ttyd::swdrv::swByteGet(1713) >= 11 && strncmp(map, "mri", 3) == 0)
            ttyd::swdrv::swSet(2884);

        // Advance Ch.5 prologue if post-cortez
        if (ttyd::swdrv::swByteGet(1717) >= 10 && ttyd::swdrv::swByteGet(1705) < 7 && strncmp(map, "muj", 3) != 0 && strncmp(map, "dou", 3) != 0)
            ttyd::swdrv::swByteSet(1705, 11);

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
    
    KEEP_FUNC void _load_Hook(const char* mapName, const char* entranceName, const char* beroName)
    {
        g__load_trampoline(mapName, entranceName, beroName);
        for (int i = 8; i < 16; i++) gState->state_msgWork[i] = 0;
        ttyd::msgdrv::msgLoad("mod", 2);
        ttyd::msgdrv::msgLoad("desc", 3);
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
                const char *message = "<select 0 0 300 40>Dang\n"
                                      "Shoot";
#else
                const char *message = "<select 0 0 0 40>Dang\n"
                                      "Shoot";
#endif
                return message;
            }
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
            case ItemId::UP_ARROW:
            {
                //If the player obtains Up Arrow, then automatically turn it in to Merlon if Cutscene Skip is on
                if (gState->apSettings->cutsceneSkip)
                {
                    ttyd::swdrv::swSet(1186);
                    return 1;
                }
                return g_pouchGetItem_trampoline(item)
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
        APSettings *apSettingsPtr = gState->apSettings;
        apSettingsPtr->inGame = static_cast<uint8_t>(checkIfInGame());

        uint8_t count = 0;
        for (int i = 114; i <= 120; i++)
        {
            if (ttyd::mario_pouch::pouchCheckItem(i) > 0)
                count++;
        }
        apSettingsPtr->collectedStars = count;

        if (apSettingsPtr->deathLinkTriggered)
        {
            apSettingsPtr->deathLinkTriggered = 0;
            ttyd::evtmgr::evtEntryType(const_cast<int32_t *>(mod::owr::deathlink_evt), 30, 0, 26);
        }

        if (apSettingsPtr->music == 1)
            for (int i = 0; i <= 1; i++) ttyd::pmario_sound::psndBGMOff_f_d(512 + i, 0, 1);

        if (checkIfInGameNotBattle() && ttyd::swdrv::swGet(6121) == 1 && ttyd::swdrv::swGet(6120) != 1)
            ttyd::seqdrv::seqSetSeq(SeqIndex::kMapChange, "end_00", 0);

        SequenceInit();
        RecieveItems();

        if (gState->apSettings->cutsceneSkip)
        {
            if (ttyd::swdrv::swByteGet(1711) == 1) //Hooktail's Castle Intro
            ttyd::swdrv::swByteSet(1711, 3);

            if (ttyd::swdrv::swByteGet(1718) == 3) //Rougeport Sewers Punio Post Passageway Opened
            ttyd::swdrv::swByteSet(1711, 5);

            if (ttyd::swdrv::swByteGet(1713) == 3) //The Great Tree Ms. Mowz Knocks Out X-Naut
            ttyd::swdrv::swByteSet(1713, 4);

            if (ttyd::swdrv::swByteGet(1713) == 6) //The Great Tree Crump Wandering Around
            ttyd::swdrv::swByteSet(1713, 7);

            if (ttyd::swdrv::swGet(2400) == 1) //Glitz Pit Rawk Hawk Defeated
            ttyd::swdrv::swByteSet(1703, 14);

            if (ttyd::swdrv::swByteGet(1703) == 15) //Glitz Pit Air Vent (Champ's Room -> Grubba's Office)
            ttyd::swdrv::swByteSet(1703, 17);

            if (ttyd::swdrv::swByteGet(1703) == 18) //Glitz Pit Finding Grubba's Paper in Desk
            ttyd::swdrv::swByteSet(1703, 19);

            if (ttyd::swdrv::swByteGet(1714) == 5) //Creepy Steeple Intro
            ttyd::swdrv::swByteSet(1714, 6);

            if (ttyd::swdrv::swByteGet(1719) == 2) //Keelhaul Key After Saving Bobbery from Tree
            ttyd::swdrv::swByteSet(1719, 3);

            if (ttyd::swdrv::swByteGet(1717) == 1) //Pirate's Grotto Post Entry
            ttyd::swdrv::swByteSet(1717, 3);

            if (ttyd::swdrv::swByteGet(1717) == 7) //Pirate's Grotto Pre-Cortez
            ttyd::swdrv::swByteSet(1717, 9);

            if (ttyd::swdrv::swByteGet(1709) == 5) //Keelhaul Key Frankie's 100 I Love Yous
            ttyd::swdrv::swByteSet(1709, 6);

            if (ttyd::swdrv::swByteGet(1709) == 10) //Rougeport Westside Post Train Ticket Check
            ttyd::swdrv::swByteSet(1709, 11);

            if (ttyd::swdrv::swByteGet(1706) == 2) //Excess Express Long Intro
            ttyd::swdrv::swByteSet(1706, 6);

            if (ttyd::swdrv::swByteGet(1706) == 7) //Excess Express Talking to Pennington
            ttyd::swdrv::swByteSet(1706, 10);

            if (ttyd::swdrv::swByteGet(1706) == 11) //Excess Express Agreed to Help Bub the Bob-omb
            ttyd::swdrv::swByteSet(1706, 12);

            if (ttyd::swdrv::swByteGet(1706) == 15) //Excess Express Agreed to Help the Conductor
            ttyd::swdrv::swByteSet(1706, 18);

            if (ttyd::swdrv::swByteGet(1706) == 23) //Excess Express Talked to Pennington & Ratooey Businessman
            ttyd::swdrv::swByteSet(1706, 24);

            if (ttyd::swdrv::swByteGet(1706) == 26) //Excess Express Examined Paper in Cabin 005
            ttyd::swdrv::swByteSet(1706, 27);

            if (ttyd::swdrv::swByteGet(1720) == 4) //Riverside Station Seeing Smorgs
            ttyd::swdrv::swByteSet(1720, 5);

            if (ttyd::swdrv::swByteGet(1706) == 33) //Excess Express Pre-Smorg
            ttyd::swdrv::swByteSet(1706, 35);

            if (ttyd::swdrv::swByteGet(1706) == 38) //Poshley Heights Pre-Garnet Star
            ttyd::swdrv::swByteSet(1706, 40);

            if (ttyd::swdrv::swByteGet(1707) == 7) //X-Naut Fortress Entry
            ttyd::swdrv::swByteSet(1707, 8);

            if (ttyd::swdrv::swByteGet(1708) == 5) //Riddle Tower Intro
            ttyd::swdrv::swByteSet(1708, 6);

            if (ttyd::swdrv::swByteGet(1708) == 14) //Palace of Shadow Pre-Grodus
            ttyd::swdrv::swByteSet(1708, 15);
        }
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