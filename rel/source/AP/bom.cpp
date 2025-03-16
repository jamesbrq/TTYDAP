#include <AP/bom.h>
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

extern int32_t bom_first_evt_00[];
extern int32_t bom_00_init_evt[];
extern int32_t bom_first_evt_01[];
extern int32_t bom_init_soncho_01[];
extern int32_t bom_talk_soncho_01[];
extern int32_t talk_nitoro[];
extern int32_t talk_pentrit[];
extern int32_t talk_surary[];
extern int32_t talk_murabito_d[];
extern int32_t bom_talk_murabito_e_01[];
extern int32_t bom_talk_murabito_f_01[];
extern int32_t bom_talk_murabito_g_01[];
extern int32_t bom_talk_murabito_h_01[];
extern int32_t bom_talk_murabito_i_01[];
extern int32_t bom_talk_white[];
extern int32_t bom_musume_talk[];
extern int32_t bom_01_init_evt[];
extern int32_t evt_taihou[];
extern int32_t bom_init_soncho_02[];
extern int32_t bom_talk_soncho_02[];
extern int32_t bom_talk_murabito_e_02[];
extern int32_t bom_talk_murabito_f_02[];
extern int32_t bom_talk_murabito_g_02[];
extern int32_t bom_talk_murabito_h_02[];
extern int32_t bom_talk_murabito_i_02[];
extern int32_t evt_white_bed[];
extern int32_t kiza_talk[];
extern int32_t bom_02_init_evt[];

//Assembly
extern int32_t bom1000_jump[];
extern int32_t jump_minnnanokoe[];


void ApplyBomPatches(OSModuleInfo* module_info)
{
	bom_first_evt_00[91] = GSW(1707);
	bom_first_evt_00[92] = 1;

	bom_00_init_evt[42] = GSW(1707);
	bom_00_init_evt[44] = 0;
	bom_00_init_evt[53] = GSW(1707);
	bom_00_init_evt[54] = 0;
	bom_00_init_evt[56] = GSW(1707);
	bom_00_init_evt[57] = 1;
	bom_00_init_evt[59] = GSW(1707);
	bom_00_init_evt[60] = 1;

	bom_first_evt_01[127] = GSW(1707);
	bom_first_evt_01[128] = 2;

	bom_init_soncho_01[1] = GSW(1707);
	bom_init_soncho_01[3] = 5;

	bom_talk_soncho_01[1] = GSW(1707);
	bom_talk_soncho_01[3] = 6;

	talk_nitoro[1] = GSW(1707);
	talk_nitoro[3] = 1;
	talk_nitoro[11] = 5;
	talk_nitoro[19] = 6;


	talk_pentrit[1] = GSW(1707);
	talk_pentrit[3] = 1;
	talk_pentrit[11] = 5;
	talk_pentrit[19] = 6;

	talk_surary[1] = GSW(1707);
	talk_surary[3] = 1;
	talk_surary[11] = 5;
	talk_surary[19] = 6;

	talk_murabito_d[1] = GSW(1707);
	talk_murabito_d[3] = 1;
	talk_murabito_d[11] = 5;
	talk_murabito_d[19] = 6;

	bom_talk_murabito_e_01[1] = GSW(1707);
	bom_talk_murabito_e_01[3] = 1;
	bom_talk_murabito_e_01[11] = 6;

	bom_talk_murabito_f_01[1] = GSW(1707);
	bom_talk_murabito_f_01[3] = 1;
	bom_talk_murabito_f_01[11] = 6;

	bom_talk_murabito_g_01[1] = GSW(1707);
	bom_talk_murabito_g_01[3] = 1;
	bom_talk_murabito_g_01[11] = 6;

	bom_talk_murabito_h_01[1] = GSW(1707);
	bom_talk_murabito_h_01[3] = 1;
	bom_talk_murabito_h_01[11] = 6;

	bom_talk_murabito_i_01[1] = GSW(1707);
	bom_talk_murabito_i_01[3] = 1;
	bom_talk_murabito_i_01[11] = 6;

	bom_talk_white[1] = GSW(1707);
	bom_talk_white[3] = 6;

	bom_musume_talk[1] = GSW(1707);
	bom_musume_talk[3] = 3;
	bom_musume_talk[23] = 5;

	bom_01_init_evt[6] = GSW(1707);
	bom_01_init_evt[8] = 1;
	bom_01_init_evt[12] = 2;
	bom_01_init_evt[20] = GSW(1708);
	bom_01_init_evt[21] = 16;
	bom_01_init_evt[127] = GSW(1707);
	bom_01_init_evt[129] = 1;
	bom_01_init_evt[130] = 6;
	bom_01_init_evt[157] = GSW(1707);
	bom_01_init_evt[159] = 1;
	bom_01_init_evt[164] = 2;
	bom_01_init_evt[169] = GSW(1708);
	bom_01_init_evt[170] = 16;

	evt_taihou[182] = GSW(1707);
	evt_taihou[183] = 5;

	bom_init_soncho_02[1] = GSW(1707);
	bom_init_soncho_02[3] = 5;

	bom_talk_soncho_02[6] = GSW(1707);
	bom_talk_soncho_02[8] = 1;
	bom_talk_soncho_02[16] = 3;
	bom_talk_soncho_02[323] = GSW(1707);
	bom_talk_soncho_02[324] = 3;
	bom_talk_soncho_02[334] = 4;
	bom_talk_soncho_02[410] = 6;

	bom_talk_murabito_e_02[238] = GSW(1707);
	bom_talk_murabito_e_02[240] = 1;
	bom_talk_murabito_e_02[248] = 5;
	bom_talk_murabito_e_02[256] = 6;

	bom_talk_murabito_f_02[1] = GSW(1707);
	bom_talk_murabito_f_02[3] = 1;
	bom_talk_murabito_f_02[11] = 5;
	bom_talk_murabito_f_02[19] = 6;

	bom_talk_murabito_g_02[38] = GSW(1707);
	bom_talk_murabito_g_02[40] = 1;
	bom_talk_murabito_g_02[48] = 5;
	bom_talk_murabito_g_02[56] = 6;

	bom_talk_murabito_h_02[108] = GSW(1707);
	bom_talk_murabito_h_02[110] = 1;
	bom_talk_murabito_h_02[118] = 5;
	bom_talk_murabito_h_02[126] = 6;

	bom_talk_murabito_i_02[1] = GSW(1707);
	bom_talk_murabito_i_02[3] = 1;
	bom_talk_murabito_i_02[11] = 5;
	bom_talk_murabito_i_02[19] = 6;

	evt_white_bed[492] = GSW(1707);
	evt_white_bed[493] = 4;

	kiza_talk[1] = GSW(1707);
	kiza_talk[3] = 3;
	kiza_talk[11] = 5;

	bom_02_init_evt[11] = GSW(1708);
	bom_02_init_evt[12] = 16;
	bom_02_init_evt[62] = GSW(1707);
	bom_02_init_evt[64] = 1;
	bom_02_init_evt[65] = 5;
	bom_02_init_evt[155] = GSW(1708);
	bom_02_init_evt[156] = 16;

	bom1000_jump[60] = 0x3883082C; // addi r4, r4, 0x82C GSW(1708)
	bom1000_jump[90] = 0x2C030011; // cmpwi r3, 0x11

	jump_minnnanokoe[44] = 0x3883082C; // addi r4, r4, 0x82C GSW(1708)
	jump_minnnanokoe[76] = 0x2C030011; // cmpwi r3, 0x11
}