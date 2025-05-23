#include "subrel_gor.h"
#include "evt_cmd.h"
#include "patch.h"
#include "AP/rel_patch_definitions.h"
#include "ttyd/evt_map.h"
#include "ttyd/evt_msg.h"

#include <cstdint>

using namespace mod;
using namespace ttyd;

extern int32_t gor_maffiaboss_ticket_check[];
extern int32_t gor_maffiaboss_init[];
extern int32_t gor_maffiaboss_maffiaboss[];
extern int32_t gor_maffiaboss_teshita_irai[];
extern int32_t gor_maffiaboss_train_ticket[];
extern int32_t gor_maffiaboss_talk_common[];
extern int32_t gor_maffiaboss_casino_init[];
extern int32_t gor_maffiaboss_casino_talk[];
extern int32_t gor_maffia1_talk[];
extern int32_t gor_maffia2_talk[];
extern int32_t gor_maffia3_talk[];
extern int32_t gor_mony_init_03[];
extern int32_t gor_mony_talk_03[];
extern int32_t gor_peton_init_03[];
extern int32_t gor_peton_talk_common_03[];
extern int32_t gor_luigi_init_03[];
extern int32_t gor_luigi_npcEnt_03[];
extern int32_t gor_roten_4_talk[];
extern int32_t gor_kinopiko_a_talk[];
extern int32_t gor_kinopiko_b_talk_03[];
extern int32_t gor_kinopio_a_talk[];
extern int32_t gor_kokinopio_a_talk[];
extern int32_t gor_kokinopio_b_talk[];
extern int32_t gor_nokonoko_a_talk[];
extern int32_t gor_kuribo_1_talk[];
extern int32_t gor_evt_door_01_0_close[];
extern int32_t gor_desk_talk_check[];
extern int32_t gor_evt_door_02_1_open[];
extern int32_t gor_evt_door_02_1_close[];
extern int32_t gor_majorin_nusumigiki[];
extern int32_t gor_yuureturn_rtn[];
extern int32_t gor_peach_mail_03[];
extern int32_t gor_debug_event_mode[];
extern int32_t gor_03_init_evt[];
extern int32_t gor_maffiaboss_nekomu00[];
extern int32_t gor_maffiaboss_nekomu01[];
extern int32_t gor_maffiaboss_nekomu02[];
extern int32_t gor_kuzi_keiziban_normal[];

// clang-format off
EVT_BEGIN(maffiaboss_talk_common_evt)
	SWITCH(GSW(1709))
		CASE_EQUAL(1)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_069"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(2)
			SET(GF(15), 1)
			SET(GF(14), 1)
			HALT(GF(14))
		CASE_EQUAL(3)
			IF_EQUAL(GSWF(6000), 0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_120"), 0, PTR("me"))
			ELSE()
				RUN_CHILD_EVT(gor_maffiaboss_teshita_irai)
			END_IF()
		CASE_END()
		CASE_EQUAL(4)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_5_037"), 0, PTR("me"))
		CASE_END()
		CASE_BETWEEN(5, 8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_5_037"), 0, PTR("me"))
		CASE_END()
		CASE_EQUAL(9)
			RUN_CHILD_EVT(gor_maffiaboss_train_ticket)
		CASE_END()
		CASE_ETC()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_03_01_01"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(maffiaboss_casino_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_03_01_05"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(maffia1_talk_evt)
	SWITCH(GSW(1709))
		CASE_EQUAL(0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_044_02"), 0, PTR("me"))
		CASE_END()
		CASE_BETWEEN(1, 2)
			IF_EQUAL(GSWF(1236), 0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_070"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_116_buka1"), 0, PTR("me"))
			END_IF()
		CASE_END()
		CASE_EQUAL(3)
			IF_EQUAL(GSWF(6000), 0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_121"), 0, PTR("me"))
			ELSE()
				RUN_CHILD_EVT(gor_maffiaboss_nekomu00)
			END_IF()
		CASE_END()
		CASE_EQUAL(4)
			RUN_CHILD_EVT(gor_maffiaboss_nekomu01)
		CASE_END()
		CASE_BETWEEN(5, 8)
			RUN_CHILD_EVT(gor_maffiaboss_nekomu02)
		CASE_END()
		CASE_EQUAL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_5_042"), 0, PTR("me"))
		CASE_END()
		CASE_LARGE_EQUAL(10)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_03_02_05"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(maffia2_talk_evt)
	SWITCH(GSW(1709))
		CASE_EQUAL(0)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_044_03"), 0, PTR("me"))
		CASE_END()
		CASE_BETWEEN(1, 2)
			IF_EQUAL(GSWF(1236), 0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_071"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_116_buka2"), 0, PTR("me"))
			END_IF()
		CASE_END()
		CASE_EQUAL(3)
			IF_EQUAL(GSWF(6000), 0)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_2_122"), 0, PTR("me"))
			ELSE()
				RUN_CHILD_EVT(gor_maffiaboss_nekomu00)
			END_IF()
		CASE_END()
		CASE_EQUAL(4)
			RUN_CHILD_EVT(gor_maffiaboss_nekomu01)
		CASE_END()
		CASE_BETWEEN(5, 8)
			RUN_CHILD_EVT(gor_maffiaboss_nekomu02)
		CASE_END()
		CASE_EQUAL(9)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_5_043"), 0, PTR("me"))
		CASE_END()
		CASE_LARGE_EQUAL(10)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_03_03_05"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(maffia3_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_03_008"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(gor_03_init_evt_evt)
	USER_FUNC(checkChapterClears, LW(0))
	SWITCH(LW(0))
		CASE_EQUAL(0)
			USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S_kou_kusas01"), 1)
		CASE_END()
		CASE_EQUAL(1)
			USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S_kou_kusas01"), 1)
		CASE_END()
		CASE_EQUAL(2)
			USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S_kou_kusas01"), 1)
		CASE_END()
		CASE_EQUAL(3)
			USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S_kou_kusas02"), 1)
		CASE_END()
		CASE_EQUAL(4)
			USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S_kou_kusas02"), 1)
		CASE_END()
		CASE_EQUAL(5)
			USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S_kou_kusas02"), 1)
		CASE_END()
		CASE_EQUAL(6)
			USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S_kou_kusas03"), 1)
		CASE_END()
		CASE_EQUAL(7)
			USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S_kou_kusas03"), 1)
		CASE_END()
		CASE_ETC()
			USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S_kou_kusas03"), 1)
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(gor_03_init_evt_hook)
	RUN_CHILD_EVT(gor_03_init_evt_evt)
	GOTO(&gor_03_init_evt[292])
EVT_PATCH_END()
// clang-format on

void ApplyGor03Patches()
{
    gor_maffiaboss_ticket_check[231] = GSW(1709);
    gor_maffiaboss_ticket_check[232] = 3;

    gor_maffiaboss_init[1] = GSW(1709);
    gor_maffiaboss_init[3] = 2;
    gor_maffiaboss_init[20] = 3;
    gor_maffiaboss_init[21] = 9;
    gor_maffiaboss_init[56] = 9;

    gor_maffiaboss_maffiaboss[482] = GSW(1709);
    gor_maffiaboss_maffiaboss[483] = 1;

    gor_maffiaboss_teshita_irai[249] = GSW(1709);
    gor_maffiaboss_teshita_irai[250] = 5;

    gor_maffiaboss_train_ticket[797] = GSW(1709);
    gor_maffiaboss_train_ticket[798] = 10;

    patch::writePatch(&gor_maffiaboss_talk_common[3],
                      maffiaboss_talk_common_evt,
                      sizeof(maffiaboss_talk_common_evt)); // Don Piata speach

    gor_maffiaboss_casino_init[1] = GSW(1709);
    gor_maffiaboss_casino_init[3] = 9;

    patch::writePatch(&gor_maffiaboss_casino_talk[0],
                      maffiaboss_casino_talk_evt,
                      sizeof(maffiaboss_casino_talk_evt)); // Don Piata casino speach

    patch::writePatch(&gor_maffia1_talk[0], maffia1_talk_evt, sizeof(maffia1_talk_evt)); // Mafia 1 speach
    patch::writePatch(&gor_maffia2_talk[0], maffia2_talk_evt, sizeof(maffia2_talk_evt)); // Mafia 2 speach
    patch::writePatch(&gor_maffia3_talk[0], maffia3_talk_evt, sizeof(maffia3_talk_evt)); // Mafia 3 speach

    gor_mony_init_03[1] = GSW(1709);
    gor_mony_init_03[3] = -1;
    gor_mony_init_03[15] = 9;
    gor_mony_init_03[27] = 10;
    gor_mony_init_03[28] = 99; // Unused

    gor_mony_talk_03[1] = GSW(1709);
    gor_mony_talk_03[3] = 0;
    gor_mony_talk_03[11] = 9;
    gor_mony_talk_03[19] = 99;

    gor_peton_init_03[16] = GSW(1709);
    gor_peton_init_03[18] = -1;
    gor_peton_init_03[30] = 9;
    gor_peton_init_03[42] = 10;
    gor_peton_init_03[43] = 99;

    gor_peton_talk_common_03[1] = GSW(1709);
    gor_peton_talk_common_03[2] = 9;
    gor_peton_talk_common_03[122] = GSW(1709);
    gor_peton_talk_common_03[124] = 10;
    gor_peton_talk_common_03[125] = 99; // Unused
    gor_peton_talk_common_03[176] = GSW(1709);
    gor_peton_talk_common_03[178] = 0;
    gor_peton_talk_common_03[186] = 9;
    gor_peton_talk_common_03[194] = 99;

    gor_luigi_init_03[1] = GSW(1710);
    gor_luigi_init_03[3] = 3;
    gor_luigi_init_03[4] = 4;

    gor_luigi_npcEnt_03[1] = GSW(1710);
    gor_luigi_npcEnt_03[3] = 3;
    gor_luigi_npcEnt_03[4] = 4;

    gor_roten_4_talk[1] = GSW(1710);
    gor_roten_4_talk[3] = 2;
    gor_roten_4_talk[11] = 3;
    gor_roten_4_talk[19] = 4;
    gor_roten_4_talk[27] = 5;
    gor_roten_4_talk[35] = 6;
    gor_roten_4_talk[43] = 7;
    gor_roten_4_talk[51] = 8;

    gor_kinopiko_a_talk[1] = GSW(1710);
    gor_kinopiko_a_talk[3] = 2;
    gor_kinopiko_a_talk[11] = 3;
    gor_kinopiko_a_talk[19] = 4;
    gor_kinopiko_a_talk[27] = 5;
    gor_kinopiko_a_talk[35] = 6;
    gor_kinopiko_a_talk[43] = 7;
    gor_kinopiko_a_talk[51] = 8;

    gor_kinopiko_b_talk_03[4] = GSW(1710);
    gor_kinopiko_b_talk_03[6] = 2;
    gor_kinopiko_b_talk_03[25] = 3;
    gor_kinopiko_b_talk_03[44] = 4;
    gor_kinopiko_b_talk_03[63] = 5;
    gor_kinopiko_b_talk_03[82] = 6;
    gor_kinopiko_b_talk_03[101] = 7;
    gor_kinopiko_b_talk_03[120] = 8;

    gor_kinopio_a_talk[1] = GSW(1710);
    gor_kinopio_a_talk[3] = 2;
    gor_kinopio_a_talk[11] = 3;
    gor_kinopio_a_talk[19] = 4;
    gor_kinopio_a_talk[27] = 5;
    gor_kinopio_a_talk[35] = 6;
    gor_kinopio_a_talk[43] = 7;
    gor_kinopio_a_talk[51] = 8;

    gor_kokinopio_a_talk[1] = GSW(1710);
    gor_kokinopio_a_talk[3] = 2;
    gor_kokinopio_a_talk[11] = 3;
    gor_kokinopio_a_talk[19] = 4;
    gor_kokinopio_a_talk[27] = 5;
    gor_kokinopio_a_talk[35] = 6;
    gor_kokinopio_a_talk[43] = 7;
    gor_kokinopio_a_talk[51] = 8;

    gor_kokinopio_b_talk[1] = GSW(1710);
    gor_kokinopio_b_talk[3] = 2;
    gor_kokinopio_b_talk[11] = 3;
    gor_kokinopio_b_talk[19] = 4;
    gor_kokinopio_b_talk[27] = 5;
    gor_kokinopio_b_talk[35] = 6;
    gor_kokinopio_b_talk[43] = 7;
    gor_kokinopio_b_talk[51] = 8;

    gor_nokonoko_a_talk[1] = GSW(1710);
    gor_nokonoko_a_talk[3] = 2;
    gor_nokonoko_a_talk[11] = 3;
    gor_nokonoko_a_talk[19] = 4;
    gor_nokonoko_a_talk[27] = 5;
    gor_nokonoko_a_talk[35] = 6;
    gor_nokonoko_a_talk[43] = 7;
    gor_nokonoko_a_talk[51] = 8;

    gor_kuribo_1_talk[1] = GSW(1710);
    gor_kuribo_1_talk[3] = 2;
    gor_kuribo_1_talk[11] = 3;
    gor_kuribo_1_talk[19] = 4;
    gor_kuribo_1_talk[27] = 5;
    gor_kuribo_1_talk[35] = 6;
    gor_kuribo_1_talk[43] = 7;
    gor_kuribo_1_talk[51] = 8;

    gor_evt_door_01_0_close[13] = GSW(1709);
    gor_evt_door_01_0_close[14] = 3;
    gor_evt_door_01_0_close[27] = GSWF(6000);
    gor_evt_door_01_0_close[28] = 1;
    gor_evt_door_01_0_close[37] = GSW(1709);
    gor_evt_door_01_0_close[38] = 10;

    gor_desk_talk_check[20] = GSW(1709);
    gor_desk_talk_check[22] = 3;
    gor_desk_talk_check[44] = 11;

    gor_evt_door_02_1_open[7] = GSW(1709);
    gor_evt_door_02_1_open[8] = 9;
    gor_evt_door_02_1_open[30] = GSW(1709);
    gor_evt_door_02_1_open[32] = 3;
    gor_evt_door_02_1_open[45] = 10;
    gor_evt_door_02_1_open[51] = 9;
    gor_evt_door_02_1_open[91] = 11;

    gor_evt_door_02_1_close[1] = GSW(1709);
    gor_evt_door_02_1_close[2] = 9;

    gor_majorin_nusumigiki[321] = GSW(1709);
    gor_majorin_nusumigiki[322] = 11;

    gor_yuureturn_rtn[80] = GSW(1708);
    gor_yuureturn_rtn[81] = 19;

    gor_kuzi_keiziban_normal[356] = GSWF(6100);

    gor_peach_mail_03[238] = GSW(1703);
    gor_peach_mail_03[239] = 29;

    gor_debug_event_mode[12] = GSW(1709);
    gor_debug_event_mode[13] = 0;
    gor_debug_event_mode[19] = GSW(1709);
    gor_debug_event_mode[20] = 2;
    gor_debug_event_mode[26] = GSW(1709);
    gor_debug_event_mode[27] = 3;
    gor_debug_event_mode[33] = GSW(1709);
    gor_debug_event_mode[34] = 3;
    gor_debug_event_mode[40] = GSW(1709);
    gor_debug_event_mode[41] = 9;
    gor_debug_event_mode[47] = GSW(1709);
    gor_debug_event_mode[48] = 10;

    gor_03_init_evt[46] = GSW(1709);
    gor_03_init_evt[48] = 9;
    gor_03_init_evt[49] = 11;
    gor_03_init_evt[59] = 11;
    patch::writePatch(&gor_03_init_evt[218], gor_03_init_evt_hook, sizeof(gor_03_init_evt_hook));
    gor_03_init_evt[365] = GSW(1703);
    gor_03_init_evt[366] = 99;
    gor_03_init_evt[610] = GSW(1709);
    gor_03_init_evt[611] = 0;
    gor_03_init_evt[616] = GSW(1709);
    gor_03_init_evt[617] = 2;
    gor_03_init_evt[626] = GSW(1709);
    gor_03_init_evt[628] = 3;
    gor_03_init_evt[638] = 10;
}
