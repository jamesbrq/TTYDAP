#include <AP/rel_patch_definitions.h>
#include <cstdint>
#include <cstring>
#include <gc/OSModule.h>
#include <gc/pad.h>
#include <mod.h>
#include "relmgr.h"
#include "visibility.h"
#include <ttyd/countdown.h>
#include <ttyd/common_types.h>
#include <ttyd/evt_mario.h>
#include <ttyd/evt_pouch.h>
#include <ttyd/evt_window.h>
#include <ttyd/mario.h>
#include <ttyd/mario_motion.h>
#include <ttyd/mario_party.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/mariost.h>
#include <ttyd/msgdrv.h>
#include <ttyd/party.h>
#include <ttyd/seq_mapchange.h>
#include <ttyd/seqdrv.h>
#include <ttyd/string.h>
#include <ttyd/swdrv.h>

#include "common.h"
#include "OWR.h"
#include "patch.h"
#include <ttyd/fontmgr.h>

using gc::pad::PadInput;
using ttyd::common::ItemData;
using ttyd::common::ShopItemData;
using ttyd::seq_mapchange::_next_area;
using ttyd::seq_mapchange::RelId;
using ttyd::seqdrv::SeqIndex;
using namespace mod::util;
using namespace ttyd::common;
using namespace ttyd::mario;
using namespace ttyd::mario_pouch;
using namespace ttyd::evt_pouch;
using namespace ttyd::seqdrv;

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

    // Pianta shop back door
    1230,

    // Attempt to enter ch.4 pipe early
    1341,

    // Goombella telling you about tattling
    1805,

    // Ch.4 talk to shopkeep once
    1925,

    // Ch.4 bird room lock
    2231,

    // Spawn General white
    3880};
constexpr int32_t GSWF_ARR_SIZE = sizeof(GSWF_ARR) / sizeof(GSWF_ARR[0]);

namespace mod::owr
{
    KEEP_VAR OWR *gSelf = nullptr;
    KEEP_VAR StateManager *gState = nullptr;

    KEEP_VAR bool (*g_OSLink_trampoline)(OSModuleInfo *, void *) = nullptr;
    KEEP_VAR void (*g_seqSetSeq_trampoline)(SeqIndex seq, const char *map, const char *bero) = nullptr;
    KEEP_VAR uint32_t (*g_pouchGetItem_trampoline)(int32_t) = nullptr;
    KEEP_VAR void (*g_partySetForceMove_trampoline)(ttyd::party::PartyEntry *ptr, float x, float z, float speed) = nullptr;
    KEEP_VAR int32_t (*g_evt_mario_set_pose_trampoline)(ttyd::evtmgr::EvtEntry *evt, bool firstCall) = nullptr;
    KEEP_VAR const char *(*g_msgSearch_trampoline)(const char *) = nullptr;

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
        ttyd::swdrv::swByteSet(1704, 1);
        ttyd::mario_pouch::pouchGetStarstone(0);

        if (gState->apSettings->apEnabled)
        {
            ttyd::mario_pouch::pouchSetPartyColor(static_cast<uint8_t>(ttyd::party::PartyMembers::Yoshi),
                                                  gState->apSettings->yoshiColor);
            ttyd::mario_pouch::pouchSetYoshiName(gState->apSettings->yoshiName);

            ttyd::mario_party::partyJoin(gState->apSettings->startingPartner);
            ttyd::mario_party::marioPartyHello(gState->apSettings->startingPartner);
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
    }

    bool checkIfInGame()
    {
        constexpr SeqIndex gameSeq = SeqIndex::kGame;
        if (seqGetNextSeq() != gameSeq)
        {
            return false;
        }

        if (seqGetSeq() != gameSeq)
        {
            return false;
        }

        const OSModuleInfo *relPtr = _globalWorkPtr->relocationBase;
        if (!relPtr)
        {
            return false;
        }

        return relPtr->id != RelId::DMO;
    }

    void OWR::HomewardWarp()
    {
        const Player *marioPtr = marioGetPtr();
        if (marioPtr->characterId != 0)
            return;
        if (!checkIfInGame())
            return;
        if ((marioStGetSystemLevel() & 15) == 15)
            return;

        uint32_t combo = PadInput::PAD_L | PadInput::PAD_R | PadInput::PAD_START;
        bool buttons = checkButtonCombo(combo);
        if (buttons)
        {
            ttyd::countdown::countDownEnd();
            ttyd::mario_motion::marioChgMot(ttyd::mario_motion::MarioMotion::kStay);
            uint32_t namePtr = 0x802c0298;
            const char *mapName = reinterpret_cast<char *>(namePtr);
            ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, mapName, 0);
        }
    }

    void OWR::RecieveItems()
    {
        SeqIndex CurrentIndex = ttyd::seqdrv::seqGetSeq();

        if (CurrentIndex != SeqIndex::kGame)
            return;

        uintptr_t item_pointer = 0x803DB864;
        uint32_t *item = reinterpret_cast<uint32_t *>(item_pointer);
        int value = *item;

        if (value != 0)
        {
            ttyd::mario_pouch::pouchGetItem(value);
            memset(reinterpret_cast<void *>(item_pointer), 0, sizeof(item_pointer));
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

    KEEP_FUNC void seqSetSeqHook(SeqIndex seq, const char *map, const char *bero)
    {
        // Check if map is equal to 1 so we dont call a strcmp with an invalid pointer
        if (map == reinterpret_cast<const char *>(1))
        {
            return g_seqSetSeq_trampoline(seq, map, bero);
        }

        if (map && !strcmp(map, "rsh_01_a"))
        {
            uint8_t value = ttyd::swdrv::swByteGet(1706);

            if ((value >= 8 && value < 14) || (value >= 29 && value < 30))
                map = "rsh_01_b";
            else if ((value >= 14 && value < 23) || (value >= 30 && value < 32))
                map = "rsh_01_c";
        }
        if (map && !strcmp(map, "rsh_02_a"))
        {
            uint8_t value = ttyd::swdrv::swByteGet(1706);

            if ((value >= 8 && value < 14) || (value >= 29 && value < 30))
                map = "rsh_02_b";
            else if ((value >= 14 && value < 23) || (value >= 30 && value < 32))
                map = "rsh_02_c";
        }
        if (map && !strcmp(map, "aaa_00"))
        {
            uint32_t namePtr = 0x802c0298;
            const char *mapName = reinterpret_cast<char *>(namePtr);
            return g_seqSetSeq_trampoline(seq, mapName, bero);
        }
        else if (map && !strncmp(map, "rsh", 3))
        {
            if (ttyd::swdrv::swByteGet(1706) < 43)
            {
                if (bero && (!strcmp(bero, "nidome") || !strcmp(bero, "nidome_start")))
                {
                    return g_seqSetSeq_trampoline(seq, map, 0);
                }
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

    KEEP_FUNC uint32_t pouchGetItemHook(int32_t item)
    {
        uint32_t return_value = 0;
        switch (item)
        {
            case ItemId::SUPER_LUIGI:
            {
                uint8_t member = static_cast<uint8_t>(ttyd::party::PartyMembers::Goombella);
                ttyd::mario_party::partyJoin(member);
                return_value = 1;
                break;
            }
            case ItemId::SUPER_LUIGI_2:
            {
                uint8_t member = static_cast<uint8_t>(ttyd::party::PartyMembers::Koops);
                ttyd::mario_party::partyJoin(member);
                return_value = 1;
                break;
            }
            case ItemId::SUPER_LUIGI_3:
            {
                uint8_t member = static_cast<uint8_t>(ttyd::party::PartyMembers::Flurrie);
                ttyd::mario_party::partyJoin(member);
                return_value = 1;
                break;
            }
            case ItemId::SUPER_LUIGI_4:
            {
                uint8_t member = static_cast<uint8_t>(ttyd::party::PartyMembers::Yoshi);
                ttyd::mario_party::partyJoin(member);
                return_value = 1;
                break;
            }
            case ItemId::SUPER_LUIGI_5:
            {
                uint8_t member = static_cast<uint8_t>(ttyd::party::PartyMembers::Vivian);
                ttyd::mario_party::partyJoin(member);
                return_value = 1;
                break;
            }
            case ItemId::INVALID_ITEM_006F:
            {
                uint8_t member = static_cast<uint8_t>(ttyd::party::PartyMembers::Bobbery);
                ttyd::mario_party::partyJoin(member);
                return_value = 1;
                break;
            }
            case ItemId::INVALID_ITEM_0070:
            {
                uint8_t member = static_cast<uint8_t>(ttyd::party::PartyMembers::MsMowz);
                ttyd::mario_party::partyJoin(member);
                return_value = 1;
                break;
            }
            case ItemId::INVALID_ITEM_0071:
            {
                return_value = 1;
                break;
            }
            case ItemId::INVALID_ITEM_PAPER_0053:
            {
                ttyd::mario_pouch::pouchAddCoin(10);
                return_value = 1;
                break;
            }
            case ItemId::BOOTS:
            {
                bool has_boots = ttyd::mario_pouch::pouchCheckItem(ItemId::BOOTS) > 0;
                bool has_sboots = ttyd::mario_pouch::pouchCheckItem(ItemId::SUPER_BOOTS) > 0;
                if (!has_boots)
                {
                    g_pouchGetItem_trampoline(item);
                    return_value = 1;
                    break;
                }
                if (!has_sboots)
                {
                    ttyd::mario_pouch::pouchGetItem(ItemId::SUPER_BOOTS);
                    return_value = 1;
                    break;
                }
                ttyd::mario_pouch::pouchGetItem(ItemId::ULTRA_BOOTS);
                return_value = 1;
                break;
            }
            case ItemId::HAMMER:
            {
                bool has_hammer = ttyd::mario_pouch::pouchCheckItem(ItemId::HAMMER) > 0;
                bool has_shammer = ttyd::mario_pouch::pouchCheckItem(ItemId::SUPER_HAMMER) > 0;
                if (!has_hammer)
                {
                    g_pouchGetItem_trampoline(item);
                    break;
                }
                if (!has_shammer)
                {
                    ttyd::mario_pouch::pouchGetItem(ItemId::SUPER_HAMMER);
                    return_value = 1;
                    break;
                }
                ttyd::mario_pouch::pouchGetItem(ItemId::ULTRA_HAMMER);
                return_value = 1;
                break;
            }
            case ItemId::COCONUT:
            {
                // If the player has already given the coconut to Flavio, then just give the coconut
                // normally
                if (ttyd::swdrv::swByteGet(1714) >= 4)
                {
                    return g_pouchGetItem_trampoline(item);
                }

                // Loop through all of the important items until either the coconut or an empty slot
                // is found
                constexpr uint32_t loopCount = sizeof(ttyd::mario_pouch::PouchData::key_items) / sizeof(int16_t);
                int16_t *keyItemsPtr = &ttyd::mario_pouch::pouchGetPtr()->key_items[0];

                for (uint32_t i = 0; i < loopCount; i++)
                {
                    const int32_t currentItem = keyItemsPtr[i];
                    if (currentItem == ItemId::COCONUT)
                    {
                        // The player already has the coconut in their important items, so just give
                        // it normally
                        return g_pouchGetItem_trampoline(item);
                    }
                    else if (currentItem == ItemId::INVALID_NONE)
                    {
                        // The player does not have the coconut and an empty slot was found, so
                        // place the coconut here
                        keyItemsPtr[i] = ItemId::COCONUT;
                        return static_cast<uint32_t>(1);
                    }
                }

                // If this is reached, then the important items part of the inventory is somehow
                // full, so do some failsafe or something
                return_value = 0;
                break;
            }
            default:
                return g_pouchGetItem_trampoline(item);
        }
        return return_value;
    }

    KEEP_FUNC void partySetForceMoveHook(ttyd::party::PartyEntry *ptr, float x, float z, float speed)
    {
        const Player *marioPtr = marioGetPtr();
        if (marioPtr->characterId == 0)
        {
            if (marioPtr->motionId == static_cast<uint32_t>(ttyd::mario_motion::MarioMotion::kYoshi))
                ttyd::mario_motion::marioChgMot(ttyd::mario_motion::MarioMotion::kStay);
        }
        return g_partySetForceMove_trampoline(ptr, x, z, speed);
    }

    KEEP_FUNC int32_t evtMarioSetPoseHook(ttyd::evtmgr::EvtEntry *evt, bool firstCall)
    {
        const Player *marioPtr = marioGetPtr();
        if (marioPtr->characterId == 0)
        {
            if (marioPtr->motionId == static_cast<uint32_t>(ttyd::mario_motion::MarioMotion::kYoshi))
                ttyd::mario_motion::marioChgMot(ttyd::mario_motion::MarioMotion::kStay);
        }
        return g_evt_mario_set_pose_trampoline(evt, firstCall);
    }

    void OWR::Update()
    {
        SequenceInit();
        RecieveItems();
        HomewardWarp();
    }

    void OWR::OnModuleLoaded(OSModuleInfo *module_info)
    {
        (void)module_info;

        RelMgr *relMgrPtr = &relMgr;

        // The vanilla rel is unloaded every time you go through a loading zone, so our custom one must be relinked
        const bool unlinked = relMgrPtr->unlinkRel();

        // If going into a new area, then load the new rel
        const char *nextArea = ttyd::seq_mapchange::_next_area;
        const bool inNewArea = relMgrPtr->inNewArea(nextArea);
        relMgrPtr->setPrevArea(nextArea);

        if (!unlinked || inNewArea)
        {
            relMgrPtr->unloadRel();

            if (!relMgrPtr->loadRel(nextArea))
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