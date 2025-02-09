#include <AP/eki.h>
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

extern int32_t evt_bridge2[];
extern int32_t eki_evt_origami_00[];
extern int32_t evt_meet_moamoa[];
extern int32_t evt_moamoa_escape[];
extern int32_t evt_elv_down[];
extern int32_t evt_elv_up[];
extern int32_t evt_elv[];
extern int32_t eki_00_init_evt[];
extern int32_t eki_evt_origami_01[];
extern int32_t evt_lock[];
extern int32_t eki_01_init_evt[];
extern int32_t eki_evt_key_02[];
extern int32_t eki_02_init_evt[];
extern int32_t eki_evt_poster[];
extern int32_t evt_chk[];
extern int32_t evt_boots[];
extern int32_t evt_elvkey[];
extern int32_t eki_05_init_evt[];
extern int32_t evt_box_open[];
extern int32_t evt_sw_complete[];
extern int32_t eki_06_init_evt[];

void ApplyEkiPatches(OSModuleInfo* module_info)
{
	evt_bridge2[111] = GSW(1720);
	evt_bridge2[112] = 8;

	eki_evt_origami_00[183] = GSW(1720);
	eki_evt_origami_00[184] = 7;

	evt_meet_moamoa[204] = GSW(1720);
	evt_meet_moamoa[205] = 5;

	evt_moamoa_escape[40] = GSW(1720);
	evt_moamoa_escape[41] = 6;

	evt_elv_down[230] = GSW(1720);
	evt_elv_down[231] = 6;
	evt_elv_down[305] = GSW(1720);
	evt_elv_down[306] = 6;
	evt_elv_down[339] = GSW(1720);
	evt_elv_down[341] = 6;
	evt_elv_down[343] = 6;
	evt_elv_down[362] = GSW(1720);
	evt_elv_down[363] = 7;
	evt_elv_down[398] = GSW(1720);
	evt_elv_down[399] = 7;
	evt_elv_down[532] = GSW(1720);
	evt_elv_down[533] = 5;
	evt_elv_down[538] = GSW(1720);
	evt_elv_down[539] = 5;

	evt_elv_up[116] = GSW(1720);
	evt_elv_up[117] = 6;
	evt_elv_up[191] = GSW(1720);
	evt_elv_up[192] = 6;
	evt_elv_up[325] = GSW(1720);
	evt_elv_up[326] = 7;
	evt_elv_up[329] = GSW(1720);
	evt_elv_up[330] = 7;
	evt_elv_up[381] = GSW(1720);
	evt_elv_up[382] = 6;

	evt_elv[4] = GSW(1720);
	evt_elv[5] = 4;
	evt_elv[25] = GSW(1720);
	evt_elv[26] = 4;

	eki_00_init_evt[26] = GSW(1720);
	eki_00_init_evt[27] = 0; //Unused
	eki_00_init_evt[49] = GSW(1720);
	eki_00_init_evt[60] = 6;

	eki_evt_origami_01[116] = GSWF(6058);
	eki_evt_origami_01[117] = 1;

	evt_lock[6] = GSWF(6060);
	evt_lock[7] = 1;

	eki_01_init_evt[15] = GSWF(6058);
	eki_01_init_evt[16] = 1;
	eki_01_init_evt[68] = GSWF(6060);
	eki_01_init_evt[69] = 1;

	eki_evt_key_02[1] = GSWF(6059);
	eki_evt_key_02[2] = 1;

	eki_02_init_evt[25] = GSWF(6059);
	eki_02_init_evt[26] = 1;

	eki_evt_poster[1] = GSWF(6061);
	eki_evt_poster[2] = 1;

	evt_chk[66] = GSWF(6061);
	evt_chk[67] = 1;
	evt_chk[69] = GSWF(6061);
	evt_chk[70] = 1;

	evt_boots[35] = GSWF(6064);
	evt_boots[36] = 1;

	evt_elvkey[1] = GSWF(6065);
	evt_elvkey[2] = 1;

	eki_05_init_evt[34] = GSWF(6064);
	eki_05_init_evt[35] = 1;
	eki_05_init_evt[48] = GSWF(6065);
	eki_05_init_evt[49] = 1;

	evt_box_open[1] = GSWF(6062);
	evt_box_open[2] = 1;
	evt_box_open[108] = GSWF(6062);
	evt_box_open[109] = 1;

	evt_sw_complete[265] = GSWF(6063);
	evt_sw_complete[266] = 1;

	eki_06_init_evt[20] = GSWF(6062);
	eki_06_init_evt[22] = 1;
	eki_06_init_evt[29] = 1;
	eki_06_init_evt[100] = GSWF(6062);
	eki_06_init_evt[101] = 1;
}