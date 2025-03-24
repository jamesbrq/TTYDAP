#include "evt_cmd.h"
#include "mod.h"
#include "OWR.h"
#include "patch.h"
#include <AP/tik.h>
#include <ttyd/evt_bero.h>
#include <ttyd/evt_case.h>
#include <ttyd/evt_msg.h>
#include <ttyd/evt_npc.h>
#include <ttyd/evtmgr_cmd.h>
#include <ttyd/mario_pouch.h>

using namespace mod;
using namespace ttyd;

EVT_DEFINE_USER_FUNC(checkChapterRequirements) {
	auto& state = gMod->state_;
	int8_t count = 0;
	for (int i = 114; i <= 120; i++)
	{
		count += mario_pouch::pouchCheckItem(i);
	}
	if (count >= state.rChapters)
		evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 1);
	else
		evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 0);
	return 2;
}

EVT_DEFINE_USER_FUNC(checkChapterClears) {
	int8_t count = 0;
	for (int i = 114; i <= 120; i++)
	{
		count += mario_pouch::pouchCheckItem(i);
	}
	evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], count);
	return 2;
}

extern int32_t uranaisi_next_evt[];
extern int32_t unk_evt_tik_0000f448[];
extern int32_t kuribo5_talk[];
extern int32_t chusan4_talk[];
extern int32_t borodo3_talk[];
extern int32_t borodo4_talk[];
extern int32_t pinkbom_talk[];
extern int32_t master_2_talk[];
extern int32_t evt_open_04_open[];
extern int32_t evt_door_04_close[];
extern int32_t tensou_init_event[];
extern int32_t tensou_sw_event[];
extern int32_t kurihakase_event[];
extern int32_t tik_00_init_evt[];
extern int32_t dokan_2_in_evt[];
extern int32_t gorotuki_sanninngumi[];
extern int32_t tik_01_init_evt[];
extern int32_t geso_battle[];
extern int32_t tik_02_init_evt[];
extern int32_t bero_05_custom_in[];
extern int32_t bero_05_custom_out[];
extern int32_t tik_guide_init[];
extern int32_t tik_guide_talk[];
extern int32_t puni_soguu_3[];
extern int32_t puni_switch[];
extern int32_t puni_nakama[];
extern int32_t tik_03_init_evt[];
extern int32_t puni_soguu_1[];
extern int32_t puni_soguu_2[];
extern int32_t tik_04_switch_evt[];
extern int32_t tik_04_init_evt[];
extern int32_t tik_kurihakase_init[];
extern int32_t tik_kurihakase_talk[];
extern int32_t large_door_stg0_init[];
extern int32_t large_door_stg0[];
extern int32_t look_large_door[];
extern int32_t kaidan_event[];
extern int32_t mahojin_event[];
extern int32_t mahojin_event1[];
extern int32_t mahojin_event2[];
extern int32_t mahojin_event3[];
extern int32_t mahojin_event4[];
extern int32_t mahojin_event5[];
extern int32_t mahojin_event6[];
extern int32_t lastdungeon_anim_init[];
extern int32_t lastdungeon_init[];
extern int32_t lastdungeon[];
extern int32_t mahojin_event7[];
extern int32_t tik_05_init_evt[];
extern int32_t tik_peach_mail[];
extern int32_t tik_07_init_evt[];
extern int32_t tik_08_init_evt[];
extern int32_t tik_15_init_evt[];
extern int32_t tik_18_init_evt[];
extern int32_t tik_evt_majin2[];

EVT_BEGIN(kuribo5_talk_evt)
	IF_SMALL(GSW(1711), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1713), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_02"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1703), 20)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_03"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 10)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_04"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1717), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_05"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1706), 43)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_06"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 16)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 17)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_08"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_001_09"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(kuribo5_talk_hook)
	RUN_CHILD_EVT(kuribo5_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(chusan4_talk_evt)
	IF_SMALL(GSW(1711), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1713), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_02"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1703), 20)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_03"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 10)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_04"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1717), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_05"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1706), 43)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_06"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 16)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 17)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_08"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_002_09"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(chusan4_talk_hook)
	RUN_CHILD_EVT(chusan4_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(borodo3_talk_evt)
	IF_SMALL(GSW(1711), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1713), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_02"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1703), 20)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_03"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 10)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_04"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1717), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_05"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1706), 43)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_06"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 16)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 17)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_08"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_003_09"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(borodo3_talk_hook)
	RUN_CHILD_EVT(borodo3_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(borodo4_talk_evt)
	IF_SMALL(GSW(1711), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1713), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_02"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1703), 20)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_03"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 10)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_04"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1717), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_05"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1706), 43)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_06"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 16)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 17)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_08"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_004_09"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(borodo4_talk_hook)
	RUN_CHILD_EVT(borodo4_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(master_2_talk_evt)
	IF_SMALL(GSW(1711), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_10"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1713), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_11"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1703), 20)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_12"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1715), 10)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_13"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1717), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_14"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1706), 43)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_15"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 16)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_16"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 17)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_17"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("tik_005_18"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(master_2_talk_hook)
	RUN_CHILD_EVT(master_2_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(tik_kurihakase_init_evt)
	IF_LARGE_EQUAL(GSW(1707), 21)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -30, 23, -190)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1708), 1)
		IF_SMALL_EQUAL(GSW(1708), 6)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -30, 23, -170)
			RETURN()
		END_IF()
	END_IF()
EVT_END()

EVT_BEGIN(tik_kurihakase_init_hook)
	RUN_CHILD_EVT(tik_kurihakase_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(tik_05_init_evt_evt)
	USER_FUNC(checkChapterRequirements, LW(0))
	IF_EQUAL(LW(0), 1)
		USER_FUNC(evt_case::evt_run_case_evt, 0, 1, PTR("a_dai"), 0, PTR(mahojin_event7), 0)
		RETURN()
	END_IF()
	IF_SMALL(GSW(1700), 17)
		RUN_EVT(kaidan_event)
		USER_FUNC(evt_case::evt_run_case_evt, 0, 1, PTR("a_dai"), 0, PTR(mahojin_event), 0)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1711), 15)
		USER_FUNC(evt_case::evt_run_case_evt, 0, 1, PTR("a_dai"), 0, PTR(mahojin_event1), 0)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1713), 18)
		USER_FUNC(evt_case::evt_run_case_evt, 0, 1, PTR("a_dai"), 0, PTR(mahojin_event2), 0)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1703), 29)
		USER_FUNC(evt_case::evt_run_case_evt, 0, 1, PTR("a_dai"), 0, PTR(mahojin_event3), 0)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1715), 15)
		USER_FUNC(evt_case::evt_run_case_evt, 0, 1, PTR("a_dai"), 0, PTR(mahojin_event4), 0)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1717), 27)
		USER_FUNC(evt_case::evt_run_case_evt, 0, 1, PTR("a_dai"), 0, PTR(mahojin_event5), 0)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1706), 50)
		USER_FUNC(evt_case::evt_run_case_evt, 0, 1, PTR("a_dai"), 0, PTR(mahojin_event6), 0)
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(tik_05_init_evt_hook)
	RUN_CHILD_EVT(tik_05_init_evt_evt)
	GOTO(99)
EVT_END()

EVT_BEGIN(tik_05_init_evt_evt2)
	IF_EQUAL(GSWF(6053), 0)
		SET(LW(0), PTR("n_bero"))
		RUN_CHILD_EVT(evt_bero::bero_case_switch_off)
		RUN_CHILD_EVT(large_door_stg0_init)
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 1)
		SET(LW(0), PTR("n_bero"))
		RUN_CHILD_EVT(evt_bero::bero_case_switch_off)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1708), 1)
		SET(LW(0), PTR("n_bero"))
		RUN_CHILD_EVT(evt_bero::bero_case_switch_off)
		RUN_CHILD_EVT(lastdungeon_anim_init)
		RUN_CHILD_EVT(lastdungeon_init)
		USER_FUNC(evt_case::evt_run_case_evt, 10, 1, PTR("n_bero"), 0, PTR(lastdungeon), 0)
		RETURN()
	END_IF()
	IF_LARGE(GSW(1708), 1)
		RUN_CHILD_EVT(lastdungeon_anim_init)
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(tik_05_init_evt_hook2)
	RUN_CHILD_EVT(tik_05_init_evt_evt2)
	GOTO(98)
EVT_END()

void ApplyTikPatches(OSModuleInfo* module_info)
{
	uranaisi_next_evt[1] = GSW(1708);
	uranaisi_next_evt[2] = 21;

	unk_evt_tik_0000f448[3] = GSW(1700);
	unk_evt_tik_0000f448[4] = 16;

	patch::writePatch(&kuribo5_talk[0], kuribo5_talk_hook, sizeof(kuribo5_talk_hook));

	patch::writePatch(&chusan4_talk[0], chusan4_talk_hook, sizeof(chusan4_talk_hook));

	patch::writePatch(&borodo3_talk[0], borodo3_talk_hook, sizeof(borodo3_talk_hook));

	patch::writePatch(&borodo4_talk[0], borodo4_talk_hook, sizeof(borodo4_talk_hook));

	patch::writePatch(&master_2_talk[0], master_2_talk_hook, sizeof(master_2_talk_hook));

	evt_open_04_open[1] = GSW(1700);
	evt_open_04_open[3] = 0; //Unused
	evt_open_04_open[58] = 0; //Unused

	evt_door_04_close[13] = GSW(1707);
	evt_door_04_close[14] = 20;
	evt_door_04_close[69] = GSW(1707);
	evt_door_04_close[70] = 20;

	tensou_init_event[4] = GSW(1700);
	tensou_init_event[6] = 0; //Unused
	tensou_init_event[23] = 0; //Unused
	tensou_init_event[100] = GSW(1707);
	tensou_init_event[101] = 20;

	tensou_sw_event[5] = GSW(1707);
	tensou_sw_event[6] = 20;
	tensou_sw_event[39] = GSW(1708);
	tensou_sw_event[40] = 19;

	kurihakase_event[212] = GSW(1707);
	kurihakase_event[213] = 21;

	tik_00_init_evt[1] = GSW(1708);
	tik_00_init_evt[2] = 18;
	tik_00_init_evt[4] = GSW(1707);
	tik_00_init_evt[5] = 20;
	tik_00_init_evt[176] = GSW(1708);
	tik_00_init_evt[177] = 18;

	dokan_2_in_evt[1] = GSW(1700);
	dokan_2_in_evt[2] = 16;
	dokan_2_in_evt[4] = GSWF(6056);
	dokan_2_in_evt[5] = 1;

	gorotuki_sanninngumi[677] = GSW(1700);
	gorotuki_sanninngumi[678] = 12;

	tik_01_init_evt[41] = GSW(1711);
	tik_01_init_evt[42] = 8;
	tik_01_init_evt[188] = GSW(1700);
	tik_01_init_evt[189] = 15;
	tik_01_init_evt[195] = GSW(1700);
	tik_01_init_evt[196] = 11;

	geso_battle[657] = GSWF(6056);
	geso_battle[658] = 1;

	tik_02_init_evt[85] = GSWF(6056);
	tik_02_init_evt[86] = 1;
	tik_02_init_evt[172] = GSWF(6056);
	tik_02_init_evt[173] = 0;
	tik_02_init_evt[178] = GSW(1701);
	tik_02_init_evt[179] = 1;

	bero_05_custom_in[1] = GSW(1713);
	bero_05_custom_in[2] = 11;

	bero_05_custom_out[1] = GSW(1718);
	bero_05_custom_out[2] = 4;
	bero_05_custom_out[4] = GSW(1718);
	bero_05_custom_out[5] = 5;

	tik_guide_init[1] = GSW(1718);
	tik_guide_init[3] = 1;
	tik_guide_init[15] = 2;
	tik_guide_init[27] = 3;

	tik_guide_talk[1] = GSW(1718);
	tik_guide_talk[3] = 2;

	puni_soguu_3[577] = GSW(1718);
	puni_soguu_3[578] = 2;

	puni_switch[111] = GSW(1718);
	puni_switch[112] = 3;

	puni_nakama[3] = GSW(1718);
	puni_nakama[4] = 3;
	puni_nakama[83] = GSW(1718);
	puni_nakama[84] = 4;

	tik_03_init_evt[22] = GSW(1718);
	tik_03_init_evt[23] = 1;
	tik_03_init_evt[28] = GSW(1718);
	tik_03_init_evt[29] = 2;
	tik_03_init_evt[72] = GSW(1718);
	tik_03_init_evt[73] = 3;
	tik_03_init_evt[97] = GSW(1702);
	tik_03_init_evt[98] = 1;

	//puni_soguu_1 SKIPPED

	puni_soguu_2[142] = GSW(1718);
	puni_soguu_2[143] = 1;

	tik_04_switch_evt[34] = GSWF(6054);
	tik_04_switch_evt[35] = 1;

	tik_04_init_evt[19] = GSW(1700);
	tik_04_init_evt[20] = 0; //Unused
	tik_04_init_evt[25] = GSW(1718);
	tik_04_init_evt[26] = 0;
	tik_04_init_evt[36] = GSWF(6055);
	tik_04_init_evt[37] = 1;
	tik_04_init_evt[202] = GSW(1717);
	tik_04_init_evt[203] = 18;

	patch::writePatch(&tik_kurihakase_init[0], tik_kurihakase_init_hook, sizeof(tik_kurihakase_init_hook));

	large_door_stg0[118] = GSWF(6053);
	large_door_stg0[119] = 1;

	look_large_door[1] = GSWF(6053);
	look_large_door[3] = 0;

	kaidan_event[62] = GSW(1700);
	kaidan_event[63] = 13;

	mahojin_event[355] = GSW(1700);
	mahojin_event[356] = 14;

	mahojin_event1[92] = GSW(1711);
	mahojin_event1[93] = 16;

	mahojin_event2[87] = GSW(1713);
	mahojin_event2[88] = 19;

	mahojin_event3[84] = GSW(1703);
	mahojin_event3[85] = 30;

	mahojin_event4[84] = GSW(1715);	 
	mahojin_event4[85] = 16;

	mahojin_event5[84] = GSW(1717);
	mahojin_event5[85] = 28;

	mahojin_event6[84] = GSW(1706);
	mahojin_event6[85] = 51;

	mahojin_event7[2499] = GSW(1708);
	mahojin_event7[2500] = 1;

	patch::writePatch(&tik_05_init_evt[58], tik_kurihakase_init_hook, sizeof(tik_kurihakase_init_hook));
	tik_05_init_evt[143] = EVT_HELPER_CMD(1, 3);
	tik_05_init_evt[144] = EVT_HELPER_OP(99);
	patch::writePatch(&tik_05_init_evt[145], tik_05_init_evt_hook2, sizeof(tik_05_init_evt_hook2));
	tik_05_init_evt[185] = EVT_HELPER_CMD(1, 3);
	tik_05_init_evt[186] = EVT_HELPER_OP(98);

	tik_peach_mail[178] = GSW(1715);
	tik_peach_mail[179] = 15;

	tik_07_init_evt[277] = GSW(1717);
	tik_07_init_evt[278] = 18;
	tik_07_init_evt[294] = GSW(1706);
	tik_07_init_evt[295] = 0; //Unused
	tik_07_init_evt[297] = GSW(1707);
	tik_07_init_evt[298] = 1;
	tik_07_init_evt[316] = GSW(1715);
	tik_07_init_evt[317] = 14;

	tik_08_init_evt[19] = GSW(1703);
	tik_08_init_evt[20] = 31;
	tik_08_init_evt[37] = GSW(1703);
	tik_08_init_evt[38] = 0; //Unused
	tik_08_init_evt[40] = GSW(1704);
	tik_08_init_evt[41] = 2;

	tik_15_init_evt[29] = GSW(1711);
	tik_15_init_evt[30] = 8;
	tik_15_init_evt[42] = GSW(1717);
	tik_15_init_evt[43] = 18;

	tik_18_init_evt[53] = GSW(1717);
	tik_18_init_evt[54] = 18;

	tik_evt_majin2[210] = GSWF(6055);
	tik_evt_majin2[211] = 1;
}