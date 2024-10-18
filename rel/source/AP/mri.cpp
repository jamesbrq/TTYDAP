#include <AP/mri.h>
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

extern int32_t mri_countdown[];
extern int32_t seal_bigtre[];
extern int32_t seal_bigtree[];
extern int32_t guide_ini[];
extern int32_t guide_tal[];
extern int32_t rival_ini[];
extern int32_t chibitoge_ini[];
extern int32_t sister_ini[];
extern int32_t elder_ini[];
extern int32_t fabio_ini[];
extern int32_t fabio_tal[];
extern int32_t first_time_w_ber[];
extern int32_t out_dokan_[];
extern int32_t first_bigtre[];
extern int32_t open_bigtre[];
extern int32_t mario_see_yo[];
extern int32_t mri_00_init_ev[];
extern int32_t guide_ini[];
extern int32_t guide_tal[];
extern int32_t rival_ini[];
extern int32_t elder_ini[];
extern int32_t elder_tal[];
extern int32_t puni_1_ini[];
extern int32_t puni_1_tal[];
extern int32_t puni_2_ini[];
extern int32_t puni_2_tal[];
extern int32_t sister_ini[];
extern int32_t sister_tal[];
extern int32_t bigtree_firstbattl[];
extern int32_t meet_riva[];
extern int32_t rival_nakam[];
extern int32_t boss_battl[];
extern int32_t boss_battle_wi[];
extern int32_t evt_ueki_0[];
extern int32_t evt_ueki_0[];
extern int32_t evt_ueki_0[];
extern int32_t evt_ueki_0[];
extern int32_t mri_01_koopa_first_ev[];
extern int32_t mri_01_koopa_second_ev[];
extern int32_t mri_01_init_ev[];
extern int32_t dokan_even[];
extern int32_t mri_02_init_ev[];
extern int32_t elder_rescu[];
extern int32_t elder_ini[];
extern int32_t sister_ini[];
extern int32_t sister_reg[];
extern int32_t puni_3_ini[];
extern int32_t puni_3_tal[];
extern int32_t bero_custom_0[];
extern int32_t bluejail_even[];
extern int32_t guide_siste[];
extern int32_t toraware_elde[];
extern int32_t guide_sister_befor[];
extern int32_t mri_03_init_ev[];
extern int32_t bero_custo[];
extern int32_t chibitoge_ini[];
extern int32_t chuchurina_sog[];
extern int32_t black_shado[];
extern int32_t mri_04_init_ev[];
extern int32_t puni_4_ini[];
extern int32_t puni_4_tal[];
extern int32_t puni_5_ini[];
extern int32_t puni_5_tal[];
extern int32_t mri_05_init_ev[];
extern int32_t rival_ini[];
extern int32_t rival_tal[];
extern int32_t puni_6_ini[];
extern int32_t puni_6_tal[];
extern int32_t puni_7_ini[];
extern int32_t puni_7_tal[];
extern int32_t puni_8_ini[];
extern int32_t puni_8_tal[];
extern int32_t puni_9_ini[];
extern int32_t puni_9_tal[];
extern int32_t puni_10_ini[];
extern int32_t puni_10_tal[];
extern int32_t puni_11_ini[];
extern int32_t puni_11_tal[];
extern int32_t apper_tog[];
extern int32_t mri_06_init_ev[];
extern int32_t chibitoge_ini[];
extern int32_t puniinfo_tal[];
extern int32_t mri_07_init_ev[];
extern int32_t toge_1_ini[];
extern int32_t toge_1_tal[];
extern int32_t toge_2_ini[];
extern int32_t toge_2_tal[];
extern int32_t toge_3_ini[];
extern int32_t toge_3_tal[];
extern int32_t toge_4_ini[];
extern int32_t toge_4_tal[];
extern int32_t toge_5_ini[];
extern int32_t toge_5_tal[];
extern int32_t toge_6_ini[];
extern int32_t toge_6_tal[];
extern int32_t mri_08_init_ev[];
extern int32_t wall_rol[];
extern int32_t slot00_even[];
extern int32_t slot01_even[];
extern int32_t slot02_even[];
extern int32_t slot03_even[];
extern int32_t get_blue_jail_ke[];
extern int32_t mri_09_init_ev[];
extern int32_t item_ev[];
extern int32_t mri_10_init_ev[];
extern int32_t toge_7_ini[];
extern int32_t toge_7_tal[];
extern int32_t toge_8_ini[];
extern int32_t toge_8_tal[];
extern int32_t toge_9_ini[];
extern int32_t toge_9_tal[];
extern int32_t toge_10_ini[];
extern int32_t toge_10_tal[];
extern int32_t toge_11_ini[];
extern int32_t toge_11_tal[];
extern int32_t mri_11_init_ev[];
extern int32_t mri_12_init_ev[];
extern int32_t punipuni_ball00_ini[];
extern int32_t punipuni_ball0[];
extern int32_t yuka_chec[];
extern int32_t yuka_even[];
extern int32_t mri_13_init_ev[];
extern int32_t first_time_w_ber[];
extern int32_t dai3_kanb[];
extern int32_t time_bom[];
extern int32_t mri_14_init_ev[];
extern int32_t dai3_battl[];
extern int32_t mri_15_init_ev[];
extern int32_t mri_16_init_ev[];
extern int32_t puni_disp_inf[];
extern int32_t mri_puni_ini[];
extern int32_t trap_even[];
extern int32_t breakyuka_mai[];
extern int32_t switch_even[];
extern int32_t mri_17_init_ev[];
extern int32_t mri_20_init_ev[];
extern int32_t mri_18_init_ev[];
extern int32_t mri_19_init_evt[];

void ApplyGorMiscPatches(OSModuleInfo* module_info)
{

}