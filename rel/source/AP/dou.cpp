#include <AP/dou.h>
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

extern int32_t bero_in_event_dou_00[];
extern int32_t totsunyu_event[];
extern int32_t korutesu_event_dou_01[];
extern int32_t dou_01_init_evt[];
extern int32_t watergate_handle_init[];
extern int32_t watergate_handle[];
extern int32_t dou_03_init_evt[];
extern int32_t bigdoor_event_init[];
extern int32_t bigdoor_event[];
extern int32_t peramajin_before[];
extern int32_t peramajin_after[];
extern int32_t dou_09_init_evt[];
extern int32_t namiyoke_check[];
extern int32_t suifu_c_init_dou_11[];
extern int32_t suifu_c_talk_dou_11[];
extern int32_t suifu_d_init_dou_11[];
extern int32_t suifu_d_talk_dou_11[];
extern int32_t suifu_e_init_dou_11[];
extern int32_t suifu_e_talk_dou_11[];
extern int32_t mony_init_dou_11[];
extern int32_t mony_talk_dou_11[];
extern int32_t peton_init_dou_11[];
extern int32_t peton_talk_dou_11[];
extern int32_t watarikiri_suifu_init[];
extern int32_t suifu_orei[];
extern int32_t escapeway_event[];
extern int32_t bombwall_bomb[];
extern int32_t bombwall_init[];
extern int32_t dou_11_init_evt[];

void ApplyDouPatches(OSModuleInfo* module_info)
{
	bero_in_event_dou_00[1] = GSW(1717);
	bero_in_event_dou_00[2] = 1;

	totsunyu_event[34] = GSW(1717);
	totsunyu_event[35] = 2;

	korutesu_event_dou_01[67] = GSW(1717);
	korutesu_event_dou_01[68] = 3;

	dou_01_init_evt[41] = GSW(1717);
	dou_01_init_evt[42] = 2;

	watergate_handle_init[1] = GSW(1717);
	watergate_handle_init[2] = 5;

	watergate_handle[1] = GSW(1717);
	watergate_handle[2] = 5;
	watergate_handle[224] = GSW(1717);
	watergate_handle[235] = 5;

	dou_03_init_evt[53] = GSW(1717);
	dou_03_init_evt[54] = 4;
	dou_03_init_evt[69] = GSW(1717);
	dou_03_init_evt[70] = 5;

	bigdoor_event_init[1] = GSW(1717);
	bigdoor_event_init[2] = 4;

	bigdoor_event[141] = GSW(1717);
	bigdoor_event[142] = 4;

	peramajin_before[1] = GSWF(6051);
	peramajin_before[2] = 1;
	peramajin_before[386] = GSWF(6051);
	peramajin_before[387] = 1;

	peramajin_after[273] = GSWF(6052);
	peramajin_after[274] = 1;

	dou_09_init_evt[31] = GSW(1717);
	dou_09_init_evt[32] = 5;
	dou_09_init_evt[204] = 12;

	namiyoke_check[1] = GSW(1717);
	namiyoke_check[2] = 5;
	namiyoke_check[4] = GSW(1717);
	namiyoke_check[5] = 6;

	suifu_c_init_dou_11[1] = GSW(1717);
	suifu_c_init_dou_11[3] = 7;
	suifu_c_init_dou_11[4] = 12;
	suifu_c_init_dou_11[13] = GSW(1717);
	suifu_c_init_dou_11[14] = 11;

	suifu_c_talk_dou_11[1] = GSW(1717);
	suifu_c_talk_dou_11[3] = 7;
	suifu_c_talk_dou_11[4] = 11;
	suifu_c_talk_dou_11[12] = 12;

	suifu_d_init_dou_11[1] = GSW(1717);
	suifu_d_init_dou_11[3] = 7;
	suifu_d_init_dou_11[4] = 12;
	suifu_d_init_dou_11[13] = GSW(1717);
	suifu_d_init_dou_11[14] = 11;

	suifu_d_talk_dou_11[1] = GSW(1717);
	suifu_d_talk_dou_11[3] = 7;
	suifu_d_talk_dou_11[4] = 11;
	suifu_d_talk_dou_11[12] = 12;

	suifu_e_init_dou_11[1] = GSW(1717);
	suifu_e_init_dou_11[3] = 7;
	suifu_e_init_dou_11[4] = 12;
	suifu_e_init_dou_11[13] = GSW(1717);
	suifu_e_init_dou_11[14] = 11;

	suifu_e_talk_dou_11[1] = GSW(1717);
	suifu_e_talk_dou_11[3] = 7;
	suifu_e_talk_dou_11[4] = 11;
	suifu_e_talk_dou_11[12] = 12;

	mony_init_dou_11[1] = GSW(1717);
	mony_init_dou_11[3] = 7;
	mony_init_dou_11[4] = 12;

	mony_talk_dou_11[1] = GSW(1717);
	mony_talk_dou_11[3] = 7;
	mony_talk_dou_11[4] = 11;
	mony_talk_dou_11[22] = 12;

	peton_init_dou_11[1] = GSW(1717);
	peton_init_dou_11[3] = 7;
	peton_init_dou_11[4] = 12;

	peton_talk_dou_11[1] = GSW(1717);
	peton_talk_dou_11[3] = 7;
	peton_talk_dou_11[4] = 11;
	peton_talk_dou_11[30] = 12;

	watarikiri_suifu_init[77] = GSW(1717);
	watarikiri_suifu_init[78] = 11;

	suifu_orei[329] = GSW(1717);
	suifu_orei[330] = 7;

	escapeway_event[91] = GSW(1717);
	escapeway_event[92] = 12;

	bombwall_bomb[176] = GSW(1717);
	bombwall_bomb[177] = 13;

	bombwall_init[31] = GSW(1717);
	bombwall_init[32] = 10;
	bombwall_init[58] = 11;
	bombwall_init[59] = 12;
	bombwall_init[101] = 13;

	dou_11_init_evt[42] = GSW(1717);
	dou_11_init_evt[44] = 6;
	dou_11_init_evt[60] = 11;
	dou_11_init_evt[74] = GSW(1717);
	dou_11_init_evt[75] = 6;
	dou_11_init_evt[109] = GSW(1717);
	dou_11_init_evt[110] = 7;
	dou_11_init_evt[112] = GSW(1717);
	dou_11_init_evt[113] = 12;
	dou_11_init_evt[141] = GSW(1717);
	dou_11_init_evt[142] = 10;
	dou_11_init_evt[174] = GSW(1717);
	dou_11_init_evt[175] = 7;
	dou_11_init_evt[291] = GSW(1717);
	dou_11_init_evt[292] = 14;
	dou_11_init_evt[294] = GSW(1717);
	dou_11_init_evt[295] = 15;
}