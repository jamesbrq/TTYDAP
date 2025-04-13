#include "subrel_las.h"
#include "evt_cmd.h"
#include "patch.h"
#include "AP/rel_patch_definitions.h"
#include "ttyd/evt_case.h"
#include "ttyd/evt_map.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_msg.h"

#include <cstdint>

using namespace ttyd;

extern int32_t las_first_evt_00[];
extern int32_t las_00_init_evt[];
extern int32_t senkaron_event[];
extern int32_t kurokaron_init[];
extern int32_t kurokaron_talk[];
extern int32_t las_key_evt_05[];
extern int32_t las_unlock_evt_05[];
extern int32_t las_05_init_evt[];
extern int32_t door_matigai_evt[];
extern int32_t door_seikai_evt[];
extern int32_t mugen_kairou_init[];
extern int32_t las_first_evt_09[];
extern int32_t majyorin_evt_main[];
extern int32_t las_09_init_evt[];
extern int32_t tenkyugi_evt2[];
extern int32_t kagi_8_on[];
extern int32_t kagi_8_init[];
extern int32_t daiza_evt[];
extern int32_t las_10_init_evt[];
extern int32_t cloud_evt[];
extern int32_t las_19_init_evt[];
extern int32_t hosi_sw_check[];
extern int32_t hosi_init[];
extern int32_t las_21_init_evt[];
extern int32_t las_key_evt_22[];
extern int32_t las_unlock_evt_22[];
extern int32_t las_22_init_evt[];
extern int32_t las_23_init_evt[];
extern int32_t break_floor_evt[];
extern int32_t box_evt[];
extern int32_t las_24_init_evt[];
extern int32_t las_unlock_evt_25[];
extern int32_t las_25_init_evt[];
extern int32_t bonbaba_evt[];
extern int32_t bonbaba_init[];
extern int32_t las_26_init_evt[];
extern int32_t las_syuryo_init[];
extern int32_t las_first_evt_28[];
extern int32_t las_koopa_evt[];
extern int32_t las_shuryolight_init_28[];
extern int32_t las_28_init_evt[];
extern int32_t last_evt_3[];
extern int32_t last_evt_3_2[];
extern int32_t last_evt_4[];
extern int32_t las_shuryolight_init_29[];
extern int32_t las_29_init_evt[];
extern int32_t las_30_init_evt[];
extern int32_t key_check_evt_22[];
extern int32_t key_check_evt_25[];
extern int32_t las_key_tbl_05[];
extern int32_t las_key_tbl_22[];
extern int32_t las_key_tbl_25[];

// clang-format off
EVT_BEGIN(stairs_revert)
    IF_LARGE_EQUAL(GSW(1708), 8)
        USER_FUNC(evt_mario::evt_mario_key_onoff, 0)
        IF_EQUAL(GSWF(6112), 1)
            USER_FUNC(evt_msg::evt_msg_print, 0, PTR("raise_text"), 0, 0)
            USER_FUNC(evt_msg::evt_msg_select, 0, PTR("raise_text_yn"))
            IF_EQUAL(LW(0), 0)
                SET(GSWF(6112), 0)
                USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("raise_text2"))
            ELSE()
                USER_FUNC(evt_msg::evt_msg_continue)
            END_IF()
        ELSE()
            USER_FUNC(evt_msg::evt_msg_print, 0, PTR("lower_text"), 0, 0)
            USER_FUNC(evt_msg::evt_msg_select, 0, PTR("lower_text_yn"))
            IF_EQUAL(LW(0), 0)
                SET(GSWF(6112), 1)
                USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("lower_text2"))
            ELSE()
                USER_FUNC(evt_msg::evt_msg_continue)
            END_IF()
        END_IF()
        USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN(las_10_init_evt_evt)
    USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 1, PTR("hosi_off"), 1)
    USER_FUNC(evt_case::evt_run_case_evt, 9, 1, PTR("A_daiza1"), 0, PTR(&stairs_revert), 0)
    RETURN()
EVT_END()

EVT_BEGIN(las_10_init_evt_hook)
    RUN_CHILD_EVT(las_10_init_evt_evt)
    GOTO(&las_10_init_evt[85])
EVT_PATCH_END()

EVT_BEGIN(tenkyugi_evt2_evt)
    USER_FUNC(evt_msg::evt_msg_print_party, PTR("stg8_las_35"))
    SET(GSW(1708), 8)
    SET(GSWF(6112), 1)
    RETURN()
EVT_END()

EVT_BEGIN(tenkyugi_evt2_hook)
    RUN_CHILD_EVT(tenkyugi_evt2_evt)
    GOTO(&tenkyugi_evt2[113])
EVT_PATCH_END()
// clang-format on

namespace mod
{
    void main()
    {
        las_first_evt_00[160] = GSW(1708);
        las_first_evt_00[161] = 2;

        las_00_init_evt[17] = GSW(1708);
        las_00_init_evt[18] = 2;
        las_00_init_evt[23] = GSW(1708);
        las_00_init_evt[24] = 2;

        senkaron_event[1] = GSW(1708);
        senkaron_event[2] = 2;
        senkaron_event[843] = GSW(1708);
        senkaron_event[844] = 3;

        kurokaron_init[1] = GSW(1708);
        kurokaron_init[2] = 2;

        kurokaron_talk[3] = GSW(1708);
        kurokaron_talk[4] = 2;

        las_key_evt_05[1] = GSWF(6071);
        las_key_evt_05[2] = 1;

        las_unlock_evt_05[1] = GSW(1708);
        las_unlock_evt_05[2] = 4;

        las_05_init_evt[19] = GSW(1708);
        las_05_init_evt[20] = 2;
        las_05_init_evt[26] = GSW(1708);
        las_05_init_evt[27] = 3;
        las_05_init_evt[40] = GSW(1708);
        las_05_init_evt[41] = 4;

        door_matigai_evt[1] = GSW(1708);
        door_matigai_evt[2] = 4;

        door_seikai_evt[1] = GSW(1708);
        door_seikai_evt[2] = 4;
        door_seikai_evt[10] = GSW(1708);
        door_seikai_evt[11] = 5;

        mugen_kairou_init[3] = GSW(1708);
        mugen_kairou_init[4] = 4;

        las_first_evt_09[195] = GSW(1708);
        las_first_evt_09[196] = 6;

        majyorin_evt_main[632] = GSW(1708);
        majyorin_evt_main[633] = 9;

        las_09_init_evt[16] = GSW(1708);
        las_09_init_evt[17] = 5;
        las_09_init_evt[52] = GSW(1708);
        las_09_init_evt[53] = 5;
        las_09_init_evt[202] = GSW(1708);
        las_09_init_evt[203] = 8;
        las_09_init_evt[249] = GSWF(6072);
        las_09_init_evt[250] = 1;

        patch::writePatch(&tenkyugi_evt2[107], tenkyugi_evt2_hook, sizeof(tenkyugi_evt2_hook));
        tenkyugi_evt2[111] = 0;
        tenkyugi_evt2[112] = 0;

        kagi_8_on[1] = GSW(1708);
        kagi_8_on[2] = 7;

        kagi_8_init[1] = GSW(1708);
        kagi_8_init[2] = 7;

        daiza_evt[48] = GSW(1708);
        daiza_evt[49] = 7;

        patch::writePatch(&las_10_init_evt[78], las_10_init_evt_hook, sizeof(las_10_init_evt_hook));
        las_10_init_evt[82] = 0;
        las_10_init_evt[83] = 0;
        las_10_init_evt[1] = GSW(1708);
        las_10_init_evt[2] = 7;
        las_10_init_evt[61] = GSW(1708);
        las_10_init_evt[62] = 7;
        las_10_init_evt[88] = GSW(1708);
        las_10_init_evt[89] = 7;
        las_10_init_evt[128] = GSW(1708);
        las_10_init_evt[129] = 8;

        cloud_evt[1] = GSW(1708);
        cloud_evt[2] = 10;

        las_19_init_evt[11] = GSW(1708);
        las_19_init_evt[12] = 8;
        las_19_init_evt[37] = GSW(1708);
        las_19_init_evt[38] = 10;
        las_19_init_evt[76] = GSWF(6112);
        las_19_init_evt[77] = 1;
        las_19_init_evt[144] = GSWF(6112);
        las_19_init_evt[145] = 1;

        hosi_sw_check[291] = GSW(1708);
        hosi_sw_check[292] = 12;

        hosi_init[1] = GSW(1708);
        hosi_init[2] = 12;

        las_21_init_evt[1] = GSW(1708);
        las_21_init_evt[2] = 8;
        las_21_init_evt[28] = GSWF(6112);
        las_21_init_evt[29] = 1;
        las_21_init_evt[77] = GSWF(6112);
        las_21_init_evt[78] = 1;

        las_key_evt_22[1] = GSWF(6073);
        las_key_evt_22[2] = 1;

        las_unlock_evt_22[1] = GSW(1708);
        las_unlock_evt_22[2] = 11;

        key_check_evt_22[2] = 46;

        las_22_init_evt[19] = GSW(1708);
        las_22_init_evt[20] = 8;
        las_22_init_evt[26] = GSW(1708);
        las_22_init_evt[27] = 12;
        las_22_init_evt[115] = GSWF(6073);
        las_22_init_evt[116] = 1;
        las_22_init_evt[129] = GSW(1708);
        las_22_init_evt[130] = 11;
        las_22_init_evt[134] = 46;

        las_23_init_evt[1] = GSW(1708);
        las_23_init_evt[2] = 8;
        las_23_init_evt[28] = GSWF(6112);
        las_23_init_evt[29] = 1;

        break_floor_evt[1] = GSW(1708);
        break_floor_evt[2] = 13;

        box_evt[26] = GSWF(6074);
        box_evt[27] = 1;

        las_24_init_evt[44] = GSW(1708);
        las_24_init_evt[45] = 13;

        key_check_evt_25[2] = 46;

        las_unlock_evt_25[1] = GSW(1708);
        las_unlock_evt_25[2] = 14;

        las_25_init_evt[1] = GSW(1708);
        las_25_init_evt[2] = 8;
        las_25_init_evt[28] = GSWF(6112);
        las_25_init_evt[29] = 1;
        las_25_init_evt[51] = GSWF(6112);
        las_25_init_evt[52] = 1;
        las_25_init_evt[112] = 46;

        bonbaba_evt[795] = GSWF(6072);
        bonbaba_evt[796] = 1;

        bonbaba_init[1] = GSWF(6072);
        bonbaba_init[3] = 0;
        bonbaba_init[25] = 1;

        las_26_init_evt[1] = GSWF(6072);
        las_26_init_evt[2] = 0;
        las_26_init_evt[15] = GSW(1708);
        las_26_init_evt[16] = 16;
        las_26_init_evt[84] = GSW(1708);
        las_26_init_evt[85] = 16;
        las_26_init_evt[111] = GSWF(6072);
        las_26_init_evt[112] = 0;
        las_26_init_evt[120] = GSWF(6072);
        las_26_init_evt[122] = 0;
        las_26_init_evt[129] = 1;
        las_26_init_evt[153] = GSWF(6072);
        las_26_init_evt[154] = 1;
        las_26_init_evt[167] = GSW(1708);
        las_26_init_evt[168] = 16;

        las_syuryo_init[6] = GSW(1708);
        las_syuryo_init[7] = 14;
        las_syuryo_init[9] = GSW(1708);
        las_syuryo_init[10] = 15;

        las_first_evt_28[152] = GSW(1708);
        las_first_evt_28[153] = 15;

        las_koopa_evt[1661] = GSW(1708);
        las_koopa_evt[1662] = 16;

        las_shuryolight_init_28[1] = GSW(1708);
        las_shuryolight_init_28[2] = 14;
        las_shuryolight_init_28[4] = GSW(1708);
        las_shuryolight_init_28[5] = 16;

        las_28_init_evt[1] = GSW(1708);
        las_28_init_evt[2] = 16;
        las_28_init_evt[28] = GSW(1708);
        las_28_init_evt[29] = 14;
        las_28_init_evt[43] = GSW(1708);
        las_28_init_evt[44] = 16;
        las_28_init_evt[93] = GSW(1708);
        las_28_init_evt[94] = 14;
        las_28_init_evt[99] = GSW(1708);
        las_28_init_evt[100] = 15;

        last_evt_3[1339] = GSW(1708);
        last_evt_3[1340] = 17;

        last_evt_3_2[1822] = GSW(1708);
        last_evt_3_2[1823] = 17;

        last_evt_3_2[137] = 5;
        last_evt_3_2[181] = 400;

        last_evt_4[1821] = GSW(1708);
        last_evt_4[1822] = 18;

        las_shuryolight_init_29[1] = GSW(1708);
        las_shuryolight_init_29[2] = 16;

        las_29_init_evt[11] = GSW(1708);
        las_29_init_evt[13] = 16;
        las_29_init_evt[121] = 17;

        las_30_init_evt[1] = GSW(1708);
        las_30_init_evt[2] = 16;

        las_key_tbl_05[0] = 46;
        las_key_tbl_22[0] = 46;
        las_key_tbl_25[0] = 46;
    }

    void exit() {}
} // namespace mod
