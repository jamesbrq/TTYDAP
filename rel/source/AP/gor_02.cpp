#include <AP/gor_01.h>
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
	IF_EQUAL(GSW(1700), 18)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1701), 99) //Unknown
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1702), 99) //Unknown
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1703), 99) //Unknown
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1704), 99) //Unknown
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1705), 99) //Unknown
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1706), 99) //Unknown
		ADD(LW(0), 1)
	END_IF()
	IF_LARGE_EQUAL(LW(0), 1)
		USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_FRANKURI1"))
		USER_FUNC(evt_snd::evt_snd_envoff, 512)
	ELSE()
		IF_NOT_EQUAL(GSW(1708), 99) //Unknown
			USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG0_GOR1"))
			USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG0_GOR3"))
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(evt_door_02_open_evt)
	SET(LW(0), 0)
	IF_EQUAL(GSW(1700), 18)
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1701), 99) //Unknown
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1702), 99) //Unknown
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1703), 99) //Unknown
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1704), 99) //Unknown
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1705), 99) //Unknown
		ADD(LW(0), 1)
	END_IF()
	IF_EQUAL(GSW(1706), 99) //Unknown
		ADD(LW(0), 1)
	END_IF()
	IF_LARGE_EQUAL(LW(0), 1)
		USER_FUNC(evt_snd::evt_snd_envoff, 18432)
		USER_FUNC(evt_snd::evt_snd_env_lpf, 0, 800)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1706), 99) //Unknown
		USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_FRANKURI1"))
		USER_FUNC(evt_snd::evt_snd_envoff, 18432)
		USER_FUNC(evt_snd::evt_snd_env_lpf, 0, 800)
		RETURN()
	END_IF()
	IF_NOT_EQUAL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_snd::evt_snd_bgmoff, 18432)
		USER_FUNC(evt_snd::evt_snd_envoff, 18432)
		USER_FUNC(evt_snd::evt_snd_env_lpf, 0, 800)
		RETURN()
	END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(gor_02_init_hook)
	RUN_CHILD_EVT(gor_02_init_evt1)
	GOTO(&gor_02_init_evt[38])
EVT_END()

EVT_BEGIN(evt_door_02_open_hook)
	RUN_CHILD_EVT(evt_door_02_open_evt)
	GOTO(&evt_door_02_open[55])
EVT_END()


void ApplyGor02Patches(OSModuleInfo* module_info)
{
	unk_evt_gor_0003ec8c[26] = GSW(1700); //???

	gor_02_event_00[1] = GSW(1700); //Thief thing

	kurihakase_init[6] = GSW(1700); //Frankley Init
	kurihakase_init[59] = EVT_HELPER_CMD(0, 49);
	kurihakase_init[60] = EVT_HELPER_CMD(2, 29);
	kurihakase_init[61] = GSW(1708);
	kurihakase_init[62] = 99; //Unknown
	kurihakase_init[60] = EVT_HELPER_CMD(2, 28);
	kurihakase_init[61] = GSW(1708);
	kurihakase_init[62] = 99; //Unknown
	kurihakase_init[63] = EVT_HELPER_CMD(5, 91);
	kurihakase_init[64] = PTR(evt_npc::evt_npc_set_position);
	kurihakase_init[65] = PTR("me");
	kurihakase_init[66] = 0;
	kurihakase_init[67] = -1000;
	kurihakase_init[68] = 0;
	kurihakase_init[69] = EVT_HELPER_CMD(0, 2);
	kurihakase_init[70] = EVT_HELPER_CMD(0, 33);
	kurihakase_init[71] = EVT_HELPER_CMD(0, 33);
	kurihakase_init[72] = PTR(evt_npc::evt_npc_set_position);
	kurihakase_init[73] = PTR("me");
	kurihakase_init[74] = -140;
	kurihakase_init[75] = 0;
	kurihakase_init[76] = -225;
	kurihakase_init[77] = EVT_HELPER_CMD(0, 2);
	kurihakase_init[77] = EVT_HELPER_CMD(0, 1);

	//kurihakase_talk[0] = 0; TODO

	oyabun_talk[0] = EVT_HELPER_CMD(1, 4); //Pianta hint guy
	oyabun_talk[1] = EVT_HELPER_OP(&oyabun_talk[26]);
	oyabun_talk[297] = EVT_HELPER_CMD(0, 2);
	oyabun_talk[297] = EVT_HELPER_CMD(0, 1);

	tozokudan1_talk[209] = GSW(1710); //Random npc speach
	tozokudan1_talk[211] = 1;
	tozokudan1_talk[219] = 2;
	tozokudan1_talk[227] = 3;
	tozokudan1_talk[235] = 4;
	tozokudan1_talk[243] = 5;
	tozokudan1_talk[251] = 6;
	tozokudan1_talk[259] = 7;
	tozokudan1_talk[267] = 8;

	tozokudan2_talk[54] = GSW(1710); //Random npc 2 speach
	tozokudan2_talk[56] = 1;
	tozokudan2_talk[64] = 3;
	tozokudan2_talk[72] = 4;
	tozokudan2_talk[80] = 5;
	tozokudan2_talk[88] = 6;
	tozokudan2_talk[96] = 7;
	tozokudan2_talk[104] = 8;

	sanders_init_02[6] = GSW(1705); //Bobbery Init
	sanders_init_02[8] = 2;
	sanders_init_02[16] = 5;

	sanders_talk_02[1] = GSW(1705); //Bobbery Speach
	sanders_talk_02[3] = 2;
	sanders_talk_02[13] = 2;
	sanders_talk_02[77] = GSW(1705);
	sanders_talk_02[78] = 3;
	sanders_talk_02[81] = 3;
	sanders_talk_02[89] = 4;
	sanders_talk_02[569] = GSW(1705);
	sanders_talk_02[570] = 5;

	patch::writePatch(&kuribo4_talk[184], kuribo4_talk_evt, sizeof(kuribo4_talk_evt)); //Random NPC Speach
	patch::writePatch(&chusan3_talk[0], chusan3_talk_evt, sizeof(chusan3_talk_evt)); //Random NPC 2 Speach
	patch::writePatch(&roten3_talk[0], roten3_talk_evt, sizeof(roten3_talk_evt)); //Random NPC 3 Speach

	borodo1_talk[6] = GSW(1705); //Random NPC Init
	borodo1_talk[8] = 1;
	borodo1_talk[9] = 99; //Unknown

	borodo1_regl[1] = GSW(1705); //Random NPC Movement
	borodo1_regl[3] = 1;
	borodo1_regl[4] = 99; //Unknown

	patch::writePatch(&borodo1_talk[0], borodo1_talk_evt, sizeof(borodo1_talk_evt)); //Random NPC 4 Speach
	patch::writePatch(&borodo2_talk[24], borodo2_talk_evt, sizeof(borodo2_talk_evt)); //Random NPC 5 Speach

	kuragarisan_write_name[428] = GSW(1704); //Darkly write name
	kuragarisan_write_name[429] = 1;

	kuragarisan_talk[1] = GSW(1704); //Darkly Speach
	kuragarisan_talk[3] = 0;
	kuragarisan_talk[17] = EVT_HELPER_CMD(0, 42);
	kuragarisan_talk[18] = EVT_HELPER_CMD(0, 0);
	kuragarisan_talk[26] = EVT_HELPER_CMD(0, 49);
	kuragarisan_talk[27] = EVT_HELPER_CMD(0, 2);
	kuragarisan_talk[28] = EVT_HELPER_CMD(0, 1);

	luigi_init_02[1] = GSW(1710); //Luigi Init
	luigi_init_02[2] = EVT_HELPER_CMD(1, 36);
	luigi_init_02[3] = 1;
	luigi_init_02[4] = EVT_HELPER_CMD(0, 0);

	steel_coin[382] = GSW(1700); //Thief steals coins

	kurihakase_first_talk[780] = GSW(1700); //Frankley prologue
	kurihakase_wall_remove[251] = GSW(1700); //Frankley break fence
	kurihakase_lecture[272] = GSW(1700); //Frankley battle tutorial
	kurihakase_lecture_badge[275] = GSW(1700); //Frankley badge tutorial

	epigraphy_map[617] = GSW(1700); //Map
	epigraphy_map_after_stage1[254] = GSW(1701);
	epigraphy_map_after_stage1[255] = 99;
	epigraphy_map_after_stage2[396] = GSW(1702);
	epigraphy_map_after_stage2[397] = 99;
	epigraphy_map_after_stage3[364] = GSW(1703);
	epigraphy_map_after_stage3[365] = 99;
	epigraphy_map_after_stage4[293] = GSW(1704);
	epigraphy_map_after_stage4[294] = 99;
	epigraphy_map_after_stage5[221] = GSW(1705);
	epigraphy_map_after_stage5[222] = 99;
	epigraphy_map_after_stage6[226] = GSW(1706);
	epigraphy_map_after_stage6[227] = 99;

	kurihakase_after3minutes[136] = GSW(1706); //Ch.6 epilogue
	kurihakase_after3minutes[137] = 99;
	
	gor_02_init_evt[531] = GSW(1708); //Gor_02 Init
	gor_02_init_evt[532] = 99; //Unknown
	gor_02_init_evt[407] = GSW(1710);
	gor_02_init_evt[409] = 0;
	gor_02_init_evt[413] = 1;
	gor_02_init_evt[417] = 2;
	gor_02_init_evt[421] = 3;
	gor_02_init_evt[425] = 4;
	gor_02_init_evt[429] = 5;
	gor_02_init_evt[433] = 6;
	gor_02_init_evt[401] = GSW(1700);
	gor_02_init_evt[378] = GSW(1700);
	gor_02_init_evt[375] = GSW(1700);
	gor_02_init_evt[360] = GSW(1700);
	gor_02_init_evt[201] = GSW(1701);
	gor_02_init_evt[202] = 1;
	gor_02_init_evt[59] = GSW(1700);
	gor_02_init_evt[46] = GSW(1705);
	gor_02_init_evt[47] = 99; //Unknown
	patch::writePatch(&gor_02_init_evt[0], gor_02_init_evt1, sizeof(gor_02_init_evt1));

	evt_door_03_close[15] = GSW(1705); //Door 3 close
	evt_door_03_close[16] = 3;
	evt_door_03_close[86] = GSW(1705);
	evt_door_03_close[87] = 5;
	evt_door_03_close[159] = GSW(1705);
	evt_door_03_close[160] = 6;

	evt_door_02_close[3] = GSW(1706); //Door 2 open
	evt_door_02_close[5] = 99; //Unknown
	evt_door_02_close[7] = 99; //Unknown
	evt_door_02_close[43] = GSW(1700);
	evt_door_02_close[51] = GSW(1700);

	evt_door_02_open[56] = GSW(1700); //Door 2 open
	evt_door_02_open[69] = GSW(1710);
	evt_door_02_open[71] = 1;
	evt_door_02_open[72] = 2;
	evt_door_02_open[80] = 3;
	evt_door_02_open[81] = 4;
	evt_door_02_open[91] = 5;
	evt_door_02_open[92] = 6;
	evt_door_02_open[102] = GSW(1700);
	patch::writePatch(&evt_door_02_open[2], evt_door_02_open_hook, sizeof(evt_door_02_open_hook));
}