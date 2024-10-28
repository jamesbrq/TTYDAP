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
extern int32_t usu00_3witch_event[];
extern int32_t gra00_2witch_find_ranpel_event[];
extern int32_t usu00_vivian_cry_event[];
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

EVT_BEGIN(villagerA_init_evt)
	IF_EQUAL(GSW(1708), 402)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_usu_31_1"), 0, PTR("me"))
		RETURN()
	END_IF()
EVT_END()

void ApplyUsuPatches(OSModuleInfo* module_info)
{
	patch::writePatch(&villagerA_init[5], villagerA_init_evt, sizeof(villagerA_init_evt));

	villagerA_pig_init[6] = GSW(1704);
	villagerA_pig_init[8] = 200;

	villagerA_talk[1] = GSW(1704);
	villagerA_talk[3] = 200;
}