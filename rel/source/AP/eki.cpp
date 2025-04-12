#include "evt_cmd.h"
#include "patch.h"
#include <AP/rel_patch_definitions.h>
#include <ttyd/evt_mario.h>
#include <ttyd/evt_msg.h>
#include <ttyd/evt_pouch.h>
#include <ttyd/evt_snd.h>
#include <ttyd/evt_window.h>

using namespace mod;
using namespace ttyd;

extern int32_t evt_bridge2[];
extern int32_t eki_evt_origami_00[];
extern int32_t eki_evt_meet_moamoa[];
extern int32_t eki_evt_moamoa_escape[];
extern int32_t evt_elv_down[];
extern int32_t evt_elv_up[];
extern int32_t evt_elv[];
extern int32_t item_tbl[];
extern int32_t eki_00_init_evt[];
extern int32_t eki_evt_origami_01[];
extern int32_t evt_lock[];
extern int32_t eki_01_init_evt[];
extern int32_t eki_evt_key_02[];
extern int32_t eki_02_init_evt[];
extern int32_t eki_evt_poster[];
extern int32_t evt_chk[];
extern int32_t evt_boots[];
extern int32_t evt_elvkey[];
extern int32_t eki_05_init_evt[];
extern int32_t evt_box_open[];
extern int32_t evt_sw_complete[];
extern int32_t eki_06_init_evt[];

// clang-format off
EVT_BEGIN(evt_elv_evt)
	IF_SMALL(GSW(1720), 4)
        USER_FUNC(evt_mario::evt_mario_key_onoff, 0)
	    USER_FUNC(evt_pouch::evt_pouch_check_item, 26, LW(0))
	    IF_SMALL_EQUAL(LW(0), 0)
		    USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg6_eki_06"), 0, 0)
		    USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
		    RETURN()
	    END_IF()
		USER_FUNC(evt_window::evt_win_item_select, 0, PTR(&item_tbl), LW(1), 0)
		IF_EQUAL(LW(1), -1)
			USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
			SET(LW(0), 0)
			RETURN()
		END_IF()
		USER_FUNC(evt_pouch::evt_pouch_remove_item, 26, LW(0))
		SET(GSW(1720), 4)
		USER_FUNC(evt_snd::evt_snd_sfxon, PTR("SFX_STG6_ELEVATOR_KEY1"), 0)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg6_eki_07"), 0, 0)
		SET(LW(0), 1)
		RETURN()
    ELSE()
        SET(LW(0), 1)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(evt_elv_hook)
	RUN_CHILD_EVT(evt_elv_evt)
	IF_EQUAL(LW(0), 0)
		RETURN()
	END_IF()
	GOTO(&evt_elv[38])
EVT_END()
// clang-format on

void ApplyEkiPatches()
{
    evt_bridge2[111] = GSW(1720);
    evt_bridge2[112] = 8;

    eki_evt_origami_00[183] = GSW(1720);
    eki_evt_origami_00[184] = 7;

    eki_evt_meet_moamoa[204] = GSW(1720);
    eki_evt_meet_moamoa[205] = 5;

    eki_evt_moamoa_escape[40] = GSW(1720);
    eki_evt_moamoa_escape[41] = 6;

    evt_elv_down[230] = GSW(1720);
    evt_elv_down[231] = 6;
    evt_elv_down[305] = GSW(1720);
    evt_elv_down[306] = 6;
    evt_elv_down[339] = GSW(1720);
    evt_elv_down[341] = 6;
    evt_elv_down[343] = 6;
    evt_elv_down[362] = GSW(1720);
    evt_elv_down[363] = 7;
    evt_elv_down[398] = GSW(1720);
    evt_elv_down[399] = 7;
    evt_elv_down[532] = GSW(1720);
    evt_elv_down[533] = 5;
    evt_elv_down[538] = GSW(1720);
    evt_elv_down[539] = 5;

    evt_elv_up[116] = GSW(1720);
    evt_elv_up[117] = 6;
    evt_elv_up[191] = GSW(1720);
    evt_elv_up[192] = 6;
    evt_elv_up[325] = GSW(1720);
    evt_elv_up[326] = 7;
    evt_elv_up[329] = GSW(1720);
    evt_elv_up[330] = 7;
    evt_elv_up[381] = GSW(1720);
    evt_elv_up[382] = 6;

    patch::writePatch(&evt_elv[0], evt_elv_hook, sizeof(evt_elv_hook));
    evt_elv[25] = GSW(1720);
    evt_elv[26] = 4;

    eki_00_init_evt[26] = GSW(1720);
    eki_00_init_evt[27] = 0; // Unused
    eki_00_init_evt[49] = GSW(1720);
    eki_00_init_evt[50] = 6;

    eki_evt_origami_01[116] = GSWF(6058);
    eki_evt_origami_01[117] = 1;

    evt_lock[6] = GSWF(6060);
    evt_lock[7] = 1;

    eki_01_init_evt[15] = GSWF(6058);
    eki_01_init_evt[16] = 1;
    eki_01_init_evt[68] = GSWF(6060);
    eki_01_init_evt[69] = 1;

    eki_evt_key_02[1] = GSWF(6059);
    eki_evt_key_02[2] = 1;

    eki_02_init_evt[25] = GSWF(6059);
    eki_02_init_evt[26] = 1;

    eki_evt_poster[1] = GSWF(6061);
    eki_evt_poster[2] = 1;

    evt_chk[66] = GSWF(6061);
    evt_chk[67] = 1;
    evt_chk[69] = GSWF(6061);
    evt_chk[70] = 1;

    evt_boots[35] = GSWF(6064);
    evt_boots[36] = 1;

    evt_elvkey[1] = GSWF(6065);
    evt_elvkey[2] = 1;

    eki_05_init_evt[34] = GSWF(6064);
    eki_05_init_evt[35] = 1;
    eki_05_init_evt[48] = GSWF(6065);
    eki_05_init_evt[49] = 1;

    evt_box_open[1] = GSW(1722);
    evt_box_open[2] = 1;
    evt_box_open[108] = GSW(1722);
    evt_box_open[109] = 1;

    evt_sw_complete[265] = GSW(1722);
    evt_sw_complete[266] = 2;

    eki_06_init_evt[20] = GSW(1722);
    eki_06_init_evt[22] = 1;
    eki_06_init_evt[29] = 1;
    eki_06_init_evt[100] = GSW(1722);
    eki_06_init_evt[101] = 1;
}
