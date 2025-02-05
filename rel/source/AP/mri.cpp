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

extern int32_t mri_countdown[];
extern int32_t seal_bigtree[];
extern int32_t seal_bigtree2[];
extern int32_t mri_guide_init_00[];
extern int32_t mri_guide_talk_00[];
extern int32_t mri_rival_init_00[];
extern int32_t mri_chibitoge_init_00[];
extern int32_t mri_sister_init_00[];
extern int32_t mri_elder_init_00[];
extern int32_t fabio_init[];
extern int32_t fabio_talk[];
extern int32_t mri_first_time_w_bero_00[];
extern int32_t out_dokan_1[];
extern int32_t first_bigtree[];
extern int32_t open_bigtree[];
extern int32_t mario_see_you[];
extern int32_t mri_00_init_evt[];
extern int32_t mri_guide_init_01[];
extern int32_t mri_guide_talk_01[];
extern int32_t mri_rival_init_01[];
extern int32_t mri_elder_init_01[];
extern int32_t elder_talk[];
extern int32_t elder_talk_sub;
extern int32_t puni_1_init[];
extern int32_t puni_1_talk[];
extern int32_t puni_2_init[];
extern int32_t puni_2_talk[];
extern int32_t mri_sister_init_01[];
extern int32_t sister_talk[];
extern int32_t bigtree_firstbattle[];
extern int32_t meet_rival[];
extern int32_t rival_nakama[];
extern int32_t boss_battle[];
extern int32_t boss_battle_win[];
extern int32_t evt_ueki_01[];
extern int32_t evt_ueki_02[];
extern int32_t evt_ueki_03[];
extern int32_t evt_ueki_08[];
extern int32_t mri_01_koopa_first_evt[];
extern int32_t mri_01_koopa_second_evt[];
extern int32_t mri_01_init_evt[];
extern int32_t dokan_event[];
extern int32_t mri_02_init_evt[];
extern int32_t elder_rescue[];
extern int32_t mri_elder_init_03[];
extern int32_t mri_sister_init_03[];
extern int32_t sister_regl[];
extern int32_t puni_3_init[];
extern int32_t puni_3_talk[];
extern int32_t bero_custom_00[];
extern int32_t guide_sister[];
extern int32_t toraware_elder[];
extern int32_t toraware_elder[];
extern int32_t mri_03_init_evt[];
extern int32_t bero_custom[];
extern int32_t mri_chibitoge_init_04[];
extern int32_t chuchurina_sogu[];
extern int32_t black_shadow[];
extern int32_t mri_04_init_evt[];
extern int32_t puni_4_init[];
extern int32_t puni_4_talk[];
extern int32_t puni_5_init[];
extern int32_t puni_5_talk[];
extern int32_t mri_05_init_evt[];
extern int32_t mri_rival_init_06[];
extern int32_t rival_talk[];
extern int32_t puni_6_init[];
extern int32_t puni_6_talk[];
extern int32_t puni_7_init[];
extern int32_t puni_7_talk[];
extern int32_t puni_8_init[];
extern int32_t puni_8_talk[];
extern int32_t puni_9_init[];
extern int32_t puni_9_talk[];
extern int32_t puni_10_init[];
extern int32_t puni_10_talk[];
extern int32_t puni_11_init[];
extern int32_t puni_11_talk[];
extern int32_t apper_toge[];
extern int32_t mri_06_init_evt[];
extern int32_t mri_chibitoge_init_07[];
extern int32_t puniinfo_talk[];
extern int32_t mri_07_init_evt[];
extern int32_t toge_1_init[];
extern int32_t toge_1_talk[];
extern int32_t toge_2_init[];
extern int32_t toge_2_talk[];
extern int32_t toge_3_init[];
extern int32_t toge_3_talk[];
extern int32_t toge_4_init[];
extern int32_t toge_4_talk[];
extern int32_t toge_5_init[];
extern int32_t toge_5_talk[];
extern int32_t toge_6_init[];
extern int32_t toge_6_talk[];
extern int32_t mri_08_init_evt[];
extern int32_t wall_roll[];
extern int32_t slot00_event[];
extern int32_t slot01_event[];
extern int32_t slot02_event[];
extern int32_t slot03_event[];
extern int32_t get_blue_jail_key[];
extern int32_t mri_09_init_evt[];
extern int32_t item_evt[];
extern int32_t mri_10_init_evt[];
extern int32_t toge_7_init[];
extern int32_t toge_7_talk[];
extern int32_t toge_8_init[];
extern int32_t toge_8_talk[];
extern int32_t toge_9_init[];
extern int32_t toge_9_talk[];
extern int32_t toge_10_init[];
extern int32_t toge_10_talk[];
extern int32_t toge_11_init[];
extern int32_t toge_11_talk[];
extern int32_t mri_11_init_evt[];
extern int32_t mri_12_init_evt[];
extern int32_t punipuni_ball00_init[];
extern int32_t punipuni_ball00[];
extern int32_t yuka_check[];
extern int32_t yuka_event[];
extern int32_t mri_13_init_evt[];
extern int32_t mri_first_time_w_bero_14[];
extern int32_t dai3_kanbu[];
extern int32_t time_bomb[];
extern int32_t mri_14_init_evt[];
extern int32_t dai3_battle[];
extern int32_t mri_15_init_evt[];
extern int32_t mri_16_init_evt[];
extern int32_t puni_disp_info[];
extern int32_t mri_puni_init[];
extern int32_t trap_event[];
extern int32_t breakyuka_main[];
extern int32_t switch_event[];
extern int32_t mri_17_init_evt[];
extern int32_t mri_20_init_evt[];
extern int32_t mri_18_init_evt[];
extern int32_t mri_19_init_evt[];

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

EVT_BEGIN(elder_talk_evt)
	IF_LARGE_EQUAL(GSW(1713), 99) //Unknown
		IF_SMALL_EQUAL(GSW(1713), 99) //Unknown
			IF_EQUAL(GSWF(2829), 0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_e15_28"), 0, PTR("me"))
				RETURN()
			ELSE()
				RUN_CHILD_EVT(elder_talk_sub)
				RETURN()
			END_IF()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1703), 99) //Unknown
		IF_SMALL_EQUAL(GSW(1703), 99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_re_001"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_re_001_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(elder_talk_hook)
	RUN_CHILD_EVT(elder_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(puni_1_init_evt)
	IF_LARGE_EQUAL(GSW(1703), 99) //Unknown
		IF_SMALL_EQUAL(GSW(1703), 99) //Unknown
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1713), 99) //Unknown
		IF_EQUAL(GSWF(2884), 1)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -280, 0, 10)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 270)
			RETURN()
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(puni_1_init_hook)
	RUN_CHILD_EVT(puni_1_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(puni_1_talk_evt)
	IF_EQUAL(GSW(1707), 99) //Unknown
		IF_EQUAL(GSWF(3879), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_re_000"), 0, PTR("me"))
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_re_000_1"), 0, PTR("me"))
			SET(GSWF(3889), 1)
		END_IF()
		RETURN()
	END_IF()
	IF_SMALL_EQUAL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_re_000"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_re_000_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(puni_1_talk_hook)
	RUN_CHILD_EVT(puni_1_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(puni_2_init_evt)
	IF_LARGE_EQUAL(GSW(1703), 99) //Unknown
		IF_SMALL_EQUAL(GSW(1703), 99) //Unknown
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1713), 99) //Unknown
		IF_EQUAL(GSWF(2884), 1)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 340, 0, 100)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 270)
			RETURN()
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(puni_2_init_hook)
	RUN_CHILD_EVT(puni_2_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(mri_sister_init_01_evt)
	IF_LARGE_EQUAL(GSW(1703), 99) //Unknown
		IF_SMALL_EQUAL(GSW(1703), 99) //Unknown
			USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 6, 0)
			SET(GF(10), 0)
			SET(GF(11), 0)
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1713), 99) //Unknown
		IF_EQUAL(GSWF(2884), 1)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 100, 0, -10)
		END_IF()
	END_IF()
	SET(GF(10), 0)
	SET(GF(11), 0)
	RETURN()
EVT_END()

EVT_BEGIN(mri_sister_init_01_hook)
	RUN_CHILD_EVT(mri_sister_init_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(mri_01_init_evt_switch)
	IF_LARGE_EQUAL(GSW(1713), 21)
		IF_SMALL_EQUAL(GSWF(1713), 22)
			USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_DANGER4"))
			USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG2_MRI9"))
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1703), 99) //Unknown
		IF_SMALL_EQUAL(GSW(1703), 99) //Unknown
			USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_STG2_PUNI1"))
			USER_FUNC(evt_snd::evt_snd_envon, 512, PTR("ENV_STG2_MRI3"))
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG2_MRI1"))
	USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG2_MRI3"))
	RETURN()
EVT_END()

EVT_BEGIN(mri_01_init_evt_hook)
	RUN_CHILD_EVT(mri_01_init_evt_switch)
	GOTO(&mri_01_init_evt[160])
EVT_END()

EVT_BEGIN(mri_03_init_evt_switch)
	IF_LARGE_EQUAL(GSW(1713), 99) //Unknown
		IF_SMALL_EQUAL(GSWF(1713), 99) //Unknown
			USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_DANGER4"))
			USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG2_MRI9"))
			RETURN()
		END_IF()
	END_IF()
	IF_EQUAL(GSWF(6020), 1) //Unknown
		USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_STG2_PUNI2"))
		USER_FUNC(evt_snd::evt_snd_envoff, 512)
		RETURN()
	END_IF()
	USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG2_MRI1"))
	USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG2_MRI4"))
	RETURN()
EVT_END()

EVT_BEGIN(mri_03_init_evt_hook)
	RUN_CHILD_EVT(mri_03_init_evt_switch)
	GOTO(&mri_03_init_evt[84])
EVT_END()

EVT_BEGIN(puniinfo_talk_evt)
	IF_SMALL(GSW(1713), 99) //Unknown
		SWITCH(LSW(714))
			CASE_EQUAL(0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_f00_00"), 0, PTR("me"))
				SET(LSW(714), 1)
				RETURN()
			CASE_END()
			CASE_EQUAL(0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_f00_01"), 0, PTR("me"))
				SET(LSW(714), 2)
				RETURN()
			CASE_END()
			CASE_EQUAL(0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_f00_02"), 0, PTR("me"))
				SET(LSW(714), 0)
				RETURN()
			CASE_END()
		END_SWITCH()
	END_IF()
	IF_SMALL(GSW(1708), 99) //Unknown
		SWITCH(LSW(714))
			CASE_EQUAL(0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_f00_00"), 0, PTR("me"))
				SET(LSW(714), 1)
				RETURN()
			CASE_END()
			CASE_EQUAL(0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_f00_01"), 0, PTR("me"))
				SET(LSW(714), 2)
				RETURN()
			CASE_END()
			CASE_EQUAL(0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_f00_02"), 0, PTR("me"))
				SET(LSW(714), 0)
				RETURN()
			CASE_END()
		END_SWITCH()
	END_IF()
	SWITCH(LSW(714))
		CASE_EQUAL(0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_f00_00"), 0, PTR("me"))
			SET(LSW(714), 1)
		CASE_END()
		CASE_EQUAL(0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_f00_01"), 0, PTR("me"))
			SET(LSW(714), 2)
		CASE_END()
		CASE_EQUAL(0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg2_mri_f00_02"), 0, PTR("me"))
			SET(LSW(714), 0)
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(puniinfo_talk_hook)
	RUN_CHILD_EVT(puniinfo_talk_hook)
	RETURN()
EVT_END()

void ApplyMriPatches(OSModuleInfo* module_info)
{
	mri_countdown[1] = GSW(1713);
	mri_countdown[2] = 31;
	mri_countdown[80] = GSW(1713);
	mri_countdown[81] = 99; //??? Idk what this does

	seal_bigtree[203] = GSW(1702);
	seal_bigtree[204] = 4;

	seal_bigtree2[122] = GSW(1702);
	seal_bigtree2[123] = 5;

	patch::writePatch(&mri_guide_init_00[0], guide_init_00_hook, sizeof(guide_init_00_hook));

	patch::writePatch(&mri_guide_talk_00[0], guide_talk_hook, sizeof(guide_talk_hook));

	mri_rival_init_00[1] = GSW(1713);
	mri_rival_init_00[3] = 99; //Unknown

	mri_chibitoge_init_00[1] = GSW(1713);
	mri_chibitoge_init_00[3] = 99; //Unknown

	mri_sister_init_00[1] = GSW(1713);
	mri_sister_init_00[3] = 99; //Unknown

	mri_elder_init_00[1] = GSW(1713);
	mri_elder_init_00[3] = 99; //Unknown

	fabio_init[1] = GSW(1708);
	fabio_init[3] = 99; //Unknown

	fabio_talk[1] = GSW(1708);
	fabio_talk[3] = 99; //Unknown

	mri_first_time_w_bero_00[1] = GSW(1702);
	mri_first_time_w_bero_00[2] = 2;

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

	mri_guide_talk_01[1] = GSW(1708);
	mri_guide_talk_01[3] = 99; //Unknown

	patch::writePatch(&mri_rival_init_01[0], mri_rival_init_01_hook, sizeof(mri_rival_init_01_hook));

	patch::writePatch(&mri_elder_init_01[0], mri_elder_init_01_hook, sizeof(mri_elder_init_01_hook));

	patch::writePatch(&elder_talk[0], elder_talk_hook, sizeof(elder_talk_hook));

	patch::writePatch(&puni_1_init[0], puni_1_init_hook, sizeof(puni_1_init_hook));
	
	patch::writePatch(&puni_1_talk[0], puni_1_talk_hook, sizeof(puni_1_talk_hook));

	patch::writePatch(&puni_2_init[0], puni_2_init_hook, sizeof(puni_2_init_hook));

	puni_2_talk[1] = GSW(1708);
	puni_2_talk[3] = 99; //Unknown

	patch::writePatch(&mri_sister_init_01[0], mri_sister_init_01_hook, sizeof(mri_sister_init_01_hook));

	sister_talk[17] = GSW(1708);
	sister_talk[19] = 99; //Unknown

	bigtree_firstbattle[641] = GSW(1713);
	bigtree_firstbattle[642] = 2;

	meet_rival[2112] = GSW(1713);
	meet_rival[2113] = 3;

	rival_nakama[1125] = GSW(1713);
	rival_nakama[1126] = 8;

	boss_battle[1133] = GSW(1713);
	boss_battle[1134] = 23;

	boss_battle_win[1448] = GSW(1713);
	boss_battle_win[1449] = 24;

	evt_ueki_01[1] = GSW(1703);
	evt_ueki_01[2] = 99; //Unknown

	evt_ueki_02[1] = GSW(1703);
	evt_ueki_02[2] = 99; //Unknown

	evt_ueki_03[1] = GSW(1703);
	evt_ueki_03[2] = 99; //Unknown

	evt_ueki_08[1] = GSW(1703);
	evt_ueki_08[2] = 99; //Unknown

	mri_01_koopa_first_evt[323] = GSW(1703);
	mri_01_koopa_first_evt[324] = 99; //Unknown

	mri_01_koopa_second_evt[639] = GSW(1703);
	mri_01_koopa_second_evt[640] = 99; //Unknown

	mri_01_init_evt[4] = GSW(1703);
	mri_01_init_evt[5] = 99; //Unknown
	mri_01_init_evt[7] = GSW(1703);
	mri_01_init_evt[8] = 99; //Unknown
	patch::writePatch(&mri_01_init_evt[126], mri_01_init_evt_hook, sizeof(mri_01_init_evt_hook));
	mri_01_init_evt[185] = GSW(1713);
	mri_01_init_evt[186] = 1;
	mri_01_init_evt[191] = GSW(1713);
	mri_01_init_evt[192] = 2;
	mri_01_init_evt[197] = GSW(1713);
	mri_01_init_evt[198] = 7;
	mri_01_init_evt[203] = GSW(1713);
	mri_01_init_evt[204] = 22;
	mri_01_init_evt[211] = GSW(1713);
	mri_01_init_evt[212] = 23;
	mri_01_init_evt[236] = GSW(1713);
	mri_01_init_evt[237] = 8;
	mri_01_init_evt[246] = GSW(1703);
	mri_01_init_evt[247] = 99; //Unknown
	mri_01_init_evt[291] = GSW(1703);
	mri_01_init_evt[292] = 99; //Unknown

	dokan_event[109] = GSWF(6018);
	dokan_event[110] = 1;

	mri_02_init_evt[60] = GSW(1713);
	mri_02_init_evt[62] = 21;
	mri_02_init_evt[63] = 22;
	mri_02_init_evt[137] = GSWF(6018);
	mri_02_init_evt[138] = 1;

	elder_rescue[761] = GSWF(6019);
	elder_rescue[762] = 1;

	mri_elder_init_03[1] = GSWF(6019);
	mri_elder_init_03[3] = 1;

	mri_sister_init_03[1] = GSWF(6020);
	mri_sister_init_03[3] = 1;

	sister_regl[1] = GSW(1713);
	sister_regl[2] = 4;
	sister_regl[6] = GSWF(6021);
	sister_regl[7] = 1;

	puni_3_init[1] = GSW(1713);
	puni_3_init[3] = 99; //Unknown

	puni_3_talk[1] = GSW(1708);
	puni_3_talk[3] = 99; //Unknown

	bero_custom_00[4] = GSW(1713);
	bero_custom_00[5] = 4;

	guide_sister[761] = GSWF(6020);
	guide_sister[762] = 1;

	patch::writePatch(&mri_03_init_evt[50], mri_03_init_evt_hook, sizeof(mri_03_init_evt_hook));
	mri_03_init_evt[128] = GSWF(6020);
	mri_03_init_evt[130] = 0;
	mri_03_init_evt[131] = 0;
	mri_03_init_evt[186] = EVT_HELPER_CMD(0, 2);

	bero_custom[1] = GSWF(6022);
	bero_custom[2] = 0;

	mri_chibitoge_init_04[1] = GSW(1713);
	mri_chibitoge_init_04[3] = 99; //Unknown

	chuchurina_sogu[802] = GSWF(6022);
	chuchurina_sogu[803] = 1;

	black_shadow[333] = GSW(1713);
	black_shadow[334] = 4;

	mri_04_init_evt[54] = GSW(1713);
	mri_04_init_evt[56] = 3;
	mri_04_init_evt[64] = 99; //Unknown
	mri_04_init_evt[65] = 99; //Unknown
	mri_04_init_evt[95] = GSW(1713);
	mri_04_init_evt[96] = 3;
	mri_04_init_evt[145] = GSW(1713);
	mri_04_init_evt[146] = 3;
	mri_04_init_evt[151] = GSWF(6022);
	mri_04_init_evt[152] = 0;
	mri_04_init_evt[157] = GSWF(6022);
	mri_04_init_evt[158] = 1;

	puni_4_init[1] = GSW(1713);
	puni_4_init[3] = 99; //Unknown

	puni_4_talk[1] = GSW(1708);
	puni_4_talk[3] = 99; //Unknown

	puni_5_init[1] = GSW(1713);
	puni_5_init[3] = 99; //Unknown

	puni_5_talk[1] = GSW(1708);
	puni_5_talk[3] = 99; //Unknown

	mri_05_init_evt[49] = GSW(1713);
	mri_05_init_evt[50] = 99; //Unknown
	mri_05_init_evt[64] = GSW(1713);
	mri_05_init_evt[66] = 99; //Unknown
	mri_05_init_evt[67] = 99; //Unknown

	mri_rival_init_06[1] = GSW(1713);
	mri_rival_init_06[3] = 99; //Unknown

	rival_talk[1] = GSW(1708);
	rival_talk[3] = 99; //Unknown

	puni_6_init[1] = GSW(1713);
	puni_6_init[3] = 99; //Unknown

	puni_6_talk[1] = GSW(1708);
	puni_6_talk[3] = 99; //Unknown

	puni_7_init[1] = GSW(1713);
	puni_7_init[3] = 99; //Unknown

	puni_7_talk[1] = GSW(1708);
	puni_7_talk[3] = 99; //Unknown

	puni_8_init[1] = GSW(1713);
	puni_8_init[3] = 99; //Unknown

	puni_8_talk[1] = GSW(1708);
	puni_8_talk[3] = 99; //Unknown

	puni_9_init[1] = GSW(1713);
	puni_9_init[3] = 99; //Unknown

	puni_9_talk[1] = GSW(1708);
	puni_9_talk[3] = 99; //Unknown

	puni_10_init[1] = GSW(1713);
	puni_10_init[3] = 99; //Unknown

	puni_10_talk[1] = GSW(1708);
	puni_10_talk[3] = 99; //Unknown

	puni_11_init[1] = GSW(1713);
	puni_11_init[3] = 99; //Unknown

	puni_11_talk[1] = GSW(1708);
	puni_11_talk[3] = 99; //Unknown

	apper_toge[242] = GSWF(6023);
	apper_toge[243] = 1;

	mri_06_init_evt[85] = GSW(1713);
	mri_06_init_evt[87] = 99; //Unknown
	mri_06_init_evt[88] = 99; //Unknown
	mri_06_init_evt[143] = GSWF(6023);
	mri_06_init_evt[144] = 0;

	mri_chibitoge_init_07[1] = GSW(1713);
	mri_chibitoge_init_07[3] = 99; //Unknown
	mri_chibitoge_init_07[4] = 99; //Unknown
	mri_chibitoge_init_07[6] = 99; //Unknown

	patch::writePatch(&puniinfo_talk[0], puniinfo_talk_hook, sizeof(puniinfo_talk_hook));

	mri_07_init_evt[66] = GSW(1713);
	mri_07_init_evt[68] = 99; //Unknown
	mri_07_init_evt[69] = 99; //Unknown

	toge_1_init[1] = GSW(1713);
	toge_1_init[3] = 99; //Unknown

	toge_1_talk[1] = GSW(1708);
	toge_1_talk[3] = 99; //Unknown

	toge_2_init[1] = GSW(1713);
	toge_2_init[3] = 99; //Unknown

	toge_2_talk[1] = GSW(1708);
	toge_2_talk[3] = 99; //Unknown

	toge_3_init[1] = GSW(1713);
	toge_3_init[3] = 99; //Unknown

	toge_3_talk[1] = GSW(1708);
	toge_3_talk[3] = 99; //Unknown

	toge_4_init[1] = GSW(1713);
	toge_4_init[3] = 99; //Unknown

	toge_4_talk[1] = GSW(1708);
	toge_4_talk[3] = 99; //Unknown

	toge_5_init[1] = GSW(1713);
	toge_5_init[3] = 99; //Unknown

	toge_5_talk[1] = GSW(1708);
	toge_5_talk[3] = 99; //Unknown

	toge_6_init[1] = GSW(1713);
	toge_6_init[3] = 99; //Unknown

	toge_6_talk[1] = GSW(1708);
	toge_6_talk[3] = 99; //Unknown

	mri_08_init_evt[71] = GSW(1713);
	mri_08_init_evt[73] = 99; //Unknown
	mri_08_init_evt[74] = 99; //Unknown

	wall_roll[261] = GSWF(6024);
	wall_roll[262] = 1;

	slot00_event[1] = GSWF(6024);
	slot00_event[2] = 1;

	slot01_event[1] = GSWF(6024);
	slot01_event[2] = 1;

	slot02_event[1] = GSWF(6024);
	slot02_event[2] = 1;

	slot03_event[1] = GSWF(6024);
	slot03_event[2] = 1;

	get_blue_jail_key[115] = GSWF(6021);
	get_blue_jail_key[116] = 1;
	get_blue_jail_key[117] = EVT_HELPER_CMD(0, 2);


	mri_09_init_evt[53] = GSW(1713);
	mri_09_init_evt[55] = 99; //Unknown
	mri_09_init_evt[56] = 99; //Unknown

	item_evt[58] = GSWF(6025);
	item_evt[59] = 1;

	mri_10_init_evt[46] = GSW(1713);
	mri_10_init_evt[48] = 99; //Unknown
	mri_10_init_evt[49] = 99; //Unknown

	toge_7_init[1] = GSW(1713);
	toge_7_init[3] = 99; //Unknown

	toge_7_talk[1] = GSW(1708);
	toge_7_talk[3] = 99; //Unknown

	toge_8_init[1] = GSW(1713);
	toge_8_init[3] = 99; //Unknown

	toge_8_talk[1] = GSW(1708);
	toge_8_talk[3] = 99; //Unknown

	toge_9_init[1] = GSW(1713);
	toge_9_init[3] = 99; //Unknown

	toge_9_talk[1] = GSW(1708);
	toge_9_talk[3] = 99; //Unknown

	toge_10_init[1] = GSW(1713);
	toge_10_init[3] = 99; //Unknown

	toge_10_talk[1] = GSW(1708);
	toge_10_talk[3] = 99; //Unknown

	toge_11_init[1] = GSW(1713);
	toge_11_init[3] = 99; //Unknown

	toge_11_talk[1] = GSW(1708);
	toge_11_talk[3] = 99; //Unknown

	mri_11_init_evt[71] = GSW(1713);
	mri_11_init_evt[73] = 99; //Unknown
	mri_11_init_evt[74] = 99; //Unknown

	mri_12_init_evt[51] = GSW(1713);
	mri_12_init_evt[53] = 99; //Unknown
	mri_12_init_evt[54] = 99; //Unknown

	punipuni_ball00_init[5] = GSW(1713);
	punipuni_ball00_init[6] = 99; //Unknown
	punipuni_ball00_init[36] = GSW(1713);
	punipuni_ball00_init[37] = 99; //Unknown

	punipuni_ball00[18] = GSW(1713);
	punipuni_ball00[19] = 99; //Unknown

	yuka_check[44] = GSW(1713);
	yuka_check[45] = 99; //Unknown

	yuka_event[402] = GSW(1713);
	yuka_event[403] = 99; //Unknown

	mri_13_init_evt[51] = GSW(1713);
	mri_13_init_evt[53] = 99; //Unknown
	mri_13_init_evt[54] = 99; //Unknown
	mri_13_init_evt[119] = GSW(1713);
	mri_13_init_evt[120] = 99; //Unknown
	mri_13_init_evt[186] = GSW(1713);
	mri_13_init_evt[187] = 99; //Unknown

	mri_first_time_w_bero_14[1] = GSW(1713);
	mri_first_time_w_bero_14[2] = 99; //Unknown

	dai3_kanbu[170] = GSW(1713);
	dai3_kanbu[171] = 99; //Unknown

	time_bomb[921] = GSW(1713);
	time_bomb[922] = 99; //Unknown

	mri_14_init_evt[33] = GSW(1713);
	mri_14_init_evt[34] = 99; //Unknown
	mri_14_init_evt[61] = GSW(1713);
	mri_14_init_evt[63] = 99; //Unknown
	mri_14_init_evt[64] = 99; //Unknown
	mri_14_init_evt[109] = GSW(1713);
	mri_14_init_evt[110] = 99; //Unknown
	mri_14_init_evt[138] = GSW(1713);
	mri_14_init_evt[139] = 99; //Unknown
	mri_14_init_evt[144] = GSW(1713);
	mri_14_init_evt[145] = 99; //Unknown

	dai3_battle[121] = GSW(1713);
	dai3_battle[122] = 99; //Unknown

	mri_15_init_evt[51] = GSW(1713);
	mri_15_init_evt[53] = 99; //Unknown
	mri_15_init_evt[54] = 99; //Unknown
	mri_15_init_evt[137] = GSW(1713);
	mri_15_init_evt[138] = 99; //Unknown

	mri_16_init_evt[46] = GSW(1713);
	mri_16_init_evt[48] = 99; //Unknown
	mri_16_init_evt[49] = 99; //Unknown

	puni_disp_info[1] = GSW(1713);
	puni_disp_info[2] = 99; //Unknown

	mri_puni_init[3] = GSW(1708);
	mri_puni_init[4] = EVT_HELPER_CMD(0, 0);
	mri_puni_init[5] = EVT_HELPER_CMD(0, 0);
	mri_puni_init[6] = EVT_HELPER_CMD(0, 0);

	breakyuka_main[121] = 0; //Unused

	mri_17_init_evt[81] = GSW(1713);
	mri_17_init_evt[83] = 99; //Unknown
	mri_17_init_evt[84] = 99; //Unknown
	mri_17_init_evt[131] = GSW(1713);
	mri_17_init_evt[132] = 0; //Unused
	mri_17_init_evt[153] = EVT_HELPER_CMD(1, 4);
	mri_17_init_evt[154] = EVT_HELPER_OP(&mri_17_init_evt[200]);

	mri_20_init_evt[46] = GSW(1713);
	mri_20_init_evt[48] = 99; //Unknown
	mri_20_init_evt[49] = 99; //Unknown
	mri_20_init_evt[69] = GSW(1713);
	mri_20_init_evt[70] = 99; //Unknown
	mri_20_init_evt[104] = GSW(1713);
	mri_20_init_evt[105] = 3;

	mri_18_init_evt[39] = GSW(1713);
	mri_18_init_evt[41] = 99; //Unknown
	mri_18_init_evt[42] = 99; //Unknown
	mri_18_init_evt[69] = GSW(1713);
	mri_18_init_evt[70] = 99; //Unknown

	mri_19_init_evt[39] = GSW(1713);
	mri_19_init_evt[41] = 99; //Unknown
	mri_19_init_evt[42] = 99; //Unknown
}
