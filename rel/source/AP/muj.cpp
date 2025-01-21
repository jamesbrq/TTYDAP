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
extern int32_t dai3_shurai_sub[];
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
extern int32_t sanders_init_muj_03[];
extern int32_t sanders_talk_sub[];
extern int32_t sanders_talk_muj_03[];
extern int32_t marco_init_muj_03[];
extern int32_t marco_talk_muj_03[];
extern int32_t sanders_kyushutsu_sub[];
extern int32_t marco_and_jewel[];
extern int32_t dokuroiwa_touch[];
extern int32_t dokuroiwa_bomb[];
extern int32_t dokuroiwa_init[];
extern int32_t yure_yashi_ha1[];
extern int32_t yure_yashi[];
extern int32_t muj_05_init_evt[];
extern int32_t megane_init_muj_10[];
extern int32_t bero_in_event[];
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




}