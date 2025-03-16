#include <AP/pik.h>
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

extern int32_t hom_10_evt_resha_start_pik_00[];
extern int32_t evt_train[];
extern int32_t talk_ekiin[];
extern int32_t talk_papa_taihou_kyoka[];
extern int32_t talk_papa[];
extern int32_t talk_mama[];
extern int32_t talk_konari[];
extern int32_t talk_madam[];
extern int32_t init_sarary[];
extern int32_t evt_pik_00_arrival[];
extern int32_t pik_00_init_evt[];
extern int32_t pik_evt_powan[];
extern int32_t pik_init_powan_01[];
extern int32_t miyageya_talk[];
extern int32_t miyageya_init[];
extern int32_t konarikin_init[];
extern int32_t narikin_papa_init[];
extern int32_t narikin_mama_init[];
extern int32_t madam_init[];
extern int32_t takun4_talk[];
extern int32_t takun5_init[];
extern int32_t takun5_talk[];
extern int32_t takun6_init[];
extern int32_t takun6_talk[];
extern int32_t pik_01_koopa_evt[];
extern int32_t pik_01_init_evt[];
extern int32_t evt_return_sub[];
extern int32_t evt_return[];
extern int32_t pik_init_powan_02[];
extern int32_t pik_talk_powan[];
extern int32_t init_majo[];
extern int32_t init_mari[];
extern int32_t init_ramp[];
extern int32_t evt_garnet[];
extern int32_t evt_sw[];
extern int32_t evt_daiza_02[];
extern int32_t pik_02_koopa_evt[];
extern int32_t nise_starstone_init[];
extern int32_t pik_02_init_evt[];
extern int32_t evt_daiza_03[];
extern int32_t pik_03_init_evt[];
extern int32_t init_musume1[];
extern int32_t init_musume2[];
extern int32_t init_musume3[];
extern int32_t talk_takun1[];
extern int32_t talk_takun2[];
extern int32_t talk_takun3[];
extern int32_t pik_init_kiza[];
extern int32_t pik_04_init_evt[];

EVT_BEGIN(miyageya_talk_evt)
	IF_LARGE_EQUAL(GSW(1707), 16)
		IF_SMALL_EQUAL(GSW(1707), 18)
			IF_EQUAL(LSWF(1), 0)
				USER_FUNC(evt_npc::evt_npc_set_force_regl_anim, PTR("me"), PTR("KNF_S_2"))
				USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), PTR("KNF_S_1"))
				USER_FUNC(evt_npc::evt_npc_set_autotalkpose, PTR("me"), PTR("KNF_S_2"), PTR("KNF_T_2"))
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("kpa_stg7_004"), 0, PTR("me"))
				USER_FUNC(evt_npc::evt_npc_flag_onoff, 0, PTR("me"), 512)
				SET(LSWF(1), 1)
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("kpa_stg7_004_1"), 0, PTR("me"))
			END_IF()
			SET(LW(0), 1)
			RETURN()
		END_IF()
	END_IF()
	IF_SMALL(GSW(1706), 53)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_pik_clear_02"), 0, PTR("me"))
		SET(LW(0), 1)
		RETURN()
	END_IF()
	SET(LW(0), 0)
	RETURN()
EVT_END()

EVT_BEGIN(miyageya_talk_hook)
	RUN_CHILD_EVT(miyageya_talk_evt)
	IF_SMALL(LW(0), 1)
		GOTO(&miyageya_talk[504])
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(takun4_talk_evt)
	IF_LARGE_EQUAL(GSW(1707), 16)
		IF_SMALL_EQUAL(GSW(1707), 18)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("kpa_stg7_011_7_1"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	IF_SMALL(GSW(1706), 51)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_pik_clear_04"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_pik_clear_05"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_pik_clear_05_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(takun4_talk_hook)
	RUN_CHILD_EVT(takun4_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(takun5_talk_evt)
	IF_SMALL(GSW(1706), 51)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_pik_clear_06"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_pik_clear_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_pik_clear_07_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(takun5_talk_hook)
	RUN_CHILD_EVT(takun5_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(takun6_talk_evt)
	IF_SMALL(GSW(1706), 51)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_pik_clear_08"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_pik_clear_09"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_pik_clear_09_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(takun6_talk_hook)
	RUN_CHILD_EVT(takun6_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(pik_evt_powan_evt)
	SET(LW(0), 0)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 0)
	USER_FUNC(evt_snd::evt_snd_bgmoff, 18432)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg6_pik_06"), 0, 0)
	WAIT_MSEC(300)
	IF_SMALL(GSW(1706), 38)
		USER_FUNC(evt_snd::evt_snd_bgmon, 288, 0)
		USER_FUNC(evt_snd::evt_snd_bgmon, 160, 0)
		USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
		SET(LW(0), 1)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(pik_evt_powan_hook)
	RUN_CHILD_EVT(pik_evt_powan_evt)
	IF_EQUAL(LW(0), 1)
		RETURN()
	END_IF()
	GOTO(&pik_evt_powan[14])
EVT_END()

EVT_BEGIN(evt_daiza_evt)
	IF_LARGE_EQUAL(GSW(1706), 51)
		IF_SMALL_EQUAL(GSW(1707), 17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_pik_clear_09_2"), 0, 0)
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg6_pik_25"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(evt_daiza_hook)
	RUN_CHILD_EVT(evt_daiza_evt)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
	RETURN()
EVT_END()


void ApplyPikPatches(OSModuleInfo* module_info)
{
	hom_10_evt_resha_start_pik_00[49] = GSW(1706);
	hom_10_evt_resha_start_pik_00[50] = 43;

	evt_train[266] = GSW(1706);
	evt_train[267] = 49;

	talk_ekiin[1] = GSW(1706);
	talk_ekiin[2] = 43;

	talk_papa_taihou_kyoka[394] = GSW(1707);
	talk_papa_taihou_kyoka[395] = 5;

	talk_papa[99] = GSW(1707);
	talk_papa[101] = 1;
	talk_papa[109] = 2;
	talk_papa[116] = EVT_HELPER_CMD(1, 41);
	talk_papa[117] = 3;

	talk_mama[261] = GSW(1707);
	talk_mama[263] = 1;
	talk_mama[271] = 2;
	talk_mama[278] = EVT_HELPER_CMD(1, 41);
	talk_mama[279] = 3;

	talk_konari[1] = GSW(1707);
	talk_konari[3] = 1;
	talk_konari[11] = 2;
	talk_konari[18] = EVT_HELPER_CMD(1, 41);
	talk_konari[19] = 3;

	talk_madam[144] = GSW(1706);
	talk_madam[145] = 51;
	talk_madam[165] = GSW(1708);
	talk_madam[166] = 18;

	init_sarary[1] = GSW(1706);
	init_sarary[2] = 49;

	evt_pik_00_arrival[62] = GSW(1706);
	evt_pik_00_arrival[64] = 38;
	evt_pik_00_arrival[73] = 43;

	pik_00_init_evt[7] = GSW(1706);
	pik_00_init_evt[8] = 43;

	patch::writePatch(&pik_evt_powan[0], pik_evt_powan_hook, sizeof(pik_evt_powan_hook));

	pik_evt_powan[314] = GSW(1706);
	pik_evt_powan[315] = 39;

	pik_init_powan_01[1] = GSW(1706);
	pik_init_powan_01[3] = 39;

	patch::writePatch(&miyageya_talk[452], miyageya_talk_hook, sizeof(miyageya_talk_hook));
	miyageya_talk[511] = GSW(1708);
	miyageya_talk[512] = 18;
	miyageya_talk[560] = 0;

	miyageya_init[7] = GSW(1707);
	miyageya_init[9] = 16;
	miyageya_init[10] = 19;

	konarikin_init[14] = GSW(1707);
	konarikin_init[15] = 16;
	konarikin_init[17] = GSW(1707);
	konarikin_init[18] = 19;

	narikin_papa_init[8] = GSW(1707);
	narikin_papa_init[9] = 16;
	narikin_papa_init[11] = GSW(1707);
	narikin_papa_init[12] = 19;

	narikin_mama_init[8] = GSW(1707);
	narikin_mama_init[9] = 16;
	narikin_mama_init[11] = GSW(1707);
	narikin_mama_init[12] = 19;

	madam_init[3] = GSW(1707);
	madam_init[4] = 16;
	madam_init[6] = GSW(1707);
	madam_init[7] = 19;

	patch::writePatch(&takun4_talk[0], takun4_talk_hook, sizeof(takun4_talk_hook));

	takun5_init[6] = GSW(1707);
	takun5_init[8] = 16;
	takun5_init[9] = 19;

	patch::writePatch(&takun5_talk[0], takun5_talk_hook, sizeof(takun5_talk_hook));

	takun6_init[6] = GSW(1707);
	takun6_init[8] = 16;
	takun6_init[9] = 19;

	patch::writePatch(&takun6_talk[0], takun6_talk_hook, sizeof(takun6_talk_hook));

	pik_01_koopa_evt[190] = GSW(1707);
	pik_01_koopa_evt[191] = 17;

	pik_01_init_evt[4] = GSW(1707);
	pik_01_init_evt[5] = 16;
	pik_01_init_evt[7] = GSW(1707);
	pik_01_init_evt[8] = 18;
	pik_01_init_evt[57] = GSW(1708);
	pik_01_init_evt[58] = 16;
	pik_01_init_evt[314] = GSW(1708);
	pik_01_init_evt[315] = 19;
	pik_01_init_evt[343] = GSW(1706);
	pik_01_init_evt[344] = 39;
	pik_01_init_evt[429] = GSW(1707);
	pik_01_init_evt[430] = 16;
	pik_01_init_evt[438] = GSW(1707);
	pik_01_init_evt[439] = 17;
	pik_01_init_evt[462] = GSW(1708);
	pik_01_init_evt[463] = 16;

	evt_return_sub[46] = GSW(1706);
	evt_return_sub[47] = 43;

	evt_return[1] = GSW(1706);
	evt_return[2] = 42;

	pik_init_powan_02[1] = GSW(1706);
	pik_init_powan_02[3] = 40;
	pik_init_powan_02[4] = 41;
	pik_init_powan_02[12] = 42;
	pik_init_powan_02[20] = 42;

	pik_talk_powan[1] = GSW(1706);
	pik_talk_powan[3] = 42;
	pik_talk_powan[11] = 43;

	init_majo[1] = GSW(1706);
	init_majo[3] = 40;

	init_mari[1] = GSW(1706);
	init_mari[3] = 40;

	init_ramp[1] = GSW(1706);
	init_ramp[3] = 40;

	evt_garnet[553] = GSW(1706);
	evt_garnet[554] = 40;

	evt_sw[62] = GSW(1706);
	evt_sw[63] = 41;

	patch::writePatch(&evt_daiza_02[3], evt_daiza_hook, sizeof(evt_daiza_hook));

	pik_02_koopa_evt[1207] = GSW(1707);
	pik_02_koopa_evt[1208] = 18;

	nise_starstone_init[1] = GSW(1706);
	nise_starstone_init[2] = 51;
	nise_starstone_init[4] = GSW(1707);
	nise_starstone_init[5] = 17;

	pik_02_init_evt[1] = GSW(1706);
	pik_02_init_evt[2] = 40;
	pik_02_init_evt[22] = GSW(1707);
	pik_02_init_evt[23] = 17;
	pik_02_init_evt[43] = GSW(1706);
	pik_02_init_evt[44] = 41;
	pik_02_init_evt[101] = GSW(1706);
	pik_02_init_evt[102] = 40;
	pik_02_init_evt[107] = GSW(1706);
	pik_02_init_evt[108] = 42;
	pik_02_init_evt[139] = GSW(1707);
	pik_02_init_evt[140] = 17;

	evt_daiza_03[4] = GSW(1706);
	evt_daiza_03[5] = 42;

	pik_03_init_evt[19] = GSW(1706);
	pik_03_init_evt[20] = 42;
	pik_03_init_evt[33] = GSW(1706);
	pik_03_init_evt[34] = 42;

	init_musume1[1] = GSW(1706);
	init_musume1[3] = 51;

	init_musume2[1] = GSW(1706);
	init_musume2[3] = 51;

	init_musume3[1] = GSW(1706);
	init_musume3[3] = 51;

	talk_takun1[1] = GSW(1706);
	talk_takun1[3] = 51;

	talk_takun2[1] = GSW(1706);
	talk_takun2[3] = 51;

	talk_takun3[1] = GSW(1706);
	talk_takun3[3] = 51;

	pik_init_kiza[1] = GSW(1706);
	pik_init_kiza[3] = 51;

	pik_04_init_evt[12] = GSW(1708);
	pik_04_init_evt[13] = 18;
	pik_04_init_evt[142] = GSW(1708);
	pik_04_init_evt[143] = 18;
}
