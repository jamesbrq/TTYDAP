#include "subrel_muj.h"
#include "evt_cmd.h"
#include "patch.h"
#include "AP/rel_patch_definitions.h"
#include "ttyd/evt_cam.h"
#include "ttyd/evt_hit.h"
#include "ttyd/evt_item.h"
#include "ttyd/evt_map.h"
#include "ttyd/evt_mario.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/evt_msg.h"
#include "ttyd/evt_npc.h"
#include "ttyd/evt_snd.h"
#include "common.h"

#include <cstdint>
#include <cstring>

using namespace ttyd::common;
using namespace ttyd;

extern int32_t muj_nannpc_mode_setup;
extern int32_t muj_garawaru_init_00[];
extern int32_t muj_odoodo_init_00[];
extern int32_t muj_suifu_c_init[];
extern int32_t muj_suifu_c_talk[];
extern int32_t muj_suifu_d_init_00[];
extern int32_t muj_suifu_d_talk_00[];
extern int32_t muj_suifu_e_init[];
extern int32_t muj_suifu_e_talk[];
extern int32_t muj_korutesu_init_00[];
extern int32_t muj_korutesu_talk_00[];
extern int32_t muj_mony_talk_00[];
extern int32_t muj_peton_talk_00[];
extern int32_t muj_mujinto_hyochaku[];
extern int32_t muj_suifu_random_sub;
extern int32_t muj_dai3_shurai_sub[];
extern int32_t muj_dai3_shurai;
extern int32_t muj_kaizoku_vs_dai3[];
extern int32_t muj_dai3_beat_back[];
extern int32_t muj_leave_island[];
extern int32_t muj_into_muj_ship[];
extern int32_t muj_mony_peton_boarding[];
extern int32_t muj_00_init_evt[];
extern int32_t muj_garawaru_init_01[];
extern int32_t muj_garawaru_talk_01[];
extern int32_t muj_marco_init_01[];
extern int32_t muj_marco_talk_01[];
extern int32_t muj_megane_init_01[];
extern int32_t muj_megane_talk_01[];
extern int32_t muj_odoodo_init_01[];
extern int32_t muj_odoodo_talk_01[];
extern int32_t muj_suifu_a_init_01[];
extern int32_t muj_suifu_a_talk_01[];
extern int32_t muj_suifu_bomb_init[];
extern int32_t muj_suifu_bomb_talk[];
extern int32_t muj_suifu_d_init_01[];
extern int32_t muj_suifu_d_regl_01[];
extern int32_t muj_suifu_d_talk_01[];
extern int32_t muj_borosuifu_a_init[];
extern int32_t muj_borosuifu_a_talk[];
extern int32_t muj_borosuifu_b_init[];
extern int32_t muj_borosuifu_b_regl[];
extern int32_t muj_borosuifu_b_talk[];
extern int32_t muj_mony_init_01[];
extern int32_t muj_peton_init_01[];
extern int32_t muj_kyoshu_event[];
extern int32_t muj_nakamaware_event[];
extern int32_t muj_settoku_event[];
extern int32_t muj_settoku_event_main[];
extern int32_t muj_01_init_evt[];
extern int32_t muj_mony_init_02[];
extern int32_t muj_mony_talk_02[];
extern int32_t muj_peton_init_02[];
extern int32_t muj_peton_talk_02[];
extern int32_t muj_02_init_evt[];
extern int32_t muj_03_event_01[];
extern int32_t muj_03_init_evt[];
extern int32_t muj_sanders_funto[];
extern int32_t muj_yashi_yure[];
extern int32_t muj_04_init_evt[];
extern int32_t muj_sanders_nakama[];
extern int32_t muj_sanders_init_05[];
extern int32_t muj_sanders_talk_sub[];
extern int32_t muj_sanders_talk_05[];
extern int32_t muj_marco_init_05[];
extern int32_t muj_marco_talk_05[];
extern int32_t muj_sanders_kyushutsu_sub[];
extern int32_t muj_marco_and_jewel[];
extern int32_t muj_dokuroiwa_touch[];
extern int32_t muj_dokuroiwa_bomb[];
extern int32_t muj_dokuroiwa_init[];
extern int32_t muj_yure_yashi_ha1[];
extern int32_t muj_yure_yashi[];
extern int32_t muj_05_init_evt[];
extern int32_t muj_megane_init_10[];
extern int32_t muj_bero_in_event_10[];
extern int32_t muj_nazo_event[];
extern int32_t muj_oidasare_event[];
extern int32_t unk_evt_muj_0002cb0c[];
extern int32_t muj_10_init_evt[];
extern int32_t muj_warning_event[];
extern int32_t muj_11_init_evt[];
extern int32_t muj_bero_custom_12[];
extern int32_t muj_korutesu_event[];
extern int32_t muj_korutesu_event_after[];
extern int32_t muj_kosho_event[];
extern int32_t muj_12_init_evt[];
extern int32_t muj_onryo_event[];
extern int32_t muj_20_koopa_evt[];
extern int32_t muj_20_init_evt[];
extern int32_t muj_koburon_dead[];
extern int32_t muj_all_party_lecture[];
extern int32_t muj_make_itemsel_table[];

const char marco[] = "\x83\x7D\x83\x8B\x83\x52";
const char sanders_tribe_01[] = "\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58\x82\xD6\x82\xEB\x82\xD6\x82\xEB";
const char sanders_tribe_02[] = "\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58\x82\xCB\x82\xDE\x82\xE9";
const char sanders_05[] = "\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58";

EVT_DEFINE_USER_FUNC(coconut_remove)
{
    (void)isFirstCall;
    (void)evt;

    // Loop through all of the important items until either the coconut or an empty slot is found
    constexpr uint32_t loopCount = sizeof(ttyd::mario_pouch::PouchData::key_items) / sizeof(int16_t);
    int16_t *keyItemsPtr = &ttyd::mario_pouch::pouchGetPtr()->key_items[0];
    bool foundCoconut = false;

    for (uint32_t i = 0; i < loopCount; i++)
    {
        const int32_t currentItem = keyItemsPtr[i];
        if (currentItem == ItemId::INVALID_NONE)
        {
            // The coconut is not in the important items, so do some failsafe or something
            break;
        }
        else if (currentItem != ItemId::COCONUT)
        {
            continue;
        }

        // Found where the coconut is, so move the rest of the items up one slot
        const uint32_t remainingSize = loopCount - i - 1;
        memcpy(&keyItemsPtr[i], &keyItemsPtr[i + 1], remainingSize * sizeof(int16_t));

        // Make sure the last slot is empty, in the event that the important items inventory was full prior to removing the
        // coconut
        keyItemsPtr[loopCount - 1] = ItemId::INVALID_NONE;
        foundCoconut = true;
        break;
    }

    if (!foundCoconut)
    {
        // The coconut is not in the important items, so do some failsafe or something
    }

    return 2;
}

// clang-format off
EVT_BEGIN(mony_talk_muj_00_evt)
	IF_EQUAL(GSW(1709), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_146_02"), 0, PTR("me"))
	ELSE()
		SWITCH(GSW(1717))
			CASE_EQUAL(13)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_94_04"), 0, PTR("me"))
			CASE_END()
			CASE_BETWEEN(14, 15)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_117_07"), 0, PTR("me"))
			CASE_END()
		END_SWITCH()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(mony_talk_muj_00_hook)
	RUN_CHILD_EVT(mony_talk_muj_00_evt)
	RETURN()
EVT_END()

EVT_BEGIN(peton_talk_muj_00_evt)
	IF_EQUAL(GSW(1709), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_146_01"), 0, PTR("me"))
	ELSE()
		SWITCH(GSW(1717))
			CASE_EQUAL(13)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_94_05"), 0, PTR("me"))
			CASE_END()
			CASE_BETWEEN(14, 15)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_117_08"), 0, PTR("me"))
			CASE_END()
		END_SWITCH()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(peton_talk_muj_00_hook)
	RUN_CHILD_EVT(peton_talk_muj_00_evt)
	RETURN()
EVT_END()

EVT_BEGIN(muj_00_init_evt_evt)
	IF_EQUAL(GSW(1705), 9)
		RUN_EVT(muj_mujinto_hyochaku)
		USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A_b_bero01"), 1, 0)
		SET(LW(0), 1)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(muj_00_init_evt_evt_2)
	RUN_CHILD_EVT(&muj_nannpc_mode_setup)
	USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S_c"), 1)
	USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A_c"), 1, 1)
	USER_FUNC(evt_hit::evt_hit_bind_mapobj, PTR("A_c"), PTR("S_c"))
	RUN_EVT(&muj_dai3_shurai)
	RUN_EVT(&muj_suifu_random_sub)
	USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("A_b_bero01"), 1, 0)
	RETURN()
EVT_END()

EVT_BEGIN(muj_00_init_evt_hook)
	RUN_CHILD_EVT(muj_00_init_evt_evt)
	IF_EQUAL(LW(0), 1)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1717), 13)
		RUN_CHILD_EVT(muj_00_init_evt_evt_2)
		RETURN()
	END_IF()
	GOTO(&muj_00_init_evt[323])
EVT_END()

EVT_BEGIN(garawaru_init_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1705), 10)
		IF_SMALL_EQUAL(GSW(1719), 5)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -155, 0, 170)
			RETURN()
		END_IF()
	END_IF()
	IF_EQUAL(GSW(1719), 6)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -110, 0, 225)
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1719), 7)
		IF_SMALL_EQUAL(GSW(1717), 11)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -155, 0, 170)
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1717), 26)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -110, 0, 225)
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_init_muj_01_hook)
	RUN_CHILD_EVT(garawaru_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_talk_muj_01_evt)
	IF_EQUAL(GSW(1705), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_22_1"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL_EQUAL(GSW(1707), 16)
		IF_EQUAL(GSWF(3878), 0)
			SET(LF(0), 0)
			IF_LARGE_EQUAL(GSW(1707), 3)
				IF_SMALL_EQUAL(GSW(1707), 3)
					IF_EQUAL(GSWF(3877), 1)
						SET(GSWF(3878), 1)
						SET(LF(0), 1)
					END_IF()
				END_IF()
			END_IF()
			IF_EQUAL(LF(0), 0)
				USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
				IF_EQUAL(LW(0), 3)
					USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_152"), 0, PTR("me"))
				ELSE()
					USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_1522_01"), 0, PTR("me"))
				END_IF()
			ELSE()
				USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
				IF_EQUAL(LW(0), 3)
					USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_muj_01"), 0, PTR("me"))
				ELSE()
					USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_muj_012_01"), 0, PTR("me"))
				END_IF()
			END_IF()
			RETURN()
		END_IF()
	END_IF()
	SWITCH(GSW(1719))
		CASE_BETWEEN(0, 5)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_32_02"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_EQUAL(6)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_71_05"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_71_04"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
		CASE_BETWEEN(7, 8)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_02"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_02_01"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
	END_SWITCH()
	SWITCH(GSW(1717))
		CASE_BETWEEN(1, 11)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_13"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_132_01"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
		CASE_BETWEEN(25, 29)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_149"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_1492_01"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
	END_SWITCH()
	SWITCH(GSW(1709))
		CASE_BETWEEN(4, 6)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_150"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_1502_01"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
		CASE_EQUAL(7)
			USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
			IF_EQUAL(LW(0), 3)
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_151"), 0, PTR("me"))
			ELSE()
				USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_1512_01"), 0, PTR("me"))
			END_IF()
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_SMALL(GSW(1708), 17)
		USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
		IF_EQUAL(LW(0), 3)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_153"), 0, PTR("me"))
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_1532_01"), 0, PTR("me"))
		END_IF()
		RETURN()
	END_IF()
	USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
	IF_EQUAL(LW(0), 3)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_153_01"), 0, PTR("me"))
	ELSE()
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_153_01_san"), 0, PTR("me"))
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_talk_muj_01_hook)
	RUN_CHILD_EVT(garawaru_talk_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(marco_init_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1705), 10)
		IF_SMALL_EQUAL(GSW(1719), 6)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -64, 0, 170)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
			RETURN()
		END_IF()
	END_IF()
	IF_LARGE_EQUAL(GSW(1717), 2)
		IF_SMALL_EQUAL(GSW(1717), 11)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -64, 0, 170)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(marco_init_muj_01_hook)
	RUN_CHILD_EVT(marco_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(marco_talk_coconut)
	USER_FUNC(coconut_remove, 0)
	EVT_HELPER_OP(0)
EVT_PATCH_END()

EVT_BEGIN(megane_init_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1719), 1)
		IF_SMALL_EQUAL(GSW(1717), 7)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 115, 0, 247)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(megane_init_muj_01_hook)
	RUN_CHILD_EVT(megane_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_init_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1719), 1)
		IF_SMALL_EQUAL(GSW(1717), 11)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 95, 0, 82)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_init_muj_01_hook)
	RUN_CHILD_EVT(odoodo_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_a_init_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1705), 10)
		IF_SMALL_EQUAL(GSW(1717), 18)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -193, 0, 77)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(suifu_a_init_muj_01_hook)
	RUN_CHILD_EVT(suifu_a_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_a_talk_muj_01_evt)
	IF_EQUAL(GSW(1705), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_22_2"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1719))
		CASE_BETWEEN(0, 5)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_32_03"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_BETWEEN(6, 8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_71_06"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	SWITCH(GSW(1717))
		CASE_BETWEEN(1, 11)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_14"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_LARGE_EQUAL(12)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_16"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(suifu_a_talk_muj_01_hook)
	RUN_CHILD_EVT(suifu_a_talk_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_bomb_init_evt)
	IF_LARGE_EQUAL(GSW(1705), 10)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -33, 0, 74)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(suifu_bomb_init_hook)
	RUN_CHILD_EVT(suifu_bomb_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_bomb_talk_evt)
	IF_EQUAL(GSW(1705), 11)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_22_3"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1705))
		CASE_BETWEEN(0, 5)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_32_04"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_BETWEEN(6, 8)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_71_07"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	SWITCH(GSW(1717))
		CASE_BETWEEN(1, 11)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_15"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_LARGE_EQUAL(12)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_90_17"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(suifu_bomb_talk_hook)
	RUN_CHILD_EVT(suifu_bomb_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_d_talk_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1717), 25)
		IF_SMALL_EQUAL(GSW(1717), 29)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_159"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	SWITCH(GSW(1709))
		CASE_BETWEEN(4, 6)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_160"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_EQUAL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_161"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_LARGE_EQUAL(GSW(1707), 20)
		IF_SMALL_EQUAL(GSW(1708), 18)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_163"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_163_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(suifu_d_talk_muj_01_hook)
	RUN_CHILD_EVT(suifu_d_talk_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(borosuifu_a_talk_evt)
	IF_LARGE_EQUAL(GSW(1717), 25)
		IF_SMALL_EQUAL(GSW(1717), 29)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_154"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	SWITCH(GSW(1709))
		CASE_BETWEEN(4, 6)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_155"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_EQUAL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_156"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_LARGE_EQUAL(GSW(1707), 20)
		IF_SMALL_EQUAL(GSW(1708), 18)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_158"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_158_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(borosuifu_a_talk_hook)
	RUN_CHILD_EVT(borosuifu_a_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(borosuifu_b_talk_evt)
	IF_LARGE_EQUAL(GSW(1717), 25)
		IF_SMALL_EQUAL(GSW(1717), 29)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_164"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	SWITCH(GSW(1709))
		CASE_BETWEEN(4, 6)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_165"), 0, PTR("me"))
			RETURN()
		CASE_END()
		CASE_EQUAL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_166"), 0, PTR("me"))
			RETURN()
		CASE_END()
	END_SWITCH()
	IF_LARGE_EQUAL(GSW(1707), 20)
		IF_SMALL_EQUAL(GSW(1708), 18)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_168"), 0, PTR("me"))
			RETURN()
		END_IF()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg5_muj_168_1"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(borosuifu_b_talk_hook)
	RUN_CHILD_EVT(borosuifu_b_talk_evt)
	RETURN()
EVT_END()

EVT_BEGIN(muj_01_init_evt_evt)
	IF_EQUAL(GSW(1705), 10)
		RUN_EVT(muj_nakamaware_event)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1719), 1)
		IF_EQUAL(GSWF(3129), 0)
			RUN_EVT(muj_settoku_event)
			RETURN()
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(muj_01_init_evt_hook)
	RUN_CHILD_EVT(muj_01_init_evt_evt)
	GOTO(&muj_01_init_evt[163])
EVT_PATCH_END()

EVT_BEGIN(koburon_dead_evt)
	USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
	USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_BTL_DAMAGED_PLIABLE1"), LW(0), LW(1), LW(2), 0)
	USER_FUNC(evt_npc::evt_npc_set_damage_anim, PTR("me"))
	WAIT_MSEC(200)
	ADD(LW(1), 30)
	ADD(LW(2), 10)
	SWITCH(GSW(721))
		CASE_EQUAL(0)
			USER_FUNC(evt_item::evt_item_entry, PTR("item_99"), LW(3), LW(0), LW(1), LW(2), 14, GSWF(6082), 0)
		CASE_EQUAL(1)
			USER_FUNC(evt_item::evt_item_entry, PTR("item_99"), LW(3), LW(0), LW(1), LW(2), 14, GSWF(6083), 0)
		CASE_EQUAL(2)
			USER_FUNC(evt_item::evt_item_entry, PTR("item_99"), LW(3), LW(0), LW(1), LW(2), 14, GSWF(6084), 0)
		CASE_EQUAL(3)
			USER_FUNC(evt_item::evt_item_entry, PTR("item_99"), LW(3), LW(0), LW(1), LW(2), 14, GSWF(6085), 0)
		CASE_EQUAL(4)
			USER_FUNC(evt_item::evt_item_entry, PTR("item_99"), LW(3), LW(0), LW(1), LW(2), 14, GSWF(6086), 0)
		CASE_EQUAL(5)
			USER_FUNC(evt_item::evt_item_entry, PTR("item_99"), LW(3), LW(0), LW(1), LW(2), 14, GSWF(6087), 0)
		CASE_EQUAL(6)
			USER_FUNC(evt_item::evt_item_entry, PTR("item_99"), LW(3), LW(0), LW(1), LW(2), 14, GSWF(6088), 0)
		CASE_EQUAL(7)
			USER_FUNC(evt_item::evt_item_entry, PTR("item_99"), LW(3), LW(0), LW(1), LW(2), 14, GSWF(6089), 0)
		CASE_ETC()
			USER_FUNC(evt_item::evt_item_entry, PTR("item_99"), 0, LW(0), LW(1), LW(2), 14, -1, 0)
	END_SWITCH()
	WAIT_MSEC(400)
	IF_EQUAL(GSW(721), 7)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("koburon_14"), 0, PTR("me"))
		GOTO(99)
	END_IF()
	USER_FUNC(muj_koburon_get_encount_info, LW(0))
	SWITCH(LW(0))
		CASE_OR(131072)
		CASE_OR(262144)
		CASE_OR(524288)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("koburon_10"), 0, PTR("me"))
		CASE_END()
		CASE_OR(1048576)
		CASE_OR(2097152)
		CASE_OR(4194304)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("koburon_10"), 0, PTR("me"))
		CASE_END()
		CASE_OR(8388608)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("koburon_11"), 0, PTR("me"))
		CASE_END()
		CASE_OR(16777216)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("koburon_13"), 0, PTR("me"))
		CASE_END()
	END_SWITCH()
	LBL(99)
	USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), PTR("S_1"))
	WAIT_MSEC(100)
	ADD(GSW(721), 1)
	SET(LSWF(2), 1)
	USER_FUNC(evt_npc::evt_npc_reaction_flag_onoff, 0, PTR("me"), 15)
	USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), PTR("A_2"))
	WAIT_MSEC(500)
	USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 1, 0)
	USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
	SET(GSWF(3151), 1)
	RETURN()
EVT_END()

EVT_BEGIN(koburon_dead_hook)
	RUN_CHILD_EVT(koburon_dead_evt)
	RETURN()
EVT_END()

EVT_BEGIN(mony_init_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1709), 4)
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1717), 26)
		IF_SMALL_EQUAL(GSW(1717), 29)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 105, 0, 274)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(mony_init_muj_01_hook)
	RUN_CHILD_EVT(mony_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(peton_init_muj_01_evt)
	IF_LARGE_EQUAL(GSW(1709), 4)
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1717), 26)
		IF_SMALL_EQUAL(GSW(1717), 29)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 45, 0, 274)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(peton_init_muj_01_hook)
	RUN_CHILD_EVT(peton_init_muj_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(muj_party_evt)
	USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
	USER_FUNC(evt_item::evt_item_entry, PTR("item01"), LW(3), LW(0), LW(1), LW(2), 16, GSWF(6081), 0)
	USER_FUNC(evt_item::evt_item_get_item, PTR("item01"))
	WAIT_MSEC(800)
	SET(GSW(1723), 2)
	USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
	USER_FUNC(evt_cam::evt_cam3d_evt_off, 500, 11)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
	USER_FUNC(evt_snd::evt_snd_bgmon, 512, PTR("BGM_STG5_MUJ1"))
	RETURN()
EVT_END()

EVT_BEGIN(muj_party_hook)
	RUN_CHILD_EVT(muj_party_evt)
	RETURN()
EVT_END()

EVT_BEGIN(dokuroiwa_bomb_evt)
	USER_FUNC(evt_hit::evt_hitobj_onoff, PTR("e_bero"), 1, 1)
	USER_FUNC(evt_npc::evt_npc_set_position, PTR(&marco), 279, 0, -67)
	RETURN()
EVT_END()

EVT_BEGIN(dokuroiwa_bomb_hook)
	RUN_CHILD_EVT(dokuroiwa_bomb_evt)
	GOTO(&muj_dokuroiwa_bomb[8])
EVT_PATCH_END()

EVT_BEGIN(muj_sanders_init_05_evt)
	IF_SMALL(GSW(1719), 2)
		USER_FUNC(evt_npc::evt_npc_set_tribe, PTR("me"), PTR(&sanders_05))
		RETURN()
	END_IF()
	SWITCH(GSW(1723))
		CASE_EQUAL(0)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -114, 22, 22)
			USER_FUNC(evt_npc::evt_npc_set_tribe, PTR("me"), PTR(&sanders_tribe_01))
		CASE_EQUAL(1)
			USER_FUNC(evt_npc::evt_npc_reaction_flag_onoff, 1, PTR("me"), 2)
			USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 8, PTR(&muj_sanders_nakama))
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -114, 22, 22)
			USER_FUNC(evt_npc::evt_npc_set_tribe, PTR("me"), PTR(&sanders_tribe_02))
			USER_FUNC(evt_npc::evt_npc_flag_onoff, 0, PTR(&sanders_05), 512)
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(muj_sanders_init_05_hook)
	RUN_CHILD_EVT(muj_sanders_init_05_evt)
	RETURN()
EVT_END()
// clang-format on

namespace mod
{
    void main()
    {
        // This sequence has to be written like this due to the LW(3) values overwriting the ones written directly into the ROM
        // via AP
        muj_koburon_dead[0] = EVT_HELPER_CMD(1, 34);
        muj_koburon_dead[1] = EVT_HELPER_OP(GSW(721));
        muj_koburon_dead[2] = EVT_HELPER_CMD(1, 36);
        muj_koburon_dead[3] = EVT_HELPER_OP(0);
        muj_koburon_dead[4] = EVT_HELPER_CMD(2, 50);
        muj_koburon_dead[5] = EVT_HELPER_OP(LW(3));
        muj_koburon_dead[7] = EVT_HELPER_CMD(1, 36);
        muj_koburon_dead[8] = EVT_HELPER_OP(1);
        muj_koburon_dead[9] = EVT_HELPER_CMD(2, 50);
        muj_koburon_dead[10] = EVT_HELPER_OP(LW(3));
        muj_koburon_dead[12] = EVT_HELPER_CMD(1, 36);
        muj_koburon_dead[13] = EVT_HELPER_OP(2);
        muj_koburon_dead[14] = EVT_HELPER_CMD(2, 50);
        muj_koburon_dead[15] = EVT_HELPER_OP(LW(3));
        muj_koburon_dead[17] = EVT_HELPER_CMD(1, 36);
        muj_koburon_dead[18] = EVT_HELPER_OP(3);
        muj_koburon_dead[19] = EVT_HELPER_CMD(2, 50);
        muj_koburon_dead[20] = EVT_HELPER_OP(LW(3));
        muj_koburon_dead[22] = EVT_HELPER_CMD(1, 36);
        muj_koburon_dead[23] = EVT_HELPER_OP(4);
        muj_koburon_dead[24] = 0;
        muj_koburon_dead[25] = EVT_HELPER_CMD(2, 50);
        muj_koburon_dead[26] = EVT_HELPER_OP(LW(3));
        muj_koburon_dead[28] = EVT_HELPER_CMD(1, 36);
        muj_koburon_dead[29] = EVT_HELPER_OP(5);
        muj_koburon_dead[30] = EVT_HELPER_CMD(2, 50);
        muj_koburon_dead[31] = EVT_HELPER_OP(LW(3));
        muj_koburon_dead[33] = EVT_HELPER_CMD(1, 36);
        muj_koburon_dead[34] = EVT_HELPER_OP(6);
        muj_koburon_dead[35] = EVT_HELPER_CMD(2, 50);
        muj_koburon_dead[36] = EVT_HELPER_OP(LW(3));
        muj_koburon_dead[38] = EVT_HELPER_CMD(1, 36);
        muj_koburon_dead[39] = EVT_HELPER_OP(7);
        muj_koburon_dead[40] = 0;
        muj_koburon_dead[41] = EVT_HELPER_CMD(2, 50);
        muj_koburon_dead[42] = EVT_HELPER_OP(LW(3));
        muj_koburon_dead[44] = EVT_HELPER_CMD(0, 49);

        patch::writePatch(&muj_koburon_dead[45], koburon_dead_hook, sizeof(koburon_dead_hook));

        muj_sanders_nakama[150] = EVT_HELPER_CMD(2, 50);
        muj_sanders_nakama[151] = EVT_HELPER_OP(LW(3));
        patch::writePatch(&muj_sanders_nakama[153], muj_party_hook, sizeof(muj_party_hook));

        muj_garawaru_init_00[1] = GSW(1717);
        muj_garawaru_init_00[3] = 14;
        muj_garawaru_init_00[4] = 15;

        muj_odoodo_init_00[1] = GSW(1717);
        muj_odoodo_init_00[3] = 14;
        muj_odoodo_init_00[4] = 15;

        muj_suifu_c_init[1] = GSW(1717);
        muj_suifu_c_init[3] = 14;
        muj_suifu_c_init[4] = 15;

        muj_suifu_c_talk[1] = GSW(1717);
        muj_suifu_c_talk[3] = 13;
        muj_suifu_c_talk[11] = 14;
        muj_suifu_c_talk[12] = 15;

        muj_suifu_d_init_00[1] = GSW(1717);
        muj_suifu_d_init_00[3] = 14;
        muj_suifu_d_init_00[4] = 15;

        muj_suifu_d_talk_00[1] = GSW(1717);
        muj_suifu_d_talk_00[3] = 13;
        muj_suifu_d_talk_00[11] = 14;
        muj_suifu_d_talk_00[12] = 15;

        muj_suifu_e_init[1] = GSW(1717);
        muj_suifu_e_init[3] = 14;
        muj_suifu_e_init[4] = 15;

        muj_suifu_e_talk[1] = GSW(1717);
        muj_suifu_e_talk[3] = 13;
        muj_suifu_e_talk[11] = 14;
        muj_suifu_e_talk[12] = 15;

        muj_korutesu_init_00[1] = GSW(1717);
        muj_korutesu_init_00[3] = 24;

        muj_korutesu_talk_00[1] = GSW(1717);
        muj_korutesu_talk_00[3] = 25;

        patch::writePatch(&muj_mony_talk_00[0], mony_talk_muj_00_hook, sizeof(mony_talk_muj_00_hook));

        patch::writePatch(&muj_peton_talk_00[0], peton_talk_muj_00_hook, sizeof(peton_talk_muj_00_hook));

        muj_mujinto_hyochaku[277] = GSW(1705);
        muj_mujinto_hyochaku[278] = 10;

        muj_dai3_shurai_sub[1096] = GSW(1717);
        muj_dai3_shurai_sub[1097] = 14;

        muj_kaizoku_vs_dai3[522] = GSW(1717);
        muj_kaizoku_vs_dai3[523] = 17;

        muj_dai3_beat_back[148] = GSW(1717);
        muj_dai3_beat_back[149] = 18;

        muj_leave_island[757] = GSW(1717);
        muj_leave_island[758] = 25;

        muj_into_muj_ship[161] = GSW(1709);
        muj_into_muj_ship[162] = 4;
        muj_into_muj_ship[164] = GSW(1709);
        muj_into_muj_ship[165] = 5;

        muj_mony_peton_boarding[4] = GSW(1709);
        muj_mony_peton_boarding[5] = 7;
        muj_mony_peton_boarding[11] = GSW(1709);
        muj_mony_peton_boarding[12] = 8;
        muj_mony_peton_boarding[45] = GSW(1709);
        muj_mony_peton_boarding[46] = 8;
        muj_mony_peton_boarding[111] = GSW(1709);
        muj_mony_peton_boarding[112] = 8;

        muj_00_init_evt[29] = GSW(1717);
        muj_00_init_evt[30] = 25;
        muj_00_init_evt[46] = GSW(1705);
        muj_00_init_evt[47] = 9;
        muj_00_init_evt[49] = GSW(1717);
        muj_00_init_evt[50] = 24;
        muj_00_init_evt[52] = GSW(1717);
        muj_00_init_evt[53] = 17;
        muj_00_init_evt[98] = GSW(1708);
        muj_00_init_evt[99] = 2;
        muj_00_init_evt[236] = GSW(1717);
        muj_00_init_evt[237] = 12;
        muj_00_init_evt[264] = GSW(1717);
        muj_00_init_evt[265] = 17;
        muj_00_init_evt[281] = GSW(1717);
        muj_00_init_evt[282] = 14;
        muj_00_init_evt[284] = GSW(1717);
        muj_00_init_evt[285] = 15;

        patch::writePatch(&muj_00_init_evt[296], muj_00_init_evt_hook, sizeof(muj_00_init_evt_hook));

        muj_00_init_evt[323] = EVT_HELPER_CMD(1, 34);
        muj_00_init_evt[324] = GSW(1717);
        muj_00_init_evt[326] = 14;
        muj_00_init_evt[332] = 16;
        muj_00_init_evt[370] = 17;
        muj_00_init_evt[411] = 24;
        muj_00_init_evt[481] = 25;

        patch::writePatch(&muj_garawaru_init_01[0], garawaru_init_muj_01_hook, sizeof(garawaru_init_muj_01_hook));

        patch::writePatch(&muj_garawaru_talk_01[0], garawaru_talk_muj_01_hook, sizeof(garawaru_talk_muj_01_hook));

        patch::writePatch(&muj_marco_init_01[0], marco_init_muj_01_hook, sizeof(marco_init_muj_01_hook));

        muj_marco_talk_01[1] = GSW(1719);
        muj_marco_talk_01[3] = 1;
        muj_marco_talk_01[4] = 4;
        muj_marco_talk_01[13] = GSW(1719);
        muj_marco_talk_01[15] = 0;
        muj_marco_talk_01[23] = 1;
        muj_marco_talk_01[24] = 1;
        muj_marco_talk_01[46] = 2;
        muj_marco_talk_01[89] = GSW(1719);
        muj_marco_talk_01[90] = 3;
        muj_marco_talk_01[98] = 3;
        patch::writePatch(&muj_marco_talk_01[290], marco_talk_coconut, sizeof(marco_talk_coconut));
        muj_marco_talk_01[358] = GSW(1719);
        muj_marco_talk_01[359] = 4;
        muj_marco_talk_01[361] = 99; // Unused
        muj_marco_talk_01[369] = 99; // Unused
        muj_marco_talk_01[377] = 4;
        muj_marco_talk_01[831] = GSW(1719);
        muj_marco_talk_01[832] = 7;
        muj_marco_talk_01[836] = EVT_HELPER_CMD(0, 42);
        muj_marco_talk_01[837] = 0;
        muj_marco_talk_01[838] = 0;

        patch::writePatch(&muj_megane_init_01[0], megane_init_muj_01_hook, sizeof(megane_init_muj_01_hook));

        muj_megane_talk_01[1] = GSW(1719);
        muj_megane_talk_01[3] = 1;
        muj_megane_talk_01[4] = 5;
        muj_megane_talk_01[12] = 6;
        muj_megane_talk_01[34] = 7;
        muj_megane_talk_01[35] = 8;
        muj_megane_talk_01[42] = EVT_HELPER_CMD(0, 42);
        muj_megane_talk_01[43] = 0;
        muj_megane_talk_01[44] = 0;

        patch::writePatch(&muj_odoodo_init_01[0], odoodo_init_muj_01_hook, sizeof(odoodo_init_muj_01_hook));

        muj_odoodo_talk_01[1] = GSW(1719);
        muj_odoodo_talk_01[3] = 1;
        muj_odoodo_talk_01[4] = 5;
        muj_odoodo_talk_01[12] = 6;
        muj_odoodo_talk_01[34] = 7;
        muj_odoodo_talk_01[35] = 8;
        muj_odoodo_talk_01[42] = EVT_HELPER_CMD(0, 42);
        muj_odoodo_talk_01[43] = 0;
        muj_odoodo_talk_01[44] = 0;

        patch::writePatch(&muj_suifu_a_init_01[0], suifu_a_init_muj_01_hook, sizeof(suifu_a_init_muj_01_hook));

        patch::writePatch(&muj_suifu_a_talk_01[0], suifu_a_talk_muj_01_hook, sizeof(suifu_a_talk_muj_01_hook));

        patch::writePatch(&muj_suifu_bomb_init[0], suifu_bomb_init_hook, sizeof(suifu_bomb_init_hook));

        patch::writePatch(&muj_suifu_bomb_talk[0], suifu_bomb_talk_hook, sizeof(suifu_bomb_talk_hook));

        muj_suifu_d_init_01[1] = GSW(1717);
        muj_suifu_d_init_01[3] = 26;

        muj_suifu_d_regl_01[1] = GSW(1717);
        muj_suifu_d_regl_01[3] = 26;

        patch::writePatch(&muj_suifu_d_talk_01[0], suifu_d_talk_muj_01_hook, sizeof(suifu_d_talk_muj_01_hook));

        muj_borosuifu_a_init[1] = GSW(1717);
        muj_borosuifu_a_init[3] = 26;

        patch::writePatch(&muj_borosuifu_a_talk[0], borosuifu_a_talk_hook, sizeof(borosuifu_a_talk_hook));

        muj_borosuifu_b_init[6] = GSW(1717);
        muj_borosuifu_b_init[8] = 26;

        muj_borosuifu_b_regl[1] = GSW(1717);
        muj_borosuifu_b_regl[3] = 26;

        patch::writePatch(&muj_borosuifu_b_talk[0], borosuifu_b_talk_hook, sizeof(borosuifu_b_talk_hook));

        patch::writePatch(&muj_mony_init_01[0], mony_init_muj_01_hook, sizeof(mony_init_muj_01_hook));

        patch::writePatch(&muj_peton_init_01[0], peton_init_muj_01_hook, sizeof(peton_init_muj_01_hook));

        muj_kyoshu_event[182] = GSW(1705);
        muj_kyoshu_event[183] = 11;

        muj_settoku_event_main[68] = GSW(1719);
        muj_settoku_event_main[69] = 2;

        muj_01_init_evt[4] = GSW(1708);
        muj_01_init_evt[5] = 16;
        muj_01_init_evt[76] = GSW(1708);
        muj_01_init_evt[77] = 2;
        patch::writePatch(&muj_01_init_evt[148], muj_01_init_evt_hook, sizeof(muj_01_init_evt_hook));
        muj_01_init_evt[164] = GSW(1708);
        muj_01_init_evt[165] = 16;
        muj_01_init_evt[187] = GSW(1708);
        muj_01_init_evt[188] = 18;

        muj_mony_init_02[6] = GSW(1709);
        muj_mony_init_02[8] = 5;
        muj_mony_init_02[9] = 6;

        muj_peton_init_02[6] = GSW(1709);
        muj_peton_init_02[8] = 5;
        muj_peton_init_02[9] = 6;

        muj_mony_talk_02[1] = GSW(1709);
        muj_mony_talk_02[3] = 5;
        muj_mony_talk_02[16] = GSW(1709);
        muj_mony_talk_02[17] = 6;
        muj_mony_talk_02[19] = 6;
        muj_mony_talk_02[44] = GSW(1709);
        muj_mony_talk_02[45] = 7;

        muj_peton_talk_02[1] = GSW(1709);
        muj_peton_talk_02[3] = 5;
        muj_peton_talk_02[16] = GSW(1709);
        muj_peton_talk_02[17] = 6;
        muj_peton_talk_02[19] = 6;
        muj_peton_talk_02[44] = GSW(1709);
        muj_peton_talk_02[45] = 7;

        muj_02_init_evt[31] = GSW(1717);
        muj_02_init_evt[32] = 26;
        muj_02_init_evt[87] = GSW(1717);
        muj_02_init_evt[88] = 0; // Unused

        muj_03_event_01[1] = GSW(1719);
        muj_03_event_01[2] = 1;

        muj_03_init_evt[32] = 11;

        muj_sanders_funto[462] = GSW(1719);
        muj_sanders_funto[463] = 1;

        muj_yashi_yure[111] = GSWF(6090);
        muj_yashi_yure[238] = GSWF(6091);

        muj_04_init_evt[70] = GSW(1719);
        muj_04_init_evt[72] = 0;

        patch::writePatch(&muj_sanders_init_05[0], muj_sanders_init_05_hook, sizeof(muj_sanders_init_05_hook));

        muj_sanders_talk_sub[63] = GSW(1723);
        muj_sanders_talk_sub[64] = 1;

        muj_sanders_talk_05[1] = GSWF(1723);
        muj_sanders_talk_05[3] = 0;
        muj_sanders_talk_05[4] = 0;
        muj_sanders_talk_05[38] = 1;

        muj_marco_init_05[1] = GSW(1719);
        muj_marco_init_05[3] = 8;

        muj_marco_talk_05[1] = GSW(1719);
        muj_marco_talk_05[3] = 8;
        muj_marco_talk_05[17] = GSWF(6108);
        muj_marco_talk_05[23] = EVT_HELPER_CMD(2, 29);
        muj_marco_talk_05[95] = GSWF(6108);

        muj_sanders_kyushutsu_sub[7] = GSW(1719);
        muj_sanders_kyushutsu_sub[8] = 2;

        muj_marco_and_jewel[77] = GSW(1719);
        muj_marco_and_jewel[78] = 8;

        muj_dokuroiwa_touch[23] = GSW(1719);
        muj_dokuroiwa_touch[24] = 7;

        patch::writePatch(&muj_dokuroiwa_bomb[3], dokuroiwa_bomb_hook, sizeof(dokuroiwa_bomb_hook));
        muj_dokuroiwa_bomb[302] = GSW(1717);
        muj_dokuroiwa_bomb[303] = 1;

        muj_dokuroiwa_init[14] = GSW(1717);
        muj_dokuroiwa_init[15] = 1;

        muj_yure_yashi_ha1[84] = GSW(1719);
        muj_yure_yashi_ha1[85] = 2;
        muj_yure_yashi_ha1[173] = GSW(1719);
        muj_yure_yashi_ha1[174] = 2;

        muj_yure_yashi[46] = GSW(1719);
        muj_yure_yashi[47] = 2;
        muj_yure_yashi[78] = GSW(1719);
        muj_yure_yashi[79] = 2;

        muj_05_init_evt[1] = GSW(1719);
        muj_05_init_evt[3] = 1;
        muj_05_init_evt[13] = 5;
        muj_05_init_evt[65] = GSW(1719);
        muj_05_init_evt[67] = 1;
        muj_05_init_evt[79] = 7;
        muj_05_init_evt[86] = GSW(1709);
        muj_05_init_evt[87] = 5;

        muj_megane_init_10[1] = GSW(1717);
        muj_megane_init_10[3] = 8;
        muj_megane_init_10[4] = 9;

        muj_bero_in_event_10[1] = GSW(1717);
        muj_bero_in_event_10[2] = 15;

        muj_nazo_event[168] = GSW(1717);
        muj_nazo_event[169] = 8;

        muj_oidasare_event[147] = GSW(1717);
        muj_oidasare_event[148] = 11;

        unk_evt_muj_0002cb0c[79] = GSW(1717);
        unk_evt_muj_0002cb0c[80] = 16;

        muj_10_init_evt[1] = GSW(1717);
        muj_10_init_evt[2] = 10;
        muj_10_init_evt[45] = GSW(1717);
        muj_10_init_evt[46] = 7;
        muj_10_init_evt[51] = GSW(1717);
        muj_10_init_evt[52] = 10;
        muj_10_init_evt[57] = GSW(1717);
        muj_10_init_evt[58] = 15;
        muj_10_init_evt[65] = GSW(1717);
        muj_10_init_evt[66] = 17;
        muj_10_init_evt[85] = GSW(1717);
        muj_10_init_evt[86] = 17;
        muj_10_init_evt[88] = GSW(1717);
        muj_10_init_evt[89] = 15;
        muj_10_init_evt[118] = GSW(1717);
        muj_10_init_evt[119] = 17;
        muj_10_init_evt[164] = GSW(1717);
        muj_10_init_evt[165] = 14;
        muj_10_init_evt[167] = GSW(1717);
        muj_10_init_evt[168] = 15;
        muj_10_init_evt[184] = GSW(1717);
        muj_10_init_evt[185] = 15;

        muj_warning_event[31] = GSW(1717);
        muj_warning_event[32] = 9;

        muj_11_init_evt[1] = GSW(1717);
        muj_11_init_evt[2] = 10;
        muj_11_init_evt[28] = GSW(1717);
        muj_11_init_evt[29] = 8;

        muj_bero_custom_12[1] = GSW(1717);
        muj_bero_custom_12[3] = 9;
        muj_bero_custom_12[57] = 14;

        muj_korutesu_event[606] = GSW(1717);
        muj_korutesu_event[607] = 10;

        muj_korutesu_event_after[132] = GSW(1717);
        muj_korutesu_event_after[133] = 10;

        muj_kosho_event[504] = GSW(1717);
        muj_kosho_event[505] = 15;

        muj_12_init_evt[8] = GSW(1717);
        muj_12_init_evt[9] = 10;
        muj_12_init_evt[28] = GSW(1717);
        muj_12_init_evt[29] = 9;
        muj_12_init_evt[44] = GSW(1717);
        muj_12_init_evt[45] = 10;
        muj_12_init_evt[50] = GSW(1717);
        muj_12_init_evt[51] = 14;

        muj_onryo_event[20] = GSW(1705);
        muj_onryo_event[21] = 8;
        muj_onryo_event[144] = GSW(1705);
        muj_onryo_event[145] = 8;
        muj_onryo_event[2235] = GSW(1705);
        muj_onryo_event[2236] = 9;

        muj_20_koopa_evt[122] = GSW(1715);
        muj_20_koopa_evt[123] = 11;

        muj_20_init_evt[109] = GSW(1715);
        muj_20_init_evt[110] = 10;
        muj_20_init_evt[210] = GSW(1705);
        muj_20_init_evt[211] = 7;
        muj_20_init_evt[224] = GSW(1705);
        muj_20_init_evt[225] = 8;
        muj_20_init_evt[334] = GSW(1708);
        muj_20_init_evt[335] = 18;

        // Assembly Patches
        patch::writeIntWithCache(&muj_make_itemsel_table[14], 0x38000079); // li r0, 0x79
        patch::writeIntWithCache(&muj_make_itemsel_table[20], 0x380300A0); // addi r0, r3, 0xA0
        patch::writeIntWithCache(&muj_make_itemsel_table[22], 0x2C0000A6); // cmpwi r0, 0xA6 (Coconut)
        patch::writeIntWithCache(&muj_make_itemsel_table[23], 0x4082000C); // bne +0xC
    }

    void exit() {}
} // namespace mod
