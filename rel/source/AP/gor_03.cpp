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
extern int32_t mony_init[];
extern int32_t mony_talk[];
extern int32_t peton_init[];
extern int32_t peton_talk_common[];
extern int32_t luigi_init_03[];
extern int32_t luigi_npcEnt_03[];
extern int32_t roten_4_talk[];
extern int32_t kinopiko_a_talk[];
extern int32_t kinopiko_b_talk[];
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
extern int32_t peach_mail[];
extern int32_t debug_event_mode[];
extern int32_t gor_03_init_evt[];

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
}