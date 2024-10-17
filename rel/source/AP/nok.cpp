#include <AP/nok.h>
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

extern int32_t nokonoko_A_init[];
extern int32_t nokonoko_A_talk[];
extern int32_t nokonoko_B_talk[];
extern int32_t nokonoko_C_talk[];
extern int32_t nokonoko_D_talk[];
extern int32_t nokonoko_F_talk[];
extern int32_t init_white[];
extern int32_t talk_white[];
extern int32_t first_time_camera[];
extern int32_t nok_00_koopa_evt[];
extern int32_t nok_00_init_evt[];
extern int32_t nokonoko_G_talk[];
extern int32_t nokonoko_H_talk[];
extern int32_t nokonoko_I_talk[];
extern int32_t open_gate[];
extern int32_t monban_init[];
extern int32_t monban_talk[];
extern int32_t nokotarou_init[];
extern int32_t nokotarou_talk[];
extern int32_t nokopapa_init[];
extern int32_t nokopapa_talk[];
extern int32_t nokonoko_K_talk[];
extern int32_t boss_noko_open_gate[];
extern int32_t boss_noko_talk[];
extern int32_t nokorin_init[];
extern int32_t nokorin_talk[];
extern int32_t odoriko_1_talk[];
extern int32_t odoriko_2_talk[];
extern int32_t odoriko_3_talk[];
extern int32_t nokotarou_tabidachi[];
extern int32_t nok_01_init_evt[];

EVT_BEGIN(nokonoko_A_talk_evt)
	IF_EQUAL(GSW(1707), 3)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_nok_01"), 0, 0)
		RETURN()
	END_IF()
	SWITCH(GSW(1701))
		CASE_SMALL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_02"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_02_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_03"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_03_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_nok_01_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_A_talk_hook)
	RUN_CHILD_EVT(nokonoko_A_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_B_talk_evt)
	SWITCH(GSW(1701))
		CASE_SMALL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_04"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_04_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_05"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_05_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_05_1_1"), 0, 0)
	RETURN()
EVT_END()


EVT_BEGIN(nokonoko_B_talk_hook)
	RUN_CHILD_EVT(nokonoko_B_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_C_talk_evt)
	SWITCH(GSW(1701))
		CASE_SMALL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_06"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_06_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_07"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_07_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_07_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_C_talk_hook)
	RUN_CHILD_EVT(nokonoko_C_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_D_talk_evt)
	SWITCH(GSW(1701))
		CASE_SMALL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_08"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_08_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_09"), 0, 0)
			USER_FUNC(evt_msg::evt_msg_select, 0, PTR("stg1_nok_09_yesno"))
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_09_01"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_09_01_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_09_01_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_D_talk_hook)
	RUN_CHILD_EVT(nokonoko_D_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_F_talk_evt)
	SWITCH(GSW(1701))
		CASE_SMALL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_17"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_17_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_18"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_18_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_18_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_F_talk_hook)
	RUN_CHILD_EVT(nokonoko_F_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(init_white_evt)
	IF_EQUAL(GSW(1702), 99) //Unknown
		RETURN()
	END_IF()
	IF_SMALL(GSW(1710), 5)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 375, 50, -105)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(init_white_hook)
	RUN_CHILD_EVT(init_white_evt)
	RETURN()
EVT_END()

EVT_BEGIN(talk_white_evt)
	SWITCH(GSW(1701))
		CASE_SMALL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_19_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_19_02"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1710), 2)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_19_03"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_19_04"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(talk_white_hook)
	RUN_CHILD_EVT(talk_white_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_G_talk_evt)
	SWITCH(GSW(1701))
		CASE_SMALL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_20"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_20_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_21"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_21_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_21_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_G_talk_hook)
	RUN_CHILD_EVT(nokonoko_G_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_H_talk_evt)
	SWITCH(GSW(1701))
		CASE_SMALL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_22"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_22_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_23"), 0, 0)
			USER_FUNC(evt_msg::evt_msg_select, 0, PTR("stg1_nok_23_yesno"))
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_23_01"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_23_01_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_23_01_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_H_talk_hook)
	RUN_CHILD_EVT(nokonoko_H_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_I_talk_evt)
	SWITCH(GSW(1701))
		CASE_SMALL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_24"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_24_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_25"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_25_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_25_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_I_talk_hook)
RUN_CHILD_EVT(nokonoko_I_talk_evt)
RETURN()
EVT_END()

EVT_BEGIN(monban_talk_evt)
	SWITCH(GSW(1701))
		CASE_SMALL(4)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_26"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(5)
			RUN_CHILD_EVT(open_gate)
			RETURN()
		CASE_END()
		CASE_SMALL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_31_01_00"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_31_01_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_31_02_1"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_31_02_2"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_31_02_2_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(monban_talk_hook)
	RUN_CHILD_EVT(monban_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokopapa_talk_evt)
	IF_SMALL(GSW(1704), 1)
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_79"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_80"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 99) //Unknown
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_81"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_82"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 99) //Unknown
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_83"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_84"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_84_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokopapa_talk_hook)
	RUN_CHILD_EVT(nokopapa_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_K_talk_evt)
	SWITCH(GSW(1701))
		CASE_SMALL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_32"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_32_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_33"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_33_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_33_1_1"), 0, 0)
	RETURN()
EVT_END()


EVT_BEGIN(nokonoko_K_talk_hook)
	RUN_CHILD_EVT(nokonoko_K_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(boss_noko_talk_evt)
	IF_SMALL(GSW(1701), 4)
		RUN_CHILD_EVT(boss_noko_open_gate)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1707), 3)
		SET(GSWF(3877), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_57_1_00"), 0, 0)
		RETURN()
	END_IF()
	IF_SMALL(GSW(1711), 1)
		IF_EQUAL(GSWF(1774), 1)
			IF_EQUAL(GSWF(1775), 1)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_56"), 0, 0)
				RETURN()
			END_IF()
		END_IF()
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_47"), 0, 0)
		RETURN()
	END_IF()
	SWITCH(GSW(1701))
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_56_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(99) //Unknown
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_57"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 99) //Unknown
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_57_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_57_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(boss_noko_talk_hook)
	RUN_CHILD_EVT(boss_noko_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokorin_talk_evt)
	IF_SMALL(GSW(1702), 1)
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_79"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_80"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	IF_SMALL(GSW(1704), 1)
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_81"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_82"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 1)
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_83"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_84"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 99) //Unknown
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_83"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_84"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_84_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokorin_talk_hook)
	RUN_CHILD_EVT(nokorin_talk_evt)
	RETURN()
EVT_END()


void ApplyNokPatches(OSModuleInfo* module_info)
{
	nokonoko_A_init[6] = GSW(1701);
	nokonoko_A_init[7] = 3;
	nokonoko_A_init[16] = GSW(1702);
	nokonoko_A_init[17] = 99; //Unknown

	patch::writePatch(&nokonoko_A_talk[0], nokonoko_A_talk_hook, sizeof(nokonoko_A_talk_hook)); //Nokonoko A Dialogue
	patch::writePatch(&nokonoko_B_talk[0], nokonoko_B_talk_hook, sizeof(nokonoko_B_talk_hook)); //Nokonoko B Dialogue
	patch::writePatch(&nokonoko_C_talk[0], nokonoko_C_talk_hook, sizeof(nokonoko_C_talk_hook)); //Nokonoko C Dialogue
	patch::writePatch(&nokonoko_D_talk[0], nokonoko_D_talk_hook, sizeof(nokonoko_D_talk_hook)); //Nokonoko D Dialogue
	patch::writePatch(&nokonoko_F_talk[274], nokonoko_F_talk_hook, sizeof(nokonoko_F_talk_hook)); //Nokonoko F Dialogue
	patch::writePatch(&init_white[8], init_white_hook, sizeof(init_white_hook)); //General White Init
	patch::writePatch(&talk_white[0], talk_white_hook, sizeof(talk_white_hook)); //General White Dialogue

	first_time_camera[179] = GSW(1701); //First time camera
	first_time_camera[180] = 3;

	nok_00_koopa_evt[1213] = GSW(1702); //Bowser intermission
	nok_00_koopa_evt[1214] = 99; //Unknown

	nok_00_init_evt[1] = GSW(1702); //Room 0 Init
	nok_00_init_evt[2] = 99; //Unknown
	nok_00_init_evt[25] = GSW(1701);
	nok_00_init_evt[26] = 3;
	nok_00_init_evt[40] = GSW(1701);
	nok_00_init_evt[41] = 1;
	nok_00_init_evt[43] = GSW(1701);
	nok_00_init_evt[44] = 99; //Unknown
	nok_00_init_evt[77] = GSW(1702);
	nok_00_init_evt[78] = 99; //Unknown

	patch::writePatch(&nokonoko_G_talk[0], nokonoko_G_talk_hook, sizeof(nokonoko_G_talk_hook)); //Nokonoko G Dialogue
	patch::writePatch(&nokonoko_H_talk[0], nokonoko_H_talk_hook, sizeof(nokonoko_H_talk_hook)); //Nokonoko H Dialogue
	patch::writePatch(&nokonoko_I_talk[0], nokonoko_I_talk_hook, sizeof(nokonoko_I_talk_hook)); //Nokonoko I Dialogue

	open_gate[217] = GSW(1701); //Open gate
	open_gate[218] = 5;

	monban_init[1] = GSW(1701); //Monban Init
	monban_init[3] = 5;

	patch::writePatch(&monban_talk[0], monban_talk_hook, sizeof(monban_talk_hook)); //Monban Dialogue

	nokotarou_init[3] = GSW(1701); //Nokotarou Init
	nokotarou_init[5] = 9;

	nokotarou_talk[1] = GSW(1701); //Nokotarou Dialogue
	nokotarou_talk[3] = 4;
	nokotarou_talk[11] = 5;
	nokotarou_talk[19] = 6;

	nokopapa_init[3] = GSW(1701); //Nokopapa Init
	nokopapa_init[5] = 99; //Unknown

	patch::writePatch(&nokopapa_talk[3], nokopapa_talk_hook, sizeof(nokopapa_talk_hook)); //Nokopapa Dialogue
	patch::writePatch(&nokonoko_K_talk[25], nokonoko_K_talk_hook, sizeof(nokonoko_K_talk_hook)); //Nokonoko K Dialogue

	boss_noko_open_gate[180] = GSW(1701); //Boss Noko Open Gate
	boss_noko_open_gate[181] = 4;

	patch::writePatch(&boss_noko_talk[278], boss_noko_talk_hook, sizeof(boss_noko_talk_hook)); //Boss Noko Dialogue

	nokorin_init[7] = GSW(1711); //Nokorin Init
	nokorin_init[8] = 99; //Unknown
	nokorin_init[10] = GSW(1701);
	nokorin_init[11] = 10;

	nokorin_talk[1] = GSW(1701); //Nokorin Dialogue
	nokorin_talk[3] = 9;
	nokorin_talk[82] = 10;
	nokorin_talk[90] = 99; //Unknown
	nokorin_talk[169] = EVT_HELPER_CMD(0, 49);
	patch::writePatch(&nokorin_talk[170], nokorin_talk_hook, sizeof(nokorin_talk_hook)); //Nokorin Dialogue

	odoriko_1_talk[1] = GSW(1701); //Odoriko 1 Dialogue
	odoriko_1_talk[3] = 9;

	odoriko_2_talk[1] = GSW(1701); //Odoriko 1 Dialogue
	odoriko_2_talk[3] = 9;

	odoriko_3_talk[1] = GSW(1701); //Odoriko 1 Dialogue
	odoriko_3_talk[3] = 9;

	nokotarou_tabidachi[306] = GSW(1701); //Nokotarou Tabidachi
	nokotarou_tabidachi[307] = 99; //Unknown

	nok_01_init_evt[13] = GSW(1708); //Room 1 Init
	nok_01_init_evt[14] = 99; //Unknown
	nok_01_init_evt[75] = GSW(1701);
	nok_01_init_evt[76] = 1;
	nok_01_init_evt[78] = GSW(1708);
	nok_01_init_evt[79] = 99; //Unknown
	nok_01_init_evt[109] = GSW(1701);
	nok_01_init_evt[110] = 5;
	nok_01_init_evt[292] = GSW(1708);
	nok_01_init_evt[293] = 99; //Unknown
	nok_01_init_evt[311] = GSW(1708);
	nok_01_init_evt[312] = 99; //Unknown

}