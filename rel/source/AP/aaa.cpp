#include <AP/aaa.h>
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

extern int32_t evt_prologue2[];
extern int32_t aaa_epilogue_evt2[];
extern int32_t aaa_00_init_evt[];

void ApplyAaaPatches(OSModuleInfo* module_info)
{
	evt_prologue2[188] = GSW(1700);
	evt_prologue2[189] = 2;

	aaa_epilogue_evt2[485] = GSW(1708);
	aaa_epilogue_evt2[486] = 19;

	aaa_00_init_evt[49] = GSW(1708);
	aaa_00_init_evt[50] = 18;
}