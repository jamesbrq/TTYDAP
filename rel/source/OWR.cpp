#include <AP/rel_patch_definitions.h>
#include <cstdint>
#include <cstring>
#include <gc/OSLink.h>
#include <gc/pad.h>
#include <mod.h>
#include <ttyd/common_types.h>
#include <ttyd/mario_party.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/msgdrv.h>
#include <ttyd/party.h>
#include <ttyd/seqdrv.h>
#include <ttyd/string.h>
#include <ttyd/swdrv.h>

#include "common.h"
#include "OWR.h"
#include "patch.h"
#include <ttyd/fontmgr.h>

using gc::OSLink::OSModuleInfo;
using gc::pad::PadInput;
using ::ttyd::common::ItemData;
using ::ttyd::common::ShopItemData;
using namespace ::ttyd::common;
using ::ttyd::seqdrv::SeqIndex;
using namespace ::mod::util;

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
    1925};
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
    void (*g_stg0_00_init_trampoline)() = nullptr;
    uint32_t (*g_pouchGetItem_trampoline)(int32_t) = nullptr;
    const char *(*g_msgSearch_trampoline)(const char *) = nullptr;

    void OWR::SequenceInit()
    {
        SeqIndex Seq = ttyd::seqdrv::seqGetNextSeq();

        if (Seq != SeqIndex::kMapChange)
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
    }

    void OWR::AAAToGOR()
    {
        uint32_t aaa_00_Address = 0x802EDE78;
        SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
        SeqIndex Load = ttyd::seqdrv::SeqIndex::kLoad;

        if (NextSeq == Load)
        {
            strcpy_String(reinterpret_cast<char *>(aaa_00_Address), "gor_01");
        }
    }

    void OWR::HomewardWarp()
    {
        SeqIndex seq = ttyd::seqdrv::seqGetSeq();
        if (seq != SeqIndex::kGame)
            return;

        uint32_t combo = PadInput::PAD_L | PadInput::PAD_R | PadInput::PAD_START;
        bool buttons = checkButtonCombo(combo);
        if (buttons)
        {
            uint32_t namePtr = 0x802c0298;
            void *mapName = reinterpret_cast<char *>(namePtr);
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

        g_OSLink_trampoline = patch::hookFunction(gc::OSLink::OSLink,
                                                  [](OSModuleInfo *new_module, void *bss)
                                                  {
                                                      bool result = g_OSLink_trampoline(new_module, bss);
                                                      if (new_module != nullptr && result)
                                                      {
                                                          gSelf->OnModuleLoaded(new_module);
                                                      }
                                                      return result;
                                                  });

        g_msgSearch_trampoline = patch::hookFunction(ttyd::msgdrv::msgSearch,
                                                     [](const char *msgKey)
                                                     {
                                                         if (!strcmp(msgKey, "jolene_fukidashi"))
                                                         {
                                                             return "Oh my!<wait 100> Excuse me.\n<k>";
                                                         }
                                                         if (!strcmp(msgKey, "stg6_rsh_diary_01"))
                                                         {
                                                             return "An unseen force prevents\nyou from opening the diary.<o>";
                                                         }
                                                         if (!strcmp(msgKey, "jolene_fukidashi_end"))
                                                         {
                                                             return "Well then.<wait 100> Shall we\nget going?\n<k>";
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
                                        default:
                                            return_value = g_pouchGetItem_trampoline(item);
                                    }
                                    return return_value;
                                });
    }

    void OWR::Update()
    {
        SequenceInit();
        AAAToGOR();
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
