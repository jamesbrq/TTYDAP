#include "AP/rel_patch_definitions.h"
#include "common.h"
#include "evt_cmd.h"
#include "OWR.h"
#include "patch.h"
#include "string.h"
#include "ttyd/battle.h"
#include "ttyd/battle_audience.h"
#include "ttyd/battle_event_cmd.h"
#include "ttyd/battle_unit.h"
#include "ttyd/evt_audience.h"
#include "ttyd/evt_bero.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_memcard.h"
#include "ttyd/evt_party.h"
#include "ttyd/evtmgr_cmd.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/seq_mapchange.h"
#include "ttyd/seqdrv.h"
#include "ttyd/swdrv.h"
#include "visibility.h"
#include <ttyd/evt_shop.h>

#include <cstdint>
#include <cstring>
#include <cstdio>

using namespace ttyd::evtmgr_cmd;
using namespace ttyd::common;
using namespace ttyd::battle;
using namespace ttyd::swdrv;
using namespace ttyd::seq_mapchange;
using namespace mod::owr;
using namespace ttyd;

using mod::owr::goods;

namespace mod::owr
{
    KEEP_VAR const char *apItemName = "ap_item";
    KEEP_VAR const char *apItemDescription = "ap_item_desc";
    KEEP_VAR const char *tenCoinsName = "10_coins";
    KEEP_VAR const char *tenCoinsDescription = "10_coins_desc";
    KEEP_VAR const char *returnPipeName = "return_pipe";
    KEEP_VAR const char *returnPipeDescription = "return_pipe_desc";
    KEEP_VAR const char *planeModeName = "plane_mode";
    KEEP_VAR const char *planeModeDescription = "plane_mode_desc";
    KEEP_VAR const char *paperModeName = "paper_mode";
    KEEP_VAR const char *paperModeDescription = "paper_mode_desc";
    KEEP_VAR const char *tubeModeName = "tube_mode";
    KEEP_VAR const char *tubeModeDescription = "tube_mode_desc";
    KEEP_VAR const char *boatModeName = "boat_mode";
    KEEP_VAR const char *boatModeDescription = "boat_mode_desc";
    KEEP_VAR const char *rshNode = "rsh";
    KEEP_VAR const char *las_09Node = "las_09";

    // Key Renames
    KEEP_VAR const char *elevatorKeyStationName = "elevator_key_station";
    KEEP_VAR const char *cardKey1Name = "card_key_1";
    KEEP_VAR const char *cardKey2Name = "card_key_2";
    KEEP_VAR const char *cardKey3Name = "card_key_3";
    KEEP_VAR const char *cardKey4Name = "card_key_4";
    KEEP_VAR const char *palaceKeyTowerName = "palace_key_tower";
    KEEP_VAR const char *elevatorKey1Name = "elevator_key_1";
    KEEP_VAR const char *elevatorKey2Name = "elevator_key_2";
    KEEP_VAR const char *stationKey1Name = "station_key_1";
    KEEP_VAR const char *stationKey2Name = "station_key_2";
    KEEP_VAR const char *storageKey1Name = "storage_key_1";
    KEEP_VAR const char *storageKey2Name = "storage_key_2";
    KEEP_VAR const char *blackKeyPlaneName = "black_key_plane";
    KEEP_VAR const char *blackKeyPaperName = "black_key_paper";
    KEEP_VAR const char *blackKeyTubeName = "black_key_tube";
    KEEP_VAR const char *blackKeyBoatName = "black_key_boat";

    //Progressive Renames
    KEEP_VAR const char *progressiveBootsName = "progressive_boots";
    KEEP_VAR const char *progressiveHammerName = "progressive_hammer";

} // namespace mod::owr

extern int32_t btlataudevtPresentItem_Get[];

// clang-format off
EVT_BEGIN_KEEP(main_buy_evt_evt)
    USER_FUNC(ttyd::evt_shop::get_ptr, LW(0))
    USER_FUNC(setShopFlags, LW(0))
    USER_FUNC(ttyd::evt_shop::get_buy_evt, LW(1))
    IF_NOT_EQUAL(LW(1), 0)
        SET(LW(0), LW(12))
        RUN_CHILD_EVT(LW(1))
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN_KEEP(main_evt_sub_starstone_evt)
    USER_FUNC(handleIntermissionSkip, LW(1), LW(2), LW(3), LF(8))
    IF_EQUAL(LW(1), 1)
        USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
        USER_FUNC(evt_bero::evt_bero_mapchange, LW(2), LW(3))
        RETURN()
    END_IF()
    SET(LF(10), 0)
    RUN_CHILD_EVT(&evt_memcard::unk_evt_803bac3c)
    USER_FUNC(evt_mario::evt_mario_init_camid)
    USER_FUNC(evt_party::evt_party_init_camid, 0)
    SET(LW(1), 0)
    RETURN()
EVT_END()

EVT_BEGIN_KEEP(main_partyChristineAttack_Monosiri_evt)
    USER_FUNC(ttyd::evt_audience::evt_audience_ap_recovery)
    USER_FUNC(ttyd::battle_event_cmd::btlevtcmd_InviteApInfoReport)
    USER_FUNC(checkTattleItem, LW(0))
    IF_EQUAL(LW(0), 1)
        RUN_CHILD_EVT(&btlataudevtPresentItem_Get)
    END_IF()
    RETURN()
EVT_END()
// clang-format on

EVT_DEFINE_USER_FUNC_KEEP(setShopFlags)
{
    (void)isFirstCall;

    int gswfBase = 6200;
    const char *nextMapPtr = &ttyd::seq_mapchange::_next_map[0];
    constexpr int loopCount = static_cast<int>(sizeof(goods) / sizeof(goods[0]));

    for (int i = 0; i < loopCount; i++)
    {
        if (strncmp(nextMapPtr, goods[i], 6) != 0)
        {
            if (i == loopCount - 1)
                return 2;

            gswfBase += 6;
            continue;
        }
        break;
    }

    char *shopWork = reinterpret_cast<char *>(evtGetValue(evt, evt->evtArguments[0]));
    uint32_t *itemIds = *reinterpret_cast<uint32_t **>(shopWork + 0x08);
    uint16_t *itemFlags = reinterpret_cast<uint16_t *>(shopWork + 0x14);
    uint32_t selectedIndex = *reinterpret_cast<uint32_t *>(shopWork + 0x2C);

    swSet(gswfBase + selectedIndex);

    if (itemIds[selectedIndex * 2] > 125)
        return 2;

    itemFlags[selectedIndex] |= 1;
    return 2;
}

EVT_DEFINE_USER_FUNC_KEEP(evt_msg_numselect)
{
    if (isFirstCall)
    {
        const char *messageText = reinterpret_cast<const char *>(ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]));
        ttyd::msgdrv::msgWindow_Entry(messageText, 0, 0);
        return 0; // Continue - window is opening
    }

    // Check if the numeric input is still active
    NumericInputData *numericInputPtr = &g_numericInput;
    if (numericInputPtr->active)
    {
        return 0; // Continue waiting - user hasn't made a selection yet
    }

    // User has made a selection (A or B pressed)
    ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], numericInputPtr->selectedValue);
    ttyd::windowdrv::windowDeleteID(numericInputPtr->window_id);

    // Clean up our global state
    numericInputPtr->clearState();
    return 2; // Event complete
}

EVT_DEFINE_USER_FUNC_KEEP(handleIntermissionSkip)
{
    (void)isFirstCall;

    const bool intermissions = static_cast<bool>(gState->apSettings->intermissions);
    if (!intermissions || ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[3]) == 1)
    {
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 0);
        return 2;
    }

    if (!strcmp(_next_area, "gon"))
    {
        ttyd::swdrv::swByteSet(1711, 17);
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], PTR("gon_10"));
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[2], PTR("w_bero"));
    }
    else if (!strcmp(_next_area, "mri"))
    {
        ttyd::swdrv::swByteSet(1713, 20);
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], PTR("mri_00"));
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[2], PTR("ne_bero"));
    }
    else if (!strcmp(_next_area, "tou"))
    {
        ttyd::swdrv::swByteSet(1703, 31);
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], PTR("tou_01"));
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[2], PTR("a_door_mon"));
    }
    else if (!strcmp(_next_area, "jin"))
    {
        ttyd::swdrv::swByteSet(1715, 17);
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], PTR("jin_00"));
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[2], PTR("n_bero_2"));
    }
    else if (!strcmp(_next_area, "muj"))
    {
        ttyd::swdrv::swByteSet(1717, 29);
        if (ttyd::swdrv::swByteGet(1705) < 7)
            ttyd::swdrv::swByteSet(1705, 10);
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], PTR("muj_01"));
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[2], PTR("n_bero_02"));
    }
    else if (!strcmp(_next_area, "pik"))
    {
        ttyd::swdrv::swByteSet(1706, 53);
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], PTR("pik_03"));
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[2], PTR("next"));
    }
    else if (!strcmp(_next_area, "aji"))
    {
        ttyd::swdrv::swByteSet(1707, 21);
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], PTR("aji_18"));
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[2], PTR("e_bero"));
    }

    ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 1);
    return 2;
}

EVT_DEFINE_USER_FUNC_KEEP(checkTattleItem)
{
    (void)isFirstCall;

    evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], static_cast<uint8_t>(gState->newTattle));
    gState->newTattle = false;
    return 2;
}

void checkShopFlag(uint32_t item, uint32_t index)
{
    if (item > 125)
        return;

    int gswfBase = 6200;
    const char *nextMapPtr = &ttyd::seq_mapchange::_next_map[0];
    constexpr int loopCount = static_cast<int>(sizeof(goods) / sizeof(goods[0]));

    for (int i = 0; i < loopCount; i++)
    {
        if (strncmp(nextMapPtr, goods[i], 6) != 0)
        {
            if (i == loopCount - 1)
                return;

            gswfBase += 6;
            continue;
        }
        break;
    }

    uintptr_t shopWorkPtr = 0x8041EB60;
    char *shopWork = *reinterpret_cast<char **>(shopWorkPtr);
    uint16_t *itemFlags = reinterpret_cast<uint16_t *>(shopWork + 0x14);

    if (swGet(gswfBase + index))
        itemFlags[index] |= 1;
}

void monosiriItemCheck(int unit_id)
{
    if (ttyd::swdrv::swGet(unit_id + 0x117A) || gState->apSettings->tattlesanity == 0)
        return;

    gState->newTattle = true;
    ttyd::battle_audience::BattleAudience_SetPresentTargetUnitId(
        ttyd::battle_unit::BtlUnit_GetUnitId(BattleGetPartyPtr(_battleWorkPtr))); // Goombella
    ttyd::battle_audience::BattleAudience_SetPresentItemNo(gState->tattleItems[unit_id - 1]);
    ttyd::battle_audience::BattleAudience_SetPresentItemType(0); // Non-damaging items
}

int applyExpMultiplier(int exp)
{
    return exp * gState->apSettings->expMultiplier;
}
