#include "common_types.h"
#include "evt_cmd.h"
#include "patch.h"
#include <AP/rel_patch_definitions.h>
#include <ttyd/evt_cam.h>
#include <ttyd/evt_item.h>
#include <ttyd/evt_mario.h>
#include <ttyd/evt_mobj.h>
#include <ttyd/evt_npc.h>

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

EVT_BEGIN(peramajin_after_item)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 0)
	USER_FUNC(evt_mobj::evt_mobj_wait_animation_end, PTR("mobj_box00"))
	USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
	USER_FUNC(evt_item::evt_item_entry, PTR("item01"), LW(3), LW(0), LW(1), LW(2), 16, -1, 0)
	USER_FUNC(evt_item::evt_item_get_item, PTR("item01"))
	WAIT_MSEC(800)
	SET(GSWF(6052), 1)
	USER_FUNC(evt_cam::evt_cam3d_evt_off, 500, 11)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
	RETURN()
EVT_END()

void ApplyDouPatches()
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

    peramajin_after[0] = EVT_HELPER_CMD(2, 50);
    peramajin_after[1] = EVT_HELPER_OP(LW(3));
    patch::writePatch(&peramajin_after[3], peramajin_after_item, sizeof(peramajin_after_item));

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
