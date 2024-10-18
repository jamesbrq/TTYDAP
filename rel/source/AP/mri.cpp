#include <AP/mri.h>
#include <ttyd/evt_cam.h>
#include <ttyd/evt_npc.h>
#include <ttyd/evt_nannpc.h>
#include <ttyd/evt_msg.h>
#include <ttyd/evt_map.h>
#include <ttyd/evt_hit.h>
#include <ttyd/evt_mario.h>
#include <ttyd/evt_party.h>
#include <ttyd/evt_bero.h>
#include <ttyd/evt_snd.h>
#include <ttyd/evt_urouro.h>
#include "evt_cmd.h"
#include "common_types.h"
#include "patch.h"

using namespace mod;
using namespace ttyd;

extern char mri_countdown[];
extern char seal_bigtree[];
extern char seal_bigtree2[];
extern char mri_guide_init_00[];
extern char guide_talk[];
extern char mri_rival_init_00[];
extern char chibitoge_init[];
extern char sister_init[];
extern char mri_elder_init_00[];
extern char fabio_init[];
extern char fabio_talk[];
extern char first_time_w_bero[];
extern char out_dokan_1[];
extern char first_bigtree[];
extern char open_bigtree[];
extern char mario_see_you[];
extern char mri_00_init_evt[];
extern char mri_guide_init_01[];
extern char guide_talk[];
extern char mri_rival_init_01[];
extern char mri_elder_init_01[];
extern char elder_talk[];
extern char puni_1_init[];
extern char puni_1_talk[];
extern char puni_2_init[];
extern char puni_2_talk[];
extern char sister_init[];
extern char sister_talk[];
extern char bigtree_firstbattle[];
extern char meet_rival[];
extern char rival_nakama[];
extern char boss_battle[];
extern char boss_battle_win[];
extern char evt_ueki_01[];
extern char evt_ueki_02[];
extern char evt_ueki_03[];
extern char evt_ueki_08[];
extern char mri_01_koopa_first_evt[];
extern char mri_01_koopa_second_evt[];
extern char mri_01_init_evt[];
extern char dokan_event[];
extern char mri_02_init_evt[];
extern char elder_rescue[];
extern char elder_init[];
extern char sister_init[];
extern char sister_regl[];
extern char puni_3_init[];
extern char puni_3_talk[];
extern char bero_custom_00[];
extern char bluejail_event[];
extern char guide_sister[];
extern char toraware_elder[];
extern char guide_sister_before[];
extern char mri_03_init_evt[];
extern char bero_custom[];
extern char chibitoge_init[];
extern char chuchurina_sogu[];
extern char black_shadow[];
extern char mri_04_init_evt[];
extern char puni_4_init[];
extern char puni_4_talk[];
extern char puni_5_init[];
extern char puni_5_talk[];
extern char mri_05_init_evt[];
extern char rival_init[];
extern char rival_talk[];
extern char puni_6_init[];
extern char puni_6_talk[];
extern char puni_7_init[];
extern char puni_7_talk[];
extern char puni_8_init[];
extern char puni_8_talk[];
extern char puni_9_init[];
extern char puni_9_talk[];
extern char puni_10_init[];
extern char puni_10_talk[];
extern char puni_11_init[];
extern char puni_11_talk[];
extern char apper_toge[];
extern char mri_06_init_evt[];
extern char chibitoge_init[];
extern char puniinfo_talk[];
extern char mri_07_init_evt[];
extern char toge_1_init[];
extern char toge_1_talk[];
extern char toge_2_init[];
extern char toge_2_talk[];
extern char toge_3_init[];
extern char toge_3_talk[];
extern char toge_4_init[];
extern char toge_4_talk[];
extern char toge_5_init[];
extern char toge_5_talk[];
extern char toge_6_init[];
extern char toge_6_talk[];
extern char mri_08_init_evt[];
extern char wall_roll[];
extern char slot00_event[];
extern char slot01_event[];
extern char slot02_event[];
extern char slot03_event[];
extern char get_blue_jail_key[];
extern char mri_09_init_evt[];
extern char item_evt[];
extern char mri_10_init_evt[];
extern char toge_7_init[];
extern char toge_7_talk[];
extern char toge_8_init[];
extern char toge_8_talk[];
extern char toge_9_init[];
extern char toge_9_talk[];
extern char toge_10_init[];
extern char toge_10_talk[];
extern char toge_11_init[];
extern char toge_11_talk[];
extern char mri_11_init_evt[];
extern char mri_12_init_evt[];
extern char punipuni_ball00_init[];
extern char punipuni_ball00[];
extern char yuka_check[];
extern char yuka_event[];
extern char mri_13_init_evt[];
extern char first_time_w_bero[];
extern char dai3_kanbu[];
extern char time_bomb[];
extern char mri_14_init_evt[];
extern char dai3_battle[];
extern char mri_15_init_evt[];
extern char mri_16_init_evt[];
extern char puni_disp_info[];
extern char mri_puni_init[];
extern char trap_event[];
extern char breakyuka_main[];
extern char switch_event[];
extern char mri_17_init_evt[];
extern char mri_20_init_evt[];
extern char mri_18_init_evt[];
extern char mri_19_init_evt[];

EVT_BEGIN(guide_init_00_evt)
	IF_EQUAL(GSW(1713), 1)
		IF_EQUAL(GSWF(2826), 0)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 140, 0, -140)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1713), 99) //Unknown
		IF_EQUAL(GSWF(2884), 0)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -34, 0, 177)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
			RETURN()
		END_IF()
	END_IF()
	IF_EQUAL(GSW(1702), 4)
		USER_FUNC(evt_npc::evt_npc_flag_onoff, 1, PTR("me"), 16777216)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 10, 155, -220)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSWF(1702), 2)
		IF_SMALL_EQUAL(GSWF(1702), 3)
			USER_FUNC(evt_npc::evt_npc_flag_onoff, 1, PTR("me"), 16777216)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 135, 0, -70)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
			RETURN()
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(guide_init_00_hook)
	RUN_CHILD_EVT(guide_init_00_evt)
	RETURN()
EVT_END()

EVT_BEGIN(guide_talk_evt)
	IF_EQUAL(GSW(1702), 3)
		RUN_EVT(&seal_bigtree)
		RETURN()
	END_IF()
	IF_EQUAL(GSWF(1702), 4)
		RUN_EVT(&seal_bigtree2)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1713), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_e10_05"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1713), 99) //Unknown
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_e26_00_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(guide_talk_hook)
	RUN_CHILD_EVT(guide_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(guide_init_01_evt)
	IF_LARGE_EQUAL(GSW(1703), 99) //Unknown
		IF_SMALL_EQUAL(GSWF(1703), 99) //Unknown
			USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 6, 0)
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1713), 99) //Unknown
		IF_EQUAL(GSWF(2884), 1)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -20, 0, 40)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 270)
			RETURN()
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(guide_init_01_hook)
	RUN_CHILD_EVT(guide_init_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(mri_rival_init_01_evt)
	IF_LARGE_EQUAL(GSW(1713), 99) //Unknown
		IF_SMALL_EQUAL(GSW(1713), 99) //Unknown
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -5, 0, 70)
			RETURN()
		END_IF()
	END_IF()
	IF_EQUAL(GSW(1713), 99) //Unknown
	USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -20, 0, 38)
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1703), 99) //Unknown
		IF_SMALL_EQUAL(GSW(1703), 99) //Unknown
			USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 6, 0)
			RETURN()
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(mri_rival_init_01_hook)
	RUN_CHILD_EVT(mri_rival_init_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(mri_elder_init_01_evt)
	IF_LARGE_EQUAL(GSW(1713), 99) //Unknown
		IF_SMALL_EQUAL(GSW(1713), 99) //Unknown
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -78, 0, 38)
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1703), 99) //Unknown
		IF_SMALL_EQUAL(GSW(1703), 99) //Unknown
			USER_FUNC(evt_npc::evt_npc_status_onoff, 1, PTR("me"), 2)
			USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 6, 0)
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1713), 99) //Unknown
		IF_EQUAL(GSWF(2884), 1) //Unknown
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -78, 0, 38)
			RETURN()
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(mri_elder_init_01_hook)
	RUN_CHILD_EVT(mri_elder_init_01_evt)
	RETURN()
EVT_END()

void ApplyGorMiscPatches(OSModuleInfo* module_info)
{
	mri_countdown[1] = GSW(1713);
	mri_countdown[2] = 31;
	mri_countdown[80] = GSW(1713);
	mri_countdown[81] = 99; //??? Idk what this does

	seal_bigtree[203] = GSW(1702);
	seal_bigtree[204] = 4; //Might be incorrect

	seal_bigtree2[122] = GSW(1702);
	seal_bigtree2[123] = 5; //Might be incorrect

	patch::writePatch(&mri_guide_init_00[0], guide_init_00_hook, sizeof(guide_init_00_hook));

	patch::writePatch(&guide_talk[0], guide_talk_hook, sizeof(guide_talk_hook));

	mri_rival_init_00[1] = GSW(1713);
	mri_rival_init_00[3] = 99; //Unknown

	chibitoge_init[1] = GSW(1713);
	chibitoge_init[3] = 99; //Unknown

	sister_init[1] = GSW(1713);
	sister_init[3] = 99; //Unknown

	mri_elder_init_00[1] = GSW(1713);
	mri_elder_init_00[3] = 99; //Unknown

	fabio_init[1] = GSW(1708);
	fabio_init[3] = 99; //Unknown

	fabio_talk[1] = GSW(1708);
	fabio_talk[3] = 99; //Unknown

	first_time_w_bero[1] = GSW(1702);
	first_time_w_bero[2] = 2;

	out_dokan_1[1] = GSW(1713);
	out_dokan_1[2] = 99; //Unknown

	first_bigtree[92] = GSW(1702);
	first_bigtree[93] = 3;

	open_bigtree[431] = GSW(1713);
	open_bigtree[432] = 1;

	mario_see_you[198] = GSW(1702);
	mario_see_you[199] = 99; //Unknown

	mri_00_init_evt[39] = GSW(1713);
	mri_00_init_evt[40] = 99; //Unknown
	mri_00_init_evt[65] = GSW(1713);
	mri_00_init_evt[67] = 99; //Unknown
	mri_00_init_evt[137] = GSW(1708);
	mri_00_init_evt[138] = 99; //Unknown
	mri_00_init_evt[175] = GSW(1702);
	mri_00_init_evt[176] = 2;
	mri_00_init_evt[214] = GSW(1713);
	mri_00_init_evt[215] = 1;
	mri_00_init_evt[217] = GSW(1713);
	mri_00_init_evt[218] = 99; //Unknown
	mri_00_init_evt[232] = GSW(1702);
	mri_00_init_evt[233] = 2;
	mri_00_init_evt[238] = GSW(1713);
	mri_00_init_evt[239] = 0;
	mri_00_init_evt[278] = GSW(1708);
	mri_00_init_evt[279] = 99; //Unknown

	patch::writePatch(&mri_guide_init_01[0], guide_init_01_hook, sizeof(guide_init_01_hook));

	patch::writePatch(&mri_rival_init_01[0], mri_rival_init_01_hook, sizeof(mri_rival_init_01_hook));

	patch::writePatch(&mri_elder_init_01[0], mri_elder_init_01_hook, sizeof(mri_elder_init_01_hook));
}