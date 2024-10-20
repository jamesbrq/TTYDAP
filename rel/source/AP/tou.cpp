#include <AP/tou.h>
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

extern int32_t evt_open[];
extern int32_t talk_gardman_doku[];
extern int32_t evt_tou_match_make_default_sub[];
extern int32_t evt_tou_match_make_default[];
extern int32_t evt_mail_1[];
extern int32_t evt_mail_3[];
extern int32_t evt_mail_5[];
extern int32_t evt_tou_match_after_default[];
extern int32_t tou_00_init_evt[];
extern int32_t talk_sakaba[];
extern int32_t talk_hot[];
extern int32_t init_papa[];
extern int32_t talk_papa[];
extern int32_t init_mama[];
extern int32_t talk_mama[];
extern int32_t init_konari[];
extern int32_t talk_konari[];
extern int32_t talk_musume[];
extern int32_t init_musume_1[];
extern int32_t init_musume_2[];
extern int32_t init_musume_3[];
extern int32_t talk_kino_1[];
extern int32_t talk_kino_2[];
extern int32_t talk_roten_1[];
extern int32_t talk_roten_2[];
extern int32_t talk_jugem_1[];
extern int32_t talk_jugem_2[];
extern int32_t init_bijyo[];
extern int32_t talk_bijyo[];
extern int32_t init_kiza[];
extern int32_t talk_kiza[];
extern int32_t init_sarary[];
extern int32_t evt_uron[];
extern int32_t evt_tamago_sleep[];
extern int32_t evt_tamago_move[];
extern int32_t evt_tamago[];
extern int32_t talk_tamago[];
extern int32_t evt_gondora[];
extern int32_t evt_gondora2[];
extern int32_t evt_peach_after[];
extern int32_t evt_key[];
extern int32_t tou_01_init_evt[];
extern int32_t evt_move_g[];
extern int32_t init_gard1[];
extern int32_t talk_gard1[];
extern int32_t init_gard2[];
extern int32_t talk_gard2[];
extern int32_t init_madam[];
extern int32_t talk_madam[];
extern int32_t talk_repo[];
extern int32_t talk_cam1[];
extern int32_t init_cam2[];
extern int32_t talk_cam2[];
extern int32_t talk_kino4[];
extern int32_t talk_kino5[];
extern int32_t talk_kinopiko[];
extern int32_t talk_kinopio_f[];
extern int32_t init_chu[];
extern int32_t talk_chu[];
extern int32_t init_champ[];
extern int32_t talk_champ[];
extern int32_t init_shikowa[];
extern int32_t talk_shikowa[];
extern int32_t init_uranoko[];
extern int32_t talk_uranoko[];
extern int32_t init_ironA[];
extern int32_t init_ironB[];
extern int32_t talk_ironB[];
extern int32_t init_borodo[];
extern int32_t talk_borodo[];
extern int32_t evt_tougi2[];
extern int32_t evt_momikucha[];
extern int32_t evt_key[];
extern int32_t evt_poster_8[];
extern int32_t tou_02_init_evt[];
extern int32_t init_g_1[];
extern int32_t move_g_1_dokimasu[];
extern int32_t talk_g_1[];
extern int32_t move_g_1[];
extern int32_t init_g_2[];
extern int32_t talk_g_2[];
extern int32_t move_g_2[];
extern int32_t init_g_4[];
extern int32_t talk_g_4[];
extern int32_t move_g_4[];
extern int32_t talk_g_7[];
extern int32_t talk_g_8[];
extern int32_t talk_g_9[];
extern int32_t init_g_10[];
extern int32_t talk_g_10[];
extern int32_t init_g_11[];
extern int32_t talk_g_11[];
extern int32_t init_brots[];
extern int32_t evt_gardman[];
extern int32_t evt_lock[];
extern int32_t evt_bred_nigeru[];
extern int32_t tou_04_init_evt[];
extern int32_t evt_takurami[];
extern int32_t evt_hiki_open[];
extern int32_t init_gans[];
extern int32_t talk_gans[];
extern int32_t init_kinoshikowa[];
extern int32_t talk_kinoshikowa[];
extern int32_t evt_sensyu2[];
extern int32_t evt_nozoki[];
extern int32_t tou_05_init_evt[];
extern int32_t evt_kontena[];
extern int32_t evt_kaidan[];
extern int32_t evt_block_broken[];
extern int32_t evt_lock[];
extern int32_t evt_chuchu[];
extern int32_t tou_06_init_evt[];
extern int32_t evt_champion[];
extern int32_t evt_dakuto[];
extern int32_t tou_07_init_evt[];
extern int32_t evt_toile[];
extern int32_t talk_togenoko[];
extern int32_t talk_togemet[];
extern int32_t talk_hammerbros[];
extern int32_t talk_uranoko[];
extern int32_t evt_1st_leagu[];
extern int32_t evt_toile_door[];
extern int32_t tou_08_init_evt[];
extern int32_t tou_09_init_evt[];
extern int32_t init_kino[];
extern int32_t talk_kino[];
extern int32_t init_michel[];
extern int32_t talk_michel[];
extern int32_t talk_octo[];
extern int32_t init_brots[];
extern int32_t talk_brots[];
extern int32_t talk_gangan[];
extern int32_t init_poku[];
extern int32_t talk_poku[];
extern int32_t init_iron[];
extern int32_t talk_iron[];
extern int32_t init_iron2[];
extern int32_t talk_iron2[];
extern int32_t evt_saisyo_after[];
extern int32_t evt_okorareru[];
extern int32_t evt_block[];
extern int32_t tou_10_init_evt[];
extern int32_t tou_11_init_evt[];
extern int32_t evt_block_broken[];
extern int32_t init_michel[];
extern int32_t init_michel_dummy[];
extern int32_t init_brots[];
extern int32_t init_brots_dummy[];
extern int32_t tou_12_init_evt[];
extern int32_t evt_dakuto[];
extern int32_t evt_nusumi[];
extern int32_t evt_first[];
extern int32_t tou_13_init_evt[];

void ApplyTouPatches(OSModuleInfo* module_info)
{

}