#include <AP/jin.h>
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

extern int32_t door_teresa_n[];
extern int32_t door_teresa_s[];
extern int32_t first_jin00[];
extern int32_t push_evt[];
extern int32_t evt_key_jin[];
extern int32_t evt_unlock_jin[];
extern int32_t tyuou_teresa_init[];
extern int32_t tyuou_teresa_talk[];
extern int32_t oku_teresa_init[];
extern int32_t temae_teresa_init[];
extern int32_t senteresa_evt[];
extern int32_t jin_00_init_evt[];
extern int32_t ranperu_toujou[];
extern int32_t irekawatta_mario[];
extern int32_t ranperu_lastbattle[];
extern int32_t hontonosugata_mario[];
extern int32_t ranperu_init[];
extern int32_t nisemario_init[];
extern int32_t jin_04_jumpstand[];
extern int32_t jin_04_init_evt[];
extern int32_t jin_06_init_evt[];
extern int32_t evt_teresa_box_open[];
extern int32_t jin_07_init_evt[];
extern int32_t evt_open[];
extern int32_t oumu_talk[];
extern int32_t oumu_check[];
extern int32_t jin_08_init_evt[];
extern int32_t evt_yuka[];
extern int32_t jin_11_init_evt[];
extern int32_t evt_usu_kagemario_party_kill_jin[];


void ApplyJinPatches(OSModuleInfo* module_info)
{

}