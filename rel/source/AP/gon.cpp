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

extern int32_t gon_01_init_evt[];
extern int32_t tegami_event[];
extern int32_t gon_senkaron_event[];
extern int32_t gon_key_get_03[];
extern int32_t gon_03_init_evt[];
extern int32_t leaderkaron_init_event[];
extern int32_t evt_kaidan_pera[];
extern int32_t evt_hashi_pera0[];
extern int32_t evt_hashi_pera1[];
extern int32_t evt_hashi[];
extern int32_t gon_evt_chk_04[];
extern int32_t gon_evt_unlock_04[];
extern int32_t gon_key_get_04[];
extern int32_t gon_04_init_evt[];
extern int32_t evt_unlock00[];
extern int32_t gon_evt_chk01_05[];
extern int32_t evt_unlock01[];
extern int32_t gon_key_get_05[];
extern int32_t gon_05_init_evt[];
extern int32_t tsuritenjo_event[];
extern int32_t gon_evt_majin2[];
extern int32_t gon_06_init_evt[];
extern int32_t out_func[];
extern int32_t gon_item_evt_07[];
extern int32_t gon_07_init_evt[];
extern int32_t gon_evt_chk_08[];
extern int32_t gon_evt_unlock_08[];
extern int32_t gon_08_init_evt[];
extern int32_t gon_10_init_evt[];
extern int32_t gonbaba_event[];
extern int32_t gon_11_init_evt[];
extern int32_t gon_chuchurina_sogu[];
extern int32_t item_get00[];
extern int32_t gon_12_init_evt[];
extern int32_t gon_key_tbl_04[];
extern int32_t gon_key_tbl00_05[];
extern int32_t gon_key_tbl01_05[];
extern int32_t gon_key_tbl_08[];

// clang-format off
EVT_BEGIN(gon_evt_majin2_item)
	USER_FUNC(evt_npc::evt_npc_change_fbat_mode, 7)
	USER_FUNC(evt_npc::evt_npc_stop_for_event)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 0)
	USER_FUNC(evt_mobj::evt_mobj_wait_animation_end, PTR("box"))
	USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
	USER_FUNC(evt_item::evt_item_entry, PTR("item02"), LW(3), LW(0), LW(1), LW(2), 16, GSWF(6080), 0)
	USER_FUNC(evt_item::evt_item_get_item, PTR("item02"))
	WAIT_MSEC(800)
	SET(GSWF(6011), 1)
	SET(GSWF(1493), 1)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
	USER_FUNC(evt_cam::evt_cam3d_evt_off, 500, 11)
	USER_FUNC(evt_npc::evt_npc_change_fbat_mode, 1)
	USER_FUNC(evt_npc::evt_npc_start_for_event)
	RETURN()
EVT_END()
// clang-format on

void ApplyGonPatches()
{
    gon_01_init_evt[36] = GSW(1711);
    gon_01_init_evt[37] = 2;
    gon_01_init_evt[39] = GSW(1711);
    gon_01_init_evt[40] = 2;
    gon_01_init_evt[94] = GSW(1711);
    gon_01_init_evt[95] = 3;
    gon_01_init_evt[97] = GSW(1711);
    gon_01_init_evt[98] = 3;

    tegami_event[17] = GSW(1711);
    tegami_event[18] = 4;

    gon_senkaron_event[1] = GSW(1711);
    gon_senkaron_event[2] = 5;
    gon_senkaron_event[559] = GSW(1711);
    gon_senkaron_event[560] = 5;

    gon_key_get_03[1] = GSWF(6002);
    gon_key_get_03[2] = 1;

    gon_03_init_evt[30] = GSW(1711);
    gon_03_init_evt[31] = 5;
    gon_03_init_evt[45] = GSW(1711);
    gon_03_init_evt[46] = 4;
    gon_03_init_evt[51] = GSWF(6002);
    gon_03_init_evt[52] = 1;

    leaderkaron_init_event[6] = GSW(1711);
    leaderkaron_init_event[7] = 5;

    evt_kaidan_pera[73] = GSW(1711);
    evt_kaidan_pera[74] = 6;
    evt_kaidan_pera[76] = GSW(1711);
    evt_kaidan_pera[77] = 6;

    evt_hashi_pera0[9] = GSWF(6003);
    evt_hashi_pera0[10] = 1;

    evt_hashi_pera1[9] = GSWF(6004);
    evt_hashi_pera1[10] = 1;

    evt_hashi[1] = GSWF(6003);
    evt_hashi[2] = 1;
    evt_hashi[25] = GSWF(6004);
    evt_hashi[26] = 1;

    gon_evt_chk_04[2] = 12;

    gon_evt_unlock_04[6] = GSWF(6005);
    gon_evt_unlock_04[7] = 1;

    gon_key_get_04[1] = GSWF(6006);
    gon_key_get_04[2] = 1;

    gon_04_init_evt[269] = GSWF(6006);
    gon_04_init_evt[270] = 1;
    gon_04_init_evt[332] = GSWF(6005);
    gon_04_init_evt[333] = 1;
    gon_04_init_evt[337] = 12;

    evt_unlock00[6] = GSWF(6007);
    evt_unlock00[7] = 1;

    gon_evt_chk01_05[2] = 12;

    evt_unlock01[6] = GSWF(6008);
    evt_unlock01[7] = 1;

    gon_key_get_05[1] = GSWF(6009);
    gon_key_get_05[2] = 1;

    gon_05_init_evt[241] = GSWF(6009);
    gon_05_init_evt[242] = 1;
    gon_05_init_evt[275] = GSWF(6007);
    gon_05_init_evt[276] = 1;
    gon_05_init_evt[295] = GSWF(6008);
    gon_05_init_evt[296] = 1;
    gon_05_init_evt[300] = 12;

    tsuritenjo_event[126] = GSWF(6010);
    tsuritenjo_event[127] = 1;

    gon_evt_majin2[0] = EVT_HELPER_CMD(2, 50);
    gon_evt_majin2[1] = EVT_HELPER_OP(LW(3));
    patch::writePatch(&gon_evt_majin2[3], gon_evt_majin2_item, sizeof(gon_evt_majin2_item));

    gon_06_init_evt[6] = GSWF(6012);
    gon_06_init_evt[7] = 1;
    gon_06_init_evt[317] = GSWF(6011);
    gon_06_init_evt[318] = 1;

    out_func[3] = GSWF(6012);
    out_func[4] = 1;

    gon_item_evt_07[1] = GSWF(6012);
    gon_item_evt_07[2] = 1;
    gon_item_evt_07[6] = GSWF(6012);
    gon_item_evt_07[7] = 1;

    gon_07_init_evt[22] = GSWF(6012);
    gon_07_init_evt[23] = 1;

    gon_evt_chk_08[2] = 12;

    gon_evt_unlock_08[6] = GSWF(6013);
    gon_evt_unlock_08[7] = 1;

    gon_08_init_evt[199] = GSWF(6013);
    gon_08_init_evt[200] = 1;
    gon_08_init_evt[204] = 12;

    gon_10_init_evt[35] = GSWF(6014);
    gon_10_init_evt[36] = 1;

    gonbaba_event[1384] = GSW(1711);
    gonbaba_event[1385] = 7;
    gonbaba_event[1387] = GSW(1711);
    gonbaba_event[1388] = 8;

    gon_11_init_evt[6] = GSWF(6014);
    gon_11_init_evt[8] = 1;
    gon_11_init_evt[14] = GSWF(6014);
    gon_11_init_evt[16] = 1;
    gon_11_init_evt[47] = GSWF(6014);
    gon_11_init_evt[48] = 1;
    gon_11_init_evt[90] = GSWF(6014);
    gon_11_init_evt[91] = 1;

    gon_chuchurina_sogu[1] = GSWF(6016);
    gon_chuchurina_sogu[2] = 1;
    gon_chuchurina_sogu[64] = GSWF(6016);
    gon_chuchurina_sogu[65] = 1;

    item_get00[33] = GSWF(6017);
    item_get00[34] = 1;

    gon_12_init_evt[6] = GSWF(6016);
    gon_12_init_evt[7] = 1;
    gon_12_init_evt[31] = GSWF(6017);
    gon_12_init_evt[32] = 1;

    gon_key_tbl_04[0] = 12;
    gon_key_tbl00_05[0] = 12;
    gon_key_tbl01_05[0] = 12;
    gon_key_tbl_08[0] = 12;
}
