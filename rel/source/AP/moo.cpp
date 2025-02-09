#include <AP/moo.h>
#include <ttyd/evt_cam.h>
#include <ttyd/evt_npc.h>
#include <ttyd/evt_nannpc.h>
#include <ttyd/evt_msg.h>
#include <ttyd/evt_map.h>
#include <ttyd/evt_hit.h>
#include <ttyd/evt_seq.h>
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

extern int32_t moo_evt_first_00[];
extern int32_t moo_00_init_evt[];
extern int32_t moo_dokan_evt_02[];
extern int32_t moo_02_init_evt[];
extern int32_t mario_fly[];
extern int32_t kanbu_fly[];
extern int32_t moo_epilogue_evt[];
extern int32_t moo_04_init_evt[];

EVT_BEGIN(moo_04_init_evt_evt)
	IF_EQUAL(GSW(1708), 18)
		USER_FUNC(evt_mario::evt_mario_kill_party, 0)
		RUN_EVT(moo_epilogue_evt)
		SET(LW(0), 1)
		RETURN()
	END_IF()
	SWITCH(GSW(1707))
		CASE_SMALL(6)
			USER_FUNC(evt_cam::evt_cam3d_evt_set, 0, 134, 533, 0, 38, -8, 0, 11)
			RUN_EVT(mario_fly)
			WAIT_FRM(1)
			SET(LW(0), 1)
			RETURN()
		CASE_END()
		CASE_SMALL(16)
			USER_FUNC(evt_cam::evt_cam3d_evt_set, 20, 200, 533, 20, 104, -8, 0, 11)
			USER_FUNC(evt_mario::evt_mario_cont_onoff, 0)
			USER_FUNC(evt_party::evt_party_cont_onoff, 0, 0)
			USER_FUNC(evt_mario::evt_mario_set_pos, 0, -1000, 0)
			USER_FUNC(evt_party::evt_party_set_pos, 0, 0, -1000, 0)
			WAIT_FRM(1)
			RUN_EVT(kanbu_fly)
			SET(LW(0), 1)
			RETURN()
		CASE_END()
	END_SWITCH()
	SET(LW(0), 0)
	RETURN()
EVT_END()

EVT_BEGIN(moo_04_init_evt_hook)
	RUN_CHILD_EVT(moo_04_init_evt_evt)
	IF_SMALL(LW(0), 1)
		GOTO(99)
	END_IF()
	RETURN()
EVT_END()

void ApplyMooPatches(OSModuleInfo* module_info)
{
	moo_evt_first_00[518] = GSW(1707);
	moo_evt_first_00[519] = 6;

	moo_00_init_evt[1] = GSW(1707);
	moo_00_init_evt[3] = 6;
	moo_00_init_evt[52] = GSW(1707);
	moo_00_init_evt[54] = 6;

	moo_dokan_evt_02[1] = GSW(1707);
	moo_dokan_evt_02[2] = 7;

	moo_02_init_evt[49] = GSW(1707);
	moo_02_init_evt[50] = 7;

	moo_04_init_evt[1] = GSW(1708);
	moo_04_init_evt[2] = 18;
	moo_04_init_evt[72] = EVT_HELPER_CMD(1, 3);
	moo_04_init_evt[73] = EVT_HELPER_OP(99);
	patch::writePatch(&moo_04_init_evt[15], moo_04_init_evt_hook, sizeof(moo_04_init_evt_hook));
}