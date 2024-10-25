#include <AP/tou2.h>
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

extern int32_t init_kinosikowa[];
extern int32_t npc_entry[];
extern int32_t evt_match[];
extern int32_t evt_kurikuri_lose[];
extern int32_t evt_shinemon_lose[];
extern int32_t evt_champ_win[];
extern int32_t evt_champ_lose[];
extern int32_t evt_default_opening[];
extern int32_t evt_default_lose[];
extern int32_t evt_boss[];
extern int32_t evt_watch_match[];
extern int32_t evt_kyaku_1[];
extern int32_t evt_kyaku_2[];
extern int32_t evt_kyaku_3[];
extern int32_t evt_kyaku_4[];
extern int32_t evt_kyaku_5[];
extern int32_t evt_kyaku_6[];
extern int32_t evt_kyaku_7[];
extern int32_t evt_kyaku_8[];
extern int32_t tou_03_init_evt[];

void ApplyTou2Patches(OSModuleInfo* module_info)
{
	init_kinosikowa[1] = GSW(1703);
	init_kinosikowa[2] = 172;

	npc_entry[1] = GSW(1703);
	npc_entry[2] = 2;
	npc_entry[55] = GSW(1703);
	npc_entry[57] = 2;
	npc_entry[58] = 18;
	npc_entry[63] = 20;
}