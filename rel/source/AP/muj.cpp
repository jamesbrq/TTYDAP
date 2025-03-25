#include "evt_cmd.h"
#include "patch.h"
#include <AP/rel_patch_definitions.h>
#include <ttyd/evt_cam.h>
#include <ttyd/evt_hit.h>
#include <ttyd/evt_item.h>
#include <ttyd/evt_map.h>
#include <ttyd/evt_mario.h>
#include <ttyd/evt_msg.h>
#include <ttyd/evt_npc.h>
#include <ttyd/evt_snd.h>

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
extern int32_t nakamaware_event[];
extern int32_t settoku_event[];
extern int32_t settoku_event_main[];
extern int32_t muj_01_init_evt[];
extern int32_t mony_init_muj_02[];
extern int32_t mony_talk_muj_02[];
extern int32_t peton_init_muj_02[];
extern int32_t peton_talk_muj_02[];
extern int32_t muj_02_init_evt[];
extern int32_t muj_03_event_01[];
extern int32_t muj_03_init_evt[];
extern int32_t sanders_funto[];
extern int32_t yashi_yure[];
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
extern int32_t koburon_dead[];
extern int32_t muj_all_party_lecture[];

EVT_BEGIN(mony_talk_muj_00_evt)
	IF_EQUAL(GSW(1709), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_146_02"), 0, PTR("me"))
	ELSE()
		SWITCH(GSW(1717))
			CASE_EQUAL(13)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_94_04"), 0, PTR("me"))
			CASE_END()
			CASE_BETWEEN(14, 15)
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
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_146_01"), 0, PTR("me"))
	ELSE()
		SWITCH(GSW(1717))
			CASE_EQUAL(13)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_94_05"), 0, PTR("me"))
			CASE_END()
			CASE_BETWEEN(14, 15)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_117_08"), 0, PTR("me"))
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
	IF_EQUAL(GSW(1705), 9)
		RUN_EVT(mujinto_hyochaku)
		USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A_b_bero01"), 1, 0)
		SET(LW(0), 1)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(muj_00_init_evt_evt_2)
	RUN_CHILD_EVT(&nannpc_mode_setup)
	USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S_c"), 1)
	USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A_c"), 1, 1)
	USER_FUNC(evt_hit::evt_hit_bind_mapobj, PTR("A_c"), PTR("S_c"))
	RUN_EVT(&dai3_shurai)
	RUN_EVT(&suifu_random_sub)
	USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A_b_bero01"), 1, 0)
	RETURN()
EVT_END()

EVT_BEGIN(muj_00_init_evt_hook)
	RUN_CHILD_EVT(muj_00_init_evt_evt)
	IF_EQUAL(LW(0), 1)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1717), 13)
		RUN_CHILD_EVT(muj_00_init_evt_evt_2)
		RETURN()
	END_IF()
	GOTO(&muj_00_init_evt[323])
EVT_END()

EVT_BEGIN(garawaru_init_muj_01_evt)
	SWITCH(GSW(1717))
		CASE_BETWEEN(1, 11)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -155, 0, 170)
		CASE_END()
		CASE_LARGE_EQUAL(26)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -110, 0, 225)
		CASE_END()
	END_SWITCH()
	IF_LARGE_EQUAL(GSW(1705), 10)
		IF_SMALL_EQUAL(GSW(1705), 11)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -155, 0, 170)
			RETURN()
		END_IF()
	END_IF()
	SWITCH(GSW(1719))
		CASE_BETWEEN(0, 5)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -155, 0, 170)
		CASE_END()
		CASE_EQUAL(6)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -110, 0, 225)
		CASE_END()
		CASE_BETWEEN(7, 8)
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
	IF_EQUAL(GSW(1705), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_22_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL_EQUAL(GSW(1707), 16)
		IF_EQUAL(GSWF(3878), 0)
			SET(LF(0), 0)
			IF_LARGE_EQUAL(GSW(1707), 3)
				IF_SMALL_EQUAL(GSW(1707), 3)
					IF_EQUAL(GSWF(3877), 1)
						SET(GSWF(3878), 1)
						SET(LF(0), 1)
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
	END_IF()
	SWITCH(GSW(1719))
		CASE_BETWEEN(0, 5)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_32_02"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_EQUAL(6)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_71_05"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_71_04"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
		CASE_BETWEEN(7, 8)
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
		CASE_BETWEEN(1, 11)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_13"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_132_01"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
		CASE_BETWEEN(25, 29)
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
	IF_SMALL(GSW(1708), 17)
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
	IF_LARGE_EQUAL(GSW(1705), 10)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -164, 0, 170)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		RETURN()
	END_IF()
	IF_SMALL_EQUAL(GSW(1705), 6)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -164, 0, 170)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		RETURN()
	END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1717), 2)
		IF_SMALL_EQUAL(GSW(1717), 11)
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
	IF_LARGE_EQUAL(GSW(1719), 1)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 115, 0, 247)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(megane_init_muj_01_hook)
	RUN_CHILD_EVT(megane_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_init_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1719), 1)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 95, 0, 82)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_init_muj_01_hook)
	RUN_CHILD_EVT(odoodo_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_a_init_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1705), 10)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -193, 0, 77)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(suifu_a_init_muj_01_hook)
	RUN_CHILD_EVT(suifu_a_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_a_talk_muj_01_evt)
	IF_EQUAL(GSW(1705), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_22_2"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1719))
		CASE_BETWEEN(0, 5)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_32_03"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_BETWEEN(6, 8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_71_06"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	SWITCH(GSW(1717))
		CASE_BETWEEN(1, 11)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_14"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_LARGE_EQUAL(12)
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
	IF_LARGE_EQUAL(GSW(1705), 10)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -33, 0, 74)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(suifu_bomb_init_hook)
	RUN_CHILD_EVT(suifu_bomb_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_bomb_talk_evt)
	IF_EQUAL(GSW(1705), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_22_3"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1705))
		CASE_BETWEEN(0, 5)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_32_04"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_BETWEEN(6, 8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_71_07"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	SWITCH(GSW(1717))
		CASE_BETWEEN(1, 11)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_15"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_LARGE_EQUAL(12)
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
	IF_LARGE_EQUAL(GSW(1717), 25)
		IF_SMALL_EQUAL(GSW(1717), 29)
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
	END_SWITCH()
	IF_LARGE_EQUAL(GSW(1707), 20)
		IF_SMALL_EQUAL(GSW(1708), 18)
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
	IF_LARGE_EQUAL(GSW(1717), 25)
		IF_SMALL_EQUAL(GSW(1717), 29)
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
	END_SWITCH()
	IF_LARGE_EQUAL(GSW(1707), 20)
		IF_SMALL_EQUAL(GSW(1708), 18)
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
	IF_LARGE_EQUAL(GSW(1717), 25)
		IF_SMALL_EQUAL(GSW(1717), 29)
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
	END_SWITCH()
	IF_LARGE_EQUAL(GSW(1707), 20)
		IF_SMALL_EQUAL(GSW(1708), 18)
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

EVT_BEGIN(muj_01_init_evt_evt)
	IF_EQUAL(GSW(1705), 10)
		RUN_EVT(nakamaware_event)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1719), 1)
		IF_EQUAL(GSWF(3129), 0)
			RUN_EVT(settoku_event)
			RETURN()
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(muj_01_init_evt_hook)
	RUN_CHILD_EVT(muj_01_init_evt_evt)
	GOTO(&muj_01_init_evt[163])
EVT_END()

EVT_BEGIN(koburon_dead_evt)
	USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
	USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_BTL_DAMAGED_PLIABLE1"), LW(0), LW(1), LW(2), 0)
	USER_FUNC(evt_npc::evt_npc_set_damage_anim, PTR("me"))
	WAIT_MSEC(200)
	ADD(LW(1), 30)
	ADD(LW(2), 10)
	SET(LW(4), GSWF(6082))
	USER_FUNC(evt_item::evt_item_entry, PTR("item_99"), LW(3), LW(0), LW(1), LW(2), 14, LW(4), 0)
	WAIT_MSEC(400)
	IF_EQUAL(GSW(721), 7)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("koburon_14"), 0, PTR("me"))
		GOTO(99)
	END_IF()
	USER_FUNC(koburon_get_encount_info, LW(0))
	SWITCH(LW(0))
		CASE_OR(131072)
		CASE_OR(262144)
		CASE_OR(524288)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("koburon_10"), 0, PTR("me"))
		CASE_END()
		CASE_OR(1048576)
		CASE_OR(2097152)
		CASE_OR(4194304)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("koburon_10"), 0, PTR("me"))
		CASE_END()
		CASE_OR(8388608)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("koburon_11"), 0, PTR("me"))
		CASE_END()
		CASE_OR(16777216)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("koburon_13"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	LBL(99)
	USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), PTR("S_1"))
	WAIT_MSEC(100)
	ADD(GSW(721), 1)
	SET(LSWF(2), 1)
	USER_FUNC(evt_npc::evt_npc_reaction_flag_onoff, 0, PTR("me"), 15)
	USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), PTR("A_2"))
	WAIT_MSEC(500)
	USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 1, 0)
	USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
	SET(GSWF(3151), 1)
	RETURN()
EVT_END()

EVT_BEGIN(koburon_dead_hook)
	SWITCH(GSW(721))
		CASE_EQUAL(0)
			SET(LW(3), 125)
			SET(LW(4), GSWF(6082))
		CASE_END()
		CASE_EQUAL(1)
			SET(LW(3), 125)
			SET(LW(4), GSWF(6083))
		CASE_END()
		CASE_EQUAL(2)
			SET(LW(3), 125)
			SET(LW(4), GSWF(6084))
		CASE_END()
		CASE_EQUAL(3)
			SET(LW(3), 125)
			SET(LW(4), GSWF(6085))
		CASE_END()
		CASE_EQUAL(4)
			SET(LW(3), 125)
			SET(LW(4), GSWF(6086))
		CASE_END()
		CASE_EQUAL(5)
			SET(LW(3), 125)
			SET(LW(4), GSWF(6087))
		CASE_END()
		CASE_EQUAL(6)
			SET(LW(3), 125)
			SET(LW(4), GSWF(6088))
		CASE_END()
		CASE_EQUAL(7)
			SET(LW(3), 125)
			SET(LW(4), GSWF(6089))
		CASE_END()
		CASE_ETC()
			SET(LW(3), 125)
			SET(LW(4), -1)
		CASE_END()
	END_SWITCH()
	RUN_CHILD_EVT(koburon_dead_evt)
	RETURN()
EVT_PATCH_END()

EVT_BEGIN(party_evt)
	USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
	USER_FUNC(evt_item::evt_item_entry, PTR("item01"), LW(3), LW(0), LW(1), LW(2), 16, GSWF(6081), 0)
	USER_FUNC(evt_item::evt_item_get_item, PTR("item01"))
	WAIT_MSEC(800)
	SET(GSW(1719), 6)
	USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
	USER_FUNC(evt_cam::evt_cam3d_evt_off, 500, 11)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
	USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG5_MUJ1"))
	RETURN()
EVT_END()


void ApplyMujPatches()
{
	patch::writePatch(&koburon_dead[0], koburon_dead_hook, sizeof(koburon_dead_hook));

	sanders_nakama[150] = EVT_HELPER_CMD(2, 50);
	sanders_nakama[151] = EVT_HELPER_OP(LW(3));
	patch::writePatch(&sanders_nakama[153], party_evt, sizeof(party_evt));

	garawaru_init_muj_00[1] = GSW(1717);
	garawaru_init_muj_00[3] = 14;
	garawaru_init_muj_00[4] = 15;

	odoodo_init_muj_00[1] = GSW(1717);
	odoodo_init_muj_00[3] = 14;
	odoodo_init_muj_00[4] = 15;

	suifu_c_init[1] = GSW(1717);
	suifu_c_init[3] = 14;
	suifu_c_init[4] = 15;

	suifu_c_talk[1] = GSW(1717);
	suifu_c_talk[3] = 13;
	suifu_c_talk[11] = 14;
	suifu_c_talk[12] = 15;

	suifu_d_init_muj_00[1] = GSW(1717);
	suifu_d_init_muj_00[3] = 14;
	suifu_d_init_muj_00[4] = 15;

	suifu_d_talk_muj_00[1] = GSW(1717);
	suifu_d_talk_muj_00[3] = 13;
	suifu_d_talk_muj_00[11] = 14;
	suifu_d_talk_muj_00[12] = 15;

	suifu_e_init[1] = GSW(1717);
	suifu_e_init[3] = 14;
	suifu_e_init[4] = 15;

	suifu_e_talk[1] = GSW(1717);
	suifu_e_talk[3] = 13;
	suifu_e_talk[11] = 14;
	suifu_e_talk[12] = 15;

	korutesu_init_muj_00[1] = GSW(1717);
	korutesu_init_muj_00[3] = 24;

	korutesu_talk_muj_00[1] = GSW(1717);
	korutesu_talk_muj_00[3] = 25;

	patch::writePatch(&mony_talk_muj_00[0], mony_talk_muj_00_hook, sizeof(mony_talk_muj_00_hook));

	patch::writePatch(&peton_talk_muj_00[0], peton_talk_muj_00_hook, sizeof(peton_talk_muj_00_hook));

	mujinto_hyochaku[277] = GSW(1705);
	mujinto_hyochaku[278] = 10;

	dai3_shurai_sub[1096] = GSW(1717);
	dai3_shurai_sub[1097] = 14;

	kaizoku_vs_dai3[522] = GSW(1717);
	kaizoku_vs_dai3[523] = 17;

	dai3_beat_back[148] = GSW(1717);
	dai3_beat_back[149] = 18;
	
	leave_island[757] = GSW(1717);
	leave_island[758] = 25;
	
	into_muj_ship[161] = GSW(1709);
	into_muj_ship[162] = 4;
	into_muj_ship[164] = GSW(1709);
	into_muj_ship[165] = 5;

	mony_peton_boarding[4] = GSW(1709);
	mony_peton_boarding[5] = 7;
	mony_peton_boarding[11] = GSW(1709);
	mony_peton_boarding[12] = 8;
	mony_peton_boarding[45] = GSW(1709);
	mony_peton_boarding[46] = 8;
	mony_peton_boarding[111] = GSW(1709);
	mony_peton_boarding[112] = 8;

	muj_00_init_evt[29] = GSW(1717);
	muj_00_init_evt[30] = 25;
	muj_00_init_evt[46] = GSW(1705);
	muj_00_init_evt[47] = 9;
	muj_00_init_evt[49] = GSW(1717);
	muj_00_init_evt[50] = 24;
	muj_00_init_evt[52] = GSW(1717);
	muj_00_init_evt[53] = 17;
	muj_00_init_evt[98] = GSW(1708);
	muj_00_init_evt[99] = 2;
	muj_00_init_evt[236] = GSW(1717);
	muj_00_init_evt[237] = 12;
	muj_00_init_evt[264] = GSW(1717);
	muj_00_init_evt[265] = 17;
	muj_00_init_evt[281] = GSW(1717);
	muj_00_init_evt[282] = 14;
	muj_00_init_evt[284] = GSW(1717);
	muj_00_init_evt[285] = 15;

	patch::writePatch(&muj_00_init_evt[296], muj_00_init_evt_hook, sizeof(muj_00_init_evt_hook));

	muj_00_init_evt[323] = EVT_HELPER_CMD(1, 34);
	muj_00_init_evt[324] = GSW(1717);
	muj_00_init_evt[326] = 14;
	muj_00_init_evt[332] = 16;
	muj_00_init_evt[370] = 17;
	muj_00_init_evt[411] = 24;
	muj_00_init_evt[481] = 25;

	patch::writePatch(&garawaru_init_muj_01[0], garawaru_init_muj_01_hook, sizeof(garawaru_init_muj_01_hook));

	patch::writePatch(&garawaru_talk_muj_01[0], garawaru_talk_muj_01_hook, sizeof(garawaru_talk_muj_01_hook));

	patch::writePatch(&marco_init_muj_01[0], marco_init_muj_01_hook, sizeof(marco_init_muj_01_hook));

	marco_talk_muj_01[1] = GSW(1719);
	marco_talk_muj_01[3] = 1;
	marco_talk_muj_01[4] = 4;
	marco_talk_muj_01[13] = GSW(1719);
	marco_talk_muj_01[15] = 0;
	marco_talk_muj_01[23] = 1;
	marco_talk_muj_01[24] = 1;
	marco_talk_muj_01[46] = 2;
	marco_talk_muj_01[89] = GSW(1719);
	marco_talk_muj_01[90] = 3;
	marco_talk_muj_01[98] = 3;
	marco_talk_muj_01[358] = GSW(1719);
	marco_talk_muj_01[359] = 4;
	marco_talk_muj_01[361] = 4;
	marco_talk_muj_01[369] = 5;
	marco_talk_muj_01[377] = 6;
	marco_talk_muj_01[831] = GSW(1719);
	marco_talk_muj_01[832] = 7;
	marco_talk_muj_01[836] = EVT_HELPER_CMD(0, 42);
	marco_talk_muj_01[837] = 0;
	marco_talk_muj_01[838] = 0;

	patch::writePatch(&megane_init_muj_01[0], megane_init_muj_01_hook, sizeof(megane_init_muj_01_hook));

	megane_talk_muj_01[1] = GSW(1719);
	megane_talk_muj_01[3] = 1;
	megane_talk_muj_01[4] = 5;
	megane_talk_muj_01[12] = 6;
	megane_talk_muj_01[34] = 7;
	megane_talk_muj_01[35] = 8;
	megane_talk_muj_01[42] = EVT_HELPER_CMD(0, 42);
	megane_talk_muj_01[43] = 0;
	megane_talk_muj_01[44] = 0;

	patch::writePatch(&odoodo_init_muj_01[0], odoodo_init_muj_01_hook, sizeof(odoodo_init_muj_01_hook));

	odoodo_talk_muj_01[1] = GSW(1719);
	odoodo_talk_muj_01[3] = 1;
	odoodo_talk_muj_01[4] = 5;
	odoodo_talk_muj_01[12] = 6;
	odoodo_talk_muj_01[34] = 7;
	odoodo_talk_muj_01[35] = 8;
	odoodo_talk_muj_01[42] = EVT_HELPER_CMD(0, 42);
	odoodo_talk_muj_01[43] = 0;
	odoodo_talk_muj_01[44] = 0;

	patch::writePatch(&suifu_a_init_muj_01[0], suifu_a_init_muj_01_hook, sizeof(suifu_a_init_muj_01_hook));

	patch::writePatch(&suifu_a_talk_muj_01[0], suifu_a_talk_muj_01_hook, sizeof(suifu_a_talk_muj_01_hook));

	patch::writePatch(&suifu_bomb_init[0], suifu_bomb_init_hook, sizeof(suifu_bomb_init_hook));

	patch::writePatch(&suifu_bomb_talk[0], suifu_bomb_talk_hook, sizeof(suifu_bomb_talk_hook));

	suifu_d_init_muj_01[1] = GSW(1717);
	suifu_d_init_muj_01[3] = 26;

	suifu_d_regl_muj_01[1] = GSW(1717);
	suifu_d_regl_muj_01[3] = 26;

	patch::writePatch(&suifu_d_talk_muj_01[0], suifu_d_talk_muj_01_hook, sizeof(suifu_d_talk_muj_01_hook));
	
	borosuifu_a_init[1] = GSW(1717);
	borosuifu_a_init[3] = 26;

	patch::writePatch(&borosuifu_a_talk[0], borosuifu_a_talk_hook, sizeof(borosuifu_a_talk_hook));

	borosuifu_b_init[6] = GSW(1717);
	borosuifu_b_init[8] = 26;

	borosuifu_b_regl[1] = GSW(1717);
	borosuifu_b_regl[3] = 26;

	patch::writePatch(&borosuifu_b_talk[0], borosuifu_b_talk_hook, sizeof(borosuifu_b_talk_hook));

	mony_init_muj_01[1] = GSW(1717);
	mony_init_muj_01[3] = 26;
	mony_init_muj_01[4] = 29;

	peton_init_muj_01[1] = GSW(1717);
	peton_init_muj_01[3] = 26;
	peton_init_muj_01[4] = 29;

	kyoshu_event[182] = GSW(1705);
	kyoshu_event[183] = 11;

	settoku_event_main[68] = GSW(1719);
	settoku_event_main[69] = 2;

	muj_01_init_evt[4] = GSW(1708);
	muj_01_init_evt[5] = 16;
	muj_01_init_evt[76] = GSW(1708);
	muj_01_init_evt[77] = 2;
	patch::writePatch(&muj_01_init_evt[148], muj_01_init_evt_hook, sizeof(muj_01_init_evt_hook));
	muj_01_init_evt[164] = GSW(1708);
	muj_01_init_evt[165] = 16;
	muj_01_init_evt[187] = GSW(1708);
	muj_01_init_evt[188] = 18;

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
	muj_02_init_evt[32] = 26;
	muj_02_init_evt[87] = GSW(1717);
	muj_02_init_evt[88] = 18;

	muj_03_event_01[1] = GSW(1719);
	muj_03_event_01[2] = 1;

	muj_03_init_evt[32] = 12;

	sanders_funto[462] = GSW(1719);
	sanders_funto[463] = 1;

	yashi_yure[111] = GSWF(6090);
	yashi_yure[238] = GSWF(6091);

	muj_04_init_evt[70] = GSW(1719);
	muj_04_init_evt[72] = 0;

	sanders_nakama[163] = GSW(1719);
	sanders_nakama[164] = 6;

	sanders_init_muj_05[1] = GSW(1719);
	sanders_init_muj_05[3] = 2;
	sanders_init_muj_05[9] = 2;
	sanders_init_muj_05[10] = 4;
	sanders_init_muj_05[22] = 5;

	sanders_talk_sub[63] = GSW(1719);
	sanders_talk_sub[64] = 5;

	sanders_talk_muj_05[1] = GSW(1719);
	sanders_talk_muj_05[3] = 2;
	sanders_talk_muj_05[4] = 4;
	sanders_talk_muj_05[38] = 5;

	marco_init_muj_05[1] = GSW(1719);
	marco_init_muj_05[3] = 8;

	marco_talk_muj_05[1] = GSW(1719);
	marco_talk_muj_05[3] = 8;
	marco_talk_muj_05[95] = GSWF(6108);

	sanders_kyushutsu_sub[7] = GSW(1719);
	sanders_kyushutsu_sub[8] = 2;

	marco_and_jewel[77] = GSW(1719);
	marco_and_jewel[78] = 8;

	dokuroiwa_touch[23] = GSW(1719);
	dokuroiwa_touch[24] = 7;

	dokuroiwa_bomb[302] = GSW(1717);
	dokuroiwa_bomb[303] = 1;

	dokuroiwa_init[14] = GSW(1717);
	dokuroiwa_init[15] = 1;

	yure_yashi_ha1[84] = GSW(1719);
	yure_yashi_ha1[85] = 2;
	yure_yashi_ha1[173] = GSW(1719);
	yure_yashi_ha1[174] = 2;

	yure_yashi[46] = GSW(1719);
	yure_yashi[47] = 2;
	yure_yashi[78] = GSW(1719);
	yure_yashi[79] = 2;

	muj_05_init_evt[1] = GSW(1719);
	muj_05_init_evt[3] = 1;
	muj_05_init_evt[13] = 5;
	muj_05_init_evt[65] = GSW(1719);
	muj_05_init_evt[67] = 1;
	muj_05_init_evt[79] = 7;
	muj_05_init_evt[86] = GSW(1709);
	muj_05_init_evt[87] = 5;

	megane_init_muj_10[1] = GSW(1717);
	megane_init_muj_10[3] = 8;
	megane_init_muj_10[4] = 9;

	bero_in_event_muj_10[1] = GSW(1717);
	bero_in_event_muj_10[2] = 15;

	nazo_event[168] = GSW(1717);
	nazo_event[169] = 8;

	oidasare_event[147] = GSW(1717);
	oidasare_event[148] = 11;

	unk_evt_muj_0002cb0c[79] = GSW(1717);
	unk_evt_muj_0002cb0c[80] = 16;

	muj_10_init_evt[1] = GSW(1717);
	muj_10_init_evt[2] = 10;
	muj_10_init_evt[45] = GSW(1717);
	muj_10_init_evt[46] = 7;
	muj_10_init_evt[51] = GSW(1717);
	muj_10_init_evt[52] = 10;
	muj_10_init_evt[57] = GSW(1717);
	muj_10_init_evt[58] = 15;
	muj_10_init_evt[65] = GSW(1717);
	muj_10_init_evt[66] = 17;
	muj_10_init_evt[85] = GSW(1717);
	muj_10_init_evt[86] = 17;
	muj_10_init_evt[88] = GSW(1717);
	muj_10_init_evt[89] = 15;
	muj_10_init_evt[118] = GSW(1717);
	muj_10_init_evt[119] = 17;
	muj_10_init_evt[164] = GSW(1717);
	muj_10_init_evt[165] = 14;
	muj_10_init_evt[167] = GSW(1717);
	muj_10_init_evt[168] = 15;
	muj_10_init_evt[184] = GSW(1717);
	muj_10_init_evt[185] = 15;

	warning_event[31] = GSW(1717);
	warning_event[32] = 9;

	muj_11_init_evt[1] = GSW(1717);
	muj_11_init_evt[2] = 10;
	muj_11_init_evt[28] = GSW(1717);
	muj_11_init_evt[29] = 8;

	bero_custom_muj_12[1] = GSW(1717);
	bero_custom_muj_12[3] = 9;
	bero_custom_muj_12[57] = 14;

	korutesu_event[606] = GSW(1717);
	korutesu_event[607] = 10;

	korutesu_event_after[132] = GSW(1717);
	korutesu_event_after[133] = 10;

	kosho_event[504] = GSW(1717);
	kosho_event[505] = 15;

	muj_12_init_evt[8] = GSW(1717);
	muj_12_init_evt[9] = 10;
	muj_12_init_evt[28] = GSW(1717);
	muj_12_init_evt[29] = 9;
	muj_12_init_evt[44] = GSW(1717);
	muj_12_init_evt[45] = 10;
	muj_12_init_evt[50] = GSW(1717);
	muj_12_init_evt[51] = 14;

	onryo_event[20] = GSW(1705);
	onryo_event[21] = 8;
	onryo_event[144] = GSW(1705);
	onryo_event[145] = 8;
	onryo_event[2235] = GSW(1705);
	onryo_event[2236] = 9;

	muj_20_koopa_evt[122] = GSW(1715);
	muj_20_koopa_evt[123] = 11;

	muj_20_init_evt[109] = GSW(1715);
	muj_20_init_evt[110] = 10;
	muj_20_init_evt[210] = GSW(1705);
	muj_20_init_evt[211] = 7;
	muj_20_init_evt[224] = GSW(1705);
	muj_20_init_evt[225] = 8;
	muj_20_init_evt[334] = GSW(1708);
	muj_20_init_evt[335] = 18;
}