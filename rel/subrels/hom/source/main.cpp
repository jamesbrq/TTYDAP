#include "subrel_hom.h"
#include "evt_cmd.h"
#include "patch.h"
#include "AP/rel_patch_definitions.h"
#include "ttyd/evt_case.h"
#include "ttyd/evt_hit.h"
#include "ttyd/evt_map.h"

#include <cstdint>

using namespace ttyd;

extern int32_t hom_evt_carry_up_bridge[];
extern int32_t hom_init_powan_toron_konari_papa_gurume[];
extern int32_t hom_talk_ekiin1[];
extern int32_t hom_init_ekiin2[];
extern int32_t hom_talk_ekiin2[];
extern int32_t hom_evt_lock[];
extern int32_t hom_evt_train_door[];
extern int32_t hom_evt_train[];
extern int32_t hom_evt_hom_touchaku[];
extern int32_t hom_00_init_evt[];
extern int32_t hom_10_evt_resha_start_hom_00[];
extern int32_t hom_evt_sub_mail_event[];

// clang-format off
EVT_BEGIN(hom_00_init_evt_evt)
    IF_EQUAL(GSW(1720), 8)
        IF_SMALL(GSW(1706), 30)
            USER_FUNC(evt_hit::evt_hitobj_attr_onoff, 0, 1, PTR("n_bero_1"), 8388608)
            USER_FUNC(evt_case::evt_run_case_evt, 8, 1, PTR("n_bero_1"), 0, PTR(&hom_evt_train), 0)
            USER_FUNC(evt_map::evt_map_playanim, PTR("n_1_door_open"), 2, 0)
        END_IF()
    END_IF()
    RUN_EVT(&hom_evt_sub_mail_event)
    USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("train_2"), 2)
    RETURN()
EVT_END()

EVT_BEGIN(hom_00_init_evt_hook)
    RUN_CHILD_EVT(hom_00_init_evt_evt)
    RETURN()
EVT_END()

EVT_BEGIN(hom_evt_train_evt)
    SET(GSW(1720), 9)
    SET(GSW(1706), 30)
    WAIT_FRM(1)
    RETURN()
EVT_END()

EVT_BEGIN(hom_evt_train_hook)
    RUN_CHILD_EVT(hom_evt_train_evt)
    GOTO(&hom_evt_train[256])
EVT_PATCH_END()
// clang-format on

namespace mod
{
    void main()
    {
        hom_evt_carry_up_bridge[82] = GSW(1720);
        hom_evt_carry_up_bridge[83] = 2;

        hom_init_powan_toron_konari_papa_gurume[1] = GSW(1720);
        hom_init_powan_toron_konari_papa_gurume[3] = 8;
        hom_init_powan_toron_konari_papa_gurume[12] = GSW(1720);
        hom_init_powan_toron_konari_papa_gurume[13] = 1;

        hom_talk_ekiin1[1] = GSW(1720);
        hom_talk_ekiin1[3] = 2;
        hom_talk_ekiin1[7] = 8;
        hom_talk_ekiin1[15] = 9;

        hom_init_ekiin2[1] = GSW(1720);
        hom_init_ekiin2[3] = 8;

        hom_talk_ekiin2[1] = GSW(1720);
        hom_talk_ekiin2[3] = 8;
        hom_talk_ekiin2[11] = 9;

        hom_evt_lock[6] = GSW(1720);
        hom_evt_lock[7] = 3;

        hom_evt_train_door[1] = GSW(1720);
        hom_evt_train_door[3] = 8;

        patch::writePatch(&hom_evt_train[251], hom_evt_train_hook, sizeof(hom_evt_train_hook));
        hom_evt_train[255] = 0;

        hom_evt_hom_touchaku[452] = GSW(1720);
        hom_evt_hom_touchaku[453] = 1;

        hom_00_init_evt[75] = GSW(1706);
        hom_00_init_evt[76] = 43;
        hom_00_init_evt[165] = GSW(1720);
        hom_00_init_evt[166] = 1;
        hom_00_init_evt[180] = GSW(1706);
        hom_00_init_evt[181] = 44;
        hom_00_init_evt[198] = GSW(1720);
        hom_00_init_evt[199] = 3;
        hom_00_init_evt[218] = GSW(1720);
        hom_00_init_evt[219] = 8;
        patch::writePatch(&hom_00_init_evt[226], hom_00_init_evt_hook, sizeof(hom_00_init_evt_hook));

        hom_10_evt_resha_start_hom_00[49] = GSW(1706);
        hom_10_evt_resha_start_hom_00[50] = 43;
    }

    void exit() {}
} // namespace mod
