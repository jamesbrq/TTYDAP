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
extern int32_t win_peach_mail[];
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
extern int32_t win_first_evt_dokan[];
extern int32_t welcome_wonderforest[];
extern int32_t win_06_init_evt[];

EVT_BEGIN(vivian_init_evt)
	IF_SMALL(GSW(1712), 3)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -535, 0, -30)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(maririn_init_evt)
	IF_SMALL(GSWF(1712), 3)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -495, 0, -60)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(majorin_init_evt)
	IF_SMALL(GSWF(1712), 3)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -495, 0, -60)
	END_IF()
	RETURN()
EVT_END()

void ApplyWinPatches(OSModuleInfo* module_info)
{
	patch::writePatch(&vivian_init[5], vivian_init_evt, sizeof(vivian_init_evt));
	patch::writePatch(&maririn_init[5], maririn_init_evt, sizeof(maririn_init_evt));
	patch::writePatch(&majorin_init[5], majorin_init_evt, sizeof(majorin_init_evt));

	witchtrio_momeru[862] = GSW(1712);
	witchtrio_momeru[863] = 1;

	witchtrio_picture[514] = GSW(1702);
	witchtrio_picture[515] = 13;

	mario_vs_witchtrio[548] = GSW(1712);
	mario_vs_witchtrio[549] = 2;

	witchtrio_lose[541] = GSW(1712);
	witchtrio_lose[542] = 3;

	win_00_init_evt[22] = GSW(1713);
	win_00_init_evt[23] = 11;
	win_00_init_evt[34] = GSW(1702);
	win_00_init_evt[35] = 1;
	win_00_init_evt[40] = GSW(1702);
	win_00_init_evt[41] = 12;
	win_00_init_evt[46] = GSW(1702);
	win_00_init_evt[47] = 13;
	win_00_init_evt[52] = GSW(1712);
	win_00_init_evt[53] = 2;
	win_00_init_evt[58] = GSW(1712);
	win_00_init_evt[59] = 3;

	evt_dokan1_message[39] = GSW(1702);
	evt_dokan1_message[40] = 8;

	evt_dokan1[1] = GSW(1702);
	evt_dokan1[2] = 7;

	guide_bigtree[155] = GSW(1702);
	guide_bigtree[156] = 2;

	win_peach_mail[247] = GSW(1713);
	win_peach_mail[248] = 18;

	win_01_init_evt[21] = GSW(1712);
	win_01_init_evt[22] = 1;
	win_01_init_evt[27] = GSW(1713);
	win_01_init_evt[28] = 17;

	lookat_house[33] = GSW(1702);
	lookat_house[34] = 9;

	cloud_lostitem[200] = GSW(1702);
	cloud_lostitem[201] = 7;

	win_03_init_evt[79] = GSW(1702);
	win_03_init_evt[80] = 8;
	win_03_init_evt[85] = GSW(1702);
	win_03_init_evt[86] = 5;
	win_03_init_evt[94] = GSW(1702);
	win_03_init_evt[95] = 6;

	meet_clouda[70] = GSW(1702);
	meet_clouda[71] = 10;

	meet_clouda2[135] = GSW(1702);
	meet_clouda2[136] = 11;

	clouda_irai[414] = GSW(1702);
	clouda_irai[415] = 12;

	clouda_nakama[1525] = GSW(1702);
	clouda_nakama[1526] = 14;

	touch_door[1] = GSW(1702);
	touch_door[3] = 11;
	touch_door[7] = 11;
	touch_door[11] = 12;
	touch_door[12] = 12;
	touch_door[16] = 13;

	win_04_init_evt[34] = GSW(1702);
	win_04_init_evt[35] = 9;
	win_04_init_evt[40] = GSW(1702);
	win_04_init_evt[41] = 10;
	win_04_init_evt[46] = GSW(1702);
	win_04_init_evt[47] = 12;
	win_04_init_evt[64] = GSW(1702);
	win_04_init_evt[65] = 13;
	win_04_init_evt[81] = GSW(1702);
	win_04_init_evt[82] = 5;

	cloud_toujou[111] = GSW(1702);
	cloud_toujou[112] = 6;

	win_05_init_evt[35] = GSW(1702);
	win_05_init_evt[36] = 5;

	win_first_evt_dokan[1] = GSW(1702);
	win_first_evt_dokan[2] = 0;

	welcome_wonderforest[239] = GSW(1702);
	welcome_wonderforest[240] = 1;

	win_06_init_evt[1] = GSW(1702);
	win_06_init_evt[2] = 1;
	win_06_init_evt[65] = GSW(1702);
	win_06_init_evt[66] = 0;
}