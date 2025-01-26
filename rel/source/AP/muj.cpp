#include <AP/muj.h>
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

extern int32_t nannpc_mode_setup;
extern int32_t garawaru_init_muj_00[];
extern int32_t odoodo_init_muj_00[];
extern int32_t suifu_c_init[];
extern int32_t suifu_c_talk[];
extern int32_t suifu_d_init_muj_00[];
extern int32_t suifu_d_talk_muj_00[];
extern int32_t suifu_e_init[];
extern int32_t suifu_e_talk[];
extern int32_t korutesu_init_muj_00[];
extern int32_t korutesu_talk_muj_00[];
extern int32_t mony_talk_muj_00[];
extern int32_t peton_talk_muj_00[];
extern int32_t mujinto_hyochaku[];
extern int32_t suifu_random_sub;
extern int32_t dai3_shurai_sub[];
extern int32_t dai3_shurai;
extern int32_t kaizoku_vs_dai3[];
extern int32_t dai3_beat_back[];
extern int32_t leave_island[];
extern int32_t into_muj_ship[];
extern int32_t mony_peton_boarding[];
extern int32_t muj_00_init_evt[];
extern int32_t garawaru_init_muj_01[];
extern int32_t garawaru_talk_muj_01[];
extern int32_t marco_init_muj_01[];
extern int32_t marco_talk_muj_01[];
extern int32_t megane_init_muj_01[];
extern int32_t megane_talk_muj_01[];
extern int32_t odoodo_init_muj_01[];
extern int32_t odoodo_talk_muj_01[];
extern int32_t suifu_a_init_muj_01[];
extern int32_t suifu_a_talk_muj_01[];
extern int32_t suifu_bomb_init[];
extern int32_t suifu_bomb_talk[];
extern int32_t suifu_d_init_muj_01[];
extern int32_t suifu_d_regl_muj_01[];
extern int32_t suifu_d_talk_muj_01[];
extern int32_t borosuifu_a_init[];
extern int32_t borosuifu_a_talk[];
extern int32_t borosuifu_b_init[];
extern int32_t borosuifu_b_regl[];
extern int32_t borosuifu_b_talk[];
extern int32_t mony_init_muj_01[];
extern int32_t peton_init_muj_01[];
extern int32_t kyoshu_event[];
extern int32_t settoku_event_main[];
extern int32_t muj_01_init_evt[];
extern int32_t mony_init_muj_02[];
extern int32_t mony_talk_muj_02[];
extern int32_t peton_init_muj_02[];
extern int32_t peton_talk_muj_02[];
extern int32_t muj_02_init_evt[];
extern int32_t muj_03_event_01[];
extern int32_t sanders_funto[];
extern int32_t muj_04_init_evt[];
extern int32_t sanders_nakama[];
extern int32_t sanders_init_muj_05[];
extern int32_t sanders_talk_sub[];
extern int32_t sanders_talk_muj_05[];
extern int32_t marco_init_muj_05[];
extern int32_t marco_talk_muj_05[];
extern int32_t sanders_kyushutsu_sub[];
extern int32_t marco_and_jewel[];
extern int32_t dokuroiwa_touch[];
extern int32_t dokuroiwa_bomb[];
extern int32_t dokuroiwa_init[];
extern int32_t yure_yashi_ha1[];
extern int32_t yure_yashi[];
extern int32_t muj_05_init_evt[];
extern int32_t megane_init_muj_10[];
extern int32_t bero_in_event_muj_10[];
extern int32_t nazo_event[];
extern int32_t oidasare_event[];
extern int32_t unk_evt_muj_0002cb0c[];
extern int32_t muj_10_init_evt[];
extern int32_t warning_event[];
extern int32_t muj_11_init_evt[];
extern int32_t bero_custom_muj_12[];
extern int32_t korutesu_event[];
extern int32_t korutesu_event_after[];
extern int32_t kosho_event[];
extern int32_t muj_12_init_evt[];
extern int32_t onryo_event[];
extern int32_t muj_20_koopa_evt[];
extern int32_t muj_20_init_evt[];

EVT_BEGIN(mony_talk_muj_00_evt)
	IF_EQUAL(GSW(1709), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_94_04"), 0, PTR("me"))
	ELSE()
		SWITCH(GSW(1750))
			CASE_EQUAL(256)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_94_04"), 0, PTR("me"))
			CASE_END()
			CASE_BETWEEN(257, 258)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_117_07"), 0, PTR("me"))
			CASE_END()
		END_SWITCH()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(mony_talk_muj_00_hook)
	RUN_CHILD_EVT(mony_talk_muj_00_evt)
	RETURN()
EVT_END()

EVT_BEGIN(peton_talk_muj_00_evt)
	IF_EQUAL(GSW(1709), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_94_04"), 0, PTR("me"))
	ELSE()
		SWITCH(GSW(1750))
			CASE_EQUAL(256)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_94_04"), 0, PTR("me"))
			CASE_END()
			CASE_BETWEEN(257, 258)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_117_07"), 0, PTR("me"))
			CASE_END()
		END_SWITCH()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(peton_talk_muj_00_hook)
	RUN_CHILD_EVT(peton_talk_muj_00_evt)
	RETURN()
EVT_END()

EVT_BEGIN(muj_00_init_evt_evt)
	IF_EQUAL(GSW(1709), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_94_04"), 0, PTR("me"))
		SET(LW(0), 1)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(muj_00_init_evt_evt_2)
	RUN_CHILD_EVT(nannpc_mode_setup)
	USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S_c"), 1)
	USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A_c"), 1, 1)
	USER_FUNC(evt_hit::evt_hit_bind_mapobj, PTR("A_c"), PTR("S_c"))
	RUN_CHILD_EVT(dai3_shurai)
	RUN_CHILD_EVT(suifu_random_sub)
	USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A_b_bero01"), 1, 0)
	RETURN()
EVT_END()

EVT_BEGIN(muj_00_init_evt_hook)
	RUN_CHILD_EVT(muj_00_init_evt_evt)
	IF_EQUAL(LW(0), 1)
		USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A_b_bero01"), 1, 0)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1717), 256)
		RUN_CHILD_EVT(muj_00_init_evt_evt_2)
		RETURN()
	END_IF()
	GOTO(90)
EVT_END()

EVT_BEGIN(garawaru_init_muj_01_evt)
	SWITCH(GSW(1717))
		CASE_BETWEEN(242, 254)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -155, 0, 170)
		CASE_END()
		CASE_LARGE_EQUAL(269)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -110, 0, 225)
		CASE_END()
	END_SWITCH()
	SWITCH(GSW(1705))
		CASE_BETWEEN(232, 238)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -155, 0, 170)
		CASE_END()
		CASE_EQUAL(239)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -110, 0, 225)
		CASE_END()
		CASE_BETWEEN(240, 2241)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -155, 0, 170)
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_init_muj_01_hook)
	RUN_CHILD_EVT(garawaru_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_talk_muj_01_evt)
	SWITCH(GSW(1705))
		CASE_EQUAL(233)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("$"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_BETWEEN(234, 238)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_32_02"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_EQUAL(239)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_71_05"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_71_04"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
		CASE_BETWEEN(240, 241)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_02"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_02_01"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
	END_SWITCH()
	SWITCH(GSW(1717))
		CASE_BETWEEN(242, 254)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_13"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_132_01"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
		CASE_BETWEEN(268, 272)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_149"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_1492_01"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
	END_SWITCH()
	SWITCH(GSW(1709))
		CASE_BETWEEN(4, 6)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_150"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_1502_01"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
		CASE_EQUAL(7)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_151"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_1512_01"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_LARGE_EQUAL(GSW(1709), 10)
		IF_SMALL_EQUAL(GSW(1707), 373)
			SET(LF(0), 0)
			IF_LARGE_EQUAL(GSW(1707), 354)
				IF_SMALL_EQUAL(GSW(1707), 354)
					IF_EQUAL(GSWF(3877), 1)
						SET(GSWF(3878), 1)
						SET(LF(0), 1)
					END_IF()
				END_IF()
			END_IF()
		END_IF()
		IF_EQUAL(LF(0), 0)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_152"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_1522_01"), 0, PTR("me"))
			END_IF()
		ELSE()
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_muj_01"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_muj_012_01"), 0, PTR("me"))
			END_IF()
		END_IF()
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 401)
		USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
		IF_EQUAL(LW(0), 3)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_153"), 0, PTR("me"))
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_1532_01"), 0, PTR("me"))
		END_IF()
		RETURN()
	END_IF()
	USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
	IF_EQUAL(LW(0), 3)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_153_01"), 0, PTR("me"))
	ELSE()
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_153_01_san"), 0, PTR("me"))
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_talk_muj_01_hook)
	RUN_CHILD_EVT(garawaru_talk_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(marco_init_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1705), 232)
		IF_SMALL_EQUAL(GSW(1705), 239)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -164, 0, 170)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1717), 243)
		IF_SMALL_EQUAL(GSW(1717), 254)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -164, 0, 170)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(marco_init_muj_01_hook)
	RUN_CHILD_EVT(marco_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(megane_init_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1705), 234)
		IF_SMALL_EQUAL(GSW(1717), 250)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 115, 0, 247)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(megane_init_muj_01_hook)
	RUN_CHILD_EVT(megane_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_init_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1705), 234)
		IF_SMALL_EQUAL(GSW(1717), 254)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 95, 0, 82)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_init_muj_01_hook)
	RUN_CHILD_EVT(odoodo_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_a_init_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1705), 232)
		IF_SMALL_EQUAL(GSW(1717), 261)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -193, 0, 77)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(suifu_a_init_muj_01_hook)
	RUN_CHILD_EVT(suifu_a_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_a_talk_muj_01_evt)
	SWITCH(GSW(1705))
		CASE_EQUAL(233)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_22_2"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_BETWEEN(234, 238)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_32_03"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_BETWEEN(239, 241)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_71_06"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	SWITCH(GSW(1717))
		CASE_BETWEEN(242, 254)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_14"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_LARGE_EQUAL(255)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_16"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(suifu_a_talk_muj_01_hook)
	RUN_CHILD_EVT(suifu_a_talk_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_bomb_init_evt)
	IF_LARGE_EQUAL(GSW(1705), 232)
		IF_SMALL_EQUAL(GSW(1717), 261)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -33, 0, 74)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(suifu_bomb_init_hook)
	RUN_CHILD_EVT(suifu_bomb_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_bomb_talk_evt)
	SWITCH(GSW(1705))
		CASE_EQUAL(233)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_22_3"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_BETWEEN(234, 238)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_32_04"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_BETWEEN(239, 241)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_71_07"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	SWITCH(GSW(1717))
		CASE_BETWEEN(242, 254)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_15"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_LARGE_EQUAL(255)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_17"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(suifu_bomb_talk_hook)
	RUN_CHILD_EVT(suifu_bomb_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_d_talk_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1717), 268)
		IF_SMALL_EQUAL(GSW(1717), 272)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_159"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	SWITCH(GSW(1709))
		CASE_BETWEEN(4, 6)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_160"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_EQUAL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_161"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_LARGE_EQUAL(10)
			IF_SMALL_EQUAL(GSW(1707), 377)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_162"), 0, PTR("me"))
				RETURN()
			END_IF()
		CASE_END()
	END_SWITCH()
	IF_LARGE_EQUAL(GSW(1707), 377)
		IF_SMALL_EQUAL(GSW(1708), 402)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_163"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_163_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(suifu_d_talk_muj_01_hook)
	RUN_CHILD_EVT(suifu_d_talk_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(borosuifu_a_talk_evt)
	IF_LARGE_EQUAL(GSW(1717), 268)
		IF_SMALL_EQUAL(GSW(1717), 272)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_154"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	SWITCH(GSW(1709))
		CASE_BETWEEN(4, 6)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_155"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_EQUAL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_156"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_LARGE_EQUAL(10)
			IF_SMALL_EQUAL(GSW(1707), 377)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_157"), 0, PTR("me"))
				RETURN()
			END_IF()
		CASE_END()
	END_SWITCH()
	IF_LARGE_EQUAL(GSW(1707), 377)
		IF_SMALL_EQUAL(GSW(1708), 402)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_158"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_158_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(borosuifu_a_talk_hook)
	RUN_CHILD_EVT(borosuifu_a_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(borosuifu_b_talk_evt)
	IF_LARGE_EQUAL(GSW(1717), 268)
		IF_SMALL_EQUAL(GSW(1717), 272)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_164"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	SWITCH(GSW(1709))
		CASE_BETWEEN(4, 6)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_165"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_EQUAL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_166"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_LARGE_EQUAL(10)
			IF_SMALL_EQUAL(GSW(1707), 377)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_167"), 0, PTR("me"))
				RETURN()
			END_IF()
		CASE_END()
	END_SWITCH()
	IF_LARGE_EQUAL(GSW(1707), 377)
		IF_SMALL_EQUAL(GSW(1708), 402)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_168"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_168_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(borosuifu_b_talk_hook)
	RUN_CHILD_EVT(borosuifu_b_talk_evt)
	RETURN()
EVT_END()

void ApplyMujPatches(OSModuleInfo* module_info)
{
	garawaru_init_muj_00[1] = GSW(1705);
	garawaru_init_muj_00[3] = 257;
	garawaru_init_muj_00[4] = 258;

	odoodo_init_muj_00[1] = GSW(1705);
	odoodo_init_muj_00[3] = 257;
	odoodo_init_muj_00[4] = 258;

	suifu_c_init[1] = GSW(1705);
	suifu_c_init[3] = 257;
	suifu_c_init[4] = 258;

	suifu_c_talk[1] = GSW(1705);
	suifu_c_talk[3] = 256;
	suifu_c_talk[11] = 257;
	suifu_c_talk[12] = 258;

	suifu_d_init_muj_00[1] = GSW(1705);
	suifu_d_init_muj_00[3] = 257;
	suifu_d_init_muj_00[4] = 258;

	suifu_d_talk_muj_00[1] = GSW(1705);
	suifu_d_talk_muj_00[3] = 256;
	suifu_d_talk_muj_00[11] = 257;
	suifu_d_talk_muj_00[12] = 258;

	suifu_e_init[1] = GSW(1705);
	suifu_e_init[3] = 257;
	suifu_e_init[4] = 258;

	suifu_e_talk[1] = GSW(1705);
	suifu_e_talk[3] = 256;
	suifu_e_talk[11] = 257;
	suifu_e_talk[12] = 258;

	korutesu_init_muj_00[1] = GSW(1705);
	korutesu_init_muj_00[3] = 267;

	korutesu_talk_muj_00[1] = GSW(1705);
	korutesu_talk_muj_00[3] = 268;

	patch::writePatch(&mony_talk_muj_00[0], mony_talk_muj_00_hook, sizeof(mony_talk_muj_00_hook));

	patch::writePatch(&peton_talk_muj_00[0], peton_talk_muj_00_hook, sizeof(peton_talk_muj_00_hook));

	mujinto_hyochaku[277] = GSW(1705);
	mujinto_hyochaku[278] = 10;

	dai3_shurai_sub[1096] = GSW(1705);
	dai3_shurai_sub[1097] = 257;

	kaizoku_vs_dai3[522] = GSW(1705);
	kaizoku_vs_dai3[523] = 260;

	dai3_beat_back[148] = GSW(1705);
	dai3_beat_back[149] = 261;
	
	leave_island[757] = GSW(1705);
	leave_island[758] = 268;
	
	into_muj_ship[161] = GSW(1709);
	into_muj_ship[162] = 3;
	into_muj_ship[164] = GSW(1709);
	into_muj_ship[165] = 4;

	mony_peton_boarding[4] = GSW(1709);
	mony_peton_boarding[5] = 7;
	mony_peton_boarding[11] = GSW(1709);
	mony_peton_boarding[12] = 8;
	mony_peton_boarding[45] = GSW(1709);
	mony_peton_boarding[46] = 8;
	mony_peton_boarding[111] = GSW(1709);
	mony_peton_boarding[112] = 8;

	muj_00_init_evt[29] = GSW(1717);
	muj_00_init_evt[30] = 268;
	muj_00_init_evt[46] = GSW(1705);
	muj_00_init_evt[47] = 9;
	muj_00_init_evt[49] = GSW(1717);
	muj_00_init_evt[50] = 267;
	muj_00_init_evt[52] = GSW(1717);
	muj_00_init_evt[53] = 260;
	muj_00_init_evt[98] = GSW(1708);
	muj_00_init_evt[99] = 382;
	muj_00_init_evt[236] = GSW(1717);
	muj_00_init_evt[237] = 255;
	muj_00_init_evt[264] = GSW(1717);
	muj_00_init_evt[265] = 260;
	muj_00_init_evt[281] = GSW(1717);
	muj_00_init_evt[282] = 257;
	muj_00_init_evt[284] = GSW(1717);
	muj_00_init_evt[285] = 258;

	patch::writePatch(&muj_00_init_evt[296], muj_00_init_evt_hook, sizeof(muj_00_init_evt_hook));

	muj_00_init_evt[321] = EVT_HELPER_CMD(1, 3);
	muj_00_init_evt[322] = 90;
	muj_00_init_evt[323] = EVT_HELPER_CMD(1, 34);
	muj_00_init_evt[324] = GSW(1717);

	patch::writePatch(&garawaru_init_muj_01[0], garawaru_init_muj_01_hook, sizeof(garawaru_init_muj_01_hook));

	patch::writePatch(&garawaru_talk_muj_01[0], garawaru_talk_muj_01_hook, sizeof(garawaru_talk_muj_01_hook));

	patch::writePatch(&marco_init_muj_01[0], marco_init_muj_01_hook, sizeof(marco_init_muj_01_hook));

	marco_talk_muj_01[1] = GSW(1705);
	marco_talk_muj_01[3] = 234;
	marco_talk_muj_01[4] = 237;
	marco_talk_muj_01[13] = GSW(1705);
	marco_talk_muj_01[15] = 233;
	marco_talk_muj_01[23] = 234;
	marco_talk_muj_01[24] = 234;
	marco_talk_muj_01[46] = 235;
	marco_talk_muj_01[98] = 236;
	marco_talk_muj_01[361] = 237;
	marco_talk_muj_01[369] = 238;
	marco_talk_muj_01[377] = 239;
	marco_talk_muj_01[836] = EVT_HELPER_CMD(0, 42);
	marco_talk_muj_01[837] = 0;
	marco_talk_muj_01[838] = 0;

	patch::writePatch(&megane_init_muj_01[0], megane_init_muj_01_hook, sizeof(megane_init_muj_01_hook));

	megane_talk_muj_01[1] = GSW(1705);
	megane_talk_muj_01[3] = 234;
	megane_talk_muj_01[4] = 238;
	megane_talk_muj_01[12] = 239;
	megane_talk_muj_01[34] = 240;
	megane_talk_muj_01[35] = 241;
	megane_talk_muj_01[42] = EVT_HELPER_CMD(0, 42);
	megane_talk_muj_01[43] = 0;
	megane_talk_muj_01[44] = 0;

	patch::writePatch(&odoodo_init_muj_01[0], odoodo_init_muj_01_hook, sizeof(odoodo_init_muj_01_hook));

	odoodo_talk_muj_01[1] = GSW(1705);
	odoodo_talk_muj_01[3] = 234;
	odoodo_talk_muj_01[4] = 238;
	odoodo_talk_muj_01[12] = 239;
	odoodo_talk_muj_01[34] = 240;
	odoodo_talk_muj_01[35] = 241;
	odoodo_talk_muj_01[42] = EVT_HELPER_CMD(0, 42);
	odoodo_talk_muj_01[43] = 0;
	odoodo_talk_muj_01[44] = 0;

	patch::writePatch(&suifu_a_init_muj_01[0], suifu_a_init_muj_01_hook, sizeof(suifu_a_init_muj_01_hook));

	patch::writePatch(&suifu_a_talk_muj_01[0], suifu_a_talk_muj_01_hook, sizeof(suifu_a_talk_muj_01_hook));

	patch::writePatch(&suifu_bomb_init[0], suifu_bomb_init_hook, sizeof(suifu_bomb_init_hook));

	patch::writePatch(&suifu_bomb_talk[0], suifu_bomb_talk_hook, sizeof(suifu_bomb_talk_hook));

	suifu_d_init_muj_01[1] = GSW(1717);
	suifu_d_init_muj_01[3] = 269;

	suifu_d_regl_muj_01[1] = GSW(1717);
	suifu_d_regl_muj_01[3] = 269;

	patch::writePatch(&suifu_d_talk_muj_01[0], suifu_d_talk_muj_01_hook, sizeof(suifu_d_talk_muj_01_hook));
	
	borosuifu_a_init[1] = GSW(1717);
	borosuifu_a_init[3] = 269;

	patch::writePatch(&borosuifu_a_talk[0], borosuifu_a_talk_hook, sizeof(borosuifu_a_talk_hook));

	borosuifu_b_init[6] = GSW(1717);
	borosuifu_b_init[8] = 269;

	borosuifu_b_regl[1] = GSW(1717);
	borosuifu_b_regl[3] = 269;

	patch::writePatch(&borosuifu_b_talk[0], borosuifu_b_talk_hook, sizeof(borosuifu_b_talk_hook));

	mony_init_muj_01[1] = GSW(1717);
	mony_init_muj_01[3] = 269;
	mony_init_muj_01[4] = 272;

	peton_init_muj_01[1] = GSW(1717);
	peton_init_muj_01[3] = 269;
	peton_init_muj_01[4] = 272;

	kyoshu_event[182] = GSW(1705);
	kyoshu_event[183] = 11;

	settoku_event_main[68] = GSW(1705);
	settoku_event_main[69] = 13;

	muj_01_init_evt[4] = GSW(1708);
	muj_01_init_evt[5] = 400;
	muj_01_init_evt[76] = GSW(1708);
	muj_01_init_evt[77] = 382;
	muj_01_init_evt[149] = GSW(1705);
	muj_01_init_evt[151] = 232;
	muj_01_init_evt[155] = 234;
	muj_01_init_evt[164] = GSW(1708);
	muj_01_init_evt[165] = 400;
	muj_01_init_evt[187] = GSW(1708);
	muj_01_init_evt[188] = 402;

	mony_init_muj_02[6] = GSW(1709);
	mony_init_muj_02[8] = 5;
	mony_init_muj_02[9] = 6;

	peton_init_muj_02[6] = GSW(1709);
	peton_init_muj_02[8] = 5;
	peton_init_muj_02[9] = 6;

	mony_talk_muj_02[1] = GSW(1709);
	mony_talk_muj_02[3] = 5;
	mony_talk_muj_02[16] = GSW(1709);
	mony_talk_muj_02[17] = 6;
	mony_talk_muj_02[19] = 6;
	mony_talk_muj_02[44] = GSW(1709);
	mony_talk_muj_02[45] = 7;

	peton_talk_muj_02[1] = GSW(1709);
	peton_talk_muj_02[3] = 5;
	peton_talk_muj_02[16] = GSW(1709);
	peton_talk_muj_02[17] = 6;
	peton_talk_muj_02[19] = 6;
	peton_talk_muj_02[44] = GSW(1709);
	peton_talk_muj_02[45] = 7;

	muj_02_init_evt[31] = GSW(1717);
	muj_02_init_evt[32] = 269;
	muj_02_init_evt[87] = GSW(1717);
	muj_02_init_evt[88] = 261;

	muj_03_event_01[1] = GSW(1705);
	muj_03_event_01[3] = 12;

	sanders_funto[462] = GSW(1705);
	sanders_funto[463] = 12;

	muj_04_init_evt[70] = GSW(1705);
	muj_04_init_evt[71] = 11;

	sanders_nakama[163] = GSW(1705);
	sanders_nakama[164] = 17;

	sanders_init_muj_05[1] = GSW(1705);
	sanders_init_muj_05[3] = 13;
	sanders_init_muj_05[9] = 13;
	sanders_init_muj_05[10] = 15;
	sanders_init_muj_05[22] = 16;

	sanders_talk_sub[63] = GSW(1705);
	sanders_talk_sub[64] = 16;

	sanders_talk_muj_05[1] = GSW(1705);
	sanders_talk_muj_05[3] = 13;
	sanders_talk_muj_05[4] = 15;
	sanders_talk_muj_05[38] = 16;

	marco_init_muj_05[1] = GSW(1705);
	marco_init_muj_05[3] = 19;

	marco_talk_muj_05[1] = GSW(1705);
	marco_talk_muj_05[3] = 19;

	sanders_kyushutsu_sub[7] = GSW(1705);
	sanders_kyushutsu_sub[8] = 13;

	marco_and_jewel[77] = GSW(1705);
	marco_and_jewel[78] = 19;

	dokuroiwa_touch[23] = GSW(1705);
	dokuroiwa_touch[24] = 18;

	dokuroiwa_bomb[302] = GSW(1717);
	dokuroiwa_bomb[303] = 1;

	dokuroiwa_init[14] = GSW(1717);
	dokuroiwa_init[15] = 1;

	yure_yashi_ha1[84] = GSW(1705);
	yure_yashi_ha1[85] = 13;
	yure_yashi_ha1[173] = GSW(1705);
	yure_yashi_ha1[174] = 13;

	yure_yashi[46] = GSW(1705);
	yure_yashi[47] = 13;
	yure_yashi[78] = GSW(1705);
	yure_yashi[79] = 13;

	muj_05_init_evt[1] = GSW(1705);
	muj_05_init_evt[3] = 12;
	muj_05_init_evt[13] = 16;
	muj_05_init_evt[65] = GSW(1705);
	muj_05_init_evt[67] = 12;
	muj_05_init_evt[79] = 18;
	muj_05_init_evt[86] = GSW(1709);
	muj_05_init_evt[87] = 5;

	megane_init_muj_10[1] = GSW(1717);
	megane_init_muj_10[3] = 251;
	megane_init_muj_10[4] = 252;

	bero_in_event_muj_10[1] = GSW(1717);
	bero_in_event_muj_10[2] = 258;

	nazo_event[168] = GSW(1717);
	nazo_event[169] = 251;

	oidasare_event[147] = GSW(1717);
	oidasare_event[148] = 254;

	unk_evt_muj_0002cb0c[79] = GSW(1717);
	unk_evt_muj_0002cb0c[80] = 259;

	muj_10_init_evt[1] = GSW(1717);
	muj_10_init_evt[2] = 253;
	muj_10_init_evt[45] = GSW(1717);
	muj_10_init_evt[46] = 250;
	muj_10_init_evt[51] = GSW(1717);
	muj_10_init_evt[52] = 253;
	muj_10_init_evt[57] = GSW(1717);
	muj_10_init_evt[58] = 258;
	muj_10_init_evt[65] = GSW(1717);
	muj_10_init_evt[66] = 260;
	muj_10_init_evt[85] = GSW(1717);
	muj_10_init_evt[86] = 260;
	muj_10_init_evt[88] = GSW(1717);
	muj_10_init_evt[89] = 258;
	muj_10_init_evt[118] = GSW(1717);
	muj_10_init_evt[119] = 260;
	muj_10_init_evt[164] = GSW(1717);
	muj_10_init_evt[165] = 257;
	muj_10_init_evt[167] = GSW(1717);
	muj_10_init_evt[168] = 258;
	muj_10_init_evt[184] = GSW(1717);
	muj_10_init_evt[185] = 258;

	warning_event[31] = GSW(1717);
	warning_event[32] = 252;

	muj_11_init_evt[1] = GSW(1717);
	muj_11_init_evt[2] = 253;
	muj_11_init_evt[28] = GSW(1717);
	muj_11_init_evt[29] = 251;

	bero_custom_muj_12[1] = GSW(1717);
	bero_custom_muj_12[3] = 252;
	bero_custom_muj_12[57] = 257;

	korutesu_event[606] = GSW(1717);
	korutesu_event[607] = 253;

	korutesu_event_after[132] = GSW(1717);
	korutesu_event_after[133] = 253;

	kosho_event[504] = GSW(1717);
	kosho_event[505] = 258;

	muj_12_init_evt[8] = GSW(1717);
	muj_12_init_evt[9] = 253;
	muj_12_init_evt[28] = GSW(1717);
	muj_12_init_evt[29] = 252;
	muj_12_init_evt[44] = GSW(1717);
	muj_12_init_evt[45] = 253;
	muj_12_init_evt[50] = GSW(1717);
	muj_12_init_evt[51] = 257;

	onryo_event[20] = GSW(1705);
	onryo_event[21] = 8;
	onryo_event[144] = GSW(1705);
	onryo_event[145] = 8;
	onryo_event[2235] = GSW(1705);
	onryo_event[2236] = 9;

	muj_20_koopa_evt[122] = GSW(1715);
	muj_20_koopa_evt[123] = 216;

	muj_20_init_evt[109] = GSW(1715);
	muj_20_init_evt[110] = 215;
	muj_20_init_evt[210] = GSW(1705);
	muj_20_init_evt[211] = 7;
	muj_20_init_evt[224] = GSW(1705);
	muj_20_init_evt[225] = 8;
	muj_20_init_evt[334] = GSW(1708);
	muj_20_init_evt[335] = 402;
}