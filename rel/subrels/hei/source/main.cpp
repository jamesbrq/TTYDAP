#include "subrel_hei.h"
#include "evt_cmd.h"
#include "patch.h"
#include "AP/rel_patch_definitions.h"
#include "ttyd/evt_case.h"
#include "ttyd/evt_item.h"
#include "ttyd/evt_map.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_npc.h"
#include "ttyd/evt_pouch.h"

#include <cstdint>

using namespace ttyd;

extern int32_t hei_first_evt[];
extern int32_t hei_gonbaba_evt[];
extern int32_t hei_first_evt_dokan[];
extern int32_t hei_dokan_evt[];
extern int32_t hei_setting_sun_stone[];
extern int32_t hei_sun_stone_set_event[];
extern int32_t hei_setting_moon_stone[];
extern int32_t hei_moon_stone_set_event[];
extern int32_t hei_00_koopa_evt[];
extern int32_t hei_00_init_evt[];
extern int32_t hei_bridge_evt[];
extern int32_t hei_nokotaro_get[];
extern int32_t hei_nokotaro_nakama[];
extern int32_t hei_nokotarou_talk_2[];
extern int32_t hei_evt_nokorin[];
extern int32_t hei_peach_mail[];
extern int32_t hei_01_init_evt[];
extern int32_t hei_door_open_03[];
extern int32_t hei_03_init_evt[];
extern int32_t hei_door_open_05[];
extern int32_t hei_05_init_evt[];
extern int32_t hei_a_sekizou_1[];
extern int32_t hei_07_init_evt[];
extern int32_t hei_evt_golden[];
extern int32_t hei_10_init_evt[];
extern int32_t hei_evt_anm_kan[];
extern int32_t hei_13_init_evt[];

const char koops[] = "\x83\x6D\x83\x52\x83\x5E\x83\x8D\x83\x45";

// clang-format off
EVT_BEGIN(hei_party_evt)
    USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    USER_FUNC(evt_item::evt_item_entry, PTR("item01"), LW(3), LW(0), LW(1), LW(2), 16, GSWF(6077), 0)
    USER_FUNC(evt_item::evt_item_get_item, PTR("item01"))
    WAIT_MSEC(800)
    SET(GSW(1701), 10)
    USER_FUNC(evt_npc::evt_npc_set_position, PTR(&koops), 0, -1000, 0)
    RETURN()
EVT_END()

EVT_BEGIN(stones_evt)
    IF_EQUAL(GSWF(1776), 0)
        USER_FUNC(evt_pouch::evt_pouch_check_item, 62, LW(0))
        IF_LARGE_EQUAL(LW(0), 1)
            USER_FUNC(evt_case::evt_run_case_evt, 9, 1, PTR("a_isi_4"), 0, PTR(&hei_sun_stone_set_event), 0)
        END_IF()
    ELSE()
        RUN_CHILD_EVT(&hei_setting_sun_stone)
    END_IF()
    USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 1, PTR("tukinoisi"), 1)
    
    IF_EQUAL(GSWF(1777), 0)
        USER_FUNC(evt_pouch::evt_pouch_check_item, 61, LW(0))
        IF_LARGE_EQUAL(LW(0), 1)
            USER_FUNC(evt_case::evt_run_case_evt, 9, 1, PTR("a_isi_3"), 0, PTR(&hei_moon_stone_set_event), 0)
        END_IF()
    ELSE()
        RUN_CHILD_EVT(&hei_setting_moon_stone)
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN(stones_hook)
	RUN_CHILD_EVT(stones_evt)
	GOTO(&hei_00_init_evt[213])
EVT_PATCH_END()
// clang-format on

namespace mod
{
    void main()
    {
        hei_first_evt[187] = GSW(1701);
        hei_first_evt[188] = 1;

        hei_gonbaba_evt[19] = EVT_HELPER_CMD(2, 24);
        hei_gonbaba_evt[21] = 0;
        hei_gonbaba_evt[424] = GSW(1701);
        hei_gonbaba_evt[425] = 1;
        hei_gonbaba_evt[434] = GSW(1701);
        hei_gonbaba_evt[435] = 2;

        hei_first_evt_dokan[1] = GSW(1701);
        hei_first_evt_dokan[2] = 1;

        hei_dokan_evt[324] = GSW(1711);
        hei_dokan_evt[325] = 1;

        hei_00_koopa_evt[197] = GSW(1713);
        hei_00_koopa_evt[198] = 13;

        hei_00_init_evt[1] = GSW(1713);
        hei_00_init_evt[2] = 13;
        hei_00_init_evt[25] = GSW(1701);
        hei_00_init_evt[26] = 1;
        hei_00_init_evt[31] = GSW(1701);
        hei_00_init_evt[32] = 2;
        hei_00_init_evt[37] = GSW(1711);
        hei_00_init_evt[38] = 1;
        hei_00_init_evt[107] = GSW(1711);
        hei_00_init_evt[108] = 1;
        patch::writePatch(&hei_00_init_evt[169], stones_hook, sizeof(stones_hook));
        hei_00_init_evt[173] = 0;
        hei_00_init_evt[174] = 0;
        hei_00_init_evt[246] = GSW(1713);
        hei_00_init_evt[247] = 13;
        hei_00_init_evt[249] = GSW(1708);
        hei_00_init_evt[250] = 18;
        hei_00_init_evt[283] = GSW(1708);
        hei_00_init_evt[284] = 18;

        hei_bridge_evt[21] = GSWF(6001);
        hei_bridge_evt[22] = 1;

        hei_nokotaro_get[37] = EVT_HELPER_CMD(2, 50);
        hei_nokotaro_get[38] = EVT_HELPER_OP(LW(3));
        patch::writePatch(&hei_nokotaro_get[40], hei_party_evt, sizeof(hei_party_evt));

        hei_nokotaro_nakama[11] = GSW(1701);
        hei_nokotaro_nakama[12] = 9;

        hei_nokotarou_talk_2[19] = GSW(1701);
        hei_nokotarou_talk_2[20] = 10;

        hei_evt_nokorin[10] = GSW(1701);
        hei_evt_nokorin[11] = 10;

        hei_peach_mail[277] = GSW(1711);
        hei_peach_mail[278] = 15;

        hei_01_init_evt[81] = GSWF(6001);
        hei_01_init_evt[82] = 1;
        hei_01_init_evt[117] = GSW(1711);
        hei_01_init_evt[118] = 14;
        hei_01_init_evt[137] = GSWF(6001);
        hei_01_init_evt[138] = 1;

        hei_door_open_03[89] = GSW(1701);
        hei_door_open_03[90] = 6;

        hei_03_init_evt[33] = GSW(1701);
        hei_03_init_evt[34] = 6;

        hei_door_open_05[1] = GSW(1701);
        hei_door_open_05[2] = 7;
        hei_door_open_05[90] = GSW(1701);
        hei_door_open_05[91] = 7;

        hei_05_init_evt[27] = GSW(1701);
        hei_05_init_evt[28] = 7;

        hei_a_sekizou_1[579] = GSW(1701);
        hei_a_sekizou_1[580] = 8;
        hei_a_sekizou_1[991] = GSW(1701);
        hei_a_sekizou_1[992] = 8;

        hei_07_init_evt[56] = GSW(1701);
        hei_07_init_evt[57] = 8;

        hei_evt_golden[267] = GSW(1701);
        hei_evt_golden[268] = 9;

        hei_10_init_evt[41] = GSW(1701);
        hei_10_init_evt[42] = 9;

        hei_evt_anm_kan[23] = GSWF(6102);
        hei_evt_anm_kan[45] = GSWF(6102);

        hei_13_init_evt[72] = 1;
    }

    void exit() {}
} // namespace mod
