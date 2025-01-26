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

}