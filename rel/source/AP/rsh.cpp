#include <AP/rsh.h>
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

extern int32_t simi_check[];
extern int32_t simi_offscreen_check[];
extern int32_t kami_check[];
extern int32_t kami_offscreen_check[];
extern int32_t evt_rsh_door_open[];
extern int32_t evt_rsh_door_close[];
extern int32_t evt_moamoa[];
extern int32_t evt_rsh_moamoa_on_window[];
extern int32_t evt_handle[];
extern int32_t regu_unten[];
extern int32_t init_unten[];
extern int32_t talk_unten[];
extern int32_t rsh_00_init_evt[];
extern int32_t rsh_00_a_init_evt[];
extern int32_t init_toron_rsh_01[];
extern int32_t talk_toron_rsh_01[];
extern int32_t init_madam_rsh_01[];
extern int32_t talk_madam_ring_return[];
extern int32_t talk_madam_rsh_01[];
extern int32_t evt_daiji_kami_rsh_01[];
extern int32_t evt_pikary_arrival[];
extern int32_t evt_access_rsh_01[];
extern int32_t rsh_01_init_evt[];
extern int32_t rsh_01_a_init_evt[];
extern int32_t evt_nabe[];
extern int32_t init_gurume[];
extern int32_t talk_gurume[];
extern int32_t init_yuurei[];
extern int32_t talk_yuurei[];
extern int32_t init_toron_rsh_02[];
extern int32_t evt_1day_end[];
extern int32_t evt_2day_end[];
extern int32_t evt_bed[];
extern int32_t evt_toron_catch[];
extern int32_t evt_toron_catch2[];
extern int32_t evt_toron_appear[];
extern int32_t evt_daiji_kami_rsh_02[];
extern int32_t evt_yuka_simi_rsh_02[];
extern int32_t evt_access_entry[];
extern int32_t rsh_02_init_evt[];
extern int32_t rsh_02_a_init_evt[];
extern int32_t init_cook[];
extern int32_t talk_cook[];
extern int32_t init_waitless_rsh_03[];
extern int32_t talk_waitless_pias_return[];
extern int32_t talk_waitless_rsh_03[];
extern int32_t move_waitless[];
extern int32_t talk_toron_rsh_03[];
extern int32_t talk_sarary_rsh_03[];
extern int32_t evt_lanch_missing[];
extern int32_t talk_powan_rsh_03[];
extern int32_t evt_shop_moamoa_talk[];
extern int32_t evt_shop_close[];
extern int32_t evt_shop_door_init[];
extern int32_t evt_yuka_simi_rsh_03[];
extern int32_t evt_access_rsh_03[];
extern int32_t rsh_03_init_evt[];
extern int32_t rsh_03_a_init_evt[];
extern int32_t rsh_03_c_init_evt[];
extern int32_t evt_rsh_konarikin_irai_party_talk[];
extern int32_t evt_allow_kamotsu[];
extern int32_t evt_shorui_funshitu[];
extern int32_t evt_daijisouna_kami[];
extern int32_t init_syashou_rsh_04[];
extern int32_t talk_syashou_rsh_04[];
extern int32_t init_sarary_rsh_04[];
extern int32_t talk_sarary_rsh_04[];
extern int32_t talk_papa_mama[];
extern int32_t init_papa_rsh_04[];
extern int32_t talk_papa_rsh_04[];
extern int32_t init_mama_rsh_04[];
extern int32_t talk_mama_rsh_04[];
extern int32_t init_konari_rsh_04[];
extern int32_t talk_konari_rsh_04[];
extern int32_t init_powan_rsh_04[];
extern int32_t talk_powan_rsh_04[];
extern int32_t evt_tana_5[];
extern int32_t evt_access_rsh_04[];
extern int32_t evt_narikin_room[];
extern int32_t evt_powan_room[];
extern int32_t rsh_04_init_evt[];
extern int32_t rsh_04_a_init_evt[];
extern int32_t rsh_04_c_init_evt[];
extern int32_t evt_meet_moamoa[];
extern int32_t evt_moamoa_escape[];
extern int32_t init_syashou_rsh_05[];
extern int32_t talk_syashou_rsh_05[];
extern int32_t init_waitless_rsh_05[];
extern int32_t talk_waitless_rsh_05[];
extern int32_t evt_ghost_diary[];
extern int32_t rsh_05_init_evt[];
extern int32_t rsh_05_a_init_evt[];
extern int32_t hom_10_evt_resha_start_rsh_06[];
extern int32_t evt_great_moamoa[];
extern int32_t rsh_06_init_evt[];
extern int32_t rsh_06_a_init_evt[];

void ApplyRshPatches(OSModuleInfo* module_info)
{
	simi_check[24] = GSW(1706);
	simi_check[26] = 3;
	simi_check[27] = 22;

	simi_offscreen_check[24] = GSW(1706);
	simi_offscreen_check[26] = 3;
	simi_offscreen_check[27] = 22;

	kami_check[7] = GSW(1706);
	kami_check[8] = 24;
	kami_check[40] = GSW(1706);
	kami_check[42] = 26;
	kami_check[43] = 27;

	kami_offscreen_check[7] = GSW(1706);
	kami_offscreen_check[8] = 24;
	kami_offscreen_check[24] = GSW(1706);
	kami_offscreen_check[26] = 26;
	kami_offscreen_check[27] = 27;

	evt_rsh_door_open[267] = GSW(1706);
	evt_rsh_door_open[268] = 21;
	evt_rsh_door_open[350] = 26;
	evt_rsh_door_open[351] = 27;
	evt_rsh_door_open[356] = 46;

	evt_rsh_door_close[38] = GSW(1706);
	evt_rsh_door_close[39] = 8;
	evt_rsh_door_close[60] = GSW(1706);
	evt_rsh_door_close[61] = 14;
	evt_rsh_door_close[81] = GSW(1706);
	evt_rsh_door_close[82] = 17;
	evt_rsh_door_close[115] = GSW(1706);
	evt_rsh_door_close[117] = 26;
	evt_rsh_door_close[118] = 27;

	evt_moamoa[121] = GSW(1706);
	evt_moamoa[122] = 49;

	evt_rsh_moamoa_on_window[1] = GSW(1706);
	evt_rsh_moamoa_on_window[3] = 49;
	evt_rsh_moamoa_on_window[4] = 51;

	evt_handle[3] = GSW(1706);
	evt_handle[4] = 49;
	evt_handle[6] = GSW(1706);
	evt_handle[7] = 54;

	regu_unten[21] = GSW(1706);
	regu_unten[22] = 30;
	regu_unten[24] = GSW(1706);
	regu_unten[25] = 46;
	regu_unten[30] = GSW(1706);
	regu_unten[31] = 49;
	regu_unten[33] = GSW(1706);
	regu_unten[34] = 59;
	regu_unten[39] = GSW(1706);
	regu_unten[40] = 66;
	regu_unten[66] = GSW(1706);
	regu_unten[67] = 48;
	regu_unten[110] = GSW(1706);
	regu_unten[111] = 48;
	regu_unten[153] = GSW(1706);
	regu_unten[154] = 48;

	init_unten[1] = GSW(1706);
	init_unten[3] = 49;
	init_unten[4] = 51;

	talk_unten[1] = GSW(1706);
	talk_unten[3] = 8;
	talk_unten[28] = 12;
	talk_unten[36] = 13;
	talk_unten[101] = GSW(1706);
	talk_unten[102] = 13;
	talk_unten[104] = 14;
	talk_unten[112] = 21;
	talk_unten[120] = 23;
	talk_unten[128] = 30;
	talk_unten[150] = 46;
	talk_unten[158] = 47;
	talk_unten[166] = 49;
	talk_unten[170] = 52;
	talk_unten[186] = 66;

	rsh_00_init_evt[1] = GSW(1706);
	rsh_00_init_evt[3] = 30;
	rsh_00_init_evt[4] = 45;
	rsh_00_init_evt[6] = 53;
	rsh_00_init_evt[81] = GSW(1706);
	rsh_00_init_evt[83] = 4;
	rsh_00_init_evt[84] = 6;
	rsh_00_init_evt[94] = 24;
	rsh_00_init_evt[95] = 29;
	rsh_00_init_evt[105] = 30;
	rsh_00_init_evt[106] = 45;
	rsh_00_init_evt[118] = 47;
	rsh_00_init_evt[119] = 48;
	rsh_00_init_evt[128] = 49;
	rsh_00_init_evt[129] = 51;
	rsh_00_init_evt[139] = 54;
	rsh_00_init_evt[140] = 64;

	rsh_00_a_init_evt[10] = GSW(1706);
	rsh_00_a_init_evt[12] = 48;
	rsh_00_a_init_evt[13] = 51;
	rsh_00_a_init_evt[42] = GSW(1706);
	rsh_00_a_init_evt[43] = 49;

	init_toron_rsh_01[1] = GSW(1706);
	init_toron_rsh_01[3] = 3;
	init_toron_rsh_01[11] = 3;
	init_toron_rsh_01[12] = 6;
	init_toron_rsh_01[20] = 7;
	init_toron_rsh_01[21] = 22;
	init_toron_rsh_01[29] = 23;
	init_toron_rsh_01[30] = 29;
	init_toron_rsh_01[38] = 30;

	talk_toron_rsh_01[1] = GSW(1706);
	talk_toron_rsh_01[3] = 7;
	talk_toron_rsh_01[25] = 11;
	talk_toron_rsh_01[33] = 14;
	talk_toron_rsh_01[46] = 21;

	init_madam_rsh_01[1] = GSW(1706);
	init_madam_rsh_01[3] = 48;
	init_madam_rsh_01[11] = 48;
	init_madam_rsh_01[12] = 52;
	init_madam_rsh_01[20] = 53;

	talk_madam_ring_return[46] = GSW(1706);
	talk_madam_ring_return[48] = 30;
	talk_madam_ring_return[49] = 45;
	talk_madam_ring_return[54] = 54;
	talk_madam_ring_return[55] = 64;

	talk_madam_rsh_01[1] = GSW(1706);
	talk_madam_rsh_01[2] = 8;
	talk_madam_rsh_01[117] = GSW(1706);
	talk_madam_rsh_01[119] = 30;
	talk_madam_rsh_01[120] = 45;
	talk_madam_rsh_01[125] = 54;
	talk_madam_rsh_01[126] = 64;
	talk_madam_rsh_01[151] = GSW(1706);
	talk_madam_rsh_01[153] = 4;
	talk_madam_rsh_01[161] = 7;
	talk_madam_rsh_01[169] = 8;
	talk_madam_rsh_01[177] = 21;
	talk_madam_rsh_01[185] = 22;
	talk_madam_rsh_01[193] = 30;
	talk_madam_rsh_01[201] = 46;

	evt_daiji_kami_rsh_01[16] = GSW(1706);
	evt_daiji_kami_rsh_01[17] = 25;

	evt_pikary_arrival[126] = GSW(1706);
	evt_pikary_arrival[127] = 53;

	evt_access_rsh_01[4] = GSW(1706);
	evt_access_rsh_01[5] = 65;

	rsh_01_init_evt[1] = GSW(1706);
	rsh_01_init_evt[3] = 30;
	rsh_01_init_evt[4] = 45;
	rsh_01_init_evt[6] = 53;
	rsh_01_init_evt[49] = GSW(1706);
	rsh_01_init_evt[51] = 30;
	rsh_01_init_evt[52] = 45;
	rsh_01_init_evt[54] = 53;
	rsh_01_init_evt[77] = GSW(1706);
	rsh_01_init_evt[79] = 4;
	rsh_01_init_evt[80] = 6;
	rsh_01_init_evt[90] = 24;
	rsh_01_init_evt[91] = 29;
	rsh_01_init_evt[101] = 30;
	rsh_01_init_evt[102] = 45;
	rsh_01_init_evt[114] = 47;
	rsh_01_init_evt[115] = 48;
	rsh_01_init_evt[124] = 49;
	rsh_01_init_evt[125] = 51;
	rsh_01_init_evt[135] = 54;
	rsh_01_init_evt[136] = 64;

	rsh_01_a_init_evt[1] = GSW(1706);
	rsh_01_a_init_evt[3] = 52;
	rsh_01_a_init_evt[28] = GSW(1706);
	rsh_01_a_init_evt[29] = 24;
	rsh_01_a_init_evt[75] = GSW(1706);
	rsh_01_a_init_evt[76] = 2;
	rsh_01_a_init_evt[98] = GSW(1706);
	rsh_01_a_init_evt[99] = 2;

	evt_nabe[4] = GSW(1706);
	evt_nabe[5] = 6;
	evt_nabe[11] = GSW(1706);
	evt_nabe[12] = 7;
	evt_nabe[232] = GSW(1706);
	evt_nabe[233] = 7;

	init_gurume[1] = GSW(1706);
	init_gurume[3] = 30;
	init_gurume[4] = 45;
	init_gurume[12] = 47;
	init_gurume[13] = 52;

	talk_gurume[1] = GSW(1706);
	talk_gurume[3] = 4;
	talk_gurume[11] = 5;
	talk_gurume[19] = 7;
	talk_gurume[21] = GSW(1706);
	talk_gurume[22] = 6;
	talk_gurume[47] = 15;
	talk_gurume[72] = 16;
	talk_gurume[105] = 21;
	talk_gurume[113] = 23;
	talk_gurume[121] = 30;
	talk_gurume[143] = 47;
	talk_gurume[151] = 66;

	init_yuurei[1] = GSW(1706);
	init_yuurei[3] = 15;
	init_yuurei[6] = 15;
	init_yuurei[7] = 22;
	init_yuurei[15] = 24;
	init_yuurei[16] = 29;
	init_yuurei[24] = 30;
	init_yuurei[25] = 46;
	init_yuurei[33] = 47;
	init_yuurei[34] = 52;
	init_yuurei[42] = 54;

	talk_yuurei[8] = GSW(1706);
	talk_yuurei[10] = 17;
	talk_yuurei[15] = GSW(1706);
	talk_yuurei[16] = 16;
	talk_yuurei[18] = GSW(1706);
	talk_yuurei[19] = 16;
	talk_yuurei[55] = GSW(1706);
	talk_yuurei[56] = 17;
	talk_yuurei[64] = 19;
	talk_yuurei[72] = 20;
	talk_yuurei[368] = GSW(1706);
	talk_yuurei[369] = 20;
	talk_yuurei[379] = 21;
	talk_yuurei[387] = 22;
	talk_yuurei[395] = 26;
	talk_yuurei[403] = 30;
	talk_yuurei[411] = 46;
	talk_yuurei[419] = 47;
	talk_yuurei[427] = 66;

	init_toron_rsh_02[1] = GSW(1706);
	init_toron_rsh_02[3] = 28;
	init_toron_rsh_02[13] = 29;

	evt_1day_end[18] = GSW(1706);
	evt_1day_end[19] = 22;

	evt_2day_end[18] = GSW(1706);
	evt_2day_end[19] = 47;

	evt_bed[4] = GSW(1706);
	evt_bed[6] = 21;
	evt_bed[10] = 46;

	evt_toron_catch[273] = GSW(1706);
	evt_toron_catch[274] = 29;

	evt_toron_catch2[316] = GSW(1706);
	evt_toron_catch2[317] = 29;

	evt_toron_appear[25] = GSW(1706);
	evt_toron_appear[26] = 27;
	evt_toron_appear[420] = GSW(1706);
	evt_toron_appear[421] = 28;

	evt_daiji_kami_rsh_02[82] = GSW(1706);
	evt_daiji_kami_rsh_02[83] = 27;

	evt_yuka_simi_rsh_02[23] = GSW(1706);
	evt_yuka_simi_rsh_02[24] = 5;
	evt_yuka_simi_rsh_02[38] = GSW(1706);
	evt_yuka_simi_rsh_02[39] = 5;

	evt_access_entry[1] = GSW(1706);
	evt_access_entry[2] = 65;

	rsh_02_init_evt[1] = GSW(1706);
	rsh_02_init_evt[3] = 30;
	rsh_02_init_evt[4] = 45;
	rsh_02_init_evt[6] = 53;
	rsh_02_init_evt[63] = GSW(1706);
	rsh_02_init_evt[64] = 3;
	rsh_02_init_evt[66] = GSW(1706);
	rsh_02_init_evt[67] = 28;
	rsh_02_init_evt[107] = GSW(1706);
	rsh_02_init_evt[109] = 2;
	rsh_02_init_evt[115] = 4;
	rsh_02_init_evt[116] = 6;
	rsh_02_init_evt[126] = 24;
	rsh_02_init_evt[127] = 27;
	rsh_02_init_evt[137] = 28;
	rsh_02_init_evt[138] = 28;
	rsh_02_init_evt[148] = 29;
	rsh_02_init_evt[158] = 30;
	rsh_02_init_evt[159] = 45;
	rsh_02_init_evt[171] = 47;
	rsh_02_init_evt[172] = 48;
	rsh_02_init_evt[181] = 49;
	rsh_02_init_evt[182] = 51;
	rsh_02_init_evt[192] = 53;
	rsh_02_init_evt[193] = 64;

	rsh_02_a_init_evt[1] = GSW(1706);
	rsh_02_a_init_evt[2] = 22;
	rsh_02_a_init_evt[8] = GSW(1706);
	rsh_02_a_init_evt[9] = 47;
	rsh_02_a_init_evt[21] = GSW(1706);
	rsh_02_a_init_evt[22] = 2;
	rsh_02_a_init_evt[31] = GSW(1706);
	rsh_02_a_init_evt[33] = 3;
	rsh_02_a_init_evt[34] = 22;
	rsh_02_a_init_evt[48] = GSW(1706);
	rsh_02_a_init_evt[49] = 3;
	rsh_02_a_init_evt[51] = GSW(1706);
	rsh_02_a_init_evt[52] = 7;
	rsh_02_a_init_evt[172] = GSW(1706);
	rsh_02_a_init_evt[174] = 2;
	rsh_02_a_init_evt[176] = GSW(1706);
	rsh_02_a_init_evt[177] = 2;
	rsh_02_a_init_evt[468] = 23;
	rsh_02_a_init_evt[470] = GSW(1706);
	rsh_02_a_init_evt[471] = 23;
	rsh_02_a_init_evt[489] = 28;
	rsh_02_a_init_evt[491] = GSW(1706);
	rsh_02_a_init_evt[492] = 28;
	rsh_02_a_init_evt[578] = 54;
	rsh_02_a_init_evt[580] = GSW(1706);
	rsh_02_a_init_evt[581] = 54;
	rsh_02_a_init_evt[625] = GSW(1706);
	rsh_02_a_init_evt[627] = 26;
	rsh_02_a_init_evt[628] = 27;
	rsh_02_a_init_evt[648] = GSW(1706);
	rsh_02_a_init_evt[649] = 27;

	init_cook[1] = GSW(1706);
	init_cook[3] = 48;
	init_cook[4] = 52;

	talk_cook[210] = GSW(1706);
	talk_cook[212] = 3;
	talk_cook[234] = 4;
	talk_cook[242] = 7;
	talk_cook[250] = 8;
	talk_cook[261] = 14;
	talk_cook[269] = 21;
	talk_cook[277] = 22;
	talk_cook[285] = 30;
	talk_cook[307] = 46;
	talk_cook[315] = 47;
	talk_cook[326] = 66;

	init_waitless_rsh_03[1] = GSW(1706);
	init_waitless_rsh_03[3] = 48;
	init_waitless_rsh_03[4] = 52;

	talk_waitless_pias_return[137] = GSW(1706);
	talk_waitless_pias_return[139] = 30;
	talk_waitless_pias_return[140] = 45;
	talk_waitless_pias_return[145] = 54;
	talk_waitless_pias_return[146] = 64;

	talk_waitless_rsh_03[1] = GSW(1706);
	talk_waitless_rsh_03[2] = 8;
	talk_waitless_rsh_03[50] = GSW(1706);
	talk_waitless_rsh_03[52] = 30;
	talk_waitless_rsh_03[53] = 45;
	talk_waitless_rsh_03[58] = 54;
	talk_waitless_rsh_03[59] = 64;
	talk_waitless_rsh_03[84] = GSW(1706);
	talk_waitless_rsh_03[86] = 3;
	talk_waitless_rsh_03[108] = 4;
	talk_waitless_rsh_03[116] = 7;
	talk_waitless_rsh_03[151] = 8;
	talk_waitless_rsh_03[159] = 21;
	talk_waitless_rsh_03[167] = 22;
	talk_waitless_rsh_03[175] = 30;
	talk_waitless_rsh_03[207] = 46;
	talk_waitless_rsh_03[225] = 47;
	talk_waitless_rsh_03[233] = 66;

	move_waitless[1] = GSW(1706);
	move_waitless[2] = 3;
	move_waitless[4] = GSW(1706);
	move_waitless[5] = 7;

	talk_toron_rsh_03[1] = GSW(1706);
	talk_toron_rsh_03[3] = 4;

	talk_sarary_rsh_03[1] = GSW(1706);
	talk_sarary_rsh_03[3] = 4;

	evt_lanch_missing[731] = GSW(1706);
	evt_lanch_missing[732] = 4;

	talk_powan_rsh_03[1] = GSW(1706);
	talk_powan_rsh_03[3] = 4;

	evt_shop_moamoa_talk[1] = GSW(1706);
	evt_shop_moamoa_talk[3] = 48;
	evt_shop_moamoa_talk[8] = 49;
	evt_shop_moamoa_talk[19] = 52;

	evt_shop_close[253] = GSW(1706);
	evt_shop_close[255] = 3;
	evt_shop_close[256] = 22;

	evt_shop_door_init[7] = GSW(1706);
	evt_shop_door_init[9] = 21;
	evt_shop_door_init[11] = 46;

	evt_yuka_simi_rsh_03[23] = GSW(1706);
	evt_yuka_simi_rsh_03[24] = 5;
	evt_yuka_simi_rsh_03[38] = GSW(1706);
	evt_yuka_simi_rsh_03[39] = 5;

	evt_access_rsh_03[4] = GSW(1706);
	evt_access_rsh_03[5] = 65;

	rsh_03_init_evt[1] = GSW(1706);
	rsh_03_init_evt[3] = 30;
	rsh_03_init_evt[4] = 45;
	rsh_03_init_evt[6] = 53;
	rsh_03_init_evt[35] = GSW(1706);
	rsh_03_init_evt[37] = 3;
	rsh_03_init_evt[38] = 6;
	rsh_03_init_evt[101] = 3;
	rsh_03_init_evt[106] = 7;
	rsh_03_init_evt[107] = 72;
	rsh_03_init_evt[124] = GSW(1706);
	rsh_03_init_evt[125] = 48;
	rsh_03_init_evt[127] = GSW(1706);
	rsh_03_init_evt[128] = 52;
	rsh_03_init_evt[156] = GSW(1706);
	rsh_03_init_evt[158] = 4;
	rsh_03_init_evt[159] = 6;
	rsh_03_init_evt[169] = 24;
	rsh_03_init_evt[170] = 29;
	rsh_03_init_evt[180] = 30;
	rsh_03_init_evt[181] = 45;
	rsh_03_init_evt[193] = 47;
	rsh_03_init_evt[194] = 48;
	rsh_03_init_evt[203] = 49;
	rsh_03_init_evt[204] = 51;
	rsh_03_init_evt[214] = 54;
	rsh_03_init_evt[215] = 64;

	rsh_03_a_init_evt[12] = GSW(1706);
	rsh_03_a_init_evt[14] = 3;
	rsh_03_a_init_evt[15] = 22;
	rsh_03_a_init_evt[29] = GSW(1706);
	rsh_03_a_init_evt[30] = 3;
	rsh_03_a_init_evt[32] = GSW(1706);
	rsh_03_a_init_evt[33] = 7;

	rsh_03_c_init_evt[91] = GSW(1706);
	rsh_03_c_init_evt[93] = 21;
	rsh_03_c_init_evt[95] = 46;

	evt_rsh_konarikin_irai_party_talk[81] = GSW(1706);
	evt_rsh_konarikin_irai_party_talk[82] = 12;
	evt_rsh_konarikin_irai_party_talk[84] = GSW(1706);
	evt_rsh_konarikin_irai_party_talk[85] = 12;

	evt_allow_kamotsu[68] = GSW(1706);
	evt_allow_kamotsu[69] = 18;

	evt_shorui_funshitu[358] = GSW(1706);
	evt_shorui_funshitu[359] = 24;

	evt_daijisouna_kami[204] = GSW(1706);
	evt_daijisouna_kami[205] = 26;

	init_syashou_rsh_04[1] = GSW(1706);
	init_syashou_rsh_04[3] = 18;
	init_syashou_rsh_04[4] = 19;
	init_syashou_rsh_04[12] = 48;
	init_syashou_rsh_04[13] = 52;

	talk_syashou_rsh_04[1] = GSW(1706);
	talk_syashou_rsh_04[3] = 14;
	talk_syashou_rsh_04[4] = 20;
	talk_syashou_rsh_04[10] = GSW(1706);
	talk_syashou_rsh_04[12] = 3;
	talk_syashou_rsh_04[61] = GSW(1706);
	talk_syashou_rsh_04[62] = 3;
	talk_syashou_rsh_04[64] = 8;
	talk_syashou_rsh_04[89] = 11;
	talk_syashou_rsh_04[97] = 14;
	talk_syashou_rsh_04[113] = GSW(1706);
	talk_syashou_rsh_04[114] = 12;
	talk_syashou_rsh_04[116] = GSW(1706);
	talk_syashou_rsh_04[117] = 12;
	talk_syashou_rsh_04[121] = 15;
	talk_syashou_rsh_04[154] = GSW(1706);
	talk_syashou_rsh_04[155] = 15;
	talk_syashou_rsh_04[163] = 17;
	talk_syashou_rsh_04[171] = 18;
	talk_syashou_rsh_04[175] = 19;
	talk_syashou_rsh_04[183] = 20;
	talk_syashou_rsh_04[191] = 21;
	talk_syashou_rsh_04[282] = GSW(1706);
	talk_syashou_rsh_04[283] = 21;
	talk_syashou_rsh_04[290] = 22;
	talk_syashou_rsh_04[298] = 30;
	talk_syashou_rsh_04[323] = 46;
	talk_syashou_rsh_04[331] = 47;
	talk_syashou_rsh_04[342] = 66;
	talk_syashou_rsh_04[402] = GSW(1706);
	talk_syashou_rsh_04[404] = 14;
	talk_syashou_rsh_04[405] = 21;

	init_sarary_rsh_04[1] = GSW(1706);
	init_sarary_rsh_04[3] = 3;
	init_sarary_rsh_04[4] = 6;
	init_sarary_rsh_04[12] = 23;
	init_sarary_rsh_04[13] = 29;
	init_sarary_rsh_04[21] = 48;
	init_sarary_rsh_04[22] = 52;
	init_sarary_rsh_04[30] = 54;

	talk_sarary_rsh_04[1] = GSW(1706);
	talk_sarary_rsh_04[3] = 7;
	talk_sarary_rsh_04[11] = 14;
	talk_sarary_rsh_04[19] = 21;
	talk_sarary_rsh_04[38] = 22;
	talk_sarary_rsh_04[46] = 30;
	talk_sarary_rsh_04[54] = 46;
	talk_sarary_rsh_04[62] = 47;

	talk_papa_mama[130] = GSW(1706);
	talk_papa_mama[131] = 11;
	talk_papa_mama[138] = GSW(1706);
	talk_papa_mama[139] = 9;
	talk_papa_mama[141] = GSW(1706);
	talk_papa_mama[142] = 10;

	init_papa_rsh_04[1] = GSW(1706);
	init_papa_rsh_04[3] = 30;
	init_papa_rsh_04[4] = 45;
	init_papa_rsh_04[12] = 48;
	init_papa_rsh_04[13] = 52;
	init_papa_rsh_04[21] = 54;

	talk_papa_rsh_04[1] = GSW(1706);
	talk_papa_rsh_04[3] = 8;
	talk_papa_rsh_04[11] = 9;
	talk_papa_rsh_04[15] = 21;
	talk_papa_rsh_04[23] = 22;
	talk_papa_rsh_04[31] = 30;
	talk_papa_rsh_04[39] = 47;

	init_mama_rsh_04[1] = GSW(1706);
	init_mama_rsh_04[3] = 48;
	init_mama_rsh_04[4] = 52;
	init_mama_rsh_04[12] = 54;

	talk_mama_rsh_04[1] = GSW(1706);
	talk_mama_rsh_04[3] = 8;
	talk_mama_rsh_04[11] = 9;
	talk_mama_rsh_04[15] = 21;
	talk_mama_rsh_04[23] = 22;
	talk_mama_rsh_04[31] = 30;
	talk_mama_rsh_04[39] = 46;
	talk_mama_rsh_04[47] = 47;

	init_konari_rsh_04[1] = GSW(1706);
	init_konari_rsh_04[3] = 30;
	init_konari_rsh_04[4] = 45;
	init_konari_rsh_04[12] = 48;
	init_konari_rsh_04[13] = 52;
	init_konari_rsh_04[21] = 54;

	talk_konari_rsh_04[1] = GSW(1706);
	talk_konari_rsh_04[3] = 8;
	talk_konari_rsh_04[11] = 9;
	talk_konari_rsh_04[19] = 11;
	talk_konari_rsh_04[24] = GSW(1706);
	talk_konari_rsh_04[25] = 10;
	talk_konari_rsh_04[27] = GSW(1706);
	talk_konari_rsh_04[28] = 10;
	talk_konari_rsh_04[55] = GSW(1706);
	talk_konari_rsh_04[56] = 11;
	talk_konari_rsh_04[102] = GSW(1706);
	talk_konari_rsh_04[103] = 12;
	talk_konari_rsh_04[105] = GSW(1706);
	talk_konari_rsh_04[106] = 12;
	talk_konari_rsh_04[222] = GSW(1706);
	talk_konari_rsh_04[223] = 14;

	init_powan_rsh_04[1] = GSW(1706);
	init_powan_rsh_04[3] = 3;
	init_powan_rsh_04[4] = 6;
	init_powan_rsh_04[12] = 23;
	init_powan_rsh_04[13] = 29;
	init_powan_rsh_04[21] = 30;
	init_powan_rsh_04[22] = 45;
	init_powan_rsh_04[30] = 48;
	init_powan_rsh_04[31] = 52;
	init_powan_rsh_04[39] = 54;

	talk_powan_rsh_04[1] = GSW(1706);
	talk_powan_rsh_04[3] = 7;
	talk_powan_rsh_04[11] = 8;
	talk_powan_rsh_04[109] = GSW(1706);
	talk_powan_rsh_04[110] = 8;
	talk_powan_rsh_04[116] = 11;
	talk_powan_rsh_04[124] = 14;
	talk_powan_rsh_04[135] = 15;
	talk_powan_rsh_04[143] = 17;
	talk_powan_rsh_04[151] = 21;
	talk_powan_rsh_04[162] = 22;
	talk_powan_rsh_04[170] = 24;
	talk_powan_rsh_04[174] = 25;
	talk_powan_rsh_04[182] = 26;
	talk_powan_rsh_04[186] = 29;
	talk_powan_rsh_04[194] = 47;

	evt_tana_5[4] = GSW(1706);
	evt_tana_5[5] = 65;

	evt_access_rsh_04[7] = GSW(1706);
	evt_access_rsh_04[8] = 65;

	evt_narikin_room[23] = GSW(1706);
	evt_narikin_room[25] = 8;
	evt_narikin_room[26] = 20;
	evt_narikin_room[65] = GSW(1706);
	evt_narikin_room[67] = 8;
	evt_narikin_room[68] = 20;

	evt_powan_room[21] = GSW(1706);
	evt_powan_room[23] = 23;
	evt_powan_room[62] = GSW(1706);
	evt_powan_room[64] = 23;

	rsh_04_init_evt[1] = GSW(1706);
	rsh_04_init_evt[3] = 30;
	rsh_04_init_evt[4] = 45;
	rsh_04_init_evt[6] = 53;
	rsh_04_init_evt[55] = GSW(1706);
	rsh_04_init_evt[57] = 18;
	rsh_04_init_evt[64] = GSW(1706);
	rsh_04_init_evt[65] = 20;
	rsh_04_init_evt[73] = GSW(1706);
	rsh_04_init_evt[74] = 48;
	rsh_04_init_evt[82] = GSW(1706);
	rsh_04_init_evt[83] = 54;
	rsh_04_init_evt[85] = GSW(1706);
	rsh_04_init_evt[86] = 60;
	rsh_04_init_evt[101] = GSW(1706);
	rsh_04_init_evt[103] = 4;
	rsh_04_init_evt[104] = 6;
	rsh_04_init_evt[114] = 24;
	rsh_04_init_evt[115] = 29;
	rsh_04_init_evt[125] = 30;
	rsh_04_init_evt[126] = 45;
	rsh_04_init_evt[138] = 47;
	rsh_04_init_evt[139] = 48;
	rsh_04_init_evt[148] = 49;
	rsh_04_init_evt[149] = 51;
	rsh_04_init_evt[159] = 54;
	rsh_04_init_evt[160] = 64;

	rsh_04_a_init_evt[12] = GSW(1706);
	rsh_04_a_init_evt[13] = 48;

	rsh_04_c_init_evt[53] = GSW(1706);
	rsh_04_c_init_evt[54] = 19;

	evt_meet_moamoa[181] = GSW(1706);
	evt_meet_moamoa[182] = 50;

	evt_moamoa_escape[122] = GSW(1706);
	evt_moamoa_escape[123] = 51;

	init_syashou_rsh_05[1] = GSW(1706);
	init_syashou_rsh_05[3] = 49;
	init_syashou_rsh_05[4] = 52;

	talk_syashou_rsh_05[1] = GSW(1706);
	talk_syashou_rsh_05[2] = 51;

	init_waitless_rsh_05[1] = GSW(1706);
	init_waitless_rsh_05[3] = 49;
	init_waitless_rsh_05[4] = 52;

	talk_waitless_rsh_05[1] = GSW(1706);
	talk_waitless_rsh_05[2] = 51;

	evt_ghost_diary[1] = GSW(1706);
	evt_ghost_diary[2] = 19;

	rsh_05_init_evt[1] = GSW(1706);
	rsh_05_init_evt[3] = 30;
	rsh_05_init_evt[4] = 45;
	rsh_05_init_evt[15] = GSW(1706);
	rsh_05_init_evt[17] = 4;
	rsh_05_init_evt[18] = 6;
	rsh_05_init_evt[28] = 24;
	rsh_05_init_evt[29] = 29;
	rsh_05_init_evt[39] = 30;
	rsh_05_init_evt[40] = 45;
	rsh_05_init_evt[52] = 47;
	rsh_05_init_evt[53] = 48;
	rsh_05_init_evt[62] = 49;
	rsh_05_init_evt[63] = 51;
	rsh_05_init_evt[73] = 54;
	rsh_05_init_evt[74] = 64;

	rsh_05_a_init_evt[1] = GSW(1706);
	rsh_05_a_init_evt[3] = 8;
	rsh_05_a_init_evt[8] = 14;
	rsh_05_a_init_evt[13] = 23;
	rsh_05_a_init_evt[18] = 29;
	rsh_05_a_init_evt[23] = 46;
	rsh_05_a_init_evt[28] = 48;
	rsh_05_a_init_evt[47] = GSW(1706);
	rsh_05_a_init_evt[49] = 49;
	rsh_05_a_init_evt[56] = 49;
	rsh_05_a_init_evt[67] = 50;
	rsh_05_a_init_evt[88] = GSW(1706);
	rsh_05_a_init_evt[89] = 19;

	hom_10_evt_resha_start_rsh_06[49] = GSW(1706);
	hom_10_evt_resha_start_rsh_06[50] = 59;

	evt_great_moamoa[729] = GSW(1706);
	evt_great_moamoa[730] = 52;

	rsh_06_init_evt[3] = GSW(1706);
	rsh_06_init_evt[5] = 4;
	rsh_06_init_evt[6] = 6;
	rsh_06_init_evt[16] = 24;
	rsh_06_init_evt[17] = 29;
	rsh_06_init_evt[27] = 30;
	rsh_06_init_evt[28] = 45;
	rsh_06_init_evt[40] = 47;
	rsh_06_init_evt[41] = 48;
	rsh_06_init_evt[50] = 49;
	rsh_06_init_evt[51] = 51;
	rsh_06_init_evt[61] = 54;
	rsh_06_init_evt[62] = 64;

	rsh_06_a_init_evt[25] = GSW(1706);
	rsh_06_a_init_evt[26] = 52;
}