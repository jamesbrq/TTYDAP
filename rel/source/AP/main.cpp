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
#include "ttyd/evt_cam.h"
#include "ttyd/evt_eff.h"
#include "ttyd/evt_item.h"
#include "ttyd/evt_lecture.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_memcard.h"
#include "ttyd/evt_msg.h"
#include "ttyd/evt_party.h"
#include "ttyd/evt_snd.h"
#include "ttyd/evt_sub.h"
#include "ttyd/evtmgr.h"
#include "ttyd/evtmgr_cmd.h"
#include "ttyd/icondrv.h"
#include "ttyd/itemdrv.h"
#include "ttyd/mario_cam.h"
#include "ttyd/mario_motion.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/seq_mapchange.h"
#include "ttyd/seqdrv.h"
#include "ttyd/swdrv.h"
#include "visibility.h"
#include <ttyd/evt_shop.h>

#include <cstdint>
#include <cstdio>
#include <cstring>

using namespace ttyd::evtmgr_cmd;
using namespace ttyd::common;
using namespace ttyd::battle;
using namespace ttyd::swdrv;
using namespace ttyd::seq_mapchange;
using namespace mod::owr;
using namespace ttyd;

namespace mod::owr
{
    KEEP_VAR const char *goombellaName = "name_party0";
    KEEP_VAR const char *goombellaDescription = "btl_hlp_cmd_operation_party1";
    KEEP_VAR const char *koopsName = "name_party1";
    KEEP_VAR const char *koopsDescription = "btl_hlp_cmd_operation_party2";
    KEEP_VAR const char *flurrieName = "name_party4";
    KEEP_VAR const char *flurrieDescription = "btl_hlp_cmd_operation_party3";
    KEEP_VAR const char *yoshiName = "name_party3";
    KEEP_VAR const char *yoshiDescription = "btl_hlp_cmd_operation_party4";
    KEEP_VAR const char *vivianName = "name_party5";
    KEEP_VAR const char *vivianDescription = "btl_hlp_cmd_operation_party5";
    KEEP_VAR const char *bobberyName = "name_party2";
    KEEP_VAR const char *bobberyDescription = "btl_hlp_cmd_operation_party6";
    KEEP_VAR const char *mowzName = "name_party6";
    KEEP_VAR const char *mowzDescription = "btl_hlp_cmd_operation_party7";
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

static const char *goods[] =
    {"gor_01", "gor_03", "tik_00", "nok_00", "mri_07", "tou_01", "usu_01", "muj_01", "rsh_03", "bom_02"};

static char result[100];

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

EVT_BEGIN_KEEP(starstone_item_evt)
    USER_FUNC(starstoneParamInit, LW(0), LW(1))
    USER_FUNC(marioGetRot, LW(2))
    USER_FUNC(evt_sub::stone_ry, LW(1), LW(2))
    SET(GSWF(6119), 1)
    USER_FUNC(evt_snd::evt_snd_bgmoff_f, 512, 2000)
    USER_FUNC(evt_snd::evt_snd_bgmoff, 513)
    USER_FUNC(evt_snd::evt_snd_envoff_f, 512, 2000)
    RUN_CHILD_EVT(evt_sub::evt_sub_starstone)
    RETURN()
EVT_END()

EVT_BEGIN_KEEP(starstone_end_evt)
    USER_FUNC(evt_snd::evt_snd_bgmoff, 513)
    SET(GSW(1725), 0)
    IF_NOT_EQUAL(GSWF(6119), 1)
        USER_FUNC(evt_snd::evt_snd_bgmon, 288, 0)
        USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_STAGE_CLEAR1"))
        RETURN()
    END_IF()
    USER_FUNC(starstoneParamClean)
    USER_FUNC(evt_item::evt_item_delete, LW(7))
    USER_FUNC(evt_eff::evt_eff_softdelete, PTR("sub_hikari"))
    USER_FUNC(evt_eff::evt_eff_delete, PTR("sub_bg"))
    USER_FUNC(evt_mario::evt_mario_set_pose, PTR("M_S_1"))
    USER_FUNC(evt_cam::evt_cam3d_evt_off, 500, 11)
    USER_FUNC(evt_party::evt_party_run, 0)
    USER_FUNC(evt_lecture::lect_set_systemlevel, 0)
    USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
    USER_FUNC(starstoneRunItemEvent)
    USER_FUNC(starstoneCheckGoalComplete)
    RETURN()
EVT_END()

EVT_BEGIN_KEEP(bero_las_deny)
    USER_FUNC(evt_mario::evt_mario_key_onoff, 0)
    USER_FUNC(lasStarsCheck, LW(7), LW(13))
    IF_EQUAL(LW(7), 1)
        USER_FUNC(evt_msg::evt_msg_print, 1, PTR("<system>\nYou do not have the\nrequired amount of crystal\nstars to proceed.\n<k>"), 0, 0)
        USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
        RETURN()
    END_IF()
    RUN_CHILD_EVT(&evt_bero::bero_case_init_check)
    USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
    RETURN()
EVT_END()

EVT_BEGIN_KEEP(starstone_cam_z)
    USER_FUNC(marioGetRot, LW(10))
    DIVF(LW(10), 90.0f)
    SWITCH(LW(10))
        CASE_EQUAL(0)
            SET(LW(11), 0)
            SET(LW(12), 397)
        CASE_EQUAL(1)
            SET(LW(11), -397)
            SET(LW(12), 0)
        CASE_EQUAL(2)
            SET(LW(11), 0)
            SET(LW(12), -397)
        CASE_EQUAL(3)
            SET(LW(11), 397)
            SET(LW(12), 0)
        CASE_ETC()
            SET(LW(11), 0)
            SET(LW(12), 397)
    END_SWITCH()
    SET(GSW(1725), LW(10))
    USER_FUNC(evt_cam::evt_cam3d_evt_set_rel, LW(11), 50, LW(12), 0, 70, 25, 2000, 11)
    RETURN()
EVT_END()

EVT_BEGIN_KEEP(starstone_eff_z)
    SWITCH(LW(10))
        CASE_EQUAL(0)
            ADD(LW(2), -10)
        CASE_EQUAL(1)
            ADD(LW(0), 10)
        CASE_EQUAL(2)
            ADD(LW(2), 10)
        CASE_EQUAL(3)
            ADD(LW(0), -10)
        CASE_ETC()
            ADD(LW(2), -10)
    END_SWITCH()
    RETURN()
EVT_END()

EVT_BEGIN_KEEP(starstone_rotation_evt)
    USER_FUNC(getStarstoneName, LW(1))
    USER_FUNC(marioGetRot, LW(2))
    USER_FUNC(evt_sub::stone_ry, LW(1), LW(2))
    RETURN()
EVT_END()

EVT_BEGIN_KEEP(starstone_interpolate_angle)
    SETF(LW(9), LW(3))
    USER_FUNC(marioGetRot, LW(11))
    SET(LW(12), 3600)
    SUB(LW(12), LW(11))
    RETURN()
EVT_END()

EVT_BEGIN_KEEP(starstone_item_handler)
    USER_FUNC(evt_mario::evt_mario_key_onoff, 0)
    USER_FUNC(evt_party::evt_party_stop, 0)
    USER_FUNC(pouchStarstoneItem, LW(2), LW(1), LW(0))
    RETURN()
EVT_END()

EVT_BEGIN_KEEP(starstone_item_z)
    USER_FUNC(evt_sub::stone_bg, LW(7))
    USER_FUNC(setIconRenderPriority, LW(7))
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
    gState->starItemPtr = nullptr;
    gState->starstoneName = nullptr;
    gState->starstoneFunctionPtr = nullptr;

    uint8_t count = 0;
    for (int i = 114; i <= 120; i++)
    {
        if (ttyd::mario_pouch::pouchCheckItem(i) > 0)
            count++;
    }

    if (gState->apSettings->goal == 2 && count >= gState->apSettings->goalStars && ttyd::swdrv::swGet(6120) == 0 &&
        ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[3]) != 1)
    {
        ttyd::swdrv::swSet(6120);
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], PTR("end_00"));
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[2], 0);
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 1);
        return 2;
    }

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
            ttyd::swdrv::swByteSet(1705, 11);
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

int getBlockVisibility(int brickType)
{
    if (gState->apSettings->blockVisibility == 1)
    {
        switch (brickType)
        {
            case BrickType::HIDDEN_EMPTY:
                return BrickType::EMPTY;
            case BrickType::HIDDEN_NORMAL:
                return BrickType::NORMAL;
            case BrickType::HIDDEN_RED:
                return BrickType::RED;
            case BrickType::HIDDEN_TEN_COIN:
                return BrickType::TEN_COIN;
            default:
                return brickType;
        }
    }
    return brickType;
}

const char* shopItemDescription(const char* itemDescription)
{
    char *base = reinterpret_cast<char *>(evt_shop::evt_shop_wp);
    uint32_t *itemIds = *reinterpret_cast<uint32_t **>(base + 0x08);
    uint32_t selectedIndex = *reinterpret_cast<uint32_t *>(base + 0x2C);

    if (itemIds[selectedIndex * 2] != 0x71) // AP Item
        return itemDescription;

    const char *nextMapPtr = &ttyd::seq_mapchange::_next_map[0];
    constexpr int loopCount = static_cast<int>(sizeof(goods) / sizeof(goods[0]));

    for (int i = 0; i < loopCount; i++)
    {
        if (strncmp(nextMapPtr, goods[i], 6) != 0)
            continue;
        snprintf(result, sizeof(result), "ap_%s_%d", goods[i], selectedIndex);
        return result;
    }
    return itemDescription;
}

int itemHandleStarstone(void* itemPtr)
{
    if (!itemPtr)
        return 0;

    if (gState->starItemPtr == itemPtr)
        return 1;

    int itemId = *(int *)((char *)itemPtr + 4);
    if (itemId < 114 || itemId > 120)
        return 0;
    gState->starItemPtr = itemPtr;
    ttyd::evtmgr::evtEntryType(const_cast<int32_t *>(starstone_item_evt), 30, 0, 26);
    ttyd::mario_motion::marioChgMot(ttyd::mario_motion::MarioMotion::kStay);
    return 1;
}

EVT_DEFINE_USER_FUNC_KEEP(starstoneParamInit)
{
    (void)isFirstCall;
    void *itemPtr = gState->starItemPtr;
    if (!itemPtr)
        return 0;

    int itemId = *(int *)((char *)itemPtr + 0x4);
    char *itemName = (char *)((char *)itemPtr + 0xC);
    ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], itemId - 113);
    ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], PTR(itemName));
    return 2;
}

EVT_DEFINE_USER_FUNC_KEEP(starstoneParamClean)
{
    (void)isFirstCall;
    (void)evt;

    // Set GSWF flag
    int32_t itemFlag = *(int32_t *)((char *)gState->starItemPtr + 0x8);
    if (itemFlag >= 0)
        swSet(itemFlag);

    // Set state to 9 (deletion state)
    *(unsigned short *)((char *)gState->starItemPtr + 0x24) = 9;
    *(unsigned short *)((char *)gState->starItemPtr + 0x26) = 0;
    ttyd::mario_cam::marioSetCamId(4);

    gState->starstoneFunctionPtr = *(int32_t **)((char *)gState->starItemPtr + 0x20);
    gState->starItemPtr = nullptr;
    return 2;
}

EVT_DEFINE_USER_FUNC_KEEP(lasStarsCheck)
{
    (void)isFirstCall;
    if (strcmp(reinterpret_cast<const char*>(ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[1])), "las_28") != 0)
    {
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 0);
        return 2;
    }
    ttyd::evtmgr_cmd::evtSetValue(evt,
                                  evt->evtArguments[0],
                                  gState->apSettings->collectedStars >= gState->apSettings->goalStars ? 0 : 1);
    return 2;
}

EVT_DEFINE_USER_FUNC_KEEP(marioGetRot) 
{
    (void)isFirstCall;
    ttyd::evtmgr_cmd::evtSetFloat(evt, evt->evtArguments[0], mario::marioGetPtr()->unk_19c + 1.0f);
    return 2;
}

void powerupBlkStarstoneRotation(const char *itemName, int itemId)
{
    if (itemId < 114 || itemId > 120)
        return;

    gState->starstoneName = itemName;
    ttyd::evtmgr::evtEntry(const_cast<int32_t *>(starstone_rotation_evt), 0, 0);
}

EVT_DEFINE_USER_FUNC_KEEP(getStarstoneName)
{
    (void)isFirstCall;
    ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], PTR(gState->starstoneName));
    return 2;
}

EVT_DEFINE_USER_FUNC_KEEP(pouchStarstoneItem)
{
    (void)isFirstCall;
    if (!gState->starItemPtr)
        gState->starItemPtr = itemdrv::itemNameToPtr((const char*)ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[1]));
    int itemId = *(int *)((char *)gState->starItemPtr + 0x4);
    if (ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[2]) != -1)
        mario_pouch::pouchGetItem(itemId);
    ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 0x80307224);
    return 2;
}

EVT_DEFINE_USER_FUNC_KEEP(setIconRenderPriority)
{
    (void)isFirstCall;
    void* iconPtr = icondrv::iconNameToPtr((const char*)evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]));
    if (iconPtr)
    {
        uint16_t *flags = (uint16_t *)iconPtr;
        *flags |= 0x0100;
    }
    return 2;
}

EVT_DEFINE_USER_FUNC_KEEP(starstoneRunItemEvent)
{
    (void)isFirstCall;
    (void)evt;
    if (!gState->starstoneFunctionPtr)
        return 2;
    ttyd::evtmgr::evtEntry(gState->starstoneFunctionPtr, 0, 0);
    gState->starstoneFunctionPtr = nullptr;
    return 2;
}

EVT_DEFINE_USER_FUNC_KEEP(starstoneCheckGoalComplete)
{
    (void)isFirstCall;
    (void)evt;
    uint8_t count = 0;
    for (int i = 114; i <= 120; i++)
    {
        if (ttyd::mario_pouch::pouchCheckItem(i) > 0)
            count++;
    }
    if (gState->apSettings->goal == 2 && count >= gState->apSettings->goalStars && ttyd::swdrv::swGet(6120) == 0)
    {
        ttyd::swdrv::swSet(6120);
        ttyd::seqdrv::seqSetSeq(SeqIndex::kMapChange, "end_00", 0);
    }
    return 2;
}
