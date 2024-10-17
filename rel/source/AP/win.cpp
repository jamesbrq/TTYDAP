#include <AP/win.h>
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

extern int32_t vivian_init[];
extern int32_t maririn_init[];
extern int32_t majorin_init[];
extern int32_t witchtrio_momeru[];
extern int32_t witchtrio_picture[];
extern int32_t mario_vs_witchtrio[];
extern int32_t witchtrio_lose[];
extern int32_t win_00_init_evt[];
extern int32_t evt_dokan1_message[];
extern int32_t evt_dokan1[];
extern int32_t guide_bigtree[];
extern int32_t peach_mail[];
extern int32_t win_01_init_evt[];
extern int32_t lookat_house[];
extern int32_t cloud_lostitem[];
extern int32_t win_03_init_evt[];
extern int32_t meet_clouda[];
extern int32_t meet_clouda2[];
extern int32_t clouda_irai[];
extern int32_t clouda_nakama[];
extern int32_t touch_door[];
extern int32_t win_04_init_evt[];
extern int32_t cloud_toujou[];
extern int32_t win_05_init_evt[];
extern int32_t first_evt_dokan[];
extern int32_t welcome_wonderforest[];
extern int32_t win_06_init_evt[];

EVT_BEGIN(vivian_init_evt)
	IF_SMALL(GSWF(2018), 1)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -535, 0, -30)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(maririn_init_evt)
	IF_SMALL(GSWF(2018), 1)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -495, 0, -60)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(majorin_init_evt)
	IF_SMALL(GSWF(2018), 1)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -495, 0, -60)
	END_IF()
	RETURN()
EVT_END()

void ApplyWinPatches(OSModuleInfo* module_info)
{
	patch::writePatch(&vivian_init[5], vivian_init_evt, sizeof(vivian_init_evt));
	patch::writePatch(&maririn_init[5], maririn_init_evt, sizeof(maririn_init_evt));
	patch::writePatch(&majorin_init[5], majorin_init_evt, sizeof(majorin_init_evt));

	mario_vs_witchtrio[548] = GSW(1712);
	mario_vs_witchtrio[549] = 1;

	witchtrio_lose[541] = GSW(1712);
	witchtrio_lose[542] = 2;

	win_00_init_evt[22] = GSW(1702);
	win_00_init_evt[23] = 99; //Unknown
	win_00_init_evt[46] = GSW(1712);
	win_00_init_evt[47] = 0;
	win_00_init_evt[52] = GSW(1712);
	win_00_init_evt[53] = 1;
	win_00_init_evt[58] = GSW(1712);
	win_00_init_evt[59] = 2;
	win_00_init_evt[33] = EVT_HELPER_CMD(1, 4);
	win_00_init_evt[34] = EVT_HELPER_OP(&win_00_init_evt[45]);
}