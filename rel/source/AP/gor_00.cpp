#include <AP/gor_00.h>
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

extern int32_t no00_custom[];
extern int32_t mony_init_gor00[];
extern int32_t frankie_run_event[];
extern int32_t mony_peton_talk[];
extern int32_t mony_talk[];
extern int32_t peton_init[];
extern int32_t francesca_run_event[];
extern int32_t peton_talk[];
extern int32_t marco_init_00[];
extern int32_t marco_talk_00[];
extern int32_t garawaru_init[];
extern int32_t garawaru_regl[];
extern int32_t garawaru_urouro2[];
extern int32_t garawaru_talk[];
extern int32_t megane_init[];
extern int32_t megane_talk[];
extern int32_t odoodo_init[];
extern int32_t odoodo_regl[];
extern int32_t odoodo_talk[];
extern int32_t odoodo_ani_init[];
extern int32_t odoodo_ani_regl[];
extern int32_t odoodo_ani_talk[];
extern int32_t suifu_a_init[];
extern int32_t suifu_a_regl[];
extern int32_t suifu_a_talk[];
extern int32_t suifu_b_init[];
extern int32_t suifu_b_regl[];
extern int32_t suifu_b_talk[];
extern int32_t suifu_d_init[];
extern int32_t suifubomb_a_urouro2[];
extern int32_t suifubomb_a_init[];
extern int32_t suifubomb_a_regl[];
extern int32_t suifubomb_a_talk[];
extern int32_t suifubomb_b_urouro2[];
extern int32_t suifubomb_b_init[];
extern int32_t suifubomb_b_regl[];
extern int32_t suifubomb_b_talk[];
extern int32_t suifubomb_c_init[];
extern int32_t sanders_init_00[];
extern int32_t korutesu_init[];
extern int32_t tentyou_init[];
extern int32_t tentyou_talk[];
extern int32_t tenin_init[];
extern int32_t tenin_talk[];
extern int32_t mokorim_init[];
extern int32_t mokorim_talk[];
extern int32_t luigi_init_00[];
extern int32_t luigi_npcEnt_00[];
extern int32_t enter_gorotsuki_town_init[];
extern int32_t christine_osoware_setup[];
extern int32_t enter_gorotsuki_town[];
extern int32_t christine_osoware[];
extern int32_t gundan1000tai_beforebattle[];
extern int32_t gundan1000tai_afterbattle[];
extern int32_t shukko_event[];
extern int32_t gorotsuki_kikan[];
extern int32_t into_gor_ship[];
extern int32_t unk_evt_gor_0002a2d4[];
extern int32_t gor_00_koopa_evt[];
extern int32_t peach_mail_00[];
extern int32_t option_ship_mario[];
extern int32_t tsumini[];
extern int32_t epilogue_npc_entry[];
extern int32_t epilogue_evt[];
extern int32_t epilogue_npc_entry2[];
extern int32_t epilogue_evt2[];
extern int32_t return_gorotsuki_town_init[];
extern int32_t return_gorotsuki_town[];
extern int32_t gor_00_init_evt[];
extern int32_t nannpc_entry_data;
extern int32_t nannpc_ext_main_sub_fast;


EVT_BEGIN(garawaru_init_evt)
	IF_SMALL(GSW(1700), 4) //Before Crump Fight
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -200, 15, -175)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1704), 99) //During Bowser Intermission (Unknown Value)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 1, 0)
		USER_FUNC(evt_npc::evt_npc_restart_regular_event, PTR("me"))
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1705), 1) //After Talking to Flavio
		IF_SMALL_EQUAL(GSW(1705), 7)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 270)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 160, 73, 135)
			RETURN()
		END_IF()
	END_IF()
	IF_SMALL(GSW(1705), 7) //Before ship leaves for keelhaul key
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -200, 15, -175)
		RETURN()
	END_IF()
	USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_init_hook)
	RUN_CHILD_EVT(garawaru_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_regl_evt)
	IF_SMALL(GSW(1700), 7) //Before Goombella Joins
		IF_EQUAL(GSWF(1179), 1)
			RETURN()
		END_IF()
		DO(0)
			USER_FUNC(evt_npc::evt_npc_move_position, PTR("me"), 450, -175, 0, FLOAT(40.00), 0)
			WAIT_MSEC(1000)
			USER_FUNC(evt_npc::evt_npc_move_position, PTR("me"), -200, -175, 0, FLOAT(40.00), 0)
			IF_EQUAL(GSWF(1179), 1)
				DO_BREAK()
			END_IF()
			WAIT_MSEC(1000)
			IF_EQUAL(GSWF(1179), 1)
				DO_BREAK()
			END_IF()
		WHILE()
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1705), 0) //Before Talking to Flavio
		RUN_CHILD_EVT(garawaru_urouro2)
		RETURN()
	END_IF()
	IF_SMALL_EQUAL(GSW(1705), 7) //After Talking to flavio
		RETURN()
	END_IF()
	IF_LARGE(GSW(1705), 7) //After Ship Leaves
		RUN_CHILD_EVT(garawaru_urouro2)
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_regl_hook)
	RUN_CHILD_EVT(garawaru_regl_evt)
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_talk_evt)
	IF_SMALL(GSW(1700), 7) // Before Goombella Joins
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_06_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1705), 0) // Before talking to flavio
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_06_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1705), 6) // Before bobbery leaves his house
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_06_08"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1705), 7) // Before sailing off
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_4_025"), 0, PTR("me"))
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_init_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -225, 15, -400)
		RETURN()
	END_IF()
	SWITCH(GSW(1705))
		CASE_BETWEEN(1, 7)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 34, 77, 40)
		CASE_END()
		CASE_ETC()
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -200, 15, -400)
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_regl_evt)
	SWITCH(GSW(1705))
		CASE_BETWEEN(1, 7)
			RETURN()
		CASE_END()
		CASE_EQUAL(46)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_LARGE_EQUAL(GSW(1700), 22)
		USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
		USER_FUNC(evt_urouro::urouro_init_func, PTR("me"), LW(0), LW(2), FLOAT(100.00), FLOAT(30.00), 0)
		USER_FUNC(evt_urouro::urouro_main_func, PTR("me"))
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_regl_hook)
	RUN_CHILD_EVT(odoodo_regl_evt)
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_talk_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_05_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1705), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_05_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1705), 7)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_05_08"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1705), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_4_024"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1705), 99) //Unknown, post ch.5
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_05_09"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_05_12"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_ani_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_07_09"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(suifu_a_talk_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_03_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1705))
		CASE_SMALL(1)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_03_07"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_03_08"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_LARGE(99) // Unknown, Post Ch.5
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_03_09"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_ETC()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_03_04"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_03_12"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(suifu_b_init_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 270)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1704), 99) // Unknown, Bowser Intermission
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -191, 15, 36)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 1, 0)
		USER_FUNC(evt_npc::evt_npc_restart_regular_event, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
	USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, 15, -300)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_b_init_hook)
	RUN_CHILD_EVT(suifu_b_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_b_talk_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_04_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1705))
		CASE_SMALL(1)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_04_07"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_04_08"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_LARGE(99) // Unknown, Post Ch.5
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_04_09"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_ETC()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_04_04"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_04_12"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(suifubomb_a_init_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -215, 15, 175)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1704), 99) // Unknown, Bowser Intermission
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -160, 15, 207)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 1, 0)
		USER_FUNC(evt_npc::evt_npc_restart_regular_event, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
	USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -215, 15, -150)
	RETURN()
EVT_END()

EVT_BEGIN(suifubomb_a_init_hook)
	RUN_CHILD_EVT(suifubomb_a_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifubomb_a_regl_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
		USER_FUNC(evt_urouro::urouro_init_func, PTR("me"), LW(0), LW(2), FLOAT(100.00), FLOAT(30.00), 0)
		USER_FUNC(evt_urouro::urouro_main_func, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1705))
		CASE_BETWEEN(1, 7)
			RETURN()
		CASE_END()
		CASE_ETC()
			RUN_CHILD_EVT(suifubomb_a_urouro2)
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(suifubomb_a_regl_hook)
	RUN_CHILD_EVT(suifubomb_a_regl_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifubomb_a_talk_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_01_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1705))
		CASE_SMALL(1)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_01_07"), 0, PTR("me"))
			USER_FUNC(evt_mario::evt_mario_set_pose, PTR("M_I_2"))
			WAIT_MSEC(1000)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_01_07_01"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_01_08"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(6)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_01_08_01"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_01_08_02"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_LARGE(99) // Unknown, Post Ch.5
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_01_09"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_ETC()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_01_04"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_01_12"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(suifubomb_b_regl_evt)
	IF_SMALL(GSW(1700), 7) //Before Goombella Joins
		IF_EQUAL(GSWF(1179), 1)
			RETURN()
		END_IF()
		DO(0)
			USER_FUNC(evt_npc::evt_npc_move_position, PTR("me"), 450, -175, 0, FLOAT(40.00), 0)
			WAIT_MSEC(1000)
			USER_FUNC(evt_npc::evt_npc_move_position, PTR("me"), -200, -175, 0, FLOAT(40.00), 0)
			IF_EQUAL(GSWF(1179), 1)
				DO_BREAK()
			END_IF()
			WAIT_MSEC(1000)
			IF_EQUAL(GSWF(1179), 1)
				DO_BREAK()
			END_IF()
		WHILE()
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		RETURN()
		END_IF()
	IF_EQUAL(GSW(1705), 0) //Before Talking to Flavio
		RUN_CHILD_EVT(suifubomb_b_urouro2)
		RETURN()
	END_IF()
	IF_SMALL_EQUAL(GSW(1705), 7) //After Talking to flavio
		RETURN()
		END_IF()
	IF_LARGE(GSW(1705), 7) //After Ship Leaves
		RUN_CHILD_EVT(suifubomb_b_urouro2)
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(suifubomb_b_regl_hook)
	RUN_CHILD_EVT(suifubomb_b_regl_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifubomb_b_talk_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_02_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1705))
		CASE_SMALL(1)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_02_07"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_02_08"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_LARGE(99) // Unknown, Post Ch.5
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_02_09"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_ETC()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_02_04"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_02_12"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(korutesu_init_evt)
	IF_EQUAL(GSW(1708), 99) // Unknown
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1705), 99) // Unknown
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 270)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 210, 40, 92)
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(korutesu_init_hook)
	RUN_CHILD_EVT(korutesu_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(gor_00_init_evt1)
	IF_EQUAL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_bero::evt_bero_get_entername, LW(0))
		IF_STR_EQUAL(LW(0), PTR("epilogue2"))
			USER_FUNC(evt_snd::evt_snd_bgmon_f, 768, PTR("BGM_ENDING2"), 800)
			USER_FUNC(evt_snd::evt_snd_bgmoff, 34816)
			USER_FUNC(evt_snd::evt_snd_envoff, 512)
		ELSE()
			USER_FUNC(evt_snd::evt_snd_bgmon_f, 768, PTR("BGM_ENDING2"), 800)
			USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG0_GOR0"))
		END_IF()
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1705), 99) //Unknown
		USER_FUNC(evt_snd::evt_snd_bgmon_f, 768, PTR("BGM_STG0_GOR1"), 2000)
		USER_FUNC(evt_snd::evt_snd_bgmoff, 30784)
		USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG0_GOR0"))
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1704), 99) //Unknown
		USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_KUPPA1"))
		USER_FUNC(evt_snd::evt_snd_bgm_scope, 0, 1)
		USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("ENV_STG0_GOR0"))
		RETURN()
	END_IF()
	SWITCH(GSW(1700))
		CASE_BETWEEN(0, 3)
			IF_NOT_EQUAL(GSW(1700), 2)
				USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG0_GOR1"))
			ELSE()
				USER_FUNC(evt_snd::evt_snd_bgmon_f, 768, PTR("BGM_STG0_GOR1"), 2000)
			END_IF()
			USER_FUNC(evt_snd::evt_snd_bgm_scope, 0, 1)
			USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG0_GOR0"))
			RETURN()
		CASE_END()
		CASE_BETWEEN(4, 6)
			USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_DANGER4"))
			USER_FUNC(evt_snd::evt_snd_envoff, 512)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_NOT_EQUAL(GSWF(1178), 1)
		USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG0_GOR1"))
		USER_FUNC(evt_snd::evt_snd_bgmoff, 30720)
	END_IF()
	USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG0_GOR0"))
	RETURN()
EVT_END()

EVT_BEGIN(gor_00_init_evt2)
	SWITCH(GSW(1709)) //Epilogue
		CASE_EQUAL(98) //Unknown
			USER_FUNC(evt_mario::evt_mario_kill_party, 0)
			USER_FUNC(evt_bero::evt_bero_get_entername, LW(0))
			IF_STR_EQUAL(LW(0), PTR("epilogue2"))
				RUN_CHILD_EVT(epilogue_npc_entry2)
				RUN_EVT(epilogue_evt2)
			ELSE()
				RUN_CHILD_EVT(epilogue_npc_entry2)
				RUN_EVT(epilogue_evt2)
			END_IF()
			RETURN()
		CASE_END()
		CASE_EQUAL(99) //Unknown
			RUN_CHILD_EVT(return_gorotsuki_town_init)
			RUN_EVT(return_gorotsuki_town)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_EQUAL(GSW(1705), 99) //Arriving back from keelhaul
		RUN_EVT(gorotsuki_kikan)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1704), 99) //Unknown, bowser internission post ch.4
		USER_FUNC(evt_mario::evt_mario_kill_party, 0)
		USER_FUNC(evt_npc::evt_npc_entry, PTR("houki"), PTR("c_kamek_be"))
		USER_FUNC(evt_npc::evt_npc_set_anim, PTR("houki"), PTR("H_2"))
		USER_FUNC(evt_npc::evt_npc_flag_onoff, 1, PTR("houki"), 32)
		USER_FUNC(evt_npc::evt_npc_flag_onoff, 1, PTR("houki"), 64)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("houki"), 0, -1000, 0)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("houki"), 90)
		RUN_EVT(gor_00_koopa_evt)
		RETURN()
	END_IF()
	SWITCH(GSW(1700))
		CASE_EQUAL(2)
			RUN_CHILD_EVT(enter_gorotsuki_town_init)
			RUN_EVT(enter_gorotsuki_town)
			RETURN()
		CASE_END()
		CASE_EQUAL(3)
			RUN_CHILD_EVT(enter_gorotsuki_town_init)
			SET(LW(0), 1)
			RUN_EVT(christine_osoware)
			RETURN()
		CASE_END()
		CASE_EQUAL(4)
			RUN_CHILD_EVT(enter_gorotsuki_town_init)
			RUN_CHILD_EVT(christine_osoware_setup)
			USER_FUNC(evt_nannpc::evt_nannpc_init, PTR(&nannpc_entry_data), 0, 0, 0, 400)
			USER_FUNC(evt_nannpc::evt_nannpc_set_subfunc, 1, PTR(nannpc_ext_main_sub_fast))
			SET(LW(0), 1)
			RUN_EVT(gundan1000tai_beforebattle)
			RETURN()
		CASE_END()
		CASE_EQUAL(5)
			RUN_CHILD_EVT(enter_gorotsuki_town_init)
			RUN_CHILD_EVT(christine_osoware_setup)
			USER_FUNC(evt_nannpc::evt_nannpc_init, PTR(&nannpc_entry_data), 0, 0, 0, 400)
			USER_FUNC(evt_nannpc::evt_nannpc_set_subfunc, 1, PTR(nannpc_ext_main_sub_fast))
			SET(LW(0), 1)
			RUN_EVT(gundan1000tai_afterbattle)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1705), 7) //Before leaving for heelkaul
		IF_LARGE_EQUAL(GSW(1700), 7)
			USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S_fune_c"), 1)
			USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A_fune_C"), 1, 1)
			RETURN()
		END_IF()
	END_IF()
EVT_END()

void ApplyGor00Patches(OSModuleInfo* module_info)
{
	switch (module_info->id)
	{
		case ModuleId::GOR:
			//gor_00.o
			no00_custom[1] = GSW(1700); //Goombella Cutscene trigger loading into gor_01
			no00_custom[2] = 5;

			mony_init_gor00[1] = GSW(1709); //Frankie spawn trigger for blimp ticket
			mony_init_gor00[3] = 1;
			mony_init_gor00[4] = 2;

			frankie_run_event[1] = GSW(1709); //Frankie running to door after keelhaul key sequence
			frankie_run_event[3] = 99; //Unknown atm

			mony_peton_talk[270] = GSW(1709); //After talking to frankie during chapter 3 sequence.
			mony_peton_talk[271] = 2;

			mony_talk[1] = GSW(1709); //Initial dialogue options for frankie in chapter 3 sequence.
			mony_talk[3] = 1;
			mony_talk[7] = 2;

			peton_init[1] = GSW(1709); //Spawning Francesca during chapter 3 sequence.
			peton_init[3] = 1;
			peton_init[4] = 2;
			francesca_run_event[1] = GSW(1709); //Francesca running to door after keelhaul key sequence
			francesca_run_event[3] = 99; //Unknown atm
			peton_talk[1] = GSW(1709); //Initial dialogue options for francesca in chapter 3 sequence.
			peton_talk[3] = 1;
			peton_talk[7] = 2;

			marco_init_00[1] = GSW(1705); //Spawning Flavio during chapter 5 sequence.
			marco_init_00[3] = 1;
			marco_init_00[4] = 7;
			marco_talk_00[1] = GSW(1705); //Various dialogue options for flavio in chapter 5 sequence.
			marco_talk_00[3] = 1;
			marco_talk_00[268] = GSW(1705);
			marco_talk_00[269] = 2;
			marco_talk_00[271] = 2;
			marco_talk_00[290] = 6;
			marco_talk_00[298] = 7;

			patch::writePatch(&garawaru_init[5], garawaru_init_hook, sizeof(garawaru_init_hook)); //Bob-omb with bones wheel
			patch::writePatch(&garawaru_regl[0], garawaru_regl_hook, sizeof(garawaru_regl_hook));
			patch::writePatch(&garawaru_talk[0], garawaru_talk_evt, sizeof(garawaru_talk_evt));

			megane_init[1] = GSW(1705); //Unknown NPC Init
			megane_init[3] = 1;
			megane_init[4] = 7;
			megane_talk[1] = GSW(1705); //Unknown NPC Speach
			megane_talk[3] = 1;
			megane_talk[4] = 7;

			patch::writePatch(&odoodo_init[0], odoodo_init_evt, sizeof(odoodo_init_evt)); //Toad who lost brother init
			patch::writePatch(&odoodo_regl[0], odoodo_regl_hook, sizeof(odoodo_regl_hook)); //Toad who lost brother Movement
			patch::writePatch(&odoodo_talk[0], odoodo_talk_evt, sizeof(odoodo_talk_evt)); //Toad who lost brother Speach

			odoodo_ani_init[1] = GSW(1705); //Toads Brother Init
			odoodo_ani_init[3] = 99; //Unknown, Post Ch.5
			odoodo_ani_regl[1] = GSW(1705); //Toads Brother Movement
			odoodo_ani_regl[3] = 99; //Unknown, Post Ch.5
			patch::writePatch(&odoodo_ani_talk[0], odoodo_ani_talk_evt, sizeof(odoodo_ani_talk_evt)); //Toad Brother Speach

			suifu_a_init[1] = GSW(1704); //Random Toad Init
			suifu_a_init[3] = 99; //Unknown, Bowser intermission
			suifu_a_regl[1] = GSW(1750); //Random Toad Movement
			suifu_a_regl[3] = 1;
			suifu_a_regl[4] = 7;
			suifu_a_regl[6] = 7;
			patch::writePatch(&suifu_a_talk[0], suifu_a_talk_evt, sizeof(suifu_a_talk_evt)); //Random Toad Speach

			patch::writePatch(&suifu_b_init[0], suifu_b_init_hook, sizeof(suifu_b_init_hook)); //Random Toad 2 Init
			patch::writePatch(&suifu_b_regl[0], odoodo_regl_hook, sizeof(odoodo_regl_hook)); //Random Toad 2 Movement
			patch::writePatch(&suifu_b_talk[0], suifu_b_talk_evt, sizeof(suifu_b_talk_evt)); //Random Toad 2 Speach

			suifu_d_init[1] = GSW(1705); //Random Toad 3 Init
			suifu_d_init[3] = 1; 
			suifu_d_init[4] = 7;

			patch::writePatch(&suifubomb_a_init[0], suifubomb_a_init_hook, sizeof(suifubomb_a_init_hook)); //Random Bomb 1 Init
			patch::writePatch(&suifubomb_a_regl[0], suifubomb_a_regl_hook, sizeof(suifubomb_a_regl_hook)); //Random Bomb 1 Movement
			patch::writePatch(&suifubomb_a_talk[91], suifubomb_a_talk_evt, sizeof(suifubomb_a_talk_evt)); //Random Bomb 1 Speach

			suifubomb_b_init[16] = GSW(1704); //Random Bomb 2 Init
			suifubomb_b_init[17] = 99; // Bowser Intermission, Post Ch.4
			patch::writePatch(&suifubomb_b_regl[0], suifubomb_b_regl_hook, sizeof(suifubomb_b_regl_hook)); //Random Bomb 2 Movement
			patch::writePatch(&suifubomb_b_talk[0], suifubomb_b_talk_evt, sizeof(suifubomb_b_talk_evt)); //Random Bomb 2 Speach

			suifubomb_c_init[1] = GSW(1705); //Random Bomb 3 Init
			suifubomb_c_init[3] = 1;
			suifubomb_c_init[4] = 7;

			sanders_init_00[1] = GSW(1705); //Bobbery init
			sanders_init_00[3] = 1;
			sanders_init_00[4] = 7;
			sanders_init_00[23] = GSW(1705);
			sanders_init_00[24] = 6;

			patch::writePatch(&korutesu_init[0], korutesu_init_hook, sizeof(korutesu_init_hook)); //Cortez Init

			tentyou_init[1] = GSW(1705); //Unknown NPC Init
			tentyou_init[3] = 1;
			tentyou_init[4] = 7;

			tentyou_talk[1] = GSW(1705); //Unknown NPC Talk
			tentyou_talk[3] = 1;
			tentyou_talk[4] = 7;

			tenin_init[1] = GSW(1705); //Unknown NPC Init
			tenin_init[3] = 1;
			tenin_init[4] = 7;

			tenin_talk[1] = GSW(1705); //Unknown NPC Talk
			tenin_talk[3] = 1;
			tenin_talk[4] = 7;

			mokorim_init[3] = GSW(1710); //Lumpy Init
			mokorim_init[5] = 0;
			mokorim_init[6] = 1;
			mokorim_init[13] = 4;

			mokorim_talk[4] = GSW(1710); //Lumpy Speach
			mokorim_talk[6] = 0;
			mokorim_talk[7] = 1;
			mokorim_talk[214] = 4;

			luigi_init_00[1] = GSW(1705); //Luigi Init
			luigi_init_00[3] = 98; // Unknown
			luigi_init_00[4] = 99;// Unknown

			luigi_npcEnt_00[1] = GSW(1705); //Luigi Partner
			luigi_npcEnt_00[3] = 99; // Unknown
			luigi_npcEnt_00[4] = 99;// Unknown

			enter_gorotsuki_town[576] = GSW(1700); //After mario arrives

			christine_osoware[334] = GSW(1700); //After goombella asks for help

			gundan1000tai_beforebattle[483] = GSW(1700); //Before Crump Fight

			gundan1000tai_afterbattle[1182] = GSW(1700); //After crump fight

			shukko_event[490] = GSW(1705); //Ship arrives back from Keelhaul Key
			shukko_event[491] = 7;

			gorotsuki_kikan[235] = GSW(1705); //After toads leave ship
			gorotsuki_kikan[236] = 99; //Unknown

			into_gor_ship[108] = GSW(1709); //Frankie and francesca cutscene positioning
			into_gor_ship[109] = 99; //Unknown

			unk_evt_gor_0002a2d4[1] = GSW(1709); //After frankie and francesca run
			unk_evt_gor_0002a2d4[2] = 99; //Unknown

			gor_00_koopa_evt[1049] = GSW(1704); //After Bowser intermission, post ch.4
			gor_00_koopa_evt[1050] = 99; //Unknown

			peach_mail_00[199] = GSW(1705); //After peach mail, post ch.5
			peach_mail_00[200] = 99; //Unknown

			option_ship_mario[89] = GSW(1709); //Spawn params for frankie and francesca
			option_ship_mario[90] = 99; //Unknown

			tsumini[1] = GSW(1704); //Change map during and after bowser intermission, post ch.4
			tsumini[2] = 99; //Unknown
			tsumini[95] = GSW(1705); //NPC Flags for stuff during ch.5 sequence??
			tsumini[97] = 1;
			tsumini[98] = 7;
			tsumini[150] = GSW(1705);
			tsumini[152] = 1;
			tsumini[153] = 7;

			return_gorotsuki_town[609] = GSW(1708); //Epilogue
			return_gorotsuki_town[610] = 99; //Unknown

			gor_00_init_evt[28] = EVT_HELPER_CMD(1, 94); //First Switch Function
			gor_00_init_evt[29] = EVT_HELPER_OP(gor_00_init_evt1);
			gor_00_init_evt[30] = EVT_HELPER_CMD(1, 4);
			gor_00_init_evt[31] = EVT_HELPER_OP(&gor_00_init_evt[140]);

			gor_00_init_evt[156] = GSW(1709); //Related to frankie and francesca arriving back from keelhaul
			gor_00_init_evt[157] = 8;

			gor_00_init_evt[187] = GSW(1704); //Disable save block during Bowser intermission, post ch.4
			gor_00_init_evt[188] = 99; //Unknown

			gor_00_init_evt[302] = GSW(1708); //Ship related stuff if not in epilogue
			gor_00_init_evt[303] = 99; //Unknown

			gor_00_init_evt[308] = GSW(1705); //Peach mail post ch.5
			gor_00_init_evt[309] = 99; //Unknown

			gor_00_init_evt[314] = GSW(1709); //Frankie and francesca speach related, ch.3 sequence
			gor_00_init_evt[315] = 2; //Unknown

			gor_00_init_evt[329] = EVT_HELPER_CMD(1, 94); //First Switch Function
			gor_00_init_evt[330] = EVT_HELPER_OP(gor_00_init_evt2);
			gor_00_init_evt[331] = EVT_HELPER_CMD(1, 4);
			gor_00_init_evt[332] = EVT_HELPER_OP(&gor_00_init_evt[469]);

			gor_00_init_evt[470] = GSW(1709); //Epilogue
			gor_00_init_evt[471] = 99; //Unknown
			gor_00_init_evt[473] = GSW(1709); //Epilogue
			gor_00_init_evt[474] = 99; //Unknown
			gor_00_init_evt[476] = GSW(1705); //After returning from keelhaul
			gor_00_init_evt[477] = 99; //Unknown
	}
}