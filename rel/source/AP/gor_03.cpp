#include <AP/gor_03.h>
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

extern int32_t maffiaboss_ticket_check[];
extern int32_t maffiaboss_init[];
extern int32_t maffiaboss_maffiaboss[];
extern int32_t maffiaboss_teshita_irai[];
extern int32_t maffiaboss_train_ticket[];
extern int32_t maffiaboss_talk_common[];
extern int32_t maffiaboss_casino_init[];
extern int32_t maffiaboss_casino_talk[];
extern int32_t maffia1_talk[];
extern int32_t maffia2_talk[];
extern int32_t maffia3_talk[];
extern int32_t mony_init_03[];
extern int32_t mony_talk_03[];
extern int32_t peton_init_03[];
extern int32_t peton_talk_common_03[];
extern int32_t luigi_init_03[];
extern int32_t luigi_npcEnt_03[];
extern int32_t roten_4_talk[];
extern int32_t kinopiko_a_talk[];
extern int32_t kinopiko_b_talk_03[];
extern int32_t kinopio_a_talk[];
extern int32_t kokinopio_a_talk[];
extern int32_t kokinopio_b_talk[];
extern int32_t nokonoko_a_talk[];
extern int32_t kuribo_1_talk[];
extern int32_t evt_door_01_0_close[];
extern int32_t desk_talk_check[];
extern int32_t evt_door_02_1_open[];
extern int32_t evt_door_02_1_close[];
extern int32_t majorin_nusumigiki[];
extern int32_t yuureturn_rtn[];
extern int32_t peach_mail_03[];
extern int32_t debug_event_mode[];
extern int32_t gor_03_init_evt[];
extern int32_t maffiaboss_nekomu00[];
extern int32_t maffiaboss_nekomu01[];
extern int32_t maffiaboss_nekomu02[];

EVT_BEGIN(maffiaboss_talk_common_evt)
	SWITCH(GSW(1709))
		CASE_EQUAL(1)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_069"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(2)
			SET(GF(15), 1)
			SET(GF(14), 1)
			HALT(GF(14))
		CASE_EQUAL(3)
			IF_EQUAL(GSWF(2000), 0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_120"), 0, PTR("me"))
			ELSE()
				RUN_CHILD_EVT(maffiaboss_teshita_irai)
			END_IF()
		CASE_END()
		CASE_EQUAL(4)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_5_037"), 0, PTR("me"))
		CASE_END()
		CASE_BETWEEN(5, 8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_5_037"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(9)
			RUN_CHILD_EVT(maffiaboss_train_ticket)
		CASE_END()
		CASE_ETC()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_03_01_01"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(maffiaboss_casino_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_03_01_05"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(maffia1_talk_evt)
	SWITCH(GSW(1709))
		CASE_EQUAL(0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_044_02"), 0, PTR("me"))
		CASE_END()
		CASE_BETWEEN(1, 2)
			IF_EQUAL(GSWF(1236), 0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_070"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_116_buka1"), 0, PTR("me"))
			END_IF()
		CASE_END()
		CASE_EQUAL(3)
			IF_EQUAL(GSWF(2000), 0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_121"), 0, PTR("me"))
			ELSE()
				RUN_CHILD_EVT(maffiaboss_nekomu00)
			END_IF()
		CASE_END()
		CASE_EQUAL(4)
			RUN_CHILD_EVT(maffiaboss_nekomu01)
		CASE_END()
		CASE_BETWEEN(5, 8)
			RUN_CHILD_EVT(maffiaboss_nekomu02)
		CASE_END()
		CASE_EQUAL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_5_042"), 0, PTR("me"))
		CASE_END()
		CASE_LARGE_EQUAL(10)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_03_02_05"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(maffia2_talk_evt)
	SWITCH(GSW(1709))
		CASE_EQUAL(0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_044_03"), 0, PTR("me"))
		CASE_END()
		CASE_BETWEEN(1, 2)
			IF_EQUAL(GSWF(1236), 0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_071"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_116_buka2"), 0, PTR("me"))
			END_IF()
		CASE_END()
		CASE_EQUAL(3)
			IF_EQUAL(GSWF(2000), 0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_122"), 0, PTR("me"))
			ELSE()
				RUN_CHILD_EVT(maffiaboss_nekomu00)
			END_IF()
		CASE_END()
		CASE_EQUAL(4)
			RUN_CHILD_EVT(maffiaboss_nekomu01)
		CASE_END()
		CASE_BETWEEN(5, 8)
			RUN_CHILD_EVT(maffiaboss_nekomu02)
		CASE_END()
		CASE_EQUAL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_5_043"), 0, PTR("me"))
		CASE_END()
		CASE_LARGE_EQUAL(10)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_03_03_05"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(maffia3_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_03_008"), 0, PTR("me"))
	RETURN()
EVT_END()

void ApplyGor03Patches(OSModuleInfo* module_info)
{
	maffiaboss_ticket_check[231] = GSW(1709); //Don Piata ticket check
	maffiaboss_ticket_check[232] = 3;

	maffiaboss_init[1] = GSW(1709); //Don Piata init
	maffiaboss_init[3] = 2;
	maffiaboss_init[20] = 3;
	maffiaboss_init[21] = 9;
	maffiaboss_init[56] = 9;

	maffiaboss_maffiaboss[482] = GSW(1709); //Don Piata first meeting
	maffiaboss_maffiaboss[483] = 1;

	maffiaboss_teshita_irai[249] = GSW(1709); //Don Piata sick
	maffiaboss_teshita_irai[250] = 4;

	maffiaboss_train_ticket[797] = GSW(1709); //Don Piata train ticket
	maffiaboss_train_ticket[798] = 9;

	patch::writePatch(&maffiaboss_talk_common[3], maffiaboss_talk_common_evt, sizeof(maffiaboss_talk_common_evt)); //Don Piata speach

	maffiaboss_casino_init[1] = GSW(1709); //Don Piata casino init
	maffiaboss_casino_init[3] = 9;

	patch::writePatch(&maffiaboss_casino_talk[0], maffiaboss_casino_talk_evt, sizeof(maffiaboss_casino_talk_evt)); //Don Piata casino speach

	patch::writePatch(&maffia1_talk[0], maffia1_talk_evt, sizeof(maffia1_talk_evt)); //Mafia 1 speach
	patch::writePatch(&maffia2_talk[0], maffia2_talk_evt, sizeof(maffia2_talk_evt)); //Mafia 2 speach
	patch::writePatch(&maffia3_talk[0], maffia3_talk_evt, sizeof(maffia3_talk_evt)); //Mafia 3 speach

	mony_init_03[1] = GSW(1709); //Frankie init
	mony_init_03[3] = -1;
	mony_init_03[15] = 9;
	mony_init_03[27] = 10;
	mony_init_03[28] = 99; //Unused

	mony_talk_03[1] = GSW(1709); //Frankie speach
	mony_talk_03[3] = 0;
	mony_talk_03[11] = 9;
	mony_talk_03[19] = 99; //Unused

	peton_init_03[16] = GSW(1709); //Francesca init
	peton_init_03[18] = -1;
	peton_init_03[30] = 9;
	peton_init_03[42] = 10;
	peton_init_03[43] = 99; //Unused

	peton_talk_common_03[1] = GSW(1709); //Francesca speach
	peton_talk_common_03[2] = 9;
	peton_talk_common_03[122] = GSW(1709);
	peton_talk_common_03[124] = 10;
	peton_talk_common_03[125] = 99; //Unused
	peton_talk_common_03[176] = GSW(1709);
	peton_talk_common_03[178] = 0;
	peton_talk_common_03[186] = 9;
	peton_talk_common_03[194] = 99;

	luigi_init_03[1] = GSW(1710); //Luigi init
	luigi_init_03[3] = 3;
	luigi_init_03[4] = 4;

	luigi_npcEnt_03[1] = GSW(1710); //Luigi NPC
	luigi_npcEnt_03[3] = 3;
	luigi_npcEnt_03[4] = 4;

	roten_4_talk[1] = GSW(1710); //Random NPC 1 Speach
	roten_4_talk[3] = 2;
	roten_4_talk[11] = 3;
	roten_4_talk[19] = 4;
	roten_4_talk[27] = 5;
	roten_4_talk[35] = 6;
	roten_4_talk[43] = 7;
	roten_4_talk[51] = 8;

	kinopiko_a_talk[1] = GSW(1710); //Random NPC 2 Speach
	kinopiko_a_talk[3] = 2;
	kinopiko_a_talk[11] = 3;
	kinopiko_a_talk[19] = 4;
	kinopiko_a_talk[27] = 5;
	kinopiko_a_talk[35] = 6;
	kinopiko_a_talk[43] = 7;
	kinopiko_a_talk[51] = 8;

	kinopiko_b_talk_03[4] = GSW(1710); //Random NPC 3 Speach
	kinopiko_b_talk_03[6] = 2;
	kinopiko_b_talk_03[25] = 3;
	kinopiko_b_talk_03[44] = 4;
	kinopiko_b_talk_03[63] = 5;
	kinopiko_b_talk_03[82] = 6;
	kinopiko_b_talk_03[101] = 7;
	kinopiko_b_talk_03[120] = 8;

	kinopio_a_talk[1] = GSW(1710); //Random NPC 4 Speach
	kinopio_a_talk[3] = 2;
	kinopio_a_talk[11] = 3;
	kinopio_a_talk[19] = 4;
	kinopio_a_talk[27] = 5;
	kinopio_a_talk[35] = 6;
	kinopio_a_talk[43] = 7;
	kinopio_a_talk[51] = 8;

	kokinopio_a_talk[1] = GSW(1710); //Random NPC 5 Speach
	kokinopio_a_talk[3] = 2;
	kokinopio_a_talk[11] = 3;
	kokinopio_a_talk[19] = 4;
	kokinopio_a_talk[27] = 5;
	kokinopio_a_talk[35] = 6;
	kokinopio_a_talk[43] = 7;
	kokinopio_a_talk[51] = 8;

	kokinopio_b_talk[1] = GSW(1710); //Random NPC 6 Speach
	kokinopio_b_talk[3] = 2;
	kokinopio_b_talk[11] = 3;
	kokinopio_b_talk[19] = 4;
	kokinopio_b_talk[27] = 5;
	kokinopio_b_talk[35] = 6;
	kokinopio_b_talk[43] = 7;
	kokinopio_b_talk[51] = 8;

	nokonoko_a_talk[1] = GSW(1710); //Random NPC 7 Speach
	nokonoko_a_talk[3] = 2;
	nokonoko_a_talk[11] = 3;
	nokonoko_a_talk[19] = 4;
	nokonoko_a_talk[27] = 5;
	nokonoko_a_talk[35] = 6;
	nokonoko_a_talk[43] = 7;
	nokonoko_a_talk[51] = 8;

	kuribo_1_talk[1] = GSW(1710); //Random NPC 8 Speach
	kuribo_1_talk[3] = 2;
	kuribo_1_talk[11] = 3;
	kuribo_1_talk[19] = 4;
	kuribo_1_talk[27] = 5;
	kuribo_1_talk[35] = 6;
	kuribo_1_talk[43] = 7;
	kuribo_1_talk[51] = 8;

	evt_door_01_0_close[13] = GSW(1709); //Door 1 close
	evt_door_01_0_close[14] = 3;
	evt_door_01_0_close[37] = GSW(1709);
	evt_door_01_0_close[38] = 10;

	desk_talk_check[20] = GSW(1709); //Desk speach
	desk_talk_check[22] = 3;
	desk_talk_check[44] = 10;

	evt_door_02_1_open[7] = GSW(1709); //Door 2 open
	evt_door_02_1_open[8] = 9;
	evt_door_02_1_open[30] = GSW(1709);
	evt_door_02_1_open[32] = 3;
	evt_door_02_1_open[45] = 10;
	evt_door_02_1_open[51] = 9;
	evt_door_02_1_open[91] = 11;

	evt_door_02_1_close[1] = GSW(1709); //Door 2 close
	evt_door_02_1_close[2] = 9;

	majorin_nusumigiki[321] = GSW(1709); //Beldam speach
	majorin_nusumigiki[322] = 11;

	yuureturn_rtn[80] = GSW(1708); //Casino maid speach
	yuureturn_rtn[81] = 99; //Unknown

	peach_mail_03[238] = GSW(1703); //Peach mail
	peach_mail_03[239] = 99; //Unknown

	debug_event_mode[12] = GSW(1709); //Debug event mode
	debug_event_mode[13] = 0;
	debug_event_mode[19] = GSW(1709);
	debug_event_mode[20] = 2;
	debug_event_mode[26] = GSW(1709);
	debug_event_mode[27] = 3;
	debug_event_mode[33] = GSW(1709);
	debug_event_mode[34] = 3;
	debug_event_mode[40] = GSW(1709);
	debug_event_mode[41] = 9;
	debug_event_mode[47] = GSW(1709);
	debug_event_mode[48] = 10;

	gor_03_init_evt[46] = GSW(1709); //Gor 03 init
	gor_03_init_evt[48] = 9;
	gor_03_init_evt[49] = 10;
	gor_03_init_evt[59] = 11;
	gor_03_init_evt[219] = GSW(1710);
	gor_03_init_evt[221] = 1;
	gor_03_init_evt[229] = 2;
	gor_03_init_evt[237] = 3;
	gor_03_init_evt[245] = 4;
	gor_03_init_evt[253] = 5;
	gor_03_init_evt[261] = 6;
	gor_03_init_evt[269] = 7;
	gor_03_init_evt[277] = 8;
	gor_03_init_evt[365] = GSW(1703);
	gor_03_init_evt[366] = 99; //Unknown
	gor_03_init_evt[610] = GSW(1709);
	gor_03_init_evt[611] = 0;
	gor_03_init_evt[616] = GSW(1709);
	gor_03_init_evt[617] = 2;
	gor_03_init_evt[626] = GSW(1709);
	gor_03_init_evt[628] = 3;
	gor_03_init_evt[638] = 10;

}