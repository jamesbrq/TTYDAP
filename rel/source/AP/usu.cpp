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
extern int32_t usu00_3witch_event[];
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
extern int32_t evt_usu_kagemario_party_kill_usu[];

const char usuVillagerA[] = "\x91\xBA\x90\x6C\x82\x60";

EVT_BEGIN(villagerA_init_evt)
	IF_LARGE_EQUAL(GSW(1717), 22)
		IF_SMALL_EQUAL(GSW(1717), 24)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR(usuVillagerA), -180, 0, 0)
			RETURN()
		END_IF()
	END_IF()
	IF_SMALL(GSW(1704), 200)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR(usuVillagerA), 0, -2000, 0)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(villagerA_init_hook)
	RUN_CHILD_EVT(villagerA_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(villagerA_talk_evt)
	IF_EQUAL(GSW(1717), 23)
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
	IF_SMALL_EQUAL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_31_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_31_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(villagerA_talk_hook)
	RUN_CHILD_EVT(villagerA_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(villagerB_talk_evt)
	IF_EQUAL(GSW(1717), 23)
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

EVT_BEGIN(villagerB_talk_hook)
	RUN_CHILD_EVT(villagerB_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(villagerC_talk_evt)
	IF_EQUAL(GSW(1717), 23)
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

EVT_BEGIN(villagerC_talk_hook)
	RUN_CHILD_EVT(villagerC_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(villagerD_talk_evt)
	IF_EQUAL(GSW(1717), 23)
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

EVT_BEGIN(villagerD_talk_hook)
	RUN_CHILD_EVT(villagerD_talk_evt)
	RETURN()
EVT_END()



EVT_BEGIN(villagerE_talk_evt)
	IF_EQUAL(GSW(1717), 23)
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

EVT_BEGIN(villagerE_talk_hook)
	RUN_CHILD_EVT(villagerE_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(villagerK_talk_evt)
	IF_EQUAL(GSW(1717), 23)
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

EVT_BEGIN(villagerK_talk_hook)
	RUN_CHILD_EVT(villagerK_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(villagerF_talk_evt)
	SWITCH(GSW(1704))
		CASE_SMALL(200)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_82"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(219)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_30"), 0, PTR("me"))
			USER_FUNC(evt_msg::evt_msg_select, 0, PTR("stg4_usu_30_yn"))
			USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg4_usu_30_00"))
			RETURN()
		CASE_END()
		CASE_SMALL(222)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_84"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 402)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_84_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_84_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(villagerF_talk_hook)
	RUN_CHILD_EVT(villagerF_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(villagerG_talk_evt)
	SWITCH(GSW(1704))
		CASE_SMALL(200)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_85"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(219)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_86"), 0, PTR("me"))
			USER_FUNC(evt_msg::evt_msg_select, 0, PTR("stg4_usu_86_yn"))
			USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg4_usu_86_00"))
			RETURN()
		CASE_END()
		CASE_SMALL(222)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_87"), 0, PTR("me"))
			RETURN()
		CASE_END()
		END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 402)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_87_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_87_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(villagerG_talk_hook)
	RUN_CHILD_EVT(villagerG_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(villagerH_talk_evt)
	SWITCH(GSW(1704))
		CASE_SMALL(200)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_88"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(219)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_89"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(222)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_90"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 402)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_90_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_90_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(villagerH_talk_hook)
	RUN_CHILD_EVT(villagerH_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(villagerI_talk_evt)
	SWITCH(GSW(1704))
		CASE_SMALL(200)
			USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
			USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_91"), 0, PTR("me"))
		RETURN()
		CASE_END()
		CASE_SMALL(219)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_92"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(222)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_93"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 402)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_93_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_93_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(villagerI_talk_hook)
	RUN_CHILD_EVT(villagerI_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(villagerJ_talk_evt)
	SWITCH(GSW(1704))
		CASE_SMALL(200)
			USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
			USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_94"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(219)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_95"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(222)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_96"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 402)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_96_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_96_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(villagerJ_talk_hook)
	RUN_CHILD_EVT(villagerJ_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(shop_man_talk_evt)
	IF_SMALL(GSW(1704), 219)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_61_5"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL_EQUAL(GSW(1708), 402)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_61_6"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_61_7"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(shop_man_talk_hook)
	RUN_CHILD_EVT(shop_man_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(shop_woman_talk_evt)
	IF_SMALL(GSWF(6042), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_58"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1704), 200)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_58_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_59"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(shop_woman_talk_hook)
	RUN_CHILD_EVT(shop_woman_talk_evt)
	RETURN()
EVT_END()

void ApplyUsuPatches(OSModuleInfo* module_info)
{
	patch::writePatch(&villagerA_init[5], villagerA_init_hook, sizeof(villagerA_init_hook));

	villagerA_pig_init[6] = GSW(1704);
	villagerA_pig_init[8] = 200;

	patch::writePatch(&villagerA_talk[0], villagerA_talk_hook, sizeof(villagerA_talk_hook));

	patch::writePatch(&villagerB_talk[0], villagerB_talk_hook, sizeof(villagerB_talk_hook));

	patch::writePatch(&villagerC_talk[0], villagerC_talk_hook, sizeof(villagerC_talk_hook));

	villagerD_init[6] = GSW(1704);
	villagerD_init[8] = 200;

	villagerD_pig_init[6] = GSW(1704);
	villagerD_pig_init[8] = 200;

	patch::writePatch(&villagerD_talk[0], villagerD_talk_hook, sizeof(villagerD_talk_hook));

	villagerE_init[6] = GSW(1704);
	villagerE_init[8] = 200;

	villagerE_pig_init[6] = GSW(1704);
	villagerE_pig_init[8] = 200;

	patch::writePatch(&villagerE_talk[0], villagerE_talk_hook, sizeof(villagerE_talk_hook));

	villagerK_init[6] = GSW(1704);
	villagerK_init[8] = 200;

	villagerK_pig_init[6] = GSW(1704);
	villagerK_pig_init[8] = 200;

	patch::writePatch(&villagerK_talk[0], villagerK_talk_hook, sizeof(villagerK_talk_hook));

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

	usu00_3witch_event[267] = GSW(1715);
	usu00_3witch_event[268] = 4;

	usu00_stage4_coming_event[810] = GSW(1704);
	usu00_stage4_coming_event[811] = 2;

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

	kusa_01_evt[1] = GSW(1717);
	kusa_01_evt[2] = 23;

	evt_ichikorori_ball_get[1] = GSWF(6040);
	evt_ichikorori_ball_get[2] = 1;

	usu_00_koopa_first_evt[181] = GSW(1717);
	usu_00_koopa_first_evt[182] = 23;

	usu_00_koopa_second_evt[1260] = GSW(1717);
	usu_00_koopa_second_evt[1261] = 24;

	evt_after_peach[125] = GSW(1704);
	evt_after_peach[126] = 219;

	usu_00_init_evt[4] = GSW(1717);
	usu_00_init_evt[5] = 22;
	usu_00_init_evt[7] = GSW(1717);
	usu_00_init_evt[8] = 23;
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
	usu_00_init_evt[504] = GSW(1717);
	usu_00_init_evt[505] = 22;
	usu_00_init_evt[523] = GSW(1717);
	usu_00_init_evt[524] = 23;
	usu_00_init_evt[586] = GSW(1704);
	usu_00_init_evt[587] = 218;
	usu_00_init_evt[621] = GSW(1708);
	usu_00_init_evt[622] = 400;
	usu_00_init_evt[644] = GSW(1708);
	usu_00_init_evt[645] = 402;

	patch::writePatch(&villagerF_talk[0], villagerF_talk_hook, sizeof(villagerF_talk_hook));

	patch::writePatch(&villagerG_talk[0], villagerG_talk_hook, sizeof(villagerG_talk_hook));

	patch::writePatch(&villagerH_talk[0], villagerH_talk_hook, sizeof(villagerH_talk_hook));

	villagerI_init[6] = GSW(1704);
	villagerI_init[8] = 200;

	villagerI_pig_init[6] = GSW(1704);
	villagerI_pig_init[8] = 200;

	patch::writePatch(&villagerI_talk[0], villagerI_talk_hook, sizeof(villagerI_talk_hook));

	villagerJ_init[6] = GSW(1704);
	villagerJ_init[8] = 200;

	villagerJ_pig_init[6] = GSW(1704);
	villagerJ_pig_init[8] = 200;

	patch::writePatch(&villagerJ_talk[0], villagerJ_talk_hook, sizeof(villagerJ_talk_hook));

	shop_man_init[6] = GSW(1704);
	shop_man_init[8] = 200;
	shop_man_init[10] = 219;

	patch::writePatch(&shop_man_talk[0], shop_man_talk_hook, sizeof(shop_man_talk_hook));

	shop_woman_talk[0] = EVT_HELPER_CMD(2, 24);
	shop_woman_talk[1] = GSWF(6041);
	shop_woman_talk[2] = 0;
	shop_woman_talk[3] = 0;
	shop_woman_talk[135] = EVT_HELPER_CMD(0, 33);

	patch::writePatch(&shop_woman_talk[136], shop_woman_talk_hook, sizeof(shop_woman_talk_hook));

	kizayarou_init[6] = GSW(1704);
	kizayarou_init[8] = 200;
	kizayarou_init[16] = 219;

	kizayarou_pig_init[1] = GSW(1704);
	kizayarou_pig_init[3] = 200;

	kizayarou_talk[1] = GSW(1704);
	kizayarou_talk[3] = 200;

	aunt_init[6] = GSW(1704);
	aunt_init[8] = 200;

	aunt_pig_init[6] = GSW(1704);
	aunt_pig_init[8] = 200;

	aunt_talk[403] = GSW(1704);
	aunt_talk[405] = 200;
	aunt_talk[654] = 219;
	aunt_talk[662] = 222;
	aunt_talk[669] = 0;
	aunt_talk[670] = 0;
	aunt_talk[671] = 0;
	aunt_talk[672] = 0;
	aunt_talk[673] = 0;
	aunt_talk[674] = 0;
	aunt_talk[675] = 0;
	aunt_talk[676] = 0;

	childA_talk[1] = GSW(1704);
	childA_talk[3] = 200;
	childA_talk[18] = 222;
	childA_talk[21] = 0;
	childA_talk[22] = 0;
	childA_talk[23] = 0;
	childA_talk[24] = 0;

	childB_talk[1] = GSW(1704);
	childB_talk[3] = 200;
	childB_talk[18] = 222;
	childB_talk[21] = 0;
	childB_talk[22] = 0;
	childB_talk[23] = 0;
	childB_talk[24] = 0;

	childC_talk[1] = GSW(1704);
	childC_talk[3] = 200;
	childC_talk[18] = 222;
	childC_talk[21] = 0;
	childC_talk[22] = 0;
	childC_talk[23] = 0;
	childC_talk[24] = 0;

	gatekeeper_init[6] = GSW(1704);
	gatekeeper_init[8] = 4;
	gatekeeper_init[21] = 200;

	gatekeeper_pig_init[6] = GSW(1704);
	gatekeeper_pig_init[8] = 4;
	gatekeeper_pig_init[16] = 200;

	gatekeeper_talk[1] = GSW(1704);
	gatekeeper_talk[3] = 3;
	gatekeeper_talk[75] = 4;
	gatekeeper_talk[83] = 5;
	gatekeeper_talk[91] = 200;
	gatekeeper_talk[112] = 219;
	gatekeeper_talk[120] = 222;
	gatekeeper_talk[127] = 0;
	gatekeeper_talk[128] = 0;
	gatekeeper_talk[129] = 0;
	gatekeeper_talk[130] = 0;
	gatekeeper_talk[131] = 0;
	gatekeeper_talk[132] = 0;
	gatekeeper_talk[133] = 0;
	gatekeeper_talk[134] = 0;

	
	musume_talk[1] = GSW(1704);
	musume_talk[3] = 200;
	musume_talk[18] = 215;

	evt_mon_open[1] = GSW(1704);
	evt_mon_open[2] = 5;
	evt_mon_open[105] = 6;

	usu01_crowFG_talk_event[193] = GSWF(6039);
	usu01_crowFG_talk_event[194] = 1;
	usu01_crowFG_talk_event[222] = GSW(1704);
	usu01_crowFG_talk_event[224] = 210;
	usu01_crowFG_talk_event[327] = 222;

	usu01_crowHI_talk_event[181] = GSWF(6039);
	usu01_crowHI_talk_event[182] = 1;
	usu01_crowHI_talk_event[210] = GSW(1704);
	usu01_crowHI_talk_event[212] = 210;
	usu01_crowHI_talk_event[226] = 222;

	usu01_crowJ_talk_event[181] = GSWF(6039);
	usu01_crowJ_talk_event[182] = 1;
	usu01_crowJ_talk_event[210] = GSW(1704);
	usu01_crowJ_talk_event[212] = 210;
	usu01_crowJ_talk_event[220] = 222;

	loop_msg[1] = GSW(1704);
	loop_msg[2] = 200;
	loop_msg[6] = GSW(1704);
	loop_msg[7] = 215;

	mawari_house_delay[1] = GSW(1704);
	mawari_house_delay[2] = 3;

	evt_ie2_door2_1[150] = GSWF(6042);
	evt_ie2_door2_1[151] = 1;

	evt_ie2_door2_2[82] = GSWF(6042);
	evt_ie2_door2_2[83] = 1;

	usu01_gusun[1] = GSW(1704);
	usu01_gusun[2] = 219;
	usu01_gusun[31] = GSW(1704);
	usu01_gusun[32] = 200;

	evt_monban_buta_main[30] = GSW(1704);
	evt_monban_buta_main[31] = 4;

	evt_monban_buta[1] = GSW(1704);
	evt_monban_buta[2] = 4;

	evt_monban_buta2[1] = GSW(1704);
	evt_monban_buta2[2] = 4;

	evt_monban_look[3] = GSW(1704);
	evt_monban_look[4] = 5;
	evt_monban_look[8] = GSW(1704);
	evt_monban_look[9] = 4;
	evt_monban_look[109] = GSW(1704);
	evt_monban_look[110] = 5;

	unlock[9] = GSWF(6042);
	unlock[10] = 1;

	evt_majin2[210] = GSWF(6043);
	evt_majin2[211] = 1;

	usu_01_init_evt[23] = GSW(1704);
	usu_01_init_evt[25] = 1;
	usu_01_init_evt[26] = 200;
	usu_01_init_evt[34] = 200;
	usu_01_init_evt[35] = 215;
	usu_01_init_evt[44] = GSWF(6042);
	usu_01_init_evt[45] = 1;
	usu_01_init_evt[93] = GSW(1704);
	usu_01_init_evt[94] = 3;
	usu_01_init_evt[106] = GSWF(6042);
	usu_01_init_evt[107] = 1;
	usu_01_init_evt[109] = GSW(1704);
	usu_01_init_evt[110] = 200;
	usu_01_init_evt[117] = GSW(1704);
	usu_01_init_evt[118] = 213;
	usu_01_init_evt[156] = GSW(1704);
	usu_01_init_evt[157] = 6;
	usu_01_init_evt[309] = GSWF(6043);
	usu_01_init_evt[310] = 1;

	evt_usu_kagemario_party_kill_usu[1] = GSW(1704);
}