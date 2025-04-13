#include "subrel_rsh.h"
#include "evt_cmd.h"
#include "patch.h"
#include "AP/rel_patch_definitions.h"
#include "ttyd/evt_bero.h"
#include "ttyd/evt_case.h"
#include "ttyd/evt_item.h"
#include "ttyd/evt_map.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_msg.h"
#include "ttyd/evt_npc.h"
#include "ttyd/evt_snd.h"
#include "ttyd/evt_sub.h"

#include <cstdint>

using namespace ttyd;

extern int32_t simi_check[];
extern int32_t simi_offscreen_check[];
extern int32_t kami_check[];
extern int32_t kami_offscreen_check[];
extern int32_t evt_rsh_door_open[];
extern int32_t evt_rsh_door_close[];
extern int32_t evt_moamoa[];
extern int32_t evt_rsh_moamoa_on_window[];
extern int32_t evt_handle[];
extern int32_t regu_unten[];
extern int32_t init_unten[];
extern int32_t talk_unten[];
extern int32_t rsh_00_init_evt[];
extern int32_t rsh_00_a_init_evt[];
extern int32_t init_toron_rsh_01[];
extern int32_t talk_toron_rsh_01[];
extern int32_t init_madam_rsh_01[];
extern int32_t talk_madam_ring_return[];
extern int32_t talk_madam_rsh_01[];
extern int32_t evt_daiji_kami_rsh_01[];
extern int32_t evt_pikary_arrival[];
extern int32_t evt_access_rsh_01[];
extern int32_t evt_access_entry_rsh_01[];
extern int32_t rsh_01_init_evt[];
extern int32_t rsh_01_a_init_evt[];
extern int32_t gor_bero[];
extern int32_t evt_nabe[];
extern int32_t init_gurume[];
extern int32_t talk_gurume[];
extern int32_t init_yuurei[];
extern int32_t talk_yuurei[];
extern int32_t init_toron_rsh_02[];
extern int32_t evt_1day_end[];
extern int32_t evt_2day_end[];
extern int32_t evt_bed[];
extern int32_t evt_toron_catch[];
extern int32_t evt_toron_catch2[];
extern int32_t evt_toron_appear[];
extern int32_t evt_daiji_kami_rsh_02[];
extern int32_t evt_yuka_simi_rsh_02[];
extern int32_t evt_access_entry[];
extern int32_t rsh_02_init_evt[];
extern int32_t rsh_02_a_init_evt[];
extern int32_t init_cook[];
extern int32_t talk_cook[];
extern int32_t init_waitless_rsh_03[];
extern int32_t talk_waitless_pias_return[];
extern int32_t talk_waitless_rsh_03[];
extern int32_t move_waitless[];
extern int32_t talk_toron_rsh_03[];
extern int32_t talk_sarary_rsh_03[];
extern int32_t evt_lanch_missing[];
extern int32_t talk_powan_rsh_03[];
extern int32_t evt_shop_moamoa_talk[];
extern int32_t evt_shop_close[];
extern int32_t evt_shop_door_init[];
extern int32_t evt_yuka_simi_rsh_03[];
extern int32_t evt_access_rsh_03[];
extern int32_t rsh_03_init_evt[];
extern int32_t rsh_03_a_init_evt[];
extern int32_t rsh_03_c_init_evt[];
extern int32_t evt_rsh_konarikin_irai_party_talk[];
extern int32_t evt_allow_kamotsu[];
extern int32_t evt_shorui_funshitu[];
extern int32_t evt_daijisouna_kami[];
extern int32_t init_syashou_rsh_04[];
extern int32_t talk_syashou_rsh_04[];
extern int32_t init_sarary_rsh_04[];
extern int32_t talk_sarary_rsh_04[];
extern int32_t talk_papa_mama[];
extern int32_t init_papa_rsh_04[];
extern int32_t talk_papa_rsh_04[];
extern int32_t init_mama_rsh_04[];
extern int32_t talk_mama_rsh_04[];
extern int32_t init_konari_rsh_04[];
extern int32_t talk_konari_rsh_04[];
extern int32_t init_powan_rsh_04[];
extern int32_t talk_powan_rsh_04[];
extern int32_t evt_tana_5[];
extern int32_t evt_access_rsh_04[];
extern int32_t evt_narikin_room[];
extern int32_t evt_powan_room[];
extern int32_t rsh_04_init_evt[];
extern int32_t rsh_04_a_init_evt[];
extern int32_t rsh_04_c_init_evt[];
extern int32_t rsh_evt_meet_moamoa[];
extern int32_t rsh_evt_moamoa_escape[];
extern int32_t init_syashou_rsh_05[];
extern int32_t talk_syashou_rsh_05[];
extern int32_t init_waitless_rsh_05[];
extern int32_t talk_waitless_rsh_05[];
extern int32_t evt_ghost_diary[];
extern int32_t rsh_05_init_evt[];
extern int32_t rsh_05_a_init_evt[];
extern int32_t hom_10_evt_resha_start_rsh_06[];
extern int32_t evt_great_moamoa[];
extern int32_t rsh_06_init_evt[];
extern int32_t rsh_06_a_init_evt[];

const char madam1[] = "\x83\x7D\x83\x5F\x83\x80";

// Assembly
extern int32_t rsh_prolog[];

// clang-format off
EVT_BEGIN(rsh_06_init_evt_evt)
IF_LARGE_EQUAL(GSW(1720), 1)
IF_SMALL(GSW(1720), 8)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_HOM1"))
USER_FUNC(evt_snd::evt_snd_envoff, 512)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
RETURN()
END_IF()
END_IF()
SWITCH(GSW(1706))
CASE_BETWEEN(4, 6)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_HOM1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(24, 29)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_POWAN1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(31, 32)
USER_FUNC(evt_snd::evt_snd_bgmoff, 512)
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(33, 35)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_DANGER4"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(38, 48)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_PIK1"))
USER_FUNC(evt_snd::evt_snd_envoff, 512)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
CASE_END()
CASE_ETC()
USER_FUNC(evt_snd::evt_snd_bgmoff, 512)
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
END_SWITCH()
RETURN()
EVT_END()

EVT_BEGIN(rsh_06_init_evt_hook)
RUN_CHILD_EVT(rsh_06_init_evt_evt)
RETURN()
EVT_END()

EVT_BEGIN(rsh_04_init_evt_music)
IF_LARGE_EQUAL(GSW(1720), 1)
IF_SMALL(GSW(1720), 8)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_HOM1"))
USER_FUNC(evt_snd::evt_snd_envoff, 512)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
RETURN()
END_IF()
END_IF()
SWITCH(GSW(1706))
CASE_BETWEEN(4, 6)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_HOM1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(24, 29)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_POWAN1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(31, 32)
USER_FUNC(evt_snd::evt_snd_bgmoff, 512)
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(33, 35)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_DANGER4"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(38, 48)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_PIK1"))
USER_FUNC(evt_snd::evt_snd_envoff, 512)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
CASE_END()
CASE_ETC()
USER_FUNC(evt_sub::evt_sub_get_mapname, LW(0))
IF_STR_EQUAL(LW(0), PTR("rsh_04_a"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1"))
END_IF()
IF_STR_EQUAL(LW(0), PTR("rsh_04_b"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH3"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1"))
END_IF()
IF_STR_EQUAL(LW(0), PTR("rsh_04_c"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH2"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1"))
END_IF()
CASE_END()
END_SWITCH()
RETURN()
EVT_END()

EVT_BEGIN(rsh_04_init_evt_music_hook)
RUN_CHILD_EVT(rsh_04_init_evt_music)
RETURN()
EVT_END()

EVT_BEGIN(rsh_00_init_evt_music)
IF_LARGE_EQUAL(GSW(1720), 1)
IF_SMALL(GSW(1720), 8)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_HOM1"))
USER_FUNC(evt_snd::evt_snd_envoff, 512)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
RETURN()
END_IF()
END_IF()
SWITCH(GSW(1706))
CASE_BETWEEN(4, 6)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_HOM1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(24, 29)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_POWAN1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(31, 32)
USER_FUNC(evt_snd::evt_snd_bgmoff, 512)
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(33, 35)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_DANGER4"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(38, 48)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_PIK1"))
USER_FUNC(evt_snd::evt_snd_envoff, 512)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
CASE_END()
CASE_ETC()
USER_FUNC(evt_sub::evt_sub_get_mapname, LW(0))
IF_STR_EQUAL(LW(0), PTR("rsh_00_a"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_4"))
END_IF()
IF_STR_EQUAL(LW(0), PTR("rsh_00_b"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH3"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_4"))
END_IF()
IF_STR_EQUAL(LW(0), PTR("rsh_00_c"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH2"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_4"))
END_IF()
CASE_END()
END_SWITCH()
RETURN()
EVT_END()

EVT_BEGIN(rsh_00_init_evt_music_hook)
RUN_CHILD_EVT(rsh_00_init_evt_music)
RETURN()
EVT_END()

EVT_BEGIN(rsh_01_init_evt_music)
IF_LARGE_EQUAL(GSW(1720), 1)
IF_SMALL(GSW(1720), 8)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_HOM1"))
USER_FUNC(evt_snd::evt_snd_envoff, 512)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
RETURN()
END_IF()
END_IF()
SWITCH(GSW(1706))
CASE_BETWEEN(4, 6)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_POWAN1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1"))
CASE_END()
CASE_BETWEEN(24, 29)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_POWAN1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1"))
CASE_END()
CASE_BETWEEN(31, 32)
USER_FUNC(evt_snd::evt_snd_bgmoff, 512)
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(33, 35)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_DANGER4"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1"))
CASE_END()
CASE_BETWEEN(38, 48)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_PIK1"))
USER_FUNC(evt_snd::evt_snd_envoff, 512)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
CASE_END()
CASE_ETC()
USER_FUNC(evt_sub::evt_sub_get_mapname, LW(0))
IF_STR_EQUAL(LW(0), PTR("rsh_01_a"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1"))
END_IF()
IF_STR_EQUAL(LW(0), PTR("rsh_01_b"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH3"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1"))
END_IF()
IF_STR_EQUAL(LW(0), PTR("rsh_01_c"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH2"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1"))
END_IF()
CASE_END()
END_SWITCH()
RETURN()
EVT_END()

EVT_BEGIN(rsh_01_init_evt_music_hook)
RUN_CHILD_EVT(rsh_01_init_evt_music)
RETURN()
EVT_END()

EVT_BEGIN(rsh_02_init_evt_music)
IF_LARGE_EQUAL(GSW(1720), 1)
IF_SMALL(GSW(1720), 8)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_HOM1"))
USER_FUNC(evt_snd::evt_snd_envoff, 512)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
RETURN()
END_IF()
END_IF()
SWITCH(GSW(1706))
CASE_SMALL(2)
USER_FUNC(evt_snd::evt_snd_bgmon, 272, PTR("ENV_STG6_RSH1"))
CASE_END()
CASE_BETWEEN(4, 6)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_POWAN1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(24, 27)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_POWAN1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(28, 28)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_STG3_EGG1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_EQUAL(29)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_POWAN1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(31, 32)
USER_FUNC(evt_snd::evt_snd_bgmoff, 512)
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(33, 35)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_DANGER4"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(38, 48)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_PIK1"))
USER_FUNC(evt_snd::evt_snd_envoff, 512)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
CASE_END()
CASE_ETC()
USER_FUNC(evt_sub::evt_sub_get_mapname, LW(0))
IF_STR_EQUAL(LW(0), PTR("rsh_02_a"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1"))
END_IF()
IF_STR_EQUAL(LW(0), PTR("rsh_02_b"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH3"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1"))
END_IF()
IF_STR_EQUAL(LW(0), PTR("rsh_02_c"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH2"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1"))
END_IF()
CASE_END()
END_SWITCH()
RETURN()
EVT_END()

EVT_BEGIN(rsh_02_init_evt_music_hook)
RUN_CHILD_EVT(rsh_02_init_evt_music)
RETURN()
EVT_END()

EVT_BEGIN(rsh_03_init_evt_music)
IF_LARGE_EQUAL(GSW(1720), 1)
IF_SMALL(GSW(1720), 8)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_HOM1"))
USER_FUNC(evt_snd::evt_snd_envoff, 512)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
RETURN()
END_IF()
END_IF()
SWITCH(GSW(1706))
CASE_BETWEEN(4, 6)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_POWAN1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(24, 29)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_POWAN1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(31, 32)
USER_FUNC(evt_snd::evt_snd_bgmoff, 512)
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(33, 35)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_DANGER4"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_3"))
CASE_END()
CASE_BETWEEN(38, 48)
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_PIK1"))
USER_FUNC(evt_snd::evt_snd_envoff, 512)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
CASE_END()
CASE_ETC()
USER_FUNC(evt_sub::evt_sub_get_mapname, LW(0))
IF_STR_EQUAL(LW(0), PTR("rsh_03_a"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH1"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_2"))
END_IF()
IF_STR_EQUAL(LW(0), PTR("rsh_03_b"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH3"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_2"))
END_IF()
IF_STR_EQUAL(LW(0), PTR("rsh_03_c"))
USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG6_RSH2"))
USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG6_RSH1_2"))
END_IF()
CASE_END()
END_SWITCH()
RETURN()
EVT_END()

EVT_BEGIN(rsh_03_init_evt_music_hook)
RUN_CHILD_EVT(rsh_03_init_evt_music)
RETURN()
EVT_END()

EVT_BEGIN(rsh_04_init_evt_evt)
	IF_SMALL(GSW(1706), 37)
		SET(GSWF(3578), 0)
	END_IF()
	IF_LARGE_EQUAL(GSW(1720), 1)
		IF_SMALL(GSW(1720), 8)
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1706), 37)
		RETURN()
	END_IF()
	USER_FUNC(evt_map::evt_map_playanim, PTR("yure"), 1, 0)
	USER_FUNC(evt_map::evt_map_playanim, PTR("hai_sora"), 1, 0)
	USER_FUNC(evt_map::evt_map_playanim, PTR("hai_yama"), 1, 0)
	USER_FUNC(evt_map::evt_map_playanim, PTR("hai_iwayama"), 1, 0)
	USER_FUNC(evt_map::evt_map_playanim, PTR("hai_saboten"), 1, 0)
	RETURN()
EVT_END()

EVT_BEGIN(rsh_01_init_evt_hook1)
RUN_CHILD_EVT(rsh_04_init_evt_evt)
GOTO(&rsh_01_init_evt[34])
EVT_PATCH_END()

EVT_BEGIN(rsh_02_init_evt_hook)
RUN_CHILD_EVT(rsh_04_init_evt_evt)
GOTO(&rsh_02_init_evt[34])
EVT_PATCH_END()

EVT_BEGIN(rsh_03_init_evt_hook)
RUN_CHILD_EVT(rsh_04_init_evt_evt)
GOTO(&rsh_03_init_evt[34])
EVT_PATCH_END()

EVT_BEGIN(rsh_04_init_evt_hook)
RUN_CHILD_EVT(rsh_04_init_evt_evt)
GOTO(&rsh_04_init_evt[34])
EVT_PATCH_END()

EVT_BEGIN(rsh_00_init_evt_evt)
IF_LARGE_EQUAL(GSW(1720), 1)
IF_SMALL(GSW(1720), 8)
RETURN()
END_IF()
END_IF()
IF_LARGE_EQUAL(GSW(1706), 37)
RETURN()
END_IF()
USER_FUNC(evt_map::evt_map_playanim, PTR("yure"), 1, 0)
USER_FUNC(evt_map::evt_map_playanim, PTR("hai_sora"), 1, 0)
USER_FUNC(evt_map::evt_map_playanim, PTR("hai_yama"), 1, 0)
USER_FUNC(evt_map::evt_map_playanim, PTR("hai_iwayama"), 1, 0)
USER_FUNC(evt_map::evt_map_playanim, PTR("hai_saboten"), 1, 0)
USER_FUNC(evt_map::evt_map_playanim, PTR("hai_hosi"), 1, 0)
RETURN()
EVT_END()

EVT_BEGIN(rsh_00_init_evt_hook)
RUN_CHILD_EVT(rsh_00_init_evt_evt)
GOTO(&rsh_00_init_evt[39])
EVT_PATCH_END()

EVT_BEGIN(rsh_01_init_evt_evt)
    IF_LARGE_EQUAL(GSW(1720), 1)
        IF_SMALL(GSW(1720), 8)
            RETURN()
        END_IF()
    END_IF()
    IF_LARGE_EQUAL(GSW(1706), 37)
        RETURN()
    END_IF()
    IF_SMALL(GSW(1706), 37)
        SET(LW(0), PTR("s_bero"))
        RUN_CHILD_EVT(evt_bero::bero_case_switch_off)
		USER_FUNC(evt_bero::evt_bero_overwrite, PTR("s_bero"), PTR(&gor_bero))
		SET(LW(0), PTR("s_bero"))
		RUN_CHILD_EVT(evt_bero::bero_case_switch_on)
		RETURN()
    END_IF()
    SET(LW(0), 2)
    RUN_CHILD_EVT(evt_bero::bero_case_switch_off)
    SET(LW(1), PTR("stg6_rsh_261"))
    SET(LW(2), PTR("stg6_rsh_261"))
    USER_FUNC(evt_case::evt_run_case_evt, 9, 1, PTR("s_bero"), 0, PTR(&evt_access_rsh_01), 0)
    RETURN()
EVT_END()

EVT_BEGIN(rsh_01_init_evt_hook2)
RUN_CHILD_EVT(rsh_01_init_evt_evt)
GOTO(&rsh_01_init_evt[76])
EVT_PATCH_END()

EVT_BEGIN(talk_waitless_evt)
IF_LARGE_EQUAL(GSW(1720), 1)
IF_SMALL(GSW(1720), 8)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
RETURN()
END_IF()
END_IF()
IF_LARGE_EQUAL(GSW(1706), 38)
IF_SMALL_EQUAL(GSW(1706), 48)
USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
RETURN()
END_IF()
END_IF()
USER_FUNC(evt_snd::evt_snd_bgmon, 160, 0)
RETURN()
EVT_END()

EVT_BEGIN(talk_waitless_hook1)
RUN_CHILD_EVT(talk_waitless_evt)
GOTO(&talk_waitless_pias_return[156])
EVT_PATCH_END()

EVT_BEGIN(talk_waitless_hook2)
RUN_CHILD_EVT(talk_waitless_evt)
GOTO(&talk_waitless_rsh_03[69])
EVT_PATCH_END()

EVT_BEGIN(init_powan_evt)
IF_LARGE_EQUAL(GSW(1720), 1)
IF_SMALL(GSW(1720), 8)
USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
RETURN()
END_IF()
END_IF()
SWITCH(GSW(1706))
CASE_BETWEEN(3, 6)
USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
CASE_END()
CASE_BETWEEN(23, 29)
USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 230, 0, -180)
CASE_END()
CASE_BETWEEN(32, 36)
USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
CASE_END()
CASE_LARGE_EQUAL(38)
USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
CASE_END()
END_SWITCH()
RETURN()
EVT_END()

EVT_BEGIN(init_powan_hook)
RUN_CHILD_EVT(init_powan_evt)
GOTO(&init_powan_rsh_04[47])
EVT_PATCH_END()

EVT_BEGIN(init_konari_evt)
IF_LARGE_EQUAL(GSW(1720), 1)
IF_SMALL(GSW(1720), 8)
USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
RETURN()
END_IF()
END_IF()
SWITCH(GSW(1706))
CASE_BETWEEN(32, 36)
USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
CASE_END()
CASE_LARGE_EQUAL(38)
USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
CASE_END()
END_SWITCH()
RETURN()
EVT_END()

EVT_BEGIN(init_papa_hook)
RUN_CHILD_EVT(init_konari_evt)
GOTO(&init_papa_rsh_04[29])
EVT_PATCH_END()

EVT_BEGIN(init_konari_hook)
RUN_CHILD_EVT(init_konari_evt)
GOTO(&init_konari_rsh_04[29])
EVT_PATCH_END()

EVT_BEGIN(talk_madam_ring_return_evt)
    IF_LARGE_EQUAL(GSW(1720), 1)
        IF_SMALL_EQUAL(GSW(1720), 8)
            USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
            RETURN()
        END_IF()
    END_IF()
    SWITCH(GSW(1706))
        CASE_BETWEEN(38, 48)
            USER_FUNC(evt_snd::evt_snd_bgmoff, 26624)
        CASE_ETC()
            USER_FUNC(evt_snd::evt_snd_bgmon, 160, 0)
    END_SWITCH()
    RETURN()
EVT_END()

EVT_BEGIN(talk_madam_ring_return_hook1)
    RUN_CHILD_EVT(talk_madam_ring_return_evt)
    RETURN()
EVT_END()

EVT_BEGIN(talk_madam_hook)
    RUN_CHILD_EVT(talk_madam_ring_return_evt)
    GOTO(&talk_madam_rsh_01[69])
EVT_PATCH_END()

EVT_BEGIN(talk_madam_ring_item)
    IF_EQUAL(GSWF(6092), 0)
        USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg6_rsh_201"), 0, PTR(madam1))
        USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        USER_FUNC(evt_item::evt_item_entry, PTR("item01"), LW(3), LW(0), LW(1), LW(2), 16, GSWF(6092), 0)
        USER_FUNC(evt_item::evt_item_get_item, PTR("item01"))
        WAIT_MSEC(800)
        USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg6_rsh_202"), 0, PTR(madam1))
        RETURN()
    ELSE()
        USER_FUNC(evt_msg::evt_msg_print, 0, PTR("madam_abort"), 0, PTR(madam1))
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN(talk_madam_ring_return_hook2)
    RUN_CHILD_EVT(talk_madam_ring_item)
    GOTO(&talk_madam_ring_return[33])
EVT_PATCH_END()
// clang-format on

namespace mod
{
    void main()
    {
        simi_check[24] = GSW(1706);
        simi_check[26] = 3;
        simi_check[27] = 22;

        simi_offscreen_check[24] = GSW(1706);
        simi_offscreen_check[26] = 3;
        simi_offscreen_check[27] = 22;

        kami_check[7] = GSW(1706);
        kami_check[8] = 24;
        kami_check[40] = GSW(1706);
        kami_check[42] = 26;
        kami_check[43] = 27;

        kami_offscreen_check[7] = GSW(1706);
        kami_offscreen_check[8] = 24;
        kami_offscreen_check[24] = GSW(1706);
        kami_offscreen_check[26] = 26;
        kami_offscreen_check[27] = 27;

        evt_rsh_door_open[267] = GSW(1706);
        evt_rsh_door_open[269] = 21;
        evt_rsh_door_open[350] = 26;
        evt_rsh_door_open[351] = 27;
        evt_rsh_door_open[356] = 30;

        evt_rsh_door_close[38] = GSW(1706);
        evt_rsh_door_close[39] = 8;
        evt_rsh_door_close[60] = GSW(1706);
        evt_rsh_door_close[61] = 14;
        evt_rsh_door_close[81] = GSW(1706);
        evt_rsh_door_close[82] = 17;
        evt_rsh_door_close[115] = GSW(1706);
        evt_rsh_door_close[117] = 26;
        evt_rsh_door_close[118] = 27;

        evt_moamoa[121] = GSW(1706);
        evt_moamoa[122] = 33;

        evt_rsh_moamoa_on_window[1] = GSW(1706);
        evt_rsh_moamoa_on_window[3] = 33;
        evt_rsh_moamoa_on_window[4] = 35;

        evt_handle[3] = GSW(1706);
        evt_handle[4] = 33;
        evt_handle[6] = GSW(1706);
        evt_handle[7] = 38;

        regu_unten[21] = GSW(1706);
        regu_unten[22] = 29;
        regu_unten[24] = GSW(1706);
        regu_unten[25] = 30;
        regu_unten[30] = GSW(1706);
        regu_unten[31] = 33;
        regu_unten[33] = GSW(1706);
        regu_unten[34] = 43;
        regu_unten[39] = GSW(1706);
        regu_unten[40] = 50;
        regu_unten[66] = GSW(1706);
        regu_unten[67] = 32;
        regu_unten[110] = GSW(1706);
        regu_unten[111] = 32;
        regu_unten[153] = GSW(1706);
        regu_unten[154] = 32;

        init_unten[1] = GSW(1706);
        init_unten[3] = 33;
        init_unten[4] = 35;

        talk_unten[1] = GSW(1706);
        talk_unten[3] = 8;
        talk_unten[28] = 12;
        talk_unten[36] = 13;
        talk_unten[101] = GSW(1706);
        talk_unten[102] = 13;
        talk_unten[104] = 14;
        talk_unten[112] = 21;
        talk_unten[120] = 23;
        talk_unten[128] = 29;
        talk_unten[150] = 30;
        talk_unten[158] = 31;
        talk_unten[166] = 33;
        talk_unten[170] = 36;
        talk_unten[186] = 50;

        patch::writePatch(&rsh_00_init_evt[0], rsh_00_init_evt_hook, sizeof(rsh_00_init_evt_hook));
        patch::writePatch(&rsh_00_init_evt[80], rsh_00_init_evt_music_hook, sizeof(rsh_00_init_evt_music_hook));

        rsh_00_a_init_evt[10] = GSW(1706);
        rsh_00_a_init_evt[12] = 32;
        rsh_00_a_init_evt[13] = 35;
        rsh_00_a_init_evt[42] = GSW(1706);
        rsh_00_a_init_evt[43] = 33;

        init_toron_rsh_01[1] = GSW(1706);
        init_toron_rsh_01[3] = 3;
        init_toron_rsh_01[11] = 3;
        init_toron_rsh_01[12] = 6;
        init_toron_rsh_01[20] = 7;
        init_toron_rsh_01[21] = 22;
        init_toron_rsh_01[29] = 23;
        init_toron_rsh_01[30] = 29;
        init_toron_rsh_01[38] = 29;

        talk_toron_rsh_01[1] = GSW(1706);
        talk_toron_rsh_01[3] = 7;
        talk_toron_rsh_01[25] = 11;
        talk_toron_rsh_01[33] = 14;
        talk_toron_rsh_01[46] = 21;

        init_madam_rsh_01[1] = GSW(1706);
        init_madam_rsh_01[3] = 32;
        init_madam_rsh_01[11] = 32;
        init_madam_rsh_01[12] = 36;
        init_madam_rsh_01[20] = 37;

        patch::writePatch(&talk_madam_ring_return[45], talk_madam_ring_return_hook1, sizeof(talk_madam_ring_return_hook1));
        talk_madam_ring_return[18] = EVT_HELPER_CMD(2, 50);
        talk_madam_ring_return[19] = EVT_HELPER_OP(LW(3));
        patch::writePatch(&talk_madam_ring_return[21], talk_madam_ring_return_hook2, sizeof(talk_madam_ring_return_hook2));
        talk_madam_ring_return[25] = 0;
        talk_madam_ring_return[26] = 0;

        talk_madam_rsh_01[1] = GSW(1706);
        talk_madam_rsh_01[2] = 8;

        patch::writePatch(&talk_madam_rsh_01[49], talk_madam_hook, sizeof(talk_madam_hook));
        talk_madam_rsh_01[53] = 0;
        talk_madam_rsh_01[84] = GSW(1706);
        talk_madam_rsh_01[86] = 4;
        talk_madam_rsh_01[94] = 7;
        talk_madam_rsh_01[102] = 8;
        talk_madam_rsh_01[110] = 21;
        talk_madam_rsh_01[118] = 22;
        talk_madam_rsh_01[126] = 29;
        talk_madam_rsh_01[134] = 46;

        evt_daiji_kami_rsh_01[16] = GSW(1706);
        evt_daiji_kami_rsh_01[17] = 25;

        evt_pikary_arrival[126] = GSW(1706);
        evt_pikary_arrival[127] = 37;

        evt_access_rsh_01[4] = GSW(1706);
        evt_access_rsh_01[5] = 49;

        patch::writePatch(&rsh_01_init_evt[0], rsh_01_init_evt_hook1, sizeof(rsh_01_init_evt_hook1));
        patch::writePatch(&rsh_01_init_evt[48], rsh_01_init_evt_hook2, sizeof(rsh_01_init_evt_hook2));
        patch::writePatch(&rsh_01_init_evt[76], rsh_01_init_evt_music_hook, sizeof(rsh_01_init_evt_music_hook));

        rsh_01_a_init_evt[1] = GSW(1706);
        rsh_01_a_init_evt[3] = 36;
        rsh_01_a_init_evt[28] = GSW(1706);
        rsh_01_a_init_evt[29] = 24;
        rsh_01_a_init_evt[75] = GSW(1706);
        rsh_01_a_init_evt[76] = 2;
        rsh_01_a_init_evt[98] = GSW(1706);
        rsh_01_a_init_evt[99] = 2;

        evt_nabe[4] = GSW(1706);
        evt_nabe[5] = 6;
        evt_nabe[11] = GSW(1706);
        evt_nabe[12] = 7;
        evt_nabe[232] = GSW(1706);
        evt_nabe[233] = 7;

        init_gurume[1] = GSW(1706);
        init_gurume[3] = 29;
        init_gurume[4] = 29;
        init_gurume[12] = 31;
        init_gurume[13] = 36;

        talk_gurume[1] = GSW(1706);
        talk_gurume[3] = 4;
        talk_gurume[11] = 5;
        talk_gurume[19] = 7;
        talk_gurume[21] = GSW(1706);
        talk_gurume[22] = 6;
        talk_gurume[47] = 15;
        talk_gurume[72] = 16;
        talk_gurume[105] = 21;
        talk_gurume[113] = 23;
        talk_gurume[121] = 29;
        talk_gurume[143] = 31;
        talk_gurume[151] = 50;

        init_yuurei[1] = GSW(1706);
        init_yuurei[3] = 15;
        init_yuurei[6] = 15;
        init_yuurei[7] = 22;
        init_yuurei[15] = 24;
        init_yuurei[16] = 29;
        init_yuurei[24] = 29;
        init_yuurei[25] = 30;
        init_yuurei[33] = 31;
        init_yuurei[34] = 36;
        init_yuurei[42] = 38;

        talk_yuurei[8] = GSW(1706);
        talk_yuurei[10] = 17;
        talk_yuurei[15] = GSW(1706);
        talk_yuurei[16] = 16;
        talk_yuurei[18] = GSW(1706);
        talk_yuurei[19] = 16;
        talk_yuurei[55] = GSW(1706);
        talk_yuurei[56] = 17;
        talk_yuurei[64] = 19;
        talk_yuurei[72] = 20;
        talk_yuurei[368] = GSW(1706);
        talk_yuurei[369] = 20;
        talk_yuurei[379] = 21;
        talk_yuurei[387] = 22;
        talk_yuurei[395] = 26;
        talk_yuurei[403] = 29;
        talk_yuurei[411] = 30;
        talk_yuurei[419] = 31;
        talk_yuurei[427] = 50;

        init_toron_rsh_02[1] = GSW(1706);
        init_toron_rsh_02[3] = 28;
        init_toron_rsh_02[13] = 29;

        evt_1day_end[18] = GSW(1706);
        evt_1day_end[19] = 22;

        evt_2day_end[18] = GSW(1706);
        evt_2day_end[19] = 31;

        evt_bed[4] = GSW(1706);
        evt_bed[6] = 21;
        evt_bed[10] = 30;

        evt_toron_catch[273] = GSW(1706);
        evt_toron_catch[274] = 29;

        evt_toron_catch2[316] = GSW(1706);
        evt_toron_catch2[317] = 29;

        evt_toron_appear[25] = GSW(1706);
        evt_toron_appear[26] = 27;
        evt_toron_appear[420] = GSW(1706);
        evt_toron_appear[421] = 28;

        evt_daiji_kami_rsh_02[82] = GSW(1706);
        evt_daiji_kami_rsh_02[83] = 27;

        evt_yuka_simi_rsh_02[23] = GSW(1706);
        evt_yuka_simi_rsh_02[24] = 5;
        evt_yuka_simi_rsh_02[38] = GSW(1706);
        evt_yuka_simi_rsh_02[39] = 5;

        evt_access_entry[1] = GSW(1706);
        evt_access_entry[2] = 49;

        patch::writePatch(&rsh_02_init_evt[0], rsh_02_init_evt_hook, sizeof(rsh_02_init_evt_hook));
        rsh_02_init_evt[63] = GSW(1706);
        rsh_02_init_evt[64] = 3;
        rsh_02_init_evt[66] = GSW(1706);
        rsh_02_init_evt[67] = 28;
        patch::writePatch(&rsh_02_init_evt[106], rsh_02_init_evt_music_hook, sizeof(rsh_02_init_evt_music_hook));

        rsh_02_a_init_evt[1] = GSW(1706);
        rsh_02_a_init_evt[2] = 22;
        rsh_02_a_init_evt[8] = GSW(1706);
        rsh_02_a_init_evt[9] = 31;
        rsh_02_a_init_evt[21] = GSW(1706);
        rsh_02_a_init_evt[22] = 2;
        rsh_02_a_init_evt[31] = GSW(1706);
        rsh_02_a_init_evt[33] = 3;
        rsh_02_a_init_evt[34] = 22;
        rsh_02_a_init_evt[48] = GSW(1706);
        rsh_02_a_init_evt[49] = 3;
        rsh_02_a_init_evt[51] = GSW(1706);
        rsh_02_a_init_evt[52] = 7;
        rsh_02_a_init_evt[172] = GSW(1706);
        rsh_02_a_init_evt[174] = 2;
        rsh_02_a_init_evt[176] = GSW(1706);
        rsh_02_a_init_evt[177] = 2;
        rsh_02_a_init_evt[468] = 23;
        rsh_02_a_init_evt[470] = GSW(1706);
        rsh_02_a_init_evt[471] = 23;
        rsh_02_a_init_evt[489] = 32;
        rsh_02_a_init_evt[491] = GSW(1706);
        rsh_02_a_init_evt[492] = 32;
        rsh_02_a_init_evt[578] = 38;
        rsh_02_a_init_evt[580] = GSW(1706);
        rsh_02_a_init_evt[581] = 38;
        rsh_02_a_init_evt[625] = GSW(1706);
        rsh_02_a_init_evt[627] = 26;
        rsh_02_a_init_evt[628] = 27;
        rsh_02_a_init_evt[648] = GSW(1706);
        rsh_02_a_init_evt[649] = 27;

        init_cook[1] = GSW(1706);
        init_cook[3] = 32;
        init_cook[4] = 36;

        talk_cook[210] = GSW(1706);
        talk_cook[212] = 3;
        talk_cook[234] = 4;
        talk_cook[242] = 7;
        talk_cook[250] = 8;
        talk_cook[261] = 14;
        talk_cook[269] = 21;
        talk_cook[277] = 22;
        talk_cook[285] = 29;
        talk_cook[307] = 30;
        talk_cook[315] = 31;
        talk_cook[326] = 50;

        init_waitless_rsh_03[1] = GSW(1706);
        init_waitless_rsh_03[3] = 32;
        init_waitless_rsh_03[4] = 36;

        patch::writePatch(&talk_waitless_pias_return[136], talk_waitless_hook1, sizeof(talk_waitless_hook1));

        talk_waitless_rsh_03[1] = GSW(1706);
        talk_waitless_rsh_03[2] = 8;
        patch::writePatch(&talk_waitless_rsh_03[49], talk_waitless_hook2, sizeof(talk_waitless_hook2));
        talk_waitless_rsh_03[84] = GSW(1706);
        talk_waitless_rsh_03[86] = 3;
        talk_waitless_rsh_03[108] = 4;
        talk_waitless_rsh_03[116] = 7;
        talk_waitless_rsh_03[151] = 8;
        talk_waitless_rsh_03[159] = 21;
        talk_waitless_rsh_03[167] = 22;
        talk_waitless_rsh_03[175] = 29;
        talk_waitless_rsh_03[207] = 30;
        talk_waitless_rsh_03[225] = 31;
        talk_waitless_rsh_03[233] = 50;

        move_waitless[1] = GSW(1706);
        move_waitless[2] = 3;
        move_waitless[4] = GSW(1706);
        move_waitless[5] = 7;

        talk_toron_rsh_03[1] = GSW(1706);
        talk_toron_rsh_03[3] = 4;

        talk_sarary_rsh_03[1] = GSW(1706);
        talk_sarary_rsh_03[3] = 4;

        evt_lanch_missing[731] = GSW(1706);
        evt_lanch_missing[732] = 4;

        talk_powan_rsh_03[1] = GSW(1706);
        talk_powan_rsh_03[3] = 4;

        evt_shop_moamoa_talk[1] = GSW(1706);
        evt_shop_moamoa_talk[3] = 32;
        evt_shop_moamoa_talk[8] = 33;
        evt_shop_moamoa_talk[19] = 36;

        evt_shop_close[253] = GSW(1706);
        evt_shop_close[255] = 3;
        evt_shop_close[256] = 22;

        evt_shop_door_init[7] = GSW(1706);
        evt_shop_door_init[9] = 21;
        evt_shop_door_init[11] = 30;

        evt_yuka_simi_rsh_03[23] = GSW(1706);
        evt_yuka_simi_rsh_03[24] = 5;
        evt_yuka_simi_rsh_03[38] = GSW(1706);
        evt_yuka_simi_rsh_03[39] = 5;

        evt_access_rsh_03[4] = GSW(1706);
        evt_access_rsh_03[5] = 49;

        patch::writePatch(&rsh_03_init_evt[0], rsh_03_init_evt_hook, sizeof(rsh_03_init_evt_hook));
        rsh_03_init_evt[35] = GSW(1706);
        rsh_03_init_evt[37] = 3;
        rsh_03_init_evt[38] = 6;
        rsh_03_init_evt[101] = 3;
        rsh_03_init_evt[106] = 7;
        rsh_03_init_evt[107] = 40;
        rsh_03_init_evt[124] = GSW(1706);
        rsh_03_init_evt[125] = 32;
        rsh_03_init_evt[127] = GSW(1706);
        rsh_03_init_evt[128] = 36;
        patch::writePatch(&rsh_03_init_evt[155], rsh_03_init_evt_music_hook, sizeof(rsh_03_init_evt_music_hook));

        rsh_03_a_init_evt[12] = GSW(1706);
        rsh_03_a_init_evt[14] = 3;
        rsh_03_a_init_evt[15] = 22;
        rsh_03_a_init_evt[29] = GSW(1706);
        rsh_03_a_init_evt[30] = 3;
        rsh_03_a_init_evt[32] = GSW(1706);
        rsh_03_a_init_evt[33] = 7;

        rsh_03_c_init_evt[91] = GSW(1706);
        rsh_03_c_init_evt[93] = 21;
        rsh_03_c_init_evt[95] = 30;

        evt_rsh_konarikin_irai_party_talk[81] = GSW(1706);
        evt_rsh_konarikin_irai_party_talk[82] = 12;
        evt_rsh_konarikin_irai_party_talk[84] = GSW(1706);
        evt_rsh_konarikin_irai_party_talk[85] = 12;

        evt_allow_kamotsu[68] = GSW(1706);
        evt_allow_kamotsu[69] = 18;

        evt_shorui_funshitu[358] = GSW(1706);
        evt_shorui_funshitu[359] = 24;

        evt_daijisouna_kami[204] = GSW(1706);
        evt_daijisouna_kami[205] = 26;

        init_syashou_rsh_04[1] = GSW(1706);
        init_syashou_rsh_04[3] = 18;
        init_syashou_rsh_04[4] = 19;
        init_syashou_rsh_04[12] = 32;
        init_syashou_rsh_04[13] = 36;

        talk_syashou_rsh_04[1] = GSW(1706);
        talk_syashou_rsh_04[3] = 14;
        talk_syashou_rsh_04[4] = 20;
        talk_syashou_rsh_04[10] = GSW(1706);
        talk_syashou_rsh_04[12] = 3;
        talk_syashou_rsh_04[61] = GSW(1706);
        talk_syashou_rsh_04[62] = 3;
        talk_syashou_rsh_04[64] = 8;
        talk_syashou_rsh_04[89] = 11;
        talk_syashou_rsh_04[97] = 14;
        talk_syashou_rsh_04[113] = GSW(1706);
        talk_syashou_rsh_04[114] = 12;
        talk_syashou_rsh_04[116] = GSW(1706);
        talk_syashou_rsh_04[117] = 12;
        talk_syashou_rsh_04[121] = 15;
        talk_syashou_rsh_04[154] = GSW(1706);
        talk_syashou_rsh_04[155] = 15;
        talk_syashou_rsh_04[163] = 17;
        talk_syashou_rsh_04[171] = 18;
        talk_syashou_rsh_04[175] = 19;
        talk_syashou_rsh_04[183] = 20;
        talk_syashou_rsh_04[191] = 21;
        talk_syashou_rsh_04[282] = GSW(1706);
        talk_syashou_rsh_04[283] = 21;
        talk_syashou_rsh_04[290] = 22;
        talk_syashou_rsh_04[298] = 29;
        talk_syashou_rsh_04[323] = 30;
        talk_syashou_rsh_04[331] = 31;
        talk_syashou_rsh_04[342] = 40;
        talk_syashou_rsh_04[402] = GSW(1706);
        talk_syashou_rsh_04[404] = 14;
        talk_syashou_rsh_04[405] = 21;

        init_sarary_rsh_04[1] = GSW(1706);
        init_sarary_rsh_04[3] = 3;
        init_sarary_rsh_04[4] = 6;
        init_sarary_rsh_04[12] = 23;
        init_sarary_rsh_04[13] = 29;
        init_sarary_rsh_04[21] = 32;
        init_sarary_rsh_04[22] = 36;
        init_sarary_rsh_04[30] = 38;

        talk_sarary_rsh_04[1] = GSW(1706);
        talk_sarary_rsh_04[3] = 7;
        talk_sarary_rsh_04[11] = 14;
        talk_sarary_rsh_04[19] = 21;
        talk_sarary_rsh_04[38] = 22;
        talk_sarary_rsh_04[46] = 29;
        talk_sarary_rsh_04[54] = 30;
        talk_sarary_rsh_04[62] = 31;

        talk_papa_mama[130] = GSW(1706);
        talk_papa_mama[131] = 11;
        talk_papa_mama[138] = GSW(1706);
        talk_papa_mama[139] = 9;
        talk_papa_mama[141] = GSW(1706);
        talk_papa_mama[142] = 10;

        patch::writePatch(&init_papa_rsh_04[0], init_papa_hook, sizeof(init_papa_hook));

        talk_papa_rsh_04[1] = GSW(1706);
        talk_papa_rsh_04[3] = 8;
        talk_papa_rsh_04[11] = 9;
        talk_papa_rsh_04[15] = 21;
        talk_papa_rsh_04[23] = 22;
        talk_papa_rsh_04[31] = 29;
        talk_papa_rsh_04[39] = 31;

        init_mama_rsh_04[1] = GSW(1706);
        init_mama_rsh_04[3] = 32;
        init_mama_rsh_04[4] = 36;
        init_mama_rsh_04[12] = 38;

        talk_mama_rsh_04[1] = GSW(1706);
        talk_mama_rsh_04[3] = 8;
        talk_mama_rsh_04[11] = 9;
        talk_mama_rsh_04[15] = 21;
        talk_mama_rsh_04[23] = 22;
        talk_mama_rsh_04[31] = 29;
        talk_mama_rsh_04[39] = 30;
        talk_mama_rsh_04[47] = 31;

        patch::writePatch(&init_konari_rsh_04[0], init_konari_hook, sizeof(init_konari_hook));

        talk_konari_rsh_04[1] = GSW(1706);
        talk_konari_rsh_04[3] = 8;
        talk_konari_rsh_04[11] = 9;
        talk_konari_rsh_04[19] = 11;
        talk_konari_rsh_04[24] = GSW(1706);
        talk_konari_rsh_04[25] = 10;
        talk_konari_rsh_04[27] = GSW(1706);
        talk_konari_rsh_04[28] = 10;
        talk_konari_rsh_04[55] = GSW(1706);
        talk_konari_rsh_04[56] = 11;
        talk_konari_rsh_04[68] = 14;
        talk_konari_rsh_04[102] = GSW(1706);
        talk_konari_rsh_04[103] = 12;
        talk_konari_rsh_04[105] = GSW(1706);
        talk_konari_rsh_04[106] = 12;
        talk_konari_rsh_04[222] = GSW(1706);
        talk_konari_rsh_04[223] = 14;
        talk_konari_rsh_04[231] = 21;
        talk_konari_rsh_04[239] = 22;
        talk_konari_rsh_04[255] = 31;

        patch::writePatch(&init_powan_rsh_04[0], init_powan_hook, sizeof(init_powan_hook));

        talk_powan_rsh_04[1] = GSW(1706);
        talk_powan_rsh_04[3] = 7;
        talk_powan_rsh_04[11] = 8;
        talk_powan_rsh_04[109] = GSW(1706);
        talk_powan_rsh_04[110] = 8;
        talk_powan_rsh_04[116] = 11;
        talk_powan_rsh_04[124] = 14;
        talk_powan_rsh_04[135] = 15;
        talk_powan_rsh_04[143] = 17;
        talk_powan_rsh_04[151] = 21;
        talk_powan_rsh_04[162] = 22;
        talk_powan_rsh_04[170] = 24;
        talk_powan_rsh_04[174] = 25;
        talk_powan_rsh_04[182] = 26;
        talk_powan_rsh_04[186] = 29;
        talk_powan_rsh_04[194] = 31;

        evt_tana_5[4] = GSW(1706);
        evt_tana_5[5] = 49;

        evt_access_rsh_04[7] = GSW(1706);
        evt_access_rsh_04[8] = 49;

        evt_narikin_room[23] = GSW(1706);
        evt_narikin_room[25] = 8;
        evt_narikin_room[26] = 20;
        evt_narikin_room[65] = GSW(1706);
        evt_narikin_room[67] = 8;
        evt_narikin_room[68] = 20;

        evt_powan_room[21] = GSW(1706);
        evt_powan_room[23] = 23;
        evt_powan_room[62] = GSW(1706);
        evt_powan_room[64] = 23;

        patch::writePatch(&rsh_04_init_evt[0], rsh_04_init_evt_hook, sizeof(rsh_04_init_evt_hook));
        rsh_04_init_evt[55] = GSW(1706);
        rsh_04_init_evt[56] = 18;
        rsh_04_init_evt[64] = GSW(1706);
        rsh_04_init_evt[65] = 20;
        rsh_04_init_evt[73] = GSW(1706);
        rsh_04_init_evt[74] = 32;
        rsh_04_init_evt[82] = GSW(1706);
        rsh_04_init_evt[83] = 38;
        rsh_04_init_evt[85] = GSW(1706);
        rsh_04_init_evt[86] = 44;
        patch::writePatch(&rsh_04_init_evt[100], rsh_04_init_evt_music_hook, sizeof(rsh_04_init_evt_music_hook));

        rsh_04_a_init_evt[12] = GSW(1706);
        rsh_04_a_init_evt[13] = 32;

        rsh_04_c_init_evt[53] = GSW(1706);
        rsh_04_c_init_evt[54] = 19;

        rsh_evt_meet_moamoa[181] = GSW(1706);
        rsh_evt_meet_moamoa[182] = 34;

        rsh_evt_moamoa_escape[219] = GSW(1706);
        rsh_evt_moamoa_escape[220] = 35;

        init_syashou_rsh_05[1] = GSW(1706);
        init_syashou_rsh_05[3] = 33;
        init_syashou_rsh_05[4] = 36;

        talk_syashou_rsh_05[1] = GSW(1706);
        talk_syashou_rsh_05[2] = 35;

        init_waitless_rsh_05[1] = GSW(1706);
        init_waitless_rsh_05[3] = 33;
        init_waitless_rsh_05[4] = 36;

        talk_waitless_rsh_05[1] = GSW(1706);
        talk_waitless_rsh_05[2] = 35;

        evt_ghost_diary[1] = GSW(1706);
        evt_ghost_diary[2] = 19;

        rsh_05_init_evt[1] = GSW(1720);
        rsh_05_init_evt[3] = 1;
        rsh_05_init_evt[4] = 8;
        patch::writePatch(&rsh_05_init_evt[14], rsh_06_init_evt_hook, sizeof(rsh_06_init_evt_hook));

        rsh_05_a_init_evt[1] = GSW(1706);
        rsh_05_a_init_evt[3] = 8;
        rsh_05_a_init_evt[8] = 14;
        rsh_05_a_init_evt[13] = 23;
        rsh_05_a_init_evt[18] = 29;
        rsh_05_a_init_evt[23] = 30;
        rsh_05_a_init_evt[28] = 32;
        rsh_05_a_init_evt[47] = GSW(1706);
        rsh_05_a_init_evt[49] = 33;
        rsh_05_a_init_evt[56] = 33;
        rsh_05_a_init_evt[67] = 34;
        rsh_05_a_init_evt[88] = GSW(1706);
        rsh_05_a_init_evt[89] = 19;

        hom_10_evt_resha_start_rsh_06[49] = GSW(1706);
        hom_10_evt_resha_start_rsh_06[50] = 43;

        evt_great_moamoa[729] = GSW(1706);
        evt_great_moamoa[730] = 36;

        patch::writePatch(&rsh_06_init_evt[2], rsh_06_init_evt_hook, sizeof(rsh_06_init_evt_hook));

        rsh_06_a_init_evt[25] = GSW(1706);
        rsh_06_a_init_evt[26] = 36;

        rsh_prolog[14] = 0x386006AA; // li r3, 0x6AA (GSW(1706))
        rsh_prolog[16] = 0x2C03002B; // cmpwi r3, 0x2B
    }

    void exit() {}
} // namespace mod
