#include "AP/rel_patch_definitions.h"
#include "evt_cmd.h"
#include "patch.h"
#include "subrel_nok.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_msg.h"
#include "ttyd/evt_npc.h"
#include "ttyd/evt_party.h"

#include <cstdint>

using namespace ttyd;

extern int32_t nok_nokonoko_A_init[];
extern int32_t nok_nokonoko_A_talk[];
extern int32_t nok_nokonoko_B_talk[];
extern int32_t nok_nokonoko_C_talk[];
extern int32_t nok_nokonoko_D_talk[];
extern int32_t nok_nokonoko_F_talk[];
extern int32_t nok_init_white[];
extern int32_t nok_talk_white[];
extern int32_t nok_first_time_camera[];
extern int32_t nok_00_koopa_evt[];
extern int32_t nok_00_init_evt[];
extern int32_t nok_nokonoko_G_talk[];
extern int32_t nok_nokonoko_H_talk[];
extern int32_t nok_nokonoko_I_talk[];
extern int32_t nok_open_gate[];
extern int32_t nok_monban_init[];
extern int32_t nok_monban_talk[];
extern int32_t nok_nokotarou_init[];
extern int32_t nok_nokotarou_talk[];
extern int32_t nok_nokopapa_init[];
extern int32_t nok_nokopapa_talk[];
extern int32_t nok_nokonoko_K_talk[];
extern int32_t nok_boss_noko_open_gate[];
extern int32_t nok_boss_noko_talk[];
extern int32_t nok_nokorin_init[];
extern int32_t nok_nokorin_talk[];
extern int32_t nok_odoriko_1_talk[];
extern int32_t nok_odoriko_2_talk[];
extern int32_t nok_odoriko_3_talk[];
extern int32_t nok_nokotarou_tabidachi[];
extern int32_t nok_evt_kusa_4[];
extern int32_t nok_evt_kusa_5[];
extern int32_t nok_evt_kusa_6[];
extern int32_t nok_01_init_evt[];

// clang-format off
EVT_BEGIN(nokonoko_A_talk_evt)
	IF_EQUAL(GSW(1707), 3)
		SET(GSWF(3877), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_nok_01"), 0, 0)
		RETURN()
	END_IF()
	IF_SMALL(GSW(1701), 9)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_02"), 0, 0)
		RETURN()
	END_IF()
	SWITCH(GSW(1711))
		CASE_SMALL(8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_02_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_03"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_03_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_nok_01_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_A_talk_hook)
	RUN_CHILD_EVT(nokonoko_A_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_B_talk_evt)
	IF_SMALL(GSW(1701), 9)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_04"), 0, 0)
		RETURN()
	END_IF()
	SWITCH(GSW(1711))
		CASE_SMALL(8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_04_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_05"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_05_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_05_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_B_talk_hook)
	RUN_CHILD_EVT(nokonoko_B_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_C_talk_evt)
	IF_SMALL(GSW(1701), 9)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_06"), 0, 0)
		RETURN()
	END_IF()
	SWITCH(GSW(1711))
		CASE_SMALL(8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_06_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_07"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_07_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_07_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_C_talk_hook)
	RUN_CHILD_EVT(nokonoko_C_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_D_talk_evt)
	IF_SMALL(GSW(1701), 9)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_08"), 0, 0)
		RETURN()
	END_IF()
	SWITCH(GSW(1711))
		CASE_SMALL(8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_08_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_09"), 0, 0)
			USER_FUNC(evt_msg::evt_msg_select, 0, PTR("stg1_nok_09_yesno"))
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_09_01"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_09_01_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_09_01_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_D_talk_hook)
	RUN_CHILD_EVT(nokonoko_D_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_F_talk_evt)
	IF_SMALL(GSW(1701), 9)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_17"), 0, 0)
		RETURN()
	END_IF()
	SWITCH(GSW(1711))
		CASE_SMALL(8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_17_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_18"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_18_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_18_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_F_talk_hook)
	RUN_CHILD_EVT(nokonoko_F_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(init_white_evt)
	IF_EQUAL(GSW(1713), 15)
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 3)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 375, 50, -105)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(init_white_hook)
	RUN_CHILD_EVT(init_white_evt)
	RETURN()
EVT_END()

EVT_BEGIN(talk_white_evt)
	IF_SMALL(GSW(1701), 9)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_19_01"), 0, 0)
		RETURN()
	END_IF()
	IF_SMALL(GSW(1711), 7)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_19_02"), 0, 0)
		RETURN()
	END_IF()
	IF_SMALL(GSW(1713), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_19_03"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_19_04"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(talk_white_hook)
	RUN_CHILD_EVT(talk_white_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_G_talk_evt)
	IF_SMALL(GSW(1701), 9)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_20"), 0, 0)
		RETURN()
	END_IF()
	SWITCH(GSW(1711))
		CASE_SMALL(8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_20_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_21"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_21_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_21_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_G_talk_hook)
	RUN_CHILD_EVT(nokonoko_G_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_H_talk_evt)
	IF_SMALL(GSW(1701), 9)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_22"), 0, 0)
		RETURN()
	END_IF()
	SWITCH(GSW(1711))
		CASE_SMALL(8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_22_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_23"), 0, 0)
			USER_FUNC(evt_msg::evt_msg_select, 0, PTR("stg1_nok_23_yesno"))
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_23_01"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_23_01_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_23_01_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_H_talk_hook)
	RUN_CHILD_EVT(nokonoko_H_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_I_talk_evt)
	IF_SMALL(GSW(1701), 9)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_24"), 0, 0)
		RETURN()
	END_IF()
	SWITCH(GSW(1711))
		CASE_SMALL(8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_24_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_25"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_25_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_25_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_I_talk_hook)
	RUN_CHILD_EVT(nokonoko_I_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(monban_talk_evt)
	SWITCH(GSW(1701))
		CASE_SMALL(4)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_26"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(5)
			RUN_CHILD_EVT(nok_open_gate)
			RETURN()
		CASE_END()
		CASE_SMALL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_31_01_00"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	SWITCH(GSW(1711))
		CASE_SMALL(8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_31_01_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_31_02_1"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_31_02_2"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_31_02_2_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(monban_talk_hook)
	RUN_CHILD_EVT(monban_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokopapa_talk_evt)
	IF_SMALL(GSW(1704), 2)
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_79"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_80"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 16)
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_81"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_82"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 16)
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_83"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_84"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_84_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokopapa_talk_hook)
	RUN_CHILD_EVT(nokopapa_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_K_talk_evt)
	IF_SMALL(GSW(1701), 9)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_32"), 0, 0)
		RETURN()
	END_IF()
	SWITCH(GSW(1711))
		CASE_SMALL(8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_32_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_33"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_33_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_33_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(nokonoko_K_talk_hook)
	RUN_CHILD_EVT(nokonoko_K_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(boss_noko_talk_evt)
	IF_SMALL(GSW(1701), 4)
		RUN_CHILD_EVT(nok_boss_noko_open_gate)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1707), 3)
		SET(GSWF(3877), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_57_1_00"), 0, 0)
		RETURN()
	END_IF()
	IF_SMALL(GSW(1711), 1)
		IF_EQUAL(GSWF(1774), 1)
			IF_EQUAL(GSWF(1775), 1)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_56"), 0, 0)
				RETURN()
			END_IF()
		END_IF()
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_47"), 0, 0)
		RETURN()
	END_IF()
	SWITCH(GSW(1711))
		CASE_SMALL(8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_56_01"), 0, 0)
			RETURN()
		CASE_END()
		CASE_SMALL(17)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_57"), 0, 0)
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 18)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_57_1"), 0, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_57_1_1"), 0, 0)
	RETURN()
EVT_END()

EVT_BEGIN(boss_noko_talk_hook)
	RUN_CHILD_EVT(boss_noko_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(nokorin_talk_evt)
	USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
	IF_SMALL(GSW(1711), 8)
		IF_EQUAL(GSWF(1627), 0)
			IF_EQUAL(LW(0), 2)
				SET(GSWF(1627), 1)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_67"), 0, 0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_68"), 0, 0)
				USER_FUNC(evt_npc::evt_npc_status_onoff, 1, PTR("me"), 2)
				USER_FUNC(evt_npc::evt_npc_flag_onoff, 1, PTR("me"), 131120)
				USER_FUNC(evt_npc::evt_npc_move_position, PTR("me"), -85, -300, 0, FLOAT(60.00), 1)
				USER_FUNC(evt_npc::evt_npc_flag_onoff, 0, PTR("me"), 131120)
				RETURN()
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_70"), 0, 0)
				RETURN()
			END_IF()
		ELSE()
			IF_EQUAL(LW(0), 2)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_69"), 0, 0)
				RETURN()
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_70"), 0, 0)
				RETURN()
			END_IF()
		END_IF()
	END_IF()
	IF_SMALL(GSW(1702), 1)
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_71"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_72"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	IF_SMALL(GSW(1704), 2)
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_73"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_74"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	IF_SMALL(GSW(1707), 1)
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_75"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_76"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	IF_SMALL(GSW(1708), 18)
		IF_EQUAL(LW(0), 2)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_77"), 0, 0)
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_78"), 0, 0)
		END_IF()
		RETURN()
	END_IF()
	IF_EQUAL(LW(0), 2)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_77"), 0, 0)
	ELSE()
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg1_nok_78"), 0, 0)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(nokorin_talk_hook)
	RUN_CHILD_EVT(nokorin_talk_evt)
	RETURN()
EVT_END()
// clang-format on

namespace mod
{
    void main()
    {
        nok_nokonoko_A_init[6] = GSW(1701);
        nok_nokonoko_A_init[7] = 3;
        nok_nokonoko_A_init[16] = GSW(1713);
        nok_nokonoko_A_init[17] = 15;

        patch::writePatch(&nok_nokonoko_A_talk[0], nokonoko_A_talk_hook, sizeof(nokonoko_A_talk_hook));

        patch::writePatch(&nok_nokonoko_B_talk[0], nokonoko_B_talk_hook, sizeof(nokonoko_B_talk_hook));

        patch::writePatch(&nok_nokonoko_C_talk[0], nokonoko_C_talk_hook, sizeof(nokonoko_C_talk_hook));

        patch::writePatch(&nok_nokonoko_D_talk[0], nokonoko_D_talk_hook, sizeof(nokonoko_D_talk_hook));

        patch::writePatch(&nok_nokonoko_F_talk[274], nokonoko_F_talk_hook, sizeof(nokonoko_F_talk_hook));

        patch::writePatch(&nok_init_white[8], init_white_hook, sizeof(init_white_hook));

        patch::writePatch(&nok_talk_white[0], talk_white_hook, sizeof(talk_white_hook));

        nok_first_time_camera[179] = GSW(1701);
        nok_first_time_camera[180] = 3;

        nok_00_koopa_evt[1213] = GSW(1713);
        nok_00_koopa_evt[1214] = 16;

        nok_00_init_evt[1] = GSW(1713);
        nok_00_init_evt[2] = 15;
        nok_00_init_evt[25] = GSW(1701);
        nok_00_init_evt[26] = 3;
        nok_00_init_evt[40] = GSW(1701);
        nok_00_init_evt[41] = 1;
        nok_00_init_evt[43] = GSW(1711);
        nok_00_init_evt[44] = 8;
        nok_00_init_evt[77] = GSW(1713);
        nok_00_init_evt[78] = 15;

        patch::writePatch(&nok_nokonoko_G_talk[0], nokonoko_G_talk_hook, sizeof(nokonoko_G_talk_hook));

        patch::writePatch(&nok_nokonoko_H_talk[0], nokonoko_H_talk_hook, sizeof(nokonoko_H_talk_hook));

        patch::writePatch(&nok_nokonoko_I_talk[0], nokonoko_I_talk_hook, sizeof(nokonoko_I_talk_hook));

        nok_open_gate[217] = GSW(1701);
        nok_open_gate[218] = 5;

        nok_monban_init[1] = GSW(1701);
        nok_monban_init[3] = 5;

        patch::writePatch(&nok_monban_talk[0], monban_talk_hook, sizeof(monban_talk_hook));

        nok_nokotarou_init[3] = GSW(1701);
        nok_nokotarou_init[5] = 9;

        nok_nokotarou_talk[1] = GSW(1701);
        nok_nokotarou_talk[3] = 4;
        nok_nokotarou_talk[11] = 5;
        nok_nokotarou_talk[19] = 6;

        nok_nokopapa_init[3] = GSW(1711);
        nok_nokopapa_init[5] = 7;

        patch::writePatch(&nok_nokopapa_talk[3], nokopapa_talk_hook, sizeof(nokopapa_talk_hook));
        patch::writePatch(&nok_nokonoko_K_talk[25], nokonoko_K_talk_hook, sizeof(nokonoko_K_talk_hook));

        nok_boss_noko_open_gate[180] = GSW(1701);
        nok_boss_noko_open_gate[181] = 4;

        patch::writePatch(&nok_boss_noko_talk[278], boss_noko_talk_hook, sizeof(boss_noko_talk_hook));

        nok_nokorin_init[7] = GSW(1711);
        nok_nokorin_init[8] = 8;
        nok_nokorin_init[10] = GSW(1701);
        nok_nokorin_init[11] = 10;

        nok_nokorin_talk[1] = GSW(1701);
        nok_nokorin_talk[3] = 9;
        nok_nokorin_talk[82] = 10;
        nok_nokorin_talk[89] = EVT_HELPER_CMD(0, 49);
        patch::writePatch(&nok_nokorin_talk[90], nokorin_talk_hook, sizeof(nokorin_talk_hook));

        nok_odoriko_1_talk[1] = GSW(1701);
        nok_odoriko_1_talk[3] = 9;

        nok_odoriko_2_talk[1] = GSW(1701);
        nok_odoriko_2_talk[3] = 9;

        nok_odoriko_3_talk[1] = GSW(1701);
        nok_odoriko_3_talk[3] = 9;

        nok_nokotarou_tabidachi[306] = GSW(1711);
        nok_nokotarou_tabidachi[307] = 14;

        nok_evt_kusa_4[20] = GSWF(6101);
        nok_evt_kusa_5[20] = GSWF(6101);
        nok_evt_kusa_6[20] = GSWF(6101);

        nok_01_init_evt[13] = GSW(1708);
        nok_01_init_evt[14] = 16;
        nok_01_init_evt[75] = GSW(1701);
        nok_01_init_evt[76] = 1;
        nok_01_init_evt[78] = GSW(1711);
        nok_01_init_evt[79] = 8;
        nok_01_init_evt[109] = GSW(1701);
        nok_01_init_evt[110] = 5;
        nok_01_init_evt[292] = GSW(1708);
        nok_01_init_evt[293] = 16;
        nok_01_init_evt[311] = GSW(1708);
        nok_01_init_evt[312] = 18;
    }

    void exit() {}
} // namespace mod
