#include <AP/usu.h>
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

extern int32_t villagerA_init[];
extern int32_t villagerA_pig_init[];
extern int32_t villagerA_talk[];
extern int32_t villagerB_talk[];
extern int32_t villagerC_talk[];
extern int32_t villagerD_init[];
extern int32_t villagerD_pig_init[];
extern int32_t villagerD_talk[];
extern int32_t villagerE_init[];
extern int32_t villagerE_pig_init[];
extern int32_t villagerE_talk[];
extern int32_t villagerK_init[];
extern int32_t villagerK_pig_init[];
extern int32_t villagerK_talk[];
extern int32_t village_chief_init[];
extern int32_t village_chief_pig_init[];
extern int32_t village_chief_talk[];
extern int32_t vivian_init[];
extern int32_t usu00_stage4_coming_event[];
extern int32_t gra00_2witch_find_ranpel_event[];
extern int32_t usu00_vivian_joinup_event[];
extern int32_t usu00_crowAB_talk_event[];
extern int32_t usu00_crowCDE_talk_event[];
extern int32_t chief_pig_event[];
extern int32_t evt_door2_chk[];
extern int32_t dokan_1_evt[];
extern int32_t kusa_01_evt[];
extern int32_t evt_ichikorori_ball_get[];
extern int32_t usu_00_koopa_first_evt[];
extern int32_t usu_00_koopa_second_evt[];
extern int32_t evt_after_peach[];
extern int32_t usu_00_init_evt[];
extern int32_t villagerF_talk[];
extern int32_t villagerG_talk[];
extern int32_t villagerH_talk[];
extern int32_t villagerI_init[];
extern int32_t villagerI_pig_init[];
extern int32_t villagerI_talk[];
extern int32_t villagerJ_init[];
extern int32_t villagerJ_pig_init[];
extern int32_t villagerJ_talk[];
extern int32_t shop_man_init[];
extern int32_t shop_man_talk[];
extern int32_t shop_woman_talk[];
extern int32_t kizayarou_init[];
extern int32_t kizayarou_pig_init[];
extern int32_t kizayarou_talk[];
extern int32_t aunt_init[];
extern int32_t aunt_pig_init[];
extern int32_t aunt_talk[];
extern int32_t childA_talk[];
extern int32_t childB_talk[];
extern int32_t childC_talk[];
extern int32_t gatekeeper_init[];
extern int32_t gatekeeper_pig_init[];
extern int32_t gatekeeper_talk[];
extern int32_t musume_talk[];
extern int32_t evt_mon_open[];
extern int32_t usu01_crowFG_talk_event[];
extern int32_t usu01_crowHI_talk_event[];
extern int32_t usu01_crowJ_talk_event[];
extern int32_t loop_msg[];
extern int32_t mawari_house_delay[];
extern int32_t evt_ie2_door2_1[];
extern int32_t evt_ie2_door2_2[];
extern int32_t usu01_gusun[];
extern int32_t evt_monban_buta_main[];
extern int32_t evt_monban_buta[];
extern int32_t evt_monban_buta2[];
extern int32_t evt_monban_look[];
extern int32_t unlock[];
extern int32_t evt_majin2[];
extern int32_t usu_01_init_evt[];
extern int32_t evt_usu_kagemario_party_kill[];

const char usuVillagerA[] = "\x91\xBA\x90\x6C\x82\x60";

EVT_BEGIN(villagerA_init_evt)
	IF_LARGE_EQUAL(GSW(1705), 265)
		IF_SMALL_EQUAL(GSW(1705), 267)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR(usuVillagerA), -180, 0, 0)
			RETURN()
		END_IF()
	END_IF()
	IF_SMALL(GSW(1704), 200)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR(usuVillagerA), 0, -2000, 0)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(villagerA_talk_evt)
	IF_EQUAL(GSW(1705), 266)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("kpa_stg5_005"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1704))
		CASE_SMALL(200)
			USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
			USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_29"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(219)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_30"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(222)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_31"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 402)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_31_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_31_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(villagerB_talk_evt)
	IF_EQUAL(GSW(1705), 266)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("kpa_stg5_006"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1704))
		CASE_SMALL(200)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_32"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(219)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_33"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(222)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_34"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 402)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_34_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_34_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(villagerC_talk_evt)
	IF_EQUAL(GSW(1705), 266)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("kpa_stg5_007"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1704))
		CASE_SMALL(200)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_35"), 0, PTR("me"))
		RETURN()
		CASE_END()
		CASE_SMALL(219)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_36"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(222)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_37"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 402)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_37_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_37_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(villagerD_talk_evt)
	IF_EQUAL(GSW(1705), 266)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("kpa_stg5_008"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1707), 354)
		IF_EQUAL(GSWF(3889), 1)
			SET(GSWF(3880), 1)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_usu_01"), 0, PTR("me"))
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_40"), 0, PTR("me"))
		END_IF()
		RETURN()
	END_IF()
	SWITCH(GSW(1704))
		CASE_SMALL(200)
			USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
			USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_38"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(219)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_39"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(222)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_40"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 402)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_40_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_40_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(villagerE_talk_evt)
	IF_EQUAL(GSW(1705), 266)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("kpa_stg5_009"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1704))
		CASE_SMALL(200)
			USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
			USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_41"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(219)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_42"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(222)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_43"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 402)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_43_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_43_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(villagerK_talk_evt)
	IF_EQUAL(GSW(1705), 266)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("kpa_stg5_006"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1704))
		CASE_SMALL(200)
			USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
			USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_43_01"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(219)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_43_02"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(222)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_43_03"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 402)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_43_03_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_43_03_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(villagerC_talk_evt)
IF_EQUAL(GSW(1705), 266)
USER_FUNC(evt_msg::evt_msg_print, 0, PTR("kpa_stg5_007"), 0, PTR("me"))
RETURN()
END_IF()
SWITCH(GSW(1704))
CASE_SMALL(200)
USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_35"), 0, PTR("me"))
RETURN()
CASE_END()
CASE_SMALL(219)
USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_36"), 0, PTR("me"))
RETURN()
CASE_END()
CASE_SMALL(222)
USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_37"), 0, PTR("me"))
RETURN()
CASE_END()
END_SWITCH()
IF_SMALL_EQUAL(GSW(1708), 402)
USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_37_1"), 0, PTR("me"))
RETURN()
END_IF()
USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_37_1_1"), 0, PTR("me"))
RETURN()
EVT_END()

void ApplyUsuPatches(OSModuleInfo* module_info)
{
	patch::writePatch(&villagerA_init[5], villagerA_init_evt, sizeof(villagerA_init_evt));

	villagerA_pig_init[6] = GSW(1704);
	villagerA_pig_init[8] = 200;

	patch::writePatch(&villagerA_talk[0], villagerA_talk_evt, sizeof(villagerA_talk_evt));

	patch::writePatch(&villagerB_talk[0], villagerB_talk_evt, sizeof(villagerB_talk_evt));

	patch::writePatch(&villagerC_talk[0], villagerC_talk_evt, sizeof(villagerC_talk_evt));

	villagerD_init[6] = GSW(1704);
	villagerD_init[8] = 200;

	villagerD_pig_init[6] = GSW(1704);
	villagerD_pig_init[8] = 200;

	patch::writePatch(&villagerD_talk[0], villagerD_talk_evt, sizeof(villagerD_talk_evt));

	villagerE_init[6] = GSW(1704);
	villagerE_init[8] = 200;

	villagerE_pig_init[6] = GSW(1704);
	villagerE_pig_init[8] = 200;

	patch::writePatch(&villagerE_talk[0], villagerE_talk_evt, sizeof(villagerE_talk_evt));

	villagerK_init[6] = GSW(1704);
	villagerK_init[8] = 200;

	villagerK_pig_init[6] = GSW(1704);
	villagerK_pig_init[8] = 200;

	patch::writePatch(&villagerK_talk[0], villagerK_talk_evt, sizeof(villagerK_talk_evt));

	village_chief_init[6] = GSW(1704);
	village_chief_init[8] = 179;
	village_chief_init[10] = 200;

	village_chief_pig_init[6] = GSW(1704);
	village_chief_pig_init[8] = 179;
	village_chief_pig_init[16] = 200;

	village_chief_talk[105] = GSW(1704);
	village_chief_talk[107] = 180;
	village_chief_talk[136] = 210;
	village_chief_talk[166] = 219;
	village_chief_talk[174] = 222;
	village_chief_talk[181] = EVT_HELPER_CMD(0, 42);
	village_chief_talk[182] = 0;
	village_chief_talk[189] = EVT_HELPER_CMD(0, 49);
	village_chief_talk[190] = EVT_HELPER_CMD(0, 2);
	village_chief_talk[191] = 1;

	vivian_init[6] = GSW(1714);
	vivian_init[8] = 0;
	vivian_init[9] = 0;
	vivian_init[30] = 0;

	usu00_stage4_coming_event[810] = GSW(1704);
	usu00_stage4_coming_event[811] = 1;

	gra00_2witch_find_ranpel_event[172] = GSW(1704);
	gra00_2witch_find_ranpel_event[173] = 214;

	usu00_vivian_joinup_event[595] = GSWF(6039);
	usu00_vivian_joinup_event[596] = 1;

	usu00_crowAB_talk_event[218] = GSW(1704);
	usu00_crowAB_talk_event[219] = 0; //Unused
	usu00_crowAB_talk_event[247] = GSW(1704);
	usu00_crowAB_talk_event[249] = 99; //Unused

	usu00_crowCDE_talk_event[214] = GSW(1704);
	usu00_crowCDE_talk_event[215] = 0; //Unused
	usu00_crowCDE_talk_event[243] = GSW(1704);
	usu00_crowCDE_talk_event[245] = 99; //Unused

	chief_pig_event[56] = GSW(1704);
	chief_pig_event[57] = 3;

	evt_door2_chk[1] = GSW(1704);
	evt_door2_chk[3] = 0;
	evt_door2_chk[4] = 0;

	dokan_1_evt[1] = GSW(1704);
	dokan_1_evt[2] = 1;

	kusa_01_evt[1] = GSW(1705);
	kusa_01_evt[2] = 266;

	evt_ichikorori_ball_get[1] = GSWF(6040);
	evt_ichikorori_ball_get[2] = 1;

	usu_00_koopa_first_evt[181] = GSW(1705);
	usu_00_koopa_first_evt[182] = 266;

	usu_00_koopa_second_evt[1260] = GSW(1705);
	usu_00_koopa_second_evt[1261] = 267;

	evt_after_peach[125] = GSW(1704);
	evt_after_peach[126] = 219;

	usu_00_init_evt[4] = GSW(1705);
	usu_00_init_evt[5] = 265;
	usu_00_init_evt[7] = GSW(1705);
	usu_00_init_evt[8] = 266;
	usu_00_init_evt[21] = GSW(1708);
	usu_00_init_evt[23] = 99; //Unused
	usu_00_init_evt[24] = 99; //Unused
	usu_00_init_evt[33] = 99; //Unused
	usu_00_init_evt[54] = 400;
	usu_00_init_evt[102] = GSW(1708);
	usu_00_init_evt[103] = 402;
	usu_00_init_evt[403] = GSW(1704);
	usu_00_init_evt[405] = 1;
	usu_00_init_evt[409] = 2;
	usu_00_init_evt[410] = 2;
	usu_00_init_evt[415] = GSW(1715);
	usu_00_init_evt[417] = 203;
	usu_00_init_evt[418] = 209;
	usu_00_init_evt[436] = GSW(1704);
	usu_00_init_evt[447] = 99; //Unused
	usu_00_init_evt[442] = GSWF(6040);
	usu_00_init_evt[443] = 1;
	usu_00_init_evt[454] = GSW(1704);
	usu_00_init_evt[455] = 213;
	usu_00_init_evt[474] = GSW(1704);
	usu_00_init_evt[475] = 99; //Unused
	usu_00_init_evt[477] = GSW(1704);
	usu_00_init_evt[478] = 99; //Unused
	usu_00_init_evt[504] = GSW(1705);
	usu_00_init_evt[505] = 265;
	usu_00_init_evt[523] = GSW(1705);
	usu_00_init_evt[524] = 266;
	usu_00_init_evt[586] = GSW(1704);
	usu_00_init_evt[587] = 218;
	usu_00_init_evt[621] = GSW(1708);
	usu_00_init_evt[622] = 400;
	usu_00_init_evt[644] = GSW(1708);
	usu_00_init_evt[645] = 402;

	villagerF_talk[0] = GSW(1704);




}