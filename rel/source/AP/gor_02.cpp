#include "evt_cmd.h"
#include "patch.h"
#include <AP/rel_patch_definitions.h>
#include <ttyd/evt_cam.h>
#include <ttyd/evt_map.h>
#include <ttyd/evt_mario.h>
#include <ttyd/evt_msg.h>
#include <ttyd/evt_npc.h>
#include <ttyd/evt_snd.h>

using namespace mod;
using namespace ttyd;

extern int32_t unk_evt_gor_0003ec8c[];
extern int32_t gor_02_event_00[];
extern int32_t kurihakase_init[];
extern int32_t kurihakase_talk[];
extern int32_t oyabun_talk[];
extern int32_t tozokudan1_talk[];
extern int32_t tozokudan2_talk[];
extern int32_t sanders_init_02[];
extern int32_t sanders_talk_02[];
extern int32_t kuribo4_talk[];
extern int32_t chusan3_talk[];
extern int32_t roten3_talk[];
extern int32_t borodo1_init[];
extern int32_t borodo1_regl[];
extern int32_t borodo1_talk[];
extern int32_t borodo2_talk[];
extern int32_t kuragarisan_write_name[];
extern int32_t kuragarisan_talk[];
extern int32_t luigi_init_02[];
extern int32_t evt_door_02_open[];
extern int32_t evt_door_02_close[];
extern int32_t evt_door_03_close[];
extern int32_t steel_coin[];
extern int32_t kurihakase_first_talk[];
extern int32_t kurihakase_wall_remove[];
extern int32_t kurihakase_lecture[];
extern int32_t kurihakase_lecture_badge[];
extern int32_t epigraphy_map[];
extern int32_t epigraphy_map_after_stage1[];
extern int32_t epigraphy_map_after_stage2[];
extern int32_t epigraphy_map_after_stage3[];
extern int32_t epigraphy_map_after_stage4[];
extern int32_t epigraphy_map_after_stage5[];
extern int32_t epigraphy_map_after_stage6[];
extern int32_t kurihakase_after3minutes[];
extern int32_t gor_02_init_evt[];

EVT_BEGIN(kuribo4_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_030_02"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(chusan3_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_031_03"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(roten3_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_032_00"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(borodo1_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_033_00"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(borodo2_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_033_08_2"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(gor_02_init_evt1)
	SET(LW(0), 0)
	IF_EQUAL(GSW(1700), 15)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1711), 16)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1713), 19)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1703), 30)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1715), 16)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1717), 28)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1706), 51)
		ADD(LW(0), 1)
	END_IF()
	IF_LARGE_EQUAL(LW(0), 1)
		USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_FRANKURI1"))
		USER_FUNC(evt_snd::evt_snd_envoff, 512)
	ELSE()
		IF_NOT_EQUAL(GSW(1708), 18)
			USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG0_GOR1"))
			USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG0_GOR3"))
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(gor_02_init_evt1_hook)
	RUN_CHILD_EVT(gor_02_init_evt1)
	GOTO(&gor_02_init_evt[38])
EVT_PATCH_END()

EVT_BEGIN(gor_02_init_evt2)
	IF_EQUAL(GSW(1700), 15)
		RUN_EVT(epigraphy_map)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1711), 16)
		RUN_EVT(epigraphy_map_after_stage1)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1713), 19)
		RUN_EVT(epigraphy_map_after_stage2)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1703), 30)
		RUN_EVT(epigraphy_map_after_stage3)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1715), 16)
		RUN_EVT(epigraphy_map_after_stage4)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1717), 28)
		RUN_EVT(epigraphy_map_after_stage6)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1706), 51)
		RUN_EVT(epigraphy_map_after_stage6)
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(gor_02_init_evt2_hook)
	RUN_CHILD_EVT(gor_02_init_evt2)
	GOTO(&gor_02_init_evt[437])
EVT_PATCH_END()

EVT_BEGIN(evt_door_02_open_evt)
	SET(LW(0), 0)
	IF_EQUAL(GSW(1700), 15)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1711), 16)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1713), 19)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1703), 30)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1715), 16)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1717), 28)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1706), 51)
		ADD(LW(0), 1)
	END_IF()
	IF_LARGE_EQUAL(LW(0), 1)
		USER_FUNC(evt_snd::evt_snd_envoff, 18432)
		USER_FUNC(evt_snd::evt_snd_env_lpf, 0, 800)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1706), 52)
		USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_FRANKURI1"))
		USER_FUNC(evt_snd::evt_snd_envoff, 18432)
		USER_FUNC(evt_snd::evt_snd_env_lpf, 0, 800)
		RETURN()
	END_IF()
	IF_NOT_EQUAL(GSW(1708), 18)
		USER_FUNC(evt_snd::evt_snd_bgmoff, 18432)
		USER_FUNC(evt_snd::evt_snd_envoff, 18432)
		USER_FUNC(evt_snd::evt_snd_env_lpf, 0, 800)
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(evt_door_02_open_hook)
	RUN_CHILD_EVT(evt_door_02_open_evt)
	GOTO(&evt_door_02_open[55])
EVT_PATCH_END()

EVT_BEGIN(evt_door_02_open_evt2)
	IF_LARGE_EQUAL(GSW(1711), 8)
		IF_SMALL_EQUAL(GSW(1718), 5)
			USER_FUNC(evt_map::evt_mapobj_trans, 1, PTR("S_open_book"), -15, 0, 0)
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1703), 20)
		IF_SMALL_EQUAL(GSW(1704), 2)
			USER_FUNC(evt_map::evt_mapobj_trans, 1, PTR("S_open_book"), -15, 0, 0)
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1706), 43)
		IF_SMALL_EQUAL(GSW(1707), 1)
			USER_FUNC(evt_map::evt_mapobj_trans, 1, PTR("S_open_book"), -15, 0, 0)
			RETURN()
		END_IF()
	END_IF()
EVT_END()

EVT_BEGIN(evt_door_02_open_hook2)
	RUN_CHILD_EVT(evt_door_02_open_evt2)
	GOTO(&evt_door_02_open[101])
EVT_PATCH_END()

EVT_BEGIN(kurihakase_talk_evt)
	IF_LARGE_EQUAL(GSW(1708), 19)
		IF_EQUAL(GSWF(1249), 0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_029"), 0, PTR("me"))
			USER_FUNC(evt_mario::evt_mario_set_pose, PTR("M_I_2"))
			WAIT_MSEC(1000)
			USER_FUNC(evt_mario::evt_mario_set_pose, PTR("M_S_1"))
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_029_01"), 0, PTR("me"))
			USER_FUNC(evt_mario::evt_mario_set_pose, PTR("M_I_N"))
			WAIT_MSEC(500)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_029_02"), 0, PTR("me"))
			USER_FUNC(evt_mario::evt_mario_set_pose, PTR("M_S_1"))
			SET(GSW(1249), 1)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_029_03"), 0, PTR("me"))
		END_IF()
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1700), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_0_085"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1701), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_013"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1702), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_015"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1703), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_017"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1704), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_020"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1705), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_022"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1705), 2)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_4_048"), 0, PTR("me"))
		SET(GSWF(1214), 1)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1705), 3)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_4_060"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1706), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_024"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 1)
		USER_FUNC(evt_cam::evt_cam_letter_box_disable, 1)
		USER_FUNC(evt_cam::evt_cam_letter_box_onoff, 1, 0)
		WAIT_MSEC(1000)
		RUN_CHILD_EVT(kurihakase_after3minutes)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_028"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(kurihakase_init_evt)
	SWITCH(GSW(1700))
		CASE_EQUAL(8)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -135, 0, -240)
			RETURN()
		CASE_END()
		CASE_EQUAL(9)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
			RETURN()
		CASE_END()
		CASE_EQUAL(10)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -220, 0, 305)
			RETURN()
		CASE_END()
		CASE_EQUAL(11)
			IF_EQUAL(GSWF(1207), 0)
				USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -220, 0, 305)
				RETURN()
			ELSE()
				USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
				RETURN()
			END_IF()
		CASE_END()
		CASE_BETWEEN(11, 17)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
			RETURN()
		CASE_END()
		CASE_EQUAL(18)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -140, 0, -225)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_EQUAL(GSW(1711), 16)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -140, 0, -225)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1713), 19)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -140, 0, -225)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1703), 30)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -140, 0, -225)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1715), 16)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -140, 0, -225)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1717), 28)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -140, 0, -225)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1706), 51)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -140, 0, -225)
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1708), 2)
		IF_SMALL_EQUAL(GSW(1708), 18)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -140, 0, -250)
	RETURN()
EVT_END()

EVT_BEGIN(kurihakase_init_hook)
	RUN_CHILD_EVT(kurihakase_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(tozokudan1_talk_evt)
	USER_FUNC(checkChapterClears, LW(0))
	SWITCH(LW(0))
		CASE_EQUAL(0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_010"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(1)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_010_01"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_010_02"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(3)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_010_03"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(4)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_010_04"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(5)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_010_05"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(6)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_010_06"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_010_07"), 0, PTR("me"))
		CASE_END()
		CASE_ETC()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_010_08"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(tozokudan1_talk_hook)
	RUN_CHILD_EVT(tozokudan1_talk_evt)
	RETURN()
EVT_PATCH_END()

EVT_BEGIN(tozokudan2_talk_evt)
	USER_FUNC(checkChapterClears, LW(0))
	SWITCH(LW(0))
		CASE_EQUAL(0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_011"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(1)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_011_01"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_011_02"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(3)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_011_03"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(4)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_011_04"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(5)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_011_05"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(6)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_011_06"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_011_07"), 0, PTR("me"))
		CASE_END()
		CASE_ETC()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_02_011_08"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(tozokudan2_talk_hook)
	RUN_CHILD_EVT(tozokudan2_talk_evt)
	RETURN()
EVT_PATCH_END()

EVT_BEGIN(luigi_init_evt)
	USER_FUNC(checkChapterClears, LW(0))
	IF_EQUAL(LW(0), 1)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -400, 0, 0)
		USER_FUNC(evt_npc::evt_npc_set_ry_lr, PTR("me"), 1)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(luigi_init_hook)
	RUN_CHILD_EVT(luigi_init_evt)
	RETURN()
EVT_END()

void ApplyGor02Patches()
{
    unk_evt_gor_0003ec8c[26] = GSW(1700);

    gor_02_event_00[1] = GSW(1700);

    patch::writePatch(&kurihakase_init[0], kurihakase_init_hook, sizeof(kurihakase_init_hook));

    patch::writePatch(&kurihakase_talk[0], kurihakase_talk_evt, sizeof(kurihakase_talk_evt));

    oyabun_talk[0] = EVT_HELPER_CMD(1, 4);
    oyabun_talk[1] = EVT_HELPER_OP(&oyabun_talk[26]);
    oyabun_talk[297] = EVT_HELPER_CMD(0, 2);
    oyabun_talk[298] = EVT_HELPER_CMD(0, 1);

    patch::writePatch(&tozokudan1_talk[208], tozokudan1_talk_hook, sizeof(tozokudan1_talk_hook));

    patch::writePatch(&tozokudan2_talk[53], tozokudan2_talk_hook, sizeof(tozokudan2_talk_hook));

    sanders_init_02[6] = GSW(1705); // Bobbery Init
    sanders_init_02[8] = 2;
    sanders_init_02[16] = 5;

    sanders_talk_02[1] = GSW(1705); // Bobbery Speach
    sanders_talk_02[3] = 2;
    sanders_talk_02[13] = 2;
    sanders_talk_02[77] = GSW(1705);
    sanders_talk_02[78] = 3;
    sanders_talk_02[81] = 3;
    sanders_talk_02[89] = 4;
    sanders_talk_02[569] = GSW(1705);
    sanders_talk_02[570] = 5;

    patch::writePatch(&kuribo4_talk[184], kuribo4_talk_evt, sizeof(kuribo4_talk_evt));
    patch::writePatch(&chusan3_talk[0], chusan3_talk_evt, sizeof(chusan3_talk_evt));
    patch::writePatch(&roten3_talk[0], roten3_talk_evt, sizeof(roten3_talk_evt));

    borodo1_talk[6] = GSW(1705); // HOOK
    borodo1_talk[8] = 1;
    borodo1_talk[9] = 99;

    borodo1_regl[1] = GSW(1705); // HOOK
    borodo1_regl[3] = 1;
    borodo1_regl[4] = 99;

    patch::writePatch(&borodo1_talk[0], borodo1_talk_evt, sizeof(borodo1_talk_evt));

    patch::writePatch(&borodo2_talk[24], borodo2_talk_evt, sizeof(borodo2_talk_evt));

    kuragarisan_write_name[428] = GSW(1704);
    kuragarisan_write_name[429] = 1;

    kuragarisan_talk[1] = GSW(1704);
    kuragarisan_talk[3] = 0;
    kuragarisan_talk[17] = EVT_HELPER_CMD(0, 42);
    kuragarisan_talk[18] = EVT_HELPER_CMD(0, 0);
    kuragarisan_talk[25] = EVT_HELPER_CMD(0, 49);
    kuragarisan_talk[26] = EVT_HELPER_CMD(0, 2);
    kuragarisan_talk[27] = EVT_HELPER_CMD(0, 1);

    patch::writePatch(&luigi_init_02[0], luigi_init_hook, sizeof(luigi_init_hook));

    steel_coin[382] = GSW(1700);

    kurihakase_first_talk[780] = GSW(1700);

    kurihakase_wall_remove[251] = GSW(1700);

    kurihakase_lecture[272] = GSW(1700);

    kurihakase_lecture_badge[275] = GSW(1700);
    kurihakase_lecture_badge[276] = 17;

    epigraphy_map[617] = GSW(1700);
    epigraphy_map[618] = 16;
    epigraphy_map_after_stage1[254] = GSW(1711);
    epigraphy_map_after_stage1[255] = 17;
    epigraphy_map_after_stage2[396] = GSW(1713);
    epigraphy_map_after_stage2[397] = 20;
    epigraphy_map_after_stage3[364] = GSW(1703);
    epigraphy_map_after_stage3[365] = 31;
    epigraphy_map_after_stage4[293] = GSW(1715);
    epigraphy_map_after_stage4[294] = 17;
    epigraphy_map_after_stage5[221] = GSW(1717);
    epigraphy_map_after_stage5[222] = 29;
    epigraphy_map_after_stage6[226] = GSW(1706);
    epigraphy_map_after_stage6[227] = 52;

    kurihakase_after3minutes[136] = GSW(1706);
    kurihakase_after3minutes[137] = 53;

    patch::writePatch(&gor_02_init_evt[0], gor_02_init_evt1_hook, sizeof(gor_02_init_evt1_hook));
    gor_02_init_evt[46] = GSW(1717);
    gor_02_init_evt[47] = 25;
    gor_02_init_evt[59] = GSW(1700);
    gor_02_init_evt[201] = GSW(1701);
    gor_02_init_evt[202] = 1;
    gor_02_init_evt[360] = GSW(1700);
    gor_02_init_evt[375] = GSW(1700);
    gor_02_init_evt[378] = GSW(1700);
    gor_02_init_evt[401] = GSW(1700);
    patch::writePatch(&gor_02_init_evt[406], gor_02_init_evt2_hook, sizeof(gor_02_init_evt2_hook));
    gor_02_init_evt[531] = GSW(1708);
    gor_02_init_evt[532] = 18;

    evt_door_03_close[15] = GSW(1705);
    evt_door_03_close[16] = 3;
    evt_door_03_close[86] = GSW(1705);
    evt_door_03_close[87] = 5;
    evt_door_03_close[159] = GSW(1705);
    evt_door_03_close[160] = 6;

    evt_door_02_close[3] = GSW(1706);
    evt_door_02_close[5] = 51;
    evt_door_02_close[7] = 52;
    evt_door_02_close[43] = GSW(1700);
    evt_door_02_close[51] = GSW(1700);
    evt_door_02_close[52] = 16;

    patch::writePatch(&evt_door_02_open[2], evt_door_02_open_hook, sizeof(evt_door_02_open_hook));
    evt_door_02_open[56] = GSW(1700);
    patch::writeEvtBranch(&evt_door_02_open[68], evt_door_02_open_hook2, sizeof(evt_door_02_open_hook2));
    evt_door_02_open[102] = GSW(1700);
    evt_door_02_open[103] = 14;
}
