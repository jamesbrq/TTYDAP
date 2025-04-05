#include "evt_cmd.h"
#include "ttyd/evtmgr.h"
#include "ttyd/evtmgr_cmd.h"
#include "ttyd/swdrv.h"
#include "patch.h"
#include <AP/rel_patch_definitions.h>
#include <ttyd/evt_aji.h>
#include <ttyd/evt_bero.h>
#include <ttyd/evt_case.h>
#include <ttyd/evt_mario.h>
#include <ttyd/evt_msg.h>
#include <ttyd/evt_npc.h>
#include <ttyd/evt_shuryolight.h>
#include <ttyd/evt_snd.h>

using namespace mod;
using namespace ttyd;
using namespace ttyd::evtmgr;
using namespace ttyd::evtmgr_cmd;
using namespace ttyd::swdrv;

extern int32_t bero_peach_data[];
extern int32_t aji_first_evt[];
extern int32_t evt_door_msg_00[];
extern int32_t aji_00_init_evt[];
extern int32_t elv_unlock1[];
extern int32_t elv_unlock2[];
extern int32_t ele_2[];
extern int32_t evt_elv_init[];
extern int32_t evt_key_init[];
extern int32_t evt_aikotoba[];
extern int32_t peach_evt_stg3_01[];
extern int32_t peach_evt_ele_2[];
extern int32_t peach_evt_ele_init[];
extern int32_t peach_evt_key_init[];
extern int32_t peach_evt_init[];
extern int32_t teshita1_init[];
extern int32_t teshita2_init[];
extern int32_t teshita3_init[];
extern int32_t aji_01_init_evt[];
extern int32_t aji_evt_box[];
extern int32_t aji_02_init_evt[];
extern int32_t evt_zenmai[];
extern int32_t aji_03_init_evt[];
extern int32_t evt_item[];
extern int32_t kuizu_sekizou[];
extern int32_t aji_04_init_evt[];
extern int32_t bero_peach_evt[];
extern int32_t peach_evt_kusuri_nomu[];
extern int32_t aji_06_init_evt[];
extern int32_t tensou_ride[];
extern int32_t aji_08_init_evt[];
extern int32_t peach_evt_kou_in[];
extern int32_t peach_evt_kou_out[];
extern int32_t peach_evt_kou1_out[];
extern int32_t peach_evt_kou2_out[];
extern int32_t peach_evt_stg3_09[];
extern int32_t evt_memo1[];
extern int32_t aji_09_init_evt[];
extern int32_t monitor_init[];
extern int32_t peach_evt_stg3_syuryo_talk[];
extern int32_t syuryo_init[];
extern int32_t peach_evt_stg1_10[];
extern int32_t peach_evt_stg2_10[];
extern int32_t peach_evt_stg3_10[];
extern int32_t peach_evt_stg4_10[];
extern int32_t peach_evt_stg5_10[];
extern int32_t peach_evt_stg6_10[];
extern int32_t aji_10_init_evt[];
extern int32_t peach_evt_bero[];
extern int32_t peach_evt_tana2[];
extern int32_t peach_evt_stg5_11[];
extern int32_t aji_11_init_evt[];
extern int32_t evt_snd_stop[];
extern int32_t aji_evt_sw[];
extern int32_t evt_bcv_sw[];
extern int32_t evt_door_msg_13[];
extern int32_t aji_13_init_evt[];
extern int32_t evt_boss2[];
extern int32_t aji_14_init_evt[];
extern int32_t aji_15_init_evt[];
extern int32_t peach_evt_stg1_16[];
extern int32_t peach_evt_stg1_1[];
extern int32_t peach_evt_stg2_1[];
extern int32_t peach_evt_stg3_16[];
extern int32_t peach_evt_stg6_16[];
extern int32_t aji_16_init_evt[];
extern int32_t evt_emergency[];
extern int32_t evt_last2[];
extern int32_t peach_evt_stg1_17[];
extern int32_t peach_evt_stg2_17[];
extern int32_t peach_evt_stg3_1[];
extern int32_t peach_evt_stg4_17[];
extern int32_t peach_evt_stg5_17[];
extern int32_t peach_evt_stg5_1[];
extern int32_t peach_evt_stg6_17[];
extern int32_t aji_17_init_evt[];
extern int32_t evt_machine[];
extern int32_t aji_18_init_evt[];
extern int32_t evt_hajimete[];
extern int32_t aji_19_init_evt[];
extern int32_t npcEnt_10[];
extern int32_t shuryolight_pow_normal;
extern int32_t shuryolight_head_normal;
extern int32_t shuryolight_tue_normal;

// Assembly
extern int32_t check_kou_evt[];
extern int32_t mario_chk[];

const char npc1[] = "\x8C\xA4\x8B\x86\x88\xF5";
const char npc2[] = "\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8C\xA4\x8B\x86\x88\xF5";
const char npc3[] = "\x82\xDD\x82\xCD\x82\xE8";
const char npc4[] = "\x83\x6F\x83\x8A\x83\x41\x81\x5B\x83\x93\x83\x4A\x83\x58\x83\x5E\x83\x80";
const char npcEnt3[] = "\x83\x6F\x83\x8A\x83\x41\x81\x5B\x83\x93\x83\x4A\x83\x58\x83\x5E\x83\x80";
const char npcEnt4[] = "\x83\x56\x83\x85\x83\x8A\x83\x87\x81\x5B";

EVT_DEFINE_USER_FUNC(checkIntermission)
{
    (void)isFirstCall;  
    int currentSW = swByteGet(1711);
    bool isIntermission = false;
    if (currentSW >= 8 && currentSW <= 11)
        isIntermission = true;
    currentSW = swByteGet(1713);
    if (currentSW >= 11 && currentSW <= 13)
        isIntermission = true;
    currentSW = swByteGet(1703);
    if (currentSW >= 20 && currentSW <= 25)
        isIntermission = true;
    currentSW = swByteGet(1715);
    if (currentSW >= 8 && currentSW <= 10)
        isIntermission = true;
    currentSW = swByteGet(1717);
    if (currentSW >= 18 && currentSW <= 22)
        isIntermission = true;
    currentSW = swByteGet(1706);
    if (currentSW >= 43 && currentSW <= 45)
        isIntermission = true;
    
    if (isIntermission)
        evtSetValue(evt, evt->evtArguments[0], 1);
    else
        evtSetValue(evt, evt->evtArguments[0], 0);
    return 2;
}

// clang-format off
EVT_BEGIN(aji_00_init_evt_evt)
	IF_LARGE_EQUAL(GSW(1707), 20)
		IF_SMALL_EQUAL(GSW(1708), 18)
			SET(LW(0), PTR("w_bero"))
			RUN_CHILD_EVT(evt_bero::bero_case_switch_off)
			USER_FUNC(evt_case::evt_run_case_evt, 9, 1, PTR("w_bero"), 0, PTR(evt_door_msg_00), 0)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(aji_00_init_evt_hook)
	RUN_CHILD_EVT(aji_00_init_evt_evt)
	RETURN()
EVT_END()

EVT_BEGIN(aji_10_init_evt_evt1)
    USER_FUNC(checkIntermission, LW(0))
	IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG7_DUN1"))
        USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG7_AJI5"))
    ELSE()
        USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG7_DUN2"))
        USER_FUNC(evt_snd::evt_snd_envon, 272, PTR("ENV_STG7_AJI5"))
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(aji_10_init_evt_hook1)
	RUN_CHILD_EVT(aji_10_init_evt_evt1)
	GOTO(&aji_10_init_evt[21])
EVT_PATCH_END()

EVT_BEGIN(aji_10_init_evt_evt2)
    USER_FUNC(checkIntermission, LW(0))
	IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(evt_npc::evt_npc_setup, PTR(&npcEnt_10))
        USER_FUNC(evt_npc::evt_npc_check_delete, PTR(&npcEnt3))
        USER_FUNC(evt_shuryolight::evt_shuryolight_init, PTR(&npcEnt4))
        USER_FUNC(evt_shuryolight::evt_shuryolight_run_evt, PTR(&shuryolight_pow_normal), PTR(&shuryolight_head_normal), PTR(&shuryolight_tue_normal))
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(aji_10_init_evt_hook2)
	RUN_CHILD_EVT(aji_10_init_evt_evt2)
	GOTO(&aji_10_init_evt[50])
EVT_PATCH_END()

EVT_BEGIN(aji_16_init_evt_evt)
    USER_FUNC(checkIntermission, LW(0))
	IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(evt_mario::evt_mario_kill_party, 0)
        USER_FUNC(evt_mario::evt_mario_set_mode, 1)
    ELSE()
        USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_EVT_PEACH1"))
        USER_FUNC(evt_snd::evt_snd_envoff, 512)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(aji_16_init_evt_hook)
	RUN_CHILD_EVT(aji_16_init_evt_evt)
	GOTO(&aji_16_init_evt[140])
EVT_PATCH_END()

EVT_BEGIN(peach_evt_ele_2_evt)
	IF_EQUAL(GSW(1703), 22)
		SET(LW(8), 2)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1717), 19)
		SET(LW(8), 2)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("peach_aji_40"), 0, PTR("mario"))
	USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
	RETURN()
EVT_END()

EVT_BEGIN(peach_evt_ele_2_hook)
	RUN_CHILD_EVT(peach_evt_ele_2_evt)
	IF_NOT_EQUAL(LW(8), 2)
		RETURN()
	END_IF()
	GOTO(&peach_evt_ele_2[33])
EVT_PATCH_END()

EVT_BEGIN(teshita2_init_evt)
	IF_EQUAL(GSW(1703), 23)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, 1000, -15)
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1717), 20)
		IF_SMALL_EQUAL(GSW(1717), 22)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, 1000, -15)
			USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 6, 0)
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 1, 0)
	USER_FUNC(evt_npc::evt_npc_flag_onoff, 1, PTR("me"), 160)
	USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 6, 0)
	RETURN()
EVT_END()

EVT_BEGIN(teshita2_init_hook)
	RUN_CHILD_EVT(teshita2_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(teshita3_init_evt)
	IF_EQUAL(GSW(1703), 23)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 750, 1000, 20)
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1717), 20)
		IF_SMALL_EQUAL(GSW(1717), 22)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 750, 1000, 20)
			USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 6, 0)
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 1, 0)
	USER_FUNC(evt_npc::evt_npc_flag_onoff, 1, PTR("me"), 160)
	USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 6, 0)
	RETURN()
EVT_END()

EVT_BEGIN(teshita3_init_hook)
	RUN_CHILD_EVT(teshita3_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(aji_10_init_evt_evt)
	IF_EQUAL(GSW(1711), 8)
		USER_FUNC(evt_mario::evt_mario_set_mode, 1)
		RUN_CHILD_EVT(monitor_init)
		RUN_EVT(peach_evt_stg1_10)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1713), 11)
		USER_FUNC(evt_mario::evt_mario_set_mode, 1)
		USER_FUNC(evt_npc::evt_npc_entry, PTR(npc1), PTR("c_zako_m"))
		USER_FUNC(evt_npc::evt_npc_set_tribe, PTR(npc1), PTR(npc2))
		USER_FUNC(evt_npc::evt_npc_set_stay_anim, PTR(npc1))
		USER_FUNC(evt_npc::evt_npc_flag_onoff, 1, PTR(npc1), 1536)
		RUN_EVT(peach_evt_stg2_10)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1703), 20)
		USER_FUNC(evt_mario::evt_mario_set_mode, 1)
		RUN_EVT(peach_evt_stg3_10)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1703), 23)
		USER_FUNC(evt_mario::evt_mario_set_mode, 1)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1715), 9)
		USER_FUNC(evt_mario::evt_mario_set_mode, 1)
		USER_FUNC(evt_npc::evt_npc_entry, PTR(npc1), PTR("c_zako_m"))
		USER_FUNC(evt_npc::evt_npc_set_tribe, PTR(npc1), PTR(npc2))
		USER_FUNC(evt_npc::evt_npc_set_stay_anim, PTR(npc1))
		USER_FUNC(evt_npc::evt_npc_flag_onoff, 1, PTR(npc1), 1536)
		RUN_EVT(peach_evt_stg4_10)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1717), 18)
		USER_FUNC(evt_mario::evt_mario_set_mode, 1)
		RUN_CHILD_EVT(&monitor_init)
		RUN_EVT(&peach_evt_stg5_10)
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1717), 20)
		IF_SMALL_EQUAL(GSW(1717), 21)
			USER_FUNC(evt_mario::evt_mario_set_mode, 1)
			USER_FUNC(evt_aji::peach_evt_toumei, 0)
			USER_FUNC(evt_npc::evt_npc_entry, PTR(npc3), PTR("c_baria_c"))
			USER_FUNC(evt_npc::evt_npc_set_tribe, PTR(npc3), PTR(npc4))
			USER_FUNC(evt_npc::evt_npc_set_position, PTR(npc3), 370, 60, -65)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR(npc3), 270)
			USER_FUNC(evt_npc::evt_npc_flag_onoff, 1, PTR(npc3), 1536)
			INLINE_EVT()
				USER_FUNC(evt_bero::evt_bero_exec_wait, 65536)
				SET(LW(0), PTR("e_bero"))
				RUN_CHILD_EVT(evt_bero::bero_case_switch_off)
				USER_FUNC(evt_bero::evt_bero_overwrite, PTR("e_bero"), PTR(bero_peach_data))
				SET(LW(0), PTR("e_bero"))
				RUN_CHILD_EVT(evt_bero::bero_case_switch_on)
			END_INLINE()
			RETURN()
		END_IF()
	END_IF()
	IF_EQUAL(GSW(1706), 43)
		USER_FUNC(evt_mario::evt_mario_set_mode, 1)
		RUN_EVT(peach_evt_stg6_10)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(aji_10_init_evt_hook)
	RUN_CHILD_EVT(aji_10_init_evt_evt)
	GOTO(&aji_10_init_evt[186])
EVT_PATCH_END()

EVT_BEGIN(aji_17_init_evt_evt)
	IF_EQUAL(GSW(1711), 10)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1703), 24)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1715), 9)
		RETURN()
	END_IF()
	RUN_CHILD_EVT(evt_bero::evt_bero_info_run)
	RETURN()
EVT_END()

EVT_BEGIN(aji_17_init_evt_hook)
	RUN_CHILD_EVT(aji_17_init_evt_evt)
	GOTO(&aji_17_init_evt[178])
EVT_PATCH_END()
// clang-format on

void ApplyAjiPatches()
{
    aji_first_evt[308] = GSW(1707);
    aji_first_evt[309] = 9;

    aji_00_init_evt[1] = GSW(1707);
    aji_00_init_evt[2] = 9;
    aji_00_init_evt[28] = GSW(1707);
    aji_00_init_evt[29] = 9;
    patch::writePatch(&aji_00_init_evt[70], aji_00_init_evt_hook, sizeof(aji_00_init_evt_hook));

    elv_unlock1[117] = GSW(1707);
    elv_unlock1[118] = 10;

    elv_unlock2[117] = GSW(1707);
    elv_unlock2[118] = 11;

    ele_2[4] = GSW(1707);
    ele_2[5] = 11;

    evt_elv_init[1] = GSW(1707);
    evt_elv_init[3] = 16;
    evt_elv_init[67] = GSW(1707);
    evt_elv_init[68] = 10;
    evt_elv_init[162] = GSW(1707);
    evt_elv_init[163] = 11;

    evt_key_init[1] = GSW(1707);
    evt_key_init[2] = 16;
    evt_key_init[18] = GSW(1707);
    evt_key_init[19] = 19;
    evt_key_init[35] = GSW(1707);
    evt_key_init[37] = 16;
    evt_key_init[38] = 18;

    evt_aikotoba[129] = GSWF(6068);
    evt_aikotoba[130] = 1;

    peach_evt_stg3_01[16] = GSW(1703);
    peach_evt_stg3_01[17] = 22;

    patch::writePatch(&peach_evt_ele_2[9], peach_evt_ele_2_hook, sizeof(peach_evt_ele_2_hook));
    peach_evt_ele_2[147] = GSW(1703);
    peach_evt_ele_2[147] = GSW(1703);
    peach_evt_ele_2[148] = 22;
    peach_evt_ele_2[372] = GSW(1703);
    peach_evt_ele_2[373] = 22;
    peach_evt_ele_2[432] = GSW(1717);
    peach_evt_ele_2[433] = 19;

    peach_evt_ele_init[13] = GSW(1717);
    peach_evt_ele_init[14] = 19;
    peach_evt_ele_init[93] = GSW(1703);
    peach_evt_ele_init[94] = 21;

    peach_evt_key_init[7] = GSW(1703);
    peach_evt_key_init[8] = 21;
    peach_evt_key_init[10] = GSW(1703);
    peach_evt_key_init[11] = 25;
    peach_evt_key_init[18] = GSW(1717);
    peach_evt_key_init[19] = 19;
    peach_evt_key_init[21] = GSW(1717);
    peach_evt_key_init[22] = 22;

    peach_evt_init[60] = GSW(1711);
    peach_evt_init[61] = 10;
    peach_evt_init[68] = GSW(1703);
    peach_evt_init[69] = 21;
    peach_evt_init[177] = GSW(1703);
    peach_evt_init[178] = 22;
    peach_evt_init[183] = GSW(1703);
    peach_evt_init[184] = 23;
    peach_evt_init[208] = GSW(1717);
    peach_evt_init[209] = 20;
    peach_evt_init[211] = GSW(1717);
    peach_evt_init[212] = 21;
    peach_evt_init[297] = GSW(1717);
    peach_evt_init[298] = 19;
    peach_evt_init[350] = GSW(1717);
    peach_evt_init[351] = 20;
    peach_evt_init[358] = GSW(1717);
    peach_evt_init[359] = 21;

    teshita1_init[13] = GSW(1703);
    teshita1_init[15] = 23;

    patch::writePatch(&teshita2_init[12], teshita2_init_hook, sizeof(teshita2_init_hook));

    patch::writePatch(&teshita3_init[7], teshita3_init_hook, sizeof(teshita3_init_hook));

    aji_01_init_evt[39] = GSWF(6068);
    aji_01_init_evt[40] = 1;
    aji_01_init_evt[95] = GSW(1707);
    aji_01_init_evt[96] = 16;

    aji_evt_box[26] = GSWF(6066);
    aji_evt_box[27] = 1;

    aji_02_init_evt[7] = GSW(1707);
    aji_02_init_evt[8] = 1;
    aji_02_init_evt[77] = GSWF(6066);
    aji_02_init_evt[78] = 1;

    evt_zenmai[7] = GSW(1707);
    evt_zenmai[8] = 15;

    aji_03_init_evt[427] = GSWF(6093);
    aji_03_init_evt[437] = GSWF(6094);
    aji_03_init_evt[447] = GSWF(6095);
    aji_03_init_evt[457] = GSWF(6096);
    aji_03_init_evt[481] = GSWF(6097);
    aji_03_init_evt[505] = GSWF(6098);

    evt_item[1] = GSWF(6070);
    evt_item[2] = 1;

    kuizu_sekizou[967] = GSWF(6069);
    kuizu_sekizou[968] = 1;

    aji_04_init_evt[19] = GSWF(6069);
    aji_04_init_evt[20] = 1;
    aji_04_init_evt[38] = GSWF(6069);
    aji_04_init_evt[39] = 1;

    bero_peach_evt[4] = GSW(1717);
    bero_peach_evt[5] = 19;

    peach_evt_kusuri_nomu[420] = GSW(1717);
    peach_evt_kusuri_nomu[421] = 20;

    aji_06_init_evt[1] = GSW(1707);
    aji_06_init_evt[2] = 1;
    aji_06_init_evt[100] = GSW(1707);
    aji_06_init_evt[101] = 16;
    aji_06_init_evt[107] = GSW(1717);
    aji_06_init_evt[108] = 19;
    aji_06_init_evt[122] = GSW(1717);
    aji_06_init_evt[123] = 20;
    aji_06_init_evt[138] = GSW(1717);
    aji_06_init_evt[139] = 21;

    tensou_ride[216] = GSW(1707);
    tensou_ride[218] = 19;
    tensou_ride[227] = 19;

    aji_08_init_evt[140] = GSW(1707);
    aji_08_init_evt[141] = 16;

    peach_evt_kou_in[28] = GSW(1703);
    peach_evt_kou_in[29] = 23;
    peach_evt_kou_in[65] = GSW(1703);
    peach_evt_kou_in[66] = 24;
    peach_evt_kou_in[100] = GSW(1703);
    peach_evt_kou_in[101] = 22;
    peach_evt_kou_in[131] = GSW(1703);
    peach_evt_kou_in[132] = 22;

    peach_evt_kou_out[32] = GSW(1703);
    peach_evt_kou_out[33] = 22;
    peach_evt_kou_out[42] = GSW(1703);
    peach_evt_kou_out[43] = 24;

    peach_evt_kou1_out[31] = GSW(1703);
    peach_evt_kou1_out[32] = 22;
    peach_evt_kou1_out[34] = GSW(1703);
    peach_evt_kou1_out[35] = 23;

    peach_evt_kou2_out[31] = GSW(1703);
    peach_evt_kou2_out[32] = 22;
    peach_evt_kou2_out[34] = GSW(1703);
    peach_evt_kou2_out[35] = 23;

    peach_evt_stg3_09[4] = GSW(1703);
    peach_evt_stg3_09[5] = 23;
    peach_evt_stg3_09[173] = GSW(1703);
    peach_evt_stg3_09[174] = 24;
    peach_evt_stg3_09[205] = GSW(1703);
    peach_evt_stg3_09[206] = 22;
    peach_evt_stg3_09[217] = GSW(1703);
    peach_evt_stg3_09[218] = 24;

    evt_memo1[13] = GSWF(6067);
    evt_memo1[14] = 1;
    evt_memo1[16] = GSWF(6067);
    evt_memo1[17] = 1;

    aji_09_init_evt[34] = GSW(1703);
    aji_09_init_evt[35] = 22;
    aji_09_init_evt[37] = GSW(1703);
    aji_09_init_evt[38] = 24;

    monitor_init[9] = GSW(1717);
    monitor_init[10] = 18;

    peach_evt_stg3_syuryo_talk[130] = GSW(1703);
    peach_evt_stg3_syuryo_talk[131] = 24;

    syuryo_init[3] = GSW(1703);
    syuryo_init[4] = 23;

    patch::writePatch(&aji_10_init_evt[30], aji_10_init_evt_hook2, sizeof(aji_10_init_evt_hook2));
    aji_10_init_evt[34] = 0;
    patch::writePatch(&aji_10_init_evt[50], aji_10_init_evt_hook, sizeof(aji_10_init_evt_hook));
    aji_10_init_evt[219] = GSW(1707);
    aji_10_init_evt[220] = 16;
    aji_10_init_evt[255] = GSW(1703);
    aji_10_init_evt[256] = 23;
    aji_10_init_evt[264] = GSW(1717);
    aji_10_init_evt[265] = 20;
    aji_10_init_evt[267] = GSW(1717);
    aji_10_init_evt[268] = 21;

    peach_evt_bero[4] = GSW(1717);
    peach_evt_bero[5] = 21;

    peach_evt_tana2[22] = GSW(1717);
    peach_evt_tana2[23] = 21;

    peach_evt_stg5_11[17] = GSW(1717);
    peach_evt_stg5_11[18] = 21;

    aji_11_init_evt[1] = GSW(1717);
    aji_11_init_evt[3] = 20;
    aji_11_init_evt[4] = 21;
    aji_11_init_evt[60] = GSW(1707);
    aji_11_init_evt[61] = 1;
    aji_11_init_evt[90] = GSW(1717);
    aji_11_init_evt[91] = 20;
    aji_11_init_evt[93] = GSW(1717);
    aji_11_init_evt[94] = 21;

    evt_snd_stop[1] = GSW(1707);
    evt_snd_stop[2] = 13;

    aji_evt_sw[114] = GSW(1707);
    aji_evt_sw[115] = 14;

    evt_bcv_sw[1] = GSW(1707);
    evt_bcv_sw[2] = 13;

    evt_door_msg_13[40] = GSW(1707);
    evt_door_msg_13[41] = 15;

    aji_13_init_evt[44] = GSW(1707);
    aji_13_init_evt[45] = 13;
    aji_13_init_evt[181] = GSW(1707);
    aji_13_init_evt[182] = 14;
    aji_13_init_evt[205] = GSW(1707);
    aji_13_init_evt[206] = 15;
    aji_13_init_evt[278] = GSW(1707);
    aji_13_init_evt[279] = 16;

    evt_boss2[133] = GSW(1707);
    evt_boss2[134] = 16;

    aji_14_init_evt[46] = GSW(1707);
    aji_14_init_evt[47] = 16;

    aji_15_init_evt[8] = GSW(1707);
    aji_15_init_evt[9] = 16;

    peach_evt_stg1_16[77] = GSW(1711);
    peach_evt_stg1_16[78] = 9;

    peach_evt_stg1_1[66] = GSW(1711);
    peach_evt_stg1_1[67] = 10;

    peach_evt_stg2_1[78] = GSW(1713);
    peach_evt_stg2_1[79] = 12;

    peach_evt_stg3_16[137] = GSW(1703);
    peach_evt_stg3_16[138] = 21;

    peach_evt_stg6_16[127] = GSW(1706);
    peach_evt_stg6_16[128] = 44;

    aji_16_init_evt[27] = GSW(1711);
    aji_16_init_evt[28] = 9;
    aji_16_init_evt[37] = GSW(1713);
    aji_16_init_evt[38] = 11;
    aji_16_init_evt[69] = GSW(1711);
    aji_16_init_evt[70] = 9;
    aji_16_init_evt[89] = GSW(1711);
    aji_16_init_evt[90] = 10;
    aji_16_init_evt[106] = GSW(1713);
    aji_16_init_evt[107] = 11;
    patch::writePatch(&aji_16_init_evt[122], aji_16_init_evt_hook, sizeof(aji_16_init_evt_hook));
    aji_16_init_evt[126] = 0;

    evt_emergency[1] = GSW(1707);
    evt_emergency[2] = 19;
    evt_emergency[6] = GSW(1707);
    evt_emergency[7] = 19;

    evt_last2[258] = GSW(1707);
    evt_last2[259] = 19;

    peach_evt_stg1_17[1066] = GSW(1711);
    peach_evt_stg1_17[1067] = 11;

    peach_evt_stg2_17[835] = GSW(1713);
    peach_evt_stg2_17[836] = 13;

    peach_evt_stg3_1[585] = GSW(1703);
    peach_evt_stg3_1[586] = 25;

    peach_evt_stg4_17[915] = GSW(1715);
    peach_evt_stg4_17[916] = 10;

    peach_evt_stg5_17[401] = GSW(1717);
    peach_evt_stg5_17[402] = 19;

    peach_evt_stg5_1[460] = GSW(1717);
    peach_evt_stg5_1[461] = 22;

    peach_evt_stg6_17[1400] = GSW(1706);
    peach_evt_stg6_17[1401] = 45;

    aji_17_init_evt[54] = GSW(1711);
    aji_17_init_evt[55] = 10;
    aji_17_init_evt[60] = GSW(1713);
    aji_17_init_evt[61] = 12;
    aji_17_init_evt[66] = GSW(1703);
    aji_17_init_evt[67] = 21;
    aji_17_init_evt[72] = GSW(1703);
    aji_17_init_evt[73] = 24;
    aji_17_init_evt[81] = GSW(1715);
    aji_17_init_evt[82] = 9;
    aji_17_init_evt[87] = GSW(1717);
    aji_17_init_evt[88] = 18;
    aji_17_init_evt[93] = GSW(1717);
    aji_17_init_evt[94] = 21;
    aji_17_init_evt[99] = GSW(1706);
    aji_17_init_evt[100] = 44;
    aji_17_init_evt[116] = GSW(1707);
    aji_17_init_evt[117] = 18;
    patch::writePatch(&aji_17_init_evt[166], aji_17_init_evt_hook, sizeof(aji_17_init_evt_hook));
    aji_17_init_evt[198] = GSW(1708);
    aji_17_init_evt[199] = 18;

    evt_machine[204] = GSW(1707);
    evt_machine[205] = 12;

    aji_18_init_evt[21] = GSW(1707);
    aji_18_init_evt[22] = 12;
    aji_18_init_evt[190] = GSW(1707);
    aji_18_init_evt[191] = 12;
    aji_18_init_evt[190] = GSW(1707);
    aji_18_init_evt[191] = 12;
    aji_18_init_evt[232] = GSW(1707);
    aji_18_init_evt[234] = 16;
    aji_18_init_evt[235] = 18;
    aji_18_init_evt[263] = GSW(1707);
    aji_18_init_evt[265] = 16;
    aji_18_init_evt[266] = 18;

    evt_hajimete[23] = GSW(1707);
    evt_hajimete[24] = 8;

    aji_19_init_evt[43] = GSW(1707);
    aji_19_init_evt[44] = 8;

    // Assembly
    check_kou_evt[13] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
    check_kou_evt[17] = 0x2C030016; // cmpwi r3, 0x16
    check_kou_evt[28] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
    check_kou_evt[30] = 0x2C030016; // cmpwi r3, 0x18

    mario_chk[173] = 0x3884082B; // addi r4, r4, 0x82B GSW(1707)
    mario_chk[175] = 0x2C03000C; // cmpwi r3, 0xC
}
