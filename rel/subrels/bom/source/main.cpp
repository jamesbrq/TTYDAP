#include "AP/rel_patch_definitions.h"
#include "common.h"
#include "evt_cmd.h"
#include "patch.h"
#include "subrel_bom.h"
#include "ttyd/evt_item.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_msg.h"
#include "ttyd/evt_pouch.h"
#include "ttyd/evt_snd.h"

#include <cstdint>

using namespace ttyd;
using namespace mod;
using namespace ttyd::common;

extern int32_t bom_first_evt_00[];
extern int32_t bom_00_init_evt[];
extern int32_t bom_first_evt_01[];
extern int32_t bom_init_soncho_01[];
extern int32_t bom_talk_soncho_01[];
extern int32_t bom_talk_nitoro[];
extern int32_t bom_talk_pentrit[];
extern int32_t bom_talk_surary[];
extern int32_t bom_talk_murabito_d[];
extern int32_t bom_talk_murabito_e_01[];
extern int32_t bom_talk_murabito_f_01[];
extern int32_t bom_talk_murabito_g_01[];
extern int32_t bom_talk_murabito_h_01[];
extern int32_t bom_talk_murabito_i_01[];
extern int32_t bom_talk_white[];
extern int32_t bom_musume_talk[];
extern int32_t bom_minnnanokoe_evt[];
extern int32_t bom_01_init_evt[];
extern int32_t bom_evt_taihou[];
extern int32_t bom_init_soncho_02[];
extern int32_t bom_talk_soncho_02[];
extern int32_t bom_talk_murabito_e_02[];
extern int32_t bom_talk_murabito_f_02[];
extern int32_t bom_talk_murabito_g_02[];
extern int32_t bom_talk_murabito_h_02[];
extern int32_t bom_talk_murabito_i_02[];
extern int32_t bom_evt_white_bed[];
extern int32_t bom_kiza_talk[];
extern int32_t bom_02_init_evt[];

// Assembly
extern int32_t bom_bom1000_jump[];
extern int32_t bom_jump_minnnanokoe[];

// clang-format off
EVT_BEGIN(bom_talk_soncho_02_evt)
    IF_EQUAL(GSWF(6114), 1)
        RUN_CHILD_EVT(&bom_evt_taihou)
        RETURN()
    END_IF()
    USER_FUNC(evt_msg::evt_msg_print, 0, PTR("goldbob_guide"), 0, PTR("me"))
    USER_FUNC(evt_pouch::evt_pouch_check_item, ItemId::GOLDBOB_GUIDE, LW(0))
    IF_EQUAL(LW(0), 0)
        USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_VOICE_MARIO_NO1"), LW(0), LW(1), LW(2), 0)
        USER_FUNC(evt_mario::evt_mario_set_pose, PTR("M_N_B"))
        WAIT_MSEC(1000)
        USER_FUNC(evt_msg::evt_msg_print, 0, PTR("no_goldbob_guide"), 0, PTR("me"))
    ELSE()
        USER_FUNC(evt_mario::evt_mario_set_pose, PTR("M_C_3"))
        USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 20)
        ADD(LW(1), 15)
        USER_FUNC(evt_item::evt_item_entry, PTR("dummy"), ItemId::GOLDBOB_GUIDE, LW(0), LW(1), LW(2), 17, -1, 0)
        WAIT_MSEC(1000)
        USER_FUNC(evt_mario::evt_mario_set_pose, PTR("M_S_1"))
        USER_FUNC(evt_item::evt_item_delete, PTR("dummy"))
        USER_FUNC(evt_msg::evt_msg_print, 0, PTR("give_goldbob_guide"), 0, PTR("me"))
        SET(GSWF(6114), 1)
        USER_FUNC(evt_pouch::evt_pouch_remove_item, ItemId::GOLDBOB_GUIDE, LW(0))
    END_IF()
    IF_EQUAL(GSWF(6114), 1)
        RUN_CHILD_EVT(&bom_evt_taihou)
    END_IF()
    RETURN()
EVT_END()
// clang-format on

namespace mod
{
    void main()
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

        bom_talk_nitoro[1] = GSW(1707);
        bom_talk_nitoro[3] = 1;
        bom_talk_nitoro[11] = 5;
        bom_talk_nitoro[19] = 6;

        bom_talk_pentrit[1] = GSW(1707);
        bom_talk_pentrit[3] = 1;
        bom_talk_pentrit[11] = 5;
        bom_talk_pentrit[19] = 6;

        bom_talk_surary[1] = GSW(1707);
        bom_talk_surary[3] = 1;
        bom_talk_surary[11] = 5;
        bom_talk_surary[19] = 6;

        bom_talk_murabito_d[1] = GSW(1707);
        bom_talk_murabito_d[3] = 1;
        bom_talk_murabito_d[11] = 5;
        bom_talk_murabito_d[19] = 6;

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

        bom_minnnanokoe_evt[413] = 400;

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

        bom_evt_taihou[182] = GSW(1707);
        bom_evt_taihou[183] = 5;

        bom_init_soncho_02[1] = GSW(1707);
        bom_init_soncho_02[3] = 5;

        bom_talk_soncho_02[6] = GSW(1707);
        bom_talk_soncho_02[8] = 1;
        bom_talk_soncho_02[16] = 3;
        bom_talk_soncho_02[323] = GSW(1707);
        bom_talk_soncho_02[324] = 3;
        bom_talk_soncho_02[334] = 4;
        bom_talk_soncho_02[410] = 5;
        bom_talk_soncho_02[412] = EVT_HELPER_OP(bom_talk_soncho_02_evt);

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

        bom_evt_white_bed[492] = GSW(1707);
        bom_evt_white_bed[493] = 4;

        bom_kiza_talk[1] = GSW(1707);
        bom_kiza_talk[3] = 3;
        bom_kiza_talk[11] = 5;

        bom_02_init_evt[11] = GSW(1708);
        bom_02_init_evt[12] = 16;
        bom_02_init_evt[62] = GSW(1707);
        bom_02_init_evt[64] = 1;
        bom_02_init_evt[65] = 5;
        bom_02_init_evt[155] = GSW(1708);
        bom_02_init_evt[156] = 16;

        bom_bom1000_jump[60] = 0x3883082C; // addi r4, r4, 0x82C GSW(1708)
        bom_bom1000_jump[90] = 0x2C030011; // cmpwi r3, 0x11

        bom_jump_minnnanokoe[44] = 0x3883082C;  // addi r4, r4, 0x82C GSW(1708)
        bom_jump_minnnanokoe[76] = 0x2C030011; // cmpwi r3, 0x11
    }

    void exit() {}
} // namespace mod
