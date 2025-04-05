#include <AP/rel_patch_definitions.h>
#include <cstdint>
#include <cstring>
#include <gc/OSModule.h>
#include <gc/pad.h>
#include <mod.h>
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
using ttyd::seqdrv::SeqIndex;
using ttyd::seq_mapchange::RelId;
using ttyd::seq_mapchange::_next_area;
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
    3880
};
constexpr int32_t GSWF_ARR_SIZE = sizeof(GSWF_ARR) / sizeof(GSWF_ARR[0]);

namespace mod::owr
{
    ItemData *item_db = common::kItemDataArr;
    OWR *gSelf = nullptr;
    StateManager *gState = nullptr;

    OWR::OWR()
    {
    }

    void *(*g_itemEntry_trampoline)(const char *, uint32_t, uint32_t, int32_t, void *, float, float, float) = nullptr;
    bool (*g_OSLink_trampoline)(OSModuleInfo *, void *) = nullptr;
    void (*g_seqSetSeq_trampoline)(SeqIndex seq, const char *map, const char *bero) = nullptr;
    void (*g_stg0_00_init_trampoline)() = nullptr;
    uint32_t (*g_pouchGetItem_trampoline)(int32_t) = nullptr;
    int32_t (*g_pouchRemoveItem_trampoline)(int32_t) = nullptr;
    void (*g_partySetForceMove_trampoline)(ttyd::party::PartyEntry *ptr, float x, float z, float speed) = nullptr;
    int32_t (*g_evt_mario_set_pose_trampoline)(ttyd::evtmgr::EvtEntry *evt, bool firstCall) = nullptr;
    const char *(*g_msgSearch_trampoline)(const char *) = nullptr;

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

        ttyd::swdrv::swByteSet(0, 1); // Set GSW(0) to 1 so that saving shows playtime
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

    void OWR::Init()
    {
        gSelf = this;
        gState = &gSelf->state;

        ApplyMainAssemblyPatches();
        ApplyMainScriptPatches();
        ApplyItemDataTablePatches();

        g_OSLink_trampoline = patch::hookFunction(OSLink,
                                                  [](OSModuleInfo *new_module, void *bss)
                                                  {
                                                      bool result = g_OSLink_trampoline(new_module, bss);
                                                      if (new_module != nullptr && result)
                                                      {
                                                          gSelf->OnModuleLoaded(new_module);
                                                      }
                                                      return result;
                                                  });

		g_seqSetSeq_trampoline = patch::hookFunction(ttyd::seqdrv::seqSetSeq,
                                                     [](SeqIndex seq, const char *map, const char *bero)
													  {
                                                        // Check if map is equal to 1 so we dont call a strcmp with an invalid pointer
                                                        if (map == reinterpret_cast<const char *>(1))
                                                        {
                                                            g_seqSetSeq_trampoline(seq, map, bero);
                                                        }
                                                        else if (map && strcmp(map, "aaa_00") == 0)
                                                        {
                                                            uint32_t namePtr = 0x802c0298;
                                                            const char *mapName = reinterpret_cast<char *>(namePtr);
                                                            g_seqSetSeq_trampoline(seq, mapName, bero);
                                                        }
                                                        else
                                                        {
                                                            g_seqSetSeq_trampoline(seq, map, bero);
                                                        }
													  });

        g_msgSearch_trampoline = patch::hookFunction(ttyd::msgdrv::msgSearch,
                                                     [](const char *msgKey)
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
                                                                 const char *message =
                                                                     "<select 0 0 300 40>Dang\nShoot";
                                                         #else
                                                                 const char *message =
                                                                     "<select 0 0 0 40>Dang\nShoot";
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
													  });

        g_pouchGetItem_trampoline =
            patch::hookFunction(ttyd::mario_pouch::pouchGetItem,
                                [](int32_t item)
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
                                });

                                g_pouchRemoveItem_trampoline = patch::hookFunction(ttyd::mario_pouch::pouchRemoveItem, [](int32_t item_data)
                                { 
                                        bool coconut_found = false;
                                        switch (item_data)
                                        {
                                            case ItemId::COCONUT:
                                            {
                                                if (ttyd::swdrv::swByteGet(1714) >= 4)
                                                    return g_pouchRemoveItem_trampoline(item_data);

                                                if (ttyd::mario_pouch::pouchCheckItem(ItemId::COCONUT) != 0)
                                                    return g_pouchRemoveItem_trampoline(item_data); 

                                                constexpr uint32_t loopCount = sizeof(ttyd::mario_pouch::PouchData::key_items) / sizeof(int16_t);
                                                int16_t *keyItemsPtr = &ttyd::mario_pouch::pouchGetPtr()->key_items[0];

                                                for (uint32_t i = 0; i < loopCount; i++)
                                                {
                                                    const int32_t currentItem = keyItemsPtr[i];
                                                    if (coconut_found)
                                                    {
                                                        if (currentItem != ItemId::INVALID_NONE)
                                                        {
     
                                                            keyItemsPtr[i - 1] = currentItem;
                                                            if (i == loopCount - 1)
                                                            {
                                                                keyItemsPtr[i] = ItemId::INVALID_NONE;
                                                                return 1;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            keyItemsPtr[i - 1] = ItemId::INVALID_NONE;
                                                            if (i == loopCount - 1)
                                                            {
                                                                keyItemsPtr[i] = ItemId::INVALID_NONE;
                                                            }
                                                            return 1;
                                                        }
                                                    }
                                                    else if (currentItem == ItemId::COCONUT)
                                                    {
                                                        keyItemsPtr[i] = ItemId::INVALID_NONE;
                                                        coconut_found = true;
                                                    }
                                                }

                                                // If this is reached, then the there is no coconut key item in the inventory
                                                return g_pouchRemoveItem_trampoline(item_data);
                                            }
                                            default:
                                                return g_pouchRemoveItem_trampoline(item_data);
                                        }
                                });

                                g_partySetForceMove_trampoline =patch::hookFunction(ttyd::party::partySetForceMove,
                                                        [](ttyd::party::PartyEntry *ptr, float x, float z, float speed)
                                                        {
                                                            const Player *marioPtr = marioGetPtr();
                                                            if (marioPtr->characterId != 0)
                                                                return;

                                                            if (marioPtr->motionId == static_cast<uint32_t>(ttyd::mario_motion::MarioMotion::kYoshi))
                                                                ttyd::mario_motion::marioChgMot(ttyd::mario_motion::MarioMotion::kStay);

                                                            g_partySetForceMove_trampoline(ptr, x, z, speed);
                                                        });

                                g_evt_mario_set_pose_trampoline = patch::hookFunction(ttyd::evt_mario::evt_mario_set_pose,
                                                        [](ttyd::evtmgr::EvtEntry *evt, bool firstCall)
                                                        {
                                                            const Player *marioPtr = marioGetPtr();
                                                            if (marioPtr->characterId == 0)
                                                            {
                                                                if (marioPtr->motionId == static_cast<uint32_t>(ttyd::mario_motion::MarioMotion::kYoshi))
                                                                    ttyd::mario_motion::marioChgMot(ttyd::mario_motion::MarioMotion::kStay);
                                                            }
                                                            return g_evt_mario_set_pose_trampoline(evt, firstCall);
                                                        });


    }

    void OWR::Update()
    {
        SequenceInit();
        RecieveItems();
        HomewardWarp();
    }

    void OWR::OnModuleLoaded(OSModuleInfo *module_info)
    {
        if (module_info == nullptr)
            return;
        switch (module_info->id)
        {
            case ModuleId::GOR:
                ApplyGorPatches();
                break;
            case ModuleId::HEI:
                ApplyHeiPatches();
                break;
            case ModuleId::GON:
                ApplyGonPatches();
                break;
            case ModuleId::NOK:
                ApplyNokPatches();
                break;
            case ModuleId::WIN:
                ApplyWinPatches();
                break;
            case ModuleId::MRI:
                ApplyMriPatches();
                break;
            case ModuleId::TOU:
                ApplyTouPatches();
                break;
            case ModuleId::TOU2:
                ApplyTou2Patches();
                break;
            case ModuleId::USU:
                ApplyUsuPatches();
                break;
            case ModuleId::GRA:
                ApplyGraPatches();
                break;
            case ModuleId::JIN:
                ApplyJinPatches();
                break;
            case ModuleId::MUJ:
                ApplyMujPatches();
                break;
            case ModuleId::DOU:
                ApplyDouPatches();
                break;
            case ModuleId::RSH:
                ApplyRshPatches();
                break;
            case ModuleId::EKI:
                ApplyEkiPatches();
                break;
            case ModuleId::END:
                ApplyEndPatches();
                break;
            case ModuleId::HOM:
                ApplyHomPatches();
                break;
            case ModuleId::PIK:
                ApplyPikPatches();
                break;
            case ModuleId::BOM:
                ApplyBomPatches();
                break;
            case ModuleId::AJI:
                ApplyAjiPatches();
                break;
            case ModuleId::AAA:
                ApplyAaaPatches();
                break;
            case ModuleId::KPA:
                ApplyKpaPatches();
                break;
            case ModuleId::TIK:
                ApplyTikPatches();
                break;
            case ModuleId::LAS:
                ApplyLasPatches();
                break;
            default:
                break;
        }
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
