#include <AP/gor_misc.h>
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

extern int32_t luigi_gor_first_talk[];
extern int32_t gor_10_init_evt[];
extern int32_t gor_12_init_evt[];
extern int32_t gor_11_init_evt[];

void ApplyGorMiscPatches(OSModuleInfo* module_info)
{
	luigi_gor_first_talk[18] = GSW(1708);
	luigi_gor_first_talk[19] = 17;
	luigi_gor_first_talk[44] = GSW(1700);
	luigi_gor_first_talk[46] = 1;
	luigi_gor_first_talk[47] = 1;
	luigi_gor_first_talk[52] = 2;
	luigi_gor_first_talk[53] = 2;
	luigi_gor_first_talk[58] = 3;
	luigi_gor_first_talk[59] = 3;
	luigi_gor_first_talk[64] = 4;
	luigi_gor_first_talk[65] = 4;
	luigi_gor_first_talk[70] = 5;
	luigi_gor_first_talk[71] = 5;
	luigi_gor_first_talk[76] = 6;
	luigi_gor_first_talk[77] = 6;
	luigi_gor_first_talk[82] = 7;
	luigi_gor_first_talk[83] = 7;
	luigi_gor_first_talk[88] = 8;

	gor_10_init_evt[19] = GSW(1700);

	gor_11_init_evt[19] = GSW(1708);
	gor_11_init_evt[20] = 18;
	gor_12_init_evt[24] = GSW(1708);
	gor_12_init_evt[25] = 18;
}