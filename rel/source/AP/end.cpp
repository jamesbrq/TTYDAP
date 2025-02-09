#include <AP/end.h>
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

extern int32_t unk_evt_end_00004654[];
extern int32_t unk_evt_end_00004718[];


void ApplyEndPatches(OSModuleInfo* module_info)
{
	unk_evt_end_00004654[17] = GSW(1708);
	unk_evt_end_00004654[18] = 20;

	unk_evt_end_00004718[59] = GSW(1708);
	unk_evt_end_00004718[60] = 20;
}