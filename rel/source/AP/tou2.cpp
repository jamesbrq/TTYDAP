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
extern int32_t tou2_entry_event[];
extern int32_t tou2_dead_event[];
extern int32_t tou2_phase_event[];

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

	evt_match[55] = GSW(1703);
	evt_match[56] = 172;
	evt_match[139] = GSW(1703);
	evt_match[140] = 172;
	evt_match[235] = GSW(1703);
	evt_match[236] = 172;
	evt_match[238] = GSW(1703);
	evt_match[239] = 14;
	evt_match[245] = GSW(1703);
	evt_match[246] = 12;
	evt_match[334] = GSW(1703);
	evt_match[335] = 7;
	evt_match[337] = GSW(1703);
	evt_match[338] = 7;
	evt_match[341] = GSW(1703);
	evt_match[342] = 172;

	evt_kurikuri_lose[104] = GSW(1703);
	evt_kurikuri_lose[105] = 172;

	evt_shinemon_lose[32] = GSW(1703);
	evt_shinemon_lose[33] = 172;

	evt_champ_win[4] = GSW(1703);
	evt_champ_win[5] = 172;
	evt_champ_win[219] = GSW(1703);
	evt_champ_win[220] = 172;

	evt_champ_lose[37] = GSW(1703);
	evt_champ_lose[38] = 172;
	evt_champ_lose[72] = GSW(1703);
	evt_champ_lose[73] = 172;

	evt_default_opening[7] = GSW(1703);
	evt_default_opening[8] = 172;
	evt_default_opening[157] = GSW(1703);
	evt_default_opening[158] = 14;
	evt_default_opening[420] = GSW(1703);
	evt_default_opening[421] = 172;
	evt_default_opening[438] = GSW(1703);
	evt_default_opening[439] = 172;
	evt_default_opening[451] = GSW(1703);
	evt_default_opening[452] = 172;
	evt_default_opening[470] = GSW(1703);
	evt_default_opening[471] = 172;
	evt_default_opening[518] = GSW(1703);
	evt_default_opening[519] = 172;
	evt_default_opening[614] = GSW(1703);
	evt_default_opening[616] = 12;
	evt_default_opening[617] = 172;
	evt_default_opening[622] = 172;
	evt_default_opening[633] = GSW(1703);
	evt_default_opening[635] = 172;
	evt_default_opening[675] = GSW(1703);
	evt_default_opening[676] = 172;

	evt_default_lose[49] = GSW(1703);
	evt_default_lose[50] = 172;
	evt_default_lose[110] = GSW(1703);
	evt_default_lose[112] = 12;
	evt_default_lose[113] = 172;
	evt_default_lose[118] = 172;

	evt_boss[1900] = GSW(1703);
	evt_boss[1901] = 20;

	evt_watch_match[1] = GSW(1703);
	evt_watch_match[2] = 172;

	evt_kyaku_1[11] = GSW(1703);
	evt_kyaku_1[13] = 5;

	evt_kyaku_2[11] = GSW(1703);
	evt_kyaku_2[13] = 5;

	evt_kyaku_3[11] = GSW(1703);
	evt_kyaku_3[13] = 5;

	evt_kyaku_4[11] = GSW(1703);
	evt_kyaku_4[13] = 5;

	evt_kyaku_5[11] = GSW(1703);
	evt_kyaku_5[13] = 5;

	evt_kyaku_6[11] = GSW(1703);
	evt_kyaku_6[13] = 5;

	evt_kyaku_7[11] = GSW(1703);
	evt_kyaku_7[13] = 5;

	evt_kyaku_8[11] = GSW(1703);
	evt_kyaku_8[13] = 5;

	tou_03_init_evt[1] = GSW(1703);
	tou_03_init_evt[3] = 19;
	tou_03_init_evt[52] = GSW(1703);
	tou_03_init_evt[54] = 19;
	tou_03_init_evt[108] = GSW(1703);
	tou_03_init_evt[110] = 2;
	tou_03_init_evt[128] = GSW(1703);
	tou_03_init_evt[130] = 2;
	tou_03_init_evt[142] = 19;
	tou_03_init_evt[178] = GSW(1703);
	tou_03_init_evt[179] = 172;
	tou_03_init_evt[250] = GSW(1703);
	tou_03_init_evt[252] = 3;
	tou_03_init_evt[253] = 18;
	tou_03_init_evt[265] = 20;
	tou_03_init_evt[280] = GSW(1703);
	tou_03_init_evt[281] = 3;

	tou2_entry_event[4] = GSW(1703);
	tou2_entry_event[5] = 14;
	tou2_entry_event[201] = GSW(1703);
	tou2_entry_event[202] = 172;

	tou2_dead_event[25] = GSW(1703);
	tou2_dead_event[26] = 14;

	tou2_phase_event[69] = GSW(1703);
	tou2_phase_event[70] = 14;
	tou2_phase_event[141] = GSW(1703);
	tou2_phase_event[142] = 14;
	tou2_phase_event[219] = GSW(1703);
	tou2_phase_event[220] = 14;
	tou2_phase_event[372] = GSW(1703);
	tou2_phase_event[373] = 14;
}