#include "evt_cmd.h"
#include "patch.h"
#include <AP/usu.h>
#include <ttyd/evt_bero.h>
#include <ttyd/evt_cam.h>
#include <ttyd/evt_item.h>
#include <ttyd/evt_mario.h>
#include <ttyd/evt_msg.h>
#include <ttyd/evt_npc.h>
#include <ttyd/evt_snd.h>

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
extern int32_t usu00_vivian_cry_event[];
extern int32_t gra00_2witch_find_ranpel_event[];
extern int32_t usu00_vivian_joinup_event[];
extern int32_t usu00_crowAB_talk_event[];
extern int32_t usu00_crowCDE_talk_event[];
extern int32_t chief_pig_event[];
extern int32_t evt_door2_chk[];
extern int32_t dokan_1_evt[];
extern int32_t kusa_01_evt[];
extern int32_t evt_ichikorori_ball_get[];
extern int32_t usu_00_touch_ichikorori_ball[];
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
extern int32_t usu_all_party_lecture[];

//Assembly
extern int32_t usu_evt_kagemario_init[];

const char usuVillagerA[] = "\x91\xBA\x90\x6C\x82\x60";
const char gatekeeper_pig[] = "\x83\x75\x83\x5E\x96\xE5\x94\xD4";
const char gatekeeper[] = "\x96\xE5\x94\xD4";
const char doopliss[] = "\x82\xC9\x82\xB9\x83\x7D\x83\x8A\x83\x49";
const char mayor_pig[] = "\x83\x75\x83\x5E\x91\xBA\x92\xB7";
const char mayor[] = "\x91\xBA\x92\xB7";
const char vivian[] = "\x83\x72\x83\x72\x83\x41\x83\x93";

EVT_BEGIN(villagerA_init_evt)
	IF_LARGE_EQUAL(GSW(1717), 22)
		IF_SMALL_EQUAL(GSW(1717), 24)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR(usuVillagerA), -180, 0, 0)
			RETURN()
		END_IF()
	END_IF()
	IF_SMALL(GSW(1715), 2)
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
	SWITCH(GSW(1715))
		CASE_SMALL(2)
			USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
			USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_29"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(14)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_30"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(17)
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
	SWITCH(GSW(1715))
		CASE_SMALL(2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_32"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(14)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_33"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_34"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 18)
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
	SWITCH(GSW(1715))
		CASE_SMALL(2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_35"), 0, PTR("me"))
		RETURN()
		CASE_END()
		CASE_SMALL(14)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_36"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_37"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 18)
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
	IF_EQUAL(GSW(1707), 3)
		IF_EQUAL(GSWF(3889), 1)
			SET(GSWF(3880), 1)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_usu_01"), 0, PTR("me"))
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_40"), 0, PTR("me"))
		END_IF()
		RETURN()
	END_IF()
	SWITCH(GSW(1715))
		CASE_SMALL(2)
			USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
			USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_38"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(14)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_39"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(177)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_40"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 18)
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
	SWITCH(GSW(1715))
		CASE_SMALL(2)
			USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
			USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_41"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(14)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_42"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_43"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 18)
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
	SWITCH(GSW(1715))
		CASE_SMALL(2)
			USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
			USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_43_01"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(14)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_43_02"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_43_03"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 18)
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
	SWITCH(GSW(1715))
		CASE_SMALL(2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_82"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(14)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_30"), 0, PTR("me"))
			USER_FUNC(evt_msg::evt_msg_select, 0, PTR("stg4_usu_30_yn"))
			USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg4_usu_30_00"))
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_84"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 18)
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
	SWITCH(GSW(1715))
		CASE_SMALL(2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_85"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(14)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_86"), 0, PTR("me"))
			USER_FUNC(evt_msg::evt_msg_select, 0, PTR("stg4_usu_86_yn"))
			USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg4_usu_86_00"))
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_87"), 0, PTR("me"))
			RETURN()
		CASE_END()
		END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 18)
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
	SWITCH(GSW(1715))
		CASE_SMALL(2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_88"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(14)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_89"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_90"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 18)
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
	SWITCH(GSW(1715))
		CASE_SMALL(2)
			USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
			USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_91"), 0, PTR("me"))
		RETURN()
		CASE_END()
		CASE_SMALL(14)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_92"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_93"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 18)
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
	SWITCH(GSW(1715))
		CASE_SMALL(2)
			USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
			USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_94"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(14)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_95"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_96"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL_EQUAL(GSW(1708), 18)
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
	IF_SMALL(GSW(1715), 14)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_61_5"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL_EQUAL(GSW(1708), 18)
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
	IF_SMALL(GSW(1704), 2)
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

EVT_BEGIN(usu_00_init_evt_evt)
	IF_EQUAL(GSW(1717), 22)
		RETURN()
	END_IF()
	SWITCH(GSW(1704))
		CASE_SMALL(2)
			RUN_EVT(&usu00_stage4_coming_event)
			RETURN()
		CASE_END()
		CASE_BETWEEN(3, 3)
			RUN_EVT(&chief_pig_event)
			RETURN()
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(usu_00_init_evt_hook)
	RUN_CHILD_EVT(usu_00_init_evt_evt)
	GOTO(&usu_00_init_evt[414])
EVT_PATCH_END()

EVT_BEGIN(gatekeeper_talk_evt)
	SWITCH(GSW(1715))
		CASE_SMALL(2)
			IF_LARGE_EQUAL(GSW(1704), 6)
				USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
				USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_48"), 0, PTR("me"))
				RETURN()
			END_IF()
		CASE_END()
		CASE_SMALL(14)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_49"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_49_1"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_49_2"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_49_2_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(gatekeeper_talk_hook)
	RUN_CHILD_EVT(gatekeeper_talk_evt)
	RETURN()
EVT_PATCH_END()

EVT_BEGIN(gatekeeper_pig_init_evt)
	IF_SMALL(GSW(1704), 5)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR(gatekeeper_pig), 0, -2000, 0)
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 2)
		IF_LARGE_EQUAL(GSW(1704), 5)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR(gatekeeper_pig), 400, 0, -55)
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_npc::evt_npc_set_position, PTR(gatekeeper_pig), 0, -2000, 0)
	RETURN()
EVT_END()

EVT_BEGIN(gatekeeper_pig_init_hook)
	RUN_CHILD_EVT(gatekeeper_pig_init_evt)
	RETURN()
EVT_PATCH_END()

EVT_BEGIN(gatekeeper_init_evt)
	IF_SMALL(GSW(1704), 5)
		SET(LW(0), PTR("e_bero"))
		RUN_CHILD_EVT(evt_bero::bero_case_switch_off)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR(gatekeeper), 400, 0, -55)
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 2)
		IF_LARGE_EQUAL(GSW(1704), 5)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR(gatekeeper), 0, -2000, 0)
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_npc::evt_npc_set_position, PTR(gatekeeper), 400, 0, -55)
	RETURN()
EVT_END()

EVT_BEGIN(gatekeeper_init_hook)
	RUN_CHILD_EVT(gatekeeper_init_evt)
	RETURN()
EVT_PATCH_END()

EVT_BEGIN(village_chief_talk_evt)
	IF_SMALL(GSW(1704), 4)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_20"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 2)
		IF_LARGE_EQUAL(GSW(1704), 4)
			USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
			USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_22"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	IF_SMALL(GSW(1715), 5)
		IF_EQUAL(GSWF(1929), 0)
			SET(GSWF(1929), 1)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_24"), 0, PTR("me"))
			USER_FUNC(evt_npc::evt_set_dir_to_target, PTR("me"), PTR(doopliss))
			RETURN()
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_24"), 0, PTR("me"))
			USER_FUNC(evt_npc::evt_set_dir_to_target, PTR("me"), PTR(doopliss))
			RETURN()
		END_IF()
	END_IF()
	IF_SMALL(GSW(1715), 14)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_25"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 17)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_157"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_157_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_157_1_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(village_chief_talk_hook)
	RUN_CHILD_EVT(village_chief_talk_evt)
	RETURN()
EVT_PATCH_END()

EVT_BEGIN(village_chief_pig_init_evt)
	IF_SMALL(GSW(1704), 3)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR(mayor_pig), 0, -2000, 0)
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 4)
		IF_LARGE_EQUAL(GSW(1704), 3)
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_npc::evt_npc_set_position, PTR(mayor_pig), 0, -2000, 0)
	RETURN()
EVT_END()

EVT_BEGIN(village_chief_pig_init_hook)
	RUN_CHILD_EVT(village_chief_pig_init_evt)
	RETURN()
EVT_PATCH_END()

EVT_BEGIN(village_chief_init_evt)
	IF_SMALL(GSW(1704), 3)
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 4)
		IF_LARGE_EQUAL(GSW(1704), 3)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR(mayor), 0, -2000, 0)
			RETURN()
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(village_chief_init_hook)
	RUN_CHILD_EVT(village_chief_init_evt)
	RETURN()
EVT_PATCH_END()

EVT_BEGIN(party_evt)
	USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
	USER_FUNC(evt_item::evt_item_entry, PTR("item01"), LW(3), LW(0), LW(1), LW(2), 16, GSWF(6080), 0)
	USER_FUNC(evt_item::evt_item_get_item, PTR("item01"))
	WAIT_MSEC(800)
	SET(GSW(1721), 2)
	USER_FUNC(evt_npc::evt_npc_set_position, PTR(vivian), 0, -1000, 0)
	USER_FUNC(evt_cam::evt_cam3d_evt_off, 100, 11)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
	USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG4_USU1"))
	USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG4_USU1"))
	RETURN()
EVT_END()

void ApplyUsuPatches(OSModuleInfo* module_info)
{
	patch::writePatch(&villagerA_init[5], villagerA_init_hook, sizeof(villagerA_init_hook));

	villagerA_pig_init[6] = GSW(1715);
	villagerA_pig_init[8] = 2;

	patch::writePatch(&villagerA_talk[0], villagerA_talk_hook, sizeof(villagerA_talk_hook));

	patch::writePatch(&villagerB_talk[0], villagerB_talk_hook, sizeof(villagerB_talk_hook));

	patch::writePatch(&villagerC_talk[0], villagerC_talk_hook, sizeof(villagerC_talk_hook));

	villagerD_init[6] = GSW(1715);
	villagerD_init[8] = 2;

	villagerD_pig_init[6] = GSW(1715);
	villagerD_pig_init[8] = 2;

	patch::writePatch(&villagerD_talk[0], villagerD_talk_hook, sizeof(villagerD_talk_hook));

	villagerE_init[6] = GSW(1715);
	villagerE_init[8] = 2;

	villagerE_pig_init[6] = GSW(1715);
	villagerE_pig_init[8] = 2;

	patch::writePatch(&villagerE_talk[0], villagerE_talk_hook, sizeof(villagerE_talk_hook));

	villagerK_init[6] = GSW(1715);
	villagerK_init[8] = 2;

	villagerK_pig_init[6] = GSW(1715);
	villagerK_pig_init[8] = 2;

	patch::writePatch(&villagerK_talk[0], villagerK_talk_hook, sizeof(villagerK_talk_hook));

	patch::writePatch(&village_chief_init[5], village_chief_init_hook, sizeof(village_chief_init_hook));

	patch::writePatch(&village_chief_pig_init[5], village_chief_pig_init_hook, sizeof(village_chief_pig_init_hook));

	patch::writePatch(&village_chief_talk[104], village_chief_talk_hook, sizeof(village_chief_talk_hook));

	vivian_init[6] = GSW(1714);
	vivian_init[8] = 0;
	vivian_init[9] = 0;
	vivian_init[30] = 0;

	usu00_3witch_event[267] = GSW(1715);
	usu00_3witch_event[268] = 4;

	usu00_stage4_coming_event[810] = GSW(1704);
	usu00_stage4_coming_event[811] = 2;

	usu00_vivian_cry_event[43] = GSW(1721);
	usu00_vivian_cry_event[44] = 1;

	gra00_2witch_find_ranpel_event[172] = GSW(1715);
	gra00_2witch_find_ranpel_event[173] = 9;

	usu00_vivian_joinup_event[548] = EVT_HELPER_CMD(2, 50);
	usu00_vivian_joinup_event[549] = EVT_HELPER_OP(LW(3));
	patch::writePatch(&usu00_vivian_joinup_event[551], party_evt, sizeof(party_evt));

	usu00_crowAB_talk_event[218] = GSW(1704);
	usu00_crowAB_talk_event[219] = 0; //Unused
	usu00_crowAB_talk_event[247] = GSW(1704);
	usu00_crowAB_talk_event[249] = 99; //Unused

	usu00_crowCDE_talk_event[214] = GSW(1704);
	usu00_crowCDE_talk_event[215] = 0; //Unused
	usu00_crowCDE_talk_event[243] = GSW(1704);
	usu00_crowCDE_talk_event[245] = 99; //Unused

	chief_pig_event[56] = GSW(1704);
	chief_pig_event[57] = 4;

	evt_door2_chk[1] = GSW(1704);
	evt_door2_chk[3] = 0;
	evt_door2_chk[4] = 0;

	dokan_1_evt[1] = GSW(1704);
	dokan_1_evt[2] = 2;

	kusa_01_evt[1] = GSW(1717);
	kusa_01_evt[2] = 23;

	evt_ichikorori_ball_get[1] = GSWF(6040);
	evt_ichikorori_ball_get[2] = 1;

	usu_00_touch_ichikorori_ball[1] = GSWF(6040);
	usu_00_touch_ichikorori_ball[2] = 0;

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
	usu_00_init_evt[43] = 99; //Unused
	usu_00_init_evt[44] = 99; //Unused
	usu_00_init_evt[54] = 16;
	usu_00_init_evt[102] = GSW(1708);
	usu_00_init_evt[103] = 18;
	patch::writePatch(&usu_00_init_evt[402], usu_00_init_evt_hook, sizeof(usu_00_init_evt_hook));
	usu_00_init_evt[415] = GSW(1715);
	usu_00_init_evt[417] = 4;
	usu_00_init_evt[418] = 5;
	usu_00_init_evt[436] = GSW(1721);
	usu_00_init_evt[447] = 0;
	usu_00_init_evt[442] = GSWF(6040);
	usu_00_init_evt[443] = 1;
	usu_00_init_evt[454] = GSW(1715);
	usu_00_init_evt[455] = 8;
	usu_00_init_evt[474] = GSW(1715);
	usu_00_init_evt[475] = 2;
	usu_00_init_evt[477] = GSW(1704);
	usu_00_init_evt[478] = 7;
	usu_00_init_evt[504] = GSW(1717);
	usu_00_init_evt[505] = 22;
	usu_00_init_evt[523] = GSW(1717);
	usu_00_init_evt[524] = 23;
	usu_00_init_evt[586] = GSW(1715);
	usu_00_init_evt[587] = 13;
	usu_00_init_evt[621] = GSW(1708);
	usu_00_init_evt[622] = 16;
	usu_00_init_evt[644] = GSW(1708);
	usu_00_init_evt[645] = 18;

	patch::writePatch(&villagerF_talk[0], villagerF_talk_hook, sizeof(villagerF_talk_hook));

	patch::writePatch(&villagerG_talk[0], villagerG_talk_hook, sizeof(villagerG_talk_hook));

	patch::writePatch(&villagerH_talk[0], villagerH_talk_hook, sizeof(villagerH_talk_hook));

	villagerI_init[6] = GSW(1715);
	villagerI_init[8] = 2;

	villagerI_pig_init[6] = GSW(1715);
	villagerI_pig_init[8] = 2;

	patch::writePatch(&villagerI_talk[0], villagerI_talk_hook, sizeof(villagerI_talk_hook));

	villagerJ_init[6] = GSW(1715);
	villagerJ_init[8] = 2;

	villagerJ_pig_init[6] = GSW(1715);
	villagerJ_pig_init[8] = 2;

	patch::writePatch(&villagerJ_talk[0], villagerJ_talk_hook, sizeof(villagerJ_talk_hook));

	shop_man_init[6] = GSW(1715);
	shop_man_init[8] = 2;
	shop_man_init[10] = 14;

	patch::writePatch(&shop_man_talk[0], shop_man_talk_hook, sizeof(shop_man_talk_hook));

	shop_woman_talk[0] = EVT_HELPER_CMD(2, 24);
	shop_woman_talk[1] = GSWF(1925);
	shop_woman_talk[2] = 0;
	shop_woman_talk[3] = 0;
	shop_woman_talk[135] = EVT_HELPER_CMD(0, 33);

	patch::writePatch(&shop_woman_talk[136], shop_woman_talk_hook, sizeof(shop_woman_talk_hook));

	kizayarou_init[6] = GSW(1715);
	kizayarou_init[8] = 2;
	kizayarou_init[16] = 14;

	kizayarou_pig_init[1] = GSW(1715);
	kizayarou_pig_init[3] = 2;

	kizayarou_talk[1] = GSW(1715);
	kizayarou_talk[3] = 2;

	aunt_init[6] = GSW(1715);
	aunt_init[8] = 2;

	aunt_pig_init[6] = GSW(1715);
	aunt_pig_init[8] = 2;

	aunt_talk[403] = GSW(1715);
	aunt_talk[405] = 2;
	aunt_talk[654] = 14;
	aunt_talk[662] = 17;
	aunt_talk[669] = 0;
	aunt_talk[670] = 0;
	aunt_talk[671] = 0;
	aunt_talk[672] = 0;
	aunt_talk[673] = 0;
	aunt_talk[674] = 0;
	aunt_talk[675] = 0;
	aunt_talk[676] = 0;

	childA_talk[1] = GSW(1715);
	childA_talk[3] = 200;
	childA_talk[18] = 222;
	childA_talk[21] = 0;
	childA_talk[22] = 0;
	childA_talk[23] = 0;
	childA_talk[24] = 0;

	childB_talk[1] = GSW(1715);
	childB_talk[3] = 2;
	childB_talk[18] = 17;
	childB_talk[21] = 0;
	childB_talk[22] = 0;
	childB_talk[23] = 0;
	childB_talk[24] = 0;

	childC_talk[1] = GSW(1715);
	childC_talk[3] = 2;
	childC_talk[18] = 17;
	childC_talk[21] = 0;
	childC_talk[22] = 0;
	childC_talk[23] = 0;
	childC_talk[24] = 0;

	patch::writePatch(&gatekeeper_init[5], gatekeeper_init_hook, sizeof(gatekeeper_init_hook));

	patch::writePatch(&gatekeeper_pig_init[5], gatekeeper_pig_init_hook, sizeof(gatekeeper_pig_init_hook));

	gatekeeper_talk[1] = GSW(1704);
	gatekeeper_talk[3] = 3;
	gatekeeper_talk[71] = GSW(1704);
	gatekeeper_talk[72] = 3;
	gatekeeper_talk[75] = 4;
	gatekeeper_talk[83] = 5;
	gatekeeper_talk[90] = EVT_HELPER_CMD(0, 49);
	patch::writePatch(&gatekeeper_talk[91], gatekeeper_talk_hook, sizeof(gatekeeper_talk_hook));
	
	musume_talk[1] = GSW(1715);
	musume_talk[3] = 2;
	musume_talk[18] = 10;

	evt_mon_open[1] = GSW(1704);
	evt_mon_open[2] = 6;
	evt_mon_open[105] = GSW(1704);
	evt_mon_open[106] = 7;

	usu01_crowFG_talk_event[193] = GSW(1704);
	usu01_crowFG_talk_event[194] = 1;
	usu01_crowFG_talk_event[222] = GSW(1717);
	usu01_crowFG_talk_event[224] = 5;
	usu01_crowFG_talk_event[327] = 17;

	usu01_crowHI_talk_event[181] = GSW(1704);
	usu01_crowHI_talk_event[182] = 1;
	usu01_crowHI_talk_event[210] = GSW(1717);
	usu01_crowHI_talk_event[212] = 5;
	usu01_crowHI_talk_event[226] = 17;

	usu01_crowJ_talk_event[181] = GSW(1704);
	usu01_crowJ_talk_event[182] = 0; //Unused
	usu01_crowJ_talk_event[210] = GSW(1717);
	usu01_crowJ_talk_event[212] = 5;
	usu01_crowJ_talk_event[220] = 17;

	loop_msg[1] = GSW(1715);
	loop_msg[2] = 2;
	loop_msg[6] = GSW(1715);
	loop_msg[7] = 10;

	mawari_house_delay[1] = GSW(1704);
	mawari_house_delay[2] = 4;

	evt_ie2_door2_1[150] = GSWF(6042);
	evt_ie2_door2_1[151] = 1;

	evt_ie2_door2_2[82] = GSWF(6042);
	evt_ie2_door2_2[83] = 1;

	usu01_gusun[1] = GSW(1715);
	usu01_gusun[2] = 14;
	usu01_gusun[31] = GSW(1715);
	usu01_gusun[32] = 2;

	evt_monban_buta_main[30] = GSW(1704);
	evt_monban_buta_main[31] = 5;

	evt_monban_buta[1] = GSW(1704);
	evt_monban_buta[2] = 5;

	evt_monban_buta2[1] = GSW(1704);
	evt_monban_buta2[2] = 5;

	evt_monban_look[3] = GSW(1704);
	evt_monban_look[4] = 6;
	evt_monban_look[8] = GSW(1704);
	evt_monban_look[9] = 5;
	evt_monban_look[109] = GSW(1704);
	evt_monban_look[110] = 6;

	unlock[9] = GSWF(6042);
	unlock[10] = 1;

	evt_majin2[210] = GSWF(6043);
	evt_majin2[211] = 1;

	usu_01_init_evt[23] = GSW(1715);
	usu_01_init_evt[25] = 0;
	usu_01_init_evt[26] = 2;
	usu_01_init_evt[34] = 2;
	usu_01_init_evt[35] = 10;
	usu_01_init_evt[44] = GSWF(6042);
	usu_01_init_evt[45] = 1;
	usu_01_init_evt[93] = GSW(1704);
	usu_01_init_evt[94] = 4;
	usu_01_init_evt[106] = GSWF(6042);
	usu_01_init_evt[107] = 1;
	usu_01_init_evt[109] = GSW(1715);
	usu_01_init_evt[110] = 2;
	usu_01_init_evt[117] = GSW(1715);
	usu_01_init_evt[118] = 8;
	usu_01_init_evt[156] = GSW(1704);
	usu_01_init_evt[157] = 7;
	usu_01_init_evt[309] = GSWF(6043);
	usu_01_init_evt[310] = 1;

	evt_usu_kagemario_party_kill_usu[1] = GSW(1704);

	//These are swByteGet
	usu_evt_kagemario_init[2] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
	usu_evt_kagemario_init[6] = 0x2C030003; // cmpwi r3, 0x3
	usu_evt_kagemario_init[8] = 0x2C030007; // cmpwi r3, 0x7
}