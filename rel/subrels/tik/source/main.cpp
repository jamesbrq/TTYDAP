#include "subrel_tik.h"
#include "evt_cmd.h"
#include "mod.h"
#include "OWR.h"
#include "patch.h"
#include "AP/rel_patch_definitions.h"
#include "ttyd/evt_bero.h"
#include "ttyd/evt_cam.h"
#include "ttyd/evt_case.h"
#include "ttyd/evt_item.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_mobj.h"
#include "ttyd/evt_msg.h"
#include "ttyd/evt_npc.h"
#include "ttyd/evt_pouch.h"
#include "ttyd/evt_window.h"
#include "ttyd/evtmgr_cmd.h"
#include "ttyd/mapdrv.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/swdrv.h"
#include "ttyd/tik.h"

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cinttypes>

using namespace ttyd;

extern int32_t tik_uranaisi_next_evt[];
extern int32_t unk_evt_tik_0000f448[];
extern int32_t tik_kuribo5_talk[];
extern int32_t tik_chusan4_talk[];
extern int32_t tik_borodo3_talk[];
extern int32_t tik_borodo4_talk[];
extern int32_t tik_pinkbom_talk[];
extern int32_t tik_master_2_talk[];
extern int32_t tik_evt_open_04_open[];
extern int32_t tik_evt_door_04_close[];
extern int32_t tik_evt_door_stg7_open[];
extern int32_t tik_tensou_init_event[];
extern int32_t tik_tensou_sw_event[];
extern int32_t tik_kurihakase_event[];
extern int32_t tik_00_init_evt[];
extern int32_t tik_dokan_2_in_evt[];
extern int32_t tik_gorotuki_sanninngumi[];
extern int32_t tik_01_init_evt[];
extern int32_t tik_geso_battle[];
extern int32_t tik_02_init_evt[];
extern int32_t tik_bero_05_custom_in[];
extern int32_t tik_bero_05_custom_out[];
extern int32_t tik_guide_init[];
extern int32_t tik_guide_talk[];
extern int32_t tik_puni_soguu_3[];
extern int32_t tik_puni_switch[];
extern int32_t tik_puni_nakama[];
extern int32_t tik_03_init_evt[];
extern int32_t tik_puni_soguu_1[];
extern int32_t tik_puni_soguu_2[];
extern int32_t tik_04_switch_evt[];
extern int32_t tik_04_init_evt[];
extern int32_t tik_kurihakase_init[];
extern int32_t tik_kurihakase_talk[];
extern int32_t tik_large_door_stg0_init[];
extern int32_t tik_large_door_stg0[];
extern int32_t tik_look_large_door[];
extern int32_t tik_kaidan_event[];
extern int32_t tik_mahojin_event[];
extern int32_t tik_mahojin_event1[];
extern int32_t tik_mahojin_event2[];
extern int32_t tik_mahojin_event3[];
extern int32_t tik_mahojin_event4[];
extern int32_t tik_mahojin_event5[];
extern int32_t tik_mahojin_event6[];
extern int32_t tik_lastdungeon_anim_init[];
extern int32_t tik_lastdungeon_init[];
extern int32_t tik_lastdungeon[];
extern int32_t tik_mahojin_event7[];
extern int32_t tik_05_init_evt[];
extern int32_t tik_06_init_evt[];
extern int32_t tik_peach_mail[];
extern int32_t tik_07_init_evt[];
extern int32_t tik_08_init_evt[];
extern int32_t tik_15_init_evt[];
extern int32_t tik_18_init_evt[];
extern int32_t tik_evt_majin2[];
extern int32_t tik_starmaniac_talk[];

int maniacPrices[] = {1, 2, 3, 4, 4, 4, 5, 6, 6, 7, 8, 10, 10, 15, 15};

EVT_DEFINE_USER_FUNC(checkChapterRequirements)
{
    (void)isFirstCall;

    int8_t count = 0;
    for (int i = 114; i <= 120; i++)
    {
        if (mario_pouch::pouchCheckItem(i) > 0)
            count++;
    }
    if (count >= mod::owr::gState->apSettings->palaceStars)
        evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 1);
    else
        evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 0);
    return 2;
}

EVT_DEFINE_USER_FUNC(doorStarsCheck)
{
    (void)isFirstCall;
    (void)evt;

    static char animNameBuffer[16];
    char *animName = animNameBuffer;

    for (uint8_t i = 0; i < (sizeof(mod::owr::gState->apSettings->requiredStars) / sizeof(uint8_t)); i++)
    {
        sprintf(animName, "anm_hosi_%" PRIu32, mod::owr::gState->apSettings->requiredStars[i]);
        mapdrv::mapPlayAnimationLv(animName, 2, 0);
    }
    return 2;
}

EVT_DEFINE_USER_FUNC(pitCheckpointBuffer)
{
    (void)isFirstCall;

    static char selectBuffer[32];
    char *selectPrompt = selectBuffer;
    int value = ttyd::swdrv::swByteGet(33) - (ttyd::swdrv::swByteGet(33) % 10);
    ttyd::swdrv::swByteSet(1724, value);
    sprintf(selectPrompt, "<numselect 10 %d 10 10>\n100", value);

	evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], (uint32_t)selectPrompt);
    return 2;
}

EVT_DEFINE_USER_FUNC(getManiacItem)
{
    (void)isFirstCall;

    int itemNum = evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]);
    evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], (uint32_t)mod::owr::gState->maniacItems[itemNum - 1]);
    return 2;
}

EVT_DEFINE_USER_FUNC(getManiacPrice)
{
    (void)isFirstCall;

    int itemNum = evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]);
    evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], maniacPrices[itemNum - 1]);
    return 2;
}

// clang-format off
EVT_BEGIN(kuribo5_talk_evt)
	IF_SMALL(GSW(1711), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1713), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_02"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1703), 20)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_03"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 10)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_04"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1717), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_05"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1706), 43)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_06"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 16)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 17)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_08"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_09"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(kuribo5_talk_hook)
	RUN_CHILD_EVT(kuribo5_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(chusan4_talk_evt)
	IF_SMALL(GSW(1711), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1713), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_02"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1703), 20)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_03"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 10)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_04"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1717), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_05"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1706), 43)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_06"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 16)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 17)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_08"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_09"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(chusan4_talk_hook)
	RUN_CHILD_EVT(chusan4_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(borodo3_talk_evt)
	IF_SMALL(GSW(1711), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1713), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_02"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1703), 20)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_03"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 10)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_04"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1717), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_05"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1706), 43)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_06"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 16)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 17)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_08"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_09"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(borodo3_talk_hook)
	RUN_CHILD_EVT(borodo3_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(borodo4_talk_evt)
	IF_SMALL(GSW(1711), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1713), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_02"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1703), 20)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_03"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 10)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_04"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1717), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_05"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1706), 43)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_06"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 16)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 17)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_08"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_09"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(borodo4_talk_hook)
	RUN_CHILD_EVT(borodo4_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(master_2_talk_evt)
	IF_SMALL(GSW(1711), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_10"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1713), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_11"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1703), 20)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_12"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 10)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_13"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1717), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_14"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1706), 43)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_15"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 16)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_16"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 17)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_17"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_18"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(master_2_talk_hook)
	RUN_CHILD_EVT(master_2_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(tik_kurihakase_init_evt)
	USER_FUNC(checkChapterRequirements, LW(0))
	IF_EQUAL(LW(0), 1)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -30, 23, -190)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1708), 1)
		IF_SMALL_EQUAL(GSW(1708), 6)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -30, 23, -170)
			RETURN()
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(tik_kurihakase_init_hook)
	RUN_CHILD_EVT(tik_kurihakase_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(tik_05_init_evt_evt)
	IF_SMALL(GSW(1708), 1)
		USER_FUNC(doorStarsCheck)
		USER_FUNC(checkChapterRequirements, LW(0))
		IF_EQUAL(LW(0), 1)
			USER_FUNC(evt_case::evt_run_case_evt, 0, 1, PTR("a_dai"), 0, PTR(&tik_mahojin_event7), 0)
			RETURN()
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(tik_05_init_evt_hook)
	RUN_CHILD_EVT(tik_05_init_evt_evt)
	GOTO(&tik_05_init_evt[145])
EVT_PATCH_END()

EVT_BEGIN(tik_05_init_evt_evt2)
	IF_SMALL(GSW(1708), 1)
		SET(LW(0), PTR("n_bero"))
		RUN_CHILD_EVT(evt_bero::bero_case_switch_off)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1708), 1)
		SET(LW(0), PTR("n_bero"))
		RUN_CHILD_EVT(evt_bero::bero_case_switch_off)
		RUN_CHILD_EVT(tik_lastdungeon_anim_init)
		RUN_CHILD_EVT(tik_lastdungeon_init)
		USER_FUNC(evt_case::evt_run_case_evt, 10, 1, PTR("n_bero"), 0, PTR(&tik_lastdungeon), 0)
		RETURN()
	END_IF()
	IF_LARGE(GSW(1708), 1)
		RUN_CHILD_EVT(tik_lastdungeon_anim_init)
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(tik_05_init_evt_hook2)
	RUN_CHILD_EVT(tik_05_init_evt_evt2)
	GOTO(&tik_05_init_evt[187])
EVT_PATCH_END()

EVT_BEGIN(tik_evt_majin2_item)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 0)
	USER_FUNC(evt_mobj::evt_mobj_wait_animation_end, PTR("box"))
	USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
	USER_FUNC(evt_item::evt_item_entry, PTR("item01"), LW(3), LW(0), LW(1), LW(2), 16, -1, 0)
	USER_FUNC(evt_item::evt_item_get_item, PTR("item01"))
	WAIT_MSEC(800)
	SET(GSWF(6055), 1)
	SET(GSWF(1352), 1)
	USER_FUNC(evt_cam::evt_cam3d_evt_off, 500, 11)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
	RETURN()
EVT_END()

EVT_BEGIN(tik_pit_checkpoint_evt)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 0)
	USER_FUNC(evt_mario::evt_mario_normalize)
	IF_SMALL(GSWF(6117), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("pit_checkpoint_explain"), 0, 0)
		SET(GSWF(6117), 1)
	END_IF()
	IF_SMALL(GSW(33), 10)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("pit_checkpoint_deny"), 0, 0)
		USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("pit_checkpoint"), 0, 0)
	USER_FUNC(pitCheckpointBuffer, LW(14))
	USER_FUNC(evt_msg_numselect, LW(14), LW(0))
	IF_SMALL_EQUAL(LW(0), 0)
		USER_FUNC(evt_msg::evt_msg_continue)
		USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
		RETURN()
	END_IF()
	SUB(LW(0), 1)
	SET(GSW(1321), LW(0))
	USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("pit_checkpoint_set"))
	USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
	RETURN()
EVT_END()

EVT_BEGIN(starmaniac_item_get)
	USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(3))
	USER_FUNC(getManiacItem, LW(2), LW(4))
	USER_FUNC(evt_item::evt_item_entry, PTR("maniac_item"), LW(4), LW(0), LW(1), LW(3), 16, -1, 0)
	USER_FUNC(evt_item::evt_item_get_item, PTR("maniac_item"))
	WAIT_MSEC(800)
	USER_FUNC(evt_item::evt_item_delete, PTR("maniac_item"))
	SET(GSW(1726), LW(2))
	RETURN()
EVT_END()

EVT_BEGIN(tik_starmaniac_talk_evt)
	IF_LARGE_EQUAL(GSW(1726), 15)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("starmaniac_evt_02"), 0, PTR("me")) // Out of items
		RETURN()
	END_IF()
	USER_FUNC(tik::unk_tik_000005c0)
	IF_EQUAL(GSWF(1325), 0)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("starmaniac_evt_00"), 0, PTR("me")) // Character intro line
		SET(GSWF(1325), 1)
	ELSE()
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("starmaniac_evt_01"), 0, PTR("me")) // Regular purchase line
	END_IF()
	USER_FUNC(evt_msg::evt_msg_select, 0, PTR("starmaniac_03")) // YesNo text
	IF_EQUAL(LW(0), 1)
		USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("starmaniac_05")) // Decline line
		USER_FUNC(tik::unk_tik_00000598)
		RETURN()
	END_IF()
	USER_FUNC(evt_pouch::evt_pouch_get_starpiece, LW(0))
	IF_SMALL_EQUAL(LW(0), 0)
		USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("starmaniac_06")) // No star pieces line
		USER_FUNC(tik::unk_tik_00000598)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_continue)
	SET(LW(2), GSW(1726))
	USER_FUNC(evt_window::evt_win_coin_on, 2, LW(12))
	SET(LW(3), PTR("starmaniac_evt_07")) // Price declaration line
LBL(0)
	ADD(LW(2), 1)
	USER_FUNC(getManiacPrice, LW(2), LW(4))
	USER_FUNC(evt_msg::evt_msg_fill_num, 0, LW(14), LW(3), LW(4)) // Fill in price
	USER_FUNC(evt_msg::evt_msg_print, 1, LW(14), 0, PTR("me"))
	USER_FUNC(evt_msg::evt_msg_select, 0, PTR("starmaniac_evt_08")) //YesNo line
	IF_EQUAL(LW(0), 1)
		USER_FUNC(evt_window::evt_win_coin_off, LW(12))
		USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("starmaniac_evt_09")) // Decline line
		USER_FUNC(tik::unk_tik_00000598)
		RETURN()
	END_IF()
	USER_FUNC(evt_pouch::evt_pouch_get_starpiece, LW(0))
	IF_SMALL(LW(0), LW(4))
		USER_FUNC(evt_window::evt_win_coin_off, LW(12))
		USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("starmaniac_evt_13")) // Not enough star pieces line
		USER_FUNC(tik::unk_tik_00000598)
		RETURN()
	END_IF()
	MUL(LW(4), -1)
	USER_FUNC(evt_pouch::evt_pouch_add_starpiece, LW(4))
	USER_FUNC(evt_window::evt_win_coin_wait, LW(12))
	WAIT_MSEC(500)
	USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("starmaniac_evt_10")) // Here you go line
	RUN_CHILD_EVT(starmaniac_item_get)
	IF_LARGE_EQUAL(GSW(1726), 15)
		USER_FUNC(evt_window::evt_win_coin_off, LW(12))
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("starmaniac_evt_14"), 0, PTR("me")) // Out of items
		USER_FUNC(tik::unk_tik_00000598)
		RETURN()
	END_IF()
	ADD(LW(2), 1)
	USER_FUNC(getManiacPrice, LW(2), LW(4))
	USER_FUNC(evt_pouch::evt_pouch_get_starpiece, LW(0))
	IF_SMALL(LW(0), LW(4))
		USER_FUNC(evt_window::evt_win_coin_off, LW(12))
		USER_FUNC(evt_msg::evt_msg_fill_num, 0, LW(14), PTR("starmaniac_evt_12"), LW(4)) // Fill in price
		USER_FUNC(evt_msg::evt_msg_print, 1, LW(14), 0, PTR("me")) // Not enough star pieces line
		USER_FUNC(tik::unk_tik_00000598)
		RETURN()
	END_IF()
	SUB(LW(2), 1)
	SET(LW(3), PTR("starmaniac_evt_11")) // Purchase again line
	GOTO(0)
	RETURN()
EVT_END()
	

EVT_BEGIN(tik_starmaniac_talk_hook)
	RUN_CHILD_EVT(tik_starmaniac_talk_evt)
	RETURN()
EVT_END()
    // clang-format on

namespace mod
{
    void main()
    {
        tik_uranaisi_next_evt[1] = GSW(1708);
        tik_uranaisi_next_evt[2] = 21;

        unk_evt_tik_0000f448[3] = GSW(1700);
        unk_evt_tik_0000f448[4] = 0;

        patch::writePatch(&tik_kuribo5_talk[0], kuribo5_talk_hook, sizeof(kuribo5_talk_hook));

        patch::writePatch(&tik_chusan4_talk[0], chusan4_talk_hook, sizeof(chusan4_talk_hook));

        patch::writePatch(&tik_borodo3_talk[0], borodo3_talk_hook, sizeof(borodo3_talk_hook));

        patch::writePatch(&tik_borodo4_talk[0], borodo4_talk_hook, sizeof(borodo4_talk_hook));

        patch::writePatch(&tik_master_2_talk[0], master_2_talk_hook, sizeof(master_2_talk_hook));

        tik_evt_open_04_open[1] = GSW(1700);
        tik_evt_open_04_open[3] = 0; // Unused

        tik_evt_door_04_close[13] = GSW(1707);
        tik_evt_door_04_close[14] = 20;
        tik_evt_door_04_close[69] = GSW(1707);
        tik_evt_door_04_close[70] = 20;

        tik_evt_door_stg7_open[0] = EVT_HELPER_CMD(2, 26);
        tik_evt_door_stg7_open[1] = EVT_HELPER_OP(GSW(1707));
        tik_evt_door_stg7_open[2] = 10;

        tik_tensou_init_event[4] = GSW(1700);
        tik_tensou_init_event[6] = 0;  // Unused
        tik_tensou_init_event[23] = 0; // Unused
        tik_tensou_init_event[100] = GSW(1707);
        tik_tensou_init_event[101] = 20;

        tik_tensou_sw_event[5] = GSW(1707);
        tik_tensou_sw_event[6] = 99; // Unused
        tik_tensou_sw_event[39] = GSW(1708);
        tik_tensou_sw_event[40] = 19;

        tik_kurihakase_event[212] = GSW(1707);
        tik_kurihakase_event[213] = 21;

        tik_00_init_evt[1] = GSW(1708);
        tik_00_init_evt[2] = 18;
        tik_00_init_evt[4] = GSW(1707);
        tik_00_init_evt[5] = 20;
        tik_00_init_evt[176] = GSW(1708);
        tik_00_init_evt[177] = 18;

        tik_dokan_2_in_evt[1] = GSW(1700);
        tik_dokan_2_in_evt[2] = 99; // Unused
        tik_dokan_2_in_evt[4] = GSWF(6056);
        tik_dokan_2_in_evt[5] = 1;

        tik_gorotuki_sanninngumi[677] = GSW(1700);
        tik_gorotuki_sanninngumi[678] = 12;

        tik_01_init_evt[41] = GSW(1711);
        tik_01_init_evt[42] = 8;
        tik_01_init_evt[188] = GSW(1700);
        tik_01_init_evt[189] = 16;
        tik_01_init_evt[195] = GSW(1700);
        tik_01_init_evt[196] = 11;

        tik_geso_battle[657] = GSWF(6056);
        tik_geso_battle[658] = 1;

        tik_02_init_evt[85] = GSWF(6056);
        tik_02_init_evt[86] = 1;
        tik_02_init_evt[172] = GSWF(6056);
        tik_02_init_evt[173] = 0;
        tik_02_init_evt[178] = GSW(1701);
        tik_02_init_evt[179] = 1;

        tik_bero_05_custom_in[1] = GSW(1713);
        tik_bero_05_custom_in[2] = 11;

        tik_bero_05_custom_out[1] = GSW(1718);
        tik_bero_05_custom_out[2] = 4;
        tik_bero_05_custom_out[4] = GSW(1718);
        tik_bero_05_custom_out[5] = 5;

        tik_guide_init[1] = GSW(1718);
        tik_guide_init[3] = 1;
        tik_guide_init[15] = 2;
        tik_guide_init[27] = 3;

        tik_guide_talk[1] = GSW(1718);
        tik_guide_talk[3] = 2;

        tik_puni_soguu_3[577] = GSW(1718);
        tik_puni_soguu_3[578] = 2;

        tik_puni_switch[111] = GSW(1718);
        tik_puni_switch[112] = 3;

        tik_puni_nakama[3] = GSW(1718);
        tik_puni_nakama[4] = 3;
        tik_puni_nakama[83] = GSW(1718);
        tik_puni_nakama[84] = 4;

        tik_03_init_evt[22] = GSW(1718);
        tik_03_init_evt[23] = 1;
        tik_03_init_evt[28] = GSW(1718);
        tik_03_init_evt[29] = 2;
        tik_03_init_evt[72] = GSW(1718);
        tik_03_init_evt[73] = 3;
        tik_03_init_evt[97] = GSW(1702);
        tik_03_init_evt[98] = 1;

        tik_puni_soguu_2[142] = GSW(1718);
        tik_puni_soguu_2[143] = 1;

        tik_04_switch_evt[34] = GSWF(6054);
        tik_04_switch_evt[35] = 1;

        tik_04_init_evt[19] = GSW(1700);
        tik_04_init_evt[20] = 0; // Unused
        tik_04_init_evt[25] = GSW(1718);
        tik_04_init_evt[26] = 0;
        tik_04_init_evt[36] = GSWF(6055);
        tik_04_init_evt[37] = 1;
        tik_04_init_evt[202] = GSW(1717);
        tik_04_init_evt[203] = 18;

        patch::writePatch(&tik_kurihakase_init[0], tik_kurihakase_init_hook, sizeof(tik_kurihakase_init_hook));

        tik_large_door_stg0[118] = GSW(1700);
        tik_large_door_stg0[119] = 13;

        tik_look_large_door[1] = GSW(1700);
        tik_look_large_door[3] = 99; // UNUSED

        tik_kaidan_event[62] = GSW(1700);
        tik_kaidan_event[63] = 14;

        tik_mahojin_event[355] = GSW(1700);
        tik_mahojin_event[356] = 15;

        tik_mahojin_event1[92] = GSW(1711);
        tik_mahojin_event1[93] = 16;

        tik_mahojin_event2[87] = GSW(1713);
        tik_mahojin_event2[88] = 19;

        tik_mahojin_event3[84] = GSW(1703);
        tik_mahojin_event3[85] = 30;

        tik_mahojin_event4[84] = GSW(1715);
        tik_mahojin_event4[85] = 16;

        tik_mahojin_event5[84] = GSW(1717);
        tik_mahojin_event5[85] = 28;

        tik_mahojin_event6[84] = GSW(1706);
        tik_mahojin_event6[85] = 51;

        tik_mahojin_event7[2499] = GSW(1708);
        tik_mahojin_event7[2500] = 1;

        patch::writePatch(&tik_05_init_evt[58], tik_05_init_evt_hook, sizeof(tik_05_init_evt_hook));
        tik_05_init_evt[62] = 0;
        patch::writePatch(&tik_05_init_evt[145], tik_05_init_evt_hook2, sizeof(tik_05_init_evt_hook2));

        tik_06_init_evt[25] = PTR(&tik_pit_checkpoint_evt);

        tik_peach_mail[178] = GSW(1715);
        tik_peach_mail[179] = 15;

        tik_07_init_evt[277] = GSW(1717);
        tik_07_init_evt[278] = 18;
        tik_07_init_evt[294] = GSW(1706);
        tik_07_init_evt[295] = 0; // Unused
        tik_07_init_evt[297] = GSW(1707);
        tik_07_init_evt[298] = 1;
        tik_07_init_evt[316] = GSW(1715);
        tik_07_init_evt[317] = 99; // Unused

        tik_08_init_evt[19] = GSW(1704);
        tik_08_init_evt[20] = 0;
        tik_08_init_evt[37] = GSW(1704);
        tik_08_init_evt[38] = 1;
        tik_08_init_evt[40] = GSW(1704);
        tik_08_init_evt[41] = 2;

        tik_15_init_evt[29] = GSW(1711);
        tik_15_init_evt[30] = 8;
        tik_15_init_evt[42] = GSW(1717);
        tik_15_init_evt[43] = 18;

        tik_18_init_evt[53] = GSW(1717);
        tik_18_init_evt[54] = 18;

        tik_evt_majin2[0] = EVT_HELPER_CMD(2, 50);
        tik_evt_majin2[1] = EVT_HELPER_OP(LW(3));
        patch::writePatch(&tik_evt_majin2[3], tik_evt_majin2_item, sizeof(tik_evt_majin2_item));

		patch::writePatch(&tik_starmaniac_talk[0], tik_starmaniac_talk_hook, sizeof(tik_starmaniac_talk_hook));
    }

    void exit() {}
} // namespace mod
