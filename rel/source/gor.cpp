#include <gor.h>
#include <ttyd/evt_npc.h>
#include <ttyd/evt_msg.h>
#include <ttyd/evt_urouro.h>
#include "evt_cmd.h"
#include "common_types.h"
#include "patch.h"

using namespace mod;
using namespace ttyd;

extern int32_t no00_custom[];
extern int32_t mony_init_gor00[];
extern int32_t frankie_run_event[];
extern int32_t mony_peton_talk[];
extern int32_t mony_talk[];
extern int32_t peton_init[];
extern int32_t francesca_run_event[];
extern int32_t peton_talk[];
extern int32_t marco_init[];
extern int32_t marco_talk[];
extern int32_t garawaru_init[];
extern int32_t garawaru_regl[];
extern int32_t garawaru_urouro2[];
extern int32_t garawaru_talk[];
extern int32_t megane_init[];
extern int32_t megane_talk[];
extern int32_t odoodo_init[];
extern int32_t odoodo_regl[];
extern int32_t odoodo_talk[];
extern int32_t odoodo_ani_init[];
extern int32_t odoodo_ani_regl[];
extern int32_t odoodo_ani_talk[];
extern int32_t suifu_a_init[];
extern int32_t suifu_a_regl[];
extern int32_t suifu_a_talk[];
extern int32_t suifu_b_init[];
extern int32_t suifu_b_regl[];
extern int32_t suifu_b_talk[];
extern int32_t suifu_d_init[];


EVT_BEGIN(garawaru_init_evt)
	IF_SMALL(GSW(1700), 4) //Before Crump Fight
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -200, 15, -175)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1704), 99) //During Bowser Intermission (Unknown Value)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 1, 0)
		USER_FUNC(evt_npc::evt_npc_restart_regular_event, PTR("me"))
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1705), 1) //After Talking to Flavio
		IF_SMALL_EQUAL(GSW(1705), 7)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 270)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 160, 73, 135)
			RETURN()
		END_IF()
	END_IF()
	IF_SMALL(GSW(1705), 7) //Before ship leaves for keelhaul key
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -200, 15, -175)
		RETURN()
	END_IF()
	USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_init_hook)
	RUN_CHILD_EVT(garawaru_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_regl_evt)
	IF_SMALL(GSW(1700), 7) //Before Goombella Joins
		IF_EQUAL(GSWF(1179), 1)
			RETURN()
		END_IF()
		DO(0)
			USER_FUNC(evt_npc::evt_npc_move_position, PTR("me"), 450, -175, 0, FLOAT(40.00), 0)
			WAIT_MSEC(1000)
			USER_FUNC(evt_npc::evt_npc_move_position, PTR("me"), -200, -175, 0, FLOAT(40.00), 0)
			IF_EQUAL(GSWF(1179), 1)
				DO_BREAK()
			END_IF()
			WAIT_MSEC(1000)
			IF_EQUAL(GSWF(1179), 1)
				DO_BREAK()
			END_IF()
		WHILE()
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1705), 0) //Before Talking to Flavio
		RUN_CHILD_EVT(garawaru_urouro2)
		RETURN()
	END_IF()
	IF_SMALL_EQUAL(GSW(1705), 7) //After Talking to flavio
		RETURN()
	END_IF()
	IF_LARGE(GSW(1705), 7) //After Ship Leaves
		RUN_CHILD_EVT(garawaru_urouro2)
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_regl_hook)
	RUN_CHILD_EVT(garawaru_regl_evt)
	RETURN()
EVT_END()

EVT_BEGIN(garawaru_talk_evt)
	IF_SMALL(GSW(1700), 7) // Before Goombella Joins
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_06_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_EQUAL(GSW(1705), 0) // Before talking to flavio
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_06_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1705), 6) // Before bobbery leaves his house
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_06_08"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1705), 7) // Before sailing off
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_4_025"), 0, PTR("me"))
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_init_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -225, 15, -400)
		RETURN()
	END_IF()
	SWITCH(GSW(1705))
		CASE_BETWEEN(1, 7)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 34, 77, 40)
		CASE_ETC()
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -200, 15, -400)
	END_SWITCH()
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_regl_evt)
	SWITCH(GSW(1705))
		CASE_BETWEEN(1, 7)
			RETURN()
		CASE_EQUAL(46)
			RETURN()
	END_SWITCH()
	IF_LARGE_EQUAL(GSW(1700), 22)
		USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
		USER_FUNC(evt_urouro::urouro_init_func, PTR("me"), LW(0), LW(2), FLOAT(100.00), FLOAT(30.00), 0)
		USER_FUNC(evt_urouro::urouro_main_func, PTR("me"))
		RETURN()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_regl_hook)
	RUN_CHILD_EVT(odoodo_regl_evt)
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_talk_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_05_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1705), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_05_07"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1705), 7)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_05_08"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1705), 8)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_4_024"), 0, PTR("me"))
		RETURN()
	END_IF()
	IF_SMALL(GSW(1705), 99) //Unknown, post ch.5
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_05_09"), 0, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_05_12"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(odoodo_ani_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_07_09"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(suifu_a_talk_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_03_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1705))
		CASE_SMALL(1)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_03_06"), 0, PTR("me"))
			RETURN()
		CASE_SMALL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_03_07"), 0, PTR("me"))
			RETURN()
		CASE_LARGE(99) // Unknown, Post Ch.5
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_03_09"), 0, PTR("me"))
			RETURN()
		CASE_ETC()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_03_04"), 0, PTR("me"))
			RETURN()
	END_SWITCH()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_03_12"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(suifu_b_init_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 270)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
		RETURN()
	END_IF()
	IF_LARGE_EQUAL(GSW(1704), 99) // Unknown, Bowser Intermission
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -191, 15, 36)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
		USER_FUNC(evt_npc::evt_npc_change_interrupt, PTR("me"), 1, 0)
		USER_FUNC(evt_npc::evt_npc_restart_regular_event, PTR("me"))
		RETURN()
	END_IF()
	USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
	USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, 15, -300)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_b_init_hook)
	RUN_CHILD_EVT(suifu_b_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(suifu_b_talk_evt)
	IF_SMALL(GSW(1700), 7)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_03_01"), 0, PTR("me"))
		RETURN()
	END_IF()
	SWITCH(GSW(1705))
		CASE_SMALL(1)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_04_06"), 0, PTR("me"))
			RETURN()
		CASE_SMALL(7)
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_04_07"), 0, PTR("me"))
			RETURN()
		CASE_LARGE(99) // Unknown, Post Ch.5
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_04_09"), 0, PTR("me"))
			RETURN()
		CASE_ETC()
			USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_04_04"), 0, PTR("me"))
			RETURN()
	END_SWITCH()
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_00_04_12"), 0, PTR("me"))
	RETURN()
EVT_END()

void ApplyGorPatches(OSModuleInfo* module_info)
{
	switch (module_info->id)
	{
		case ModuleId::GOR:
			//gor_00.o
			no00_custom[1] = GSW(1700); //Goombella Cutscene trigger loading into gor_01
			no00_custom[2] = 5;

			mony_init_gor00[1] = GSW(1709); //Frankie spawn trigger for blimp ticket
			mony_init_gor00[3] = 1;
			mony_init_gor00[4] = 2;

			frankie_run_event[1] = GSW(1709); //Frankie running to door after keelhaul key sequence
			//frankie_run_event[3] = 1; This value is not known yet.

			mony_peton_talk[270] = GSW(1709); //After talking to frankie during chapter 3 sequence.
			mony_peton_talk[271] = 2;

			mony_talk[1] = GSW(1709); //Initial dialogue options for frankie in chapter 3 sequence.
			mony_talk[3] = 1;
			mony_talk[7] = 2;

			peton_init[1] = GSW(1709); //Spawning Francesca during chapter 3 sequence.
			peton_init[3] = 1;
			peton_init[4] = 2;

			francesca_run_event[1] = GSW(1709); //Francesca running to door after keelhaul key sequence
			//francesca_run_event[3] = 1; This value is not known yet.

			peton_talk[1] = GSW(1709); //Initial dialogue options for francesca in chapter 3 sequence.
			peton_talk[3] = 1;
			peton_talk[7] = 2;

			marco_init[1] = GSW(1705); //Spawning Flavio during chapter 5 sequence.
			marco_init[3] = 1;
			marco_init[4] = 7;

			marco_talk[1] = GSW(1705); //Various dialogue options for flavio in chapter 5 sequence.
			marco_talk[3] = 1;
			marco_talk[268] = GSW(1705);
			marco_talk[269] = 2;
			marco_talk[271] = 2;
			marco_talk[290] = 6;
			marco_talk[298] = 7;

			patch::writePatch(&garawaru_init[5], garawaru_init_hook, sizeof(garawaru_init_hook)); //Bob-omb with bones wheel
			patch::writePatch(&garawaru_regl[0], garawaru_regl_hook, sizeof(garawaru_regl_hook));
			patch::writePatch(&garawaru_talk[0], garawaru_talk_evt, sizeof(garawaru_talk_evt));

			megane_init[1] = GSW(1705); //Unknown NPC Init
			megane_init[3] = 1;
			megane_init[4] = 7;

			megane_talk[1] = GSW(1705); //Unknown NPC Speach
			megane_talk[3] = 1;
			megane_talk[4] = 7;

			patch::writePatch(&odoodo_init[0], odoodo_init_evt, sizeof(odoodo_init_evt)); //Toad who lost brother init
			patch::writePatch(&odoodo_regl[0], odoodo_regl_hook, sizeof(odoodo_regl_hook)); //Toad who lost brother Movement
			patch::writePatch(&odoodo_talk[0], odoodo_talk_evt, sizeof(odoodo_talk_evt)); //Toad who lost brother Speach

			odoodo_ani_init[1] = GSW(1705); //Toads Brother Init
			odoodo_ani_init[3] = 99; //Unknown, Post Ch.5

			odoodo_ani_regl[1] = GSW(1705); //Toads Brother Movement
			odoodo_ani_regl[3] = 99; //Unknown, Post Ch.5
			patch::writePatch(&odoodo_ani_talk[0], odoodo_ani_talk_evt, sizeof(odoodo_ani_talk_evt)); //Toad Brother Speach

			suifu_a_init[1] = GSW(1704); //Random Toad Init
			suifu_a_init[3] = 99; //Unknown, Bowser intermission

			suifu_a_regl[1] = GSW(1750); //Random Toad Movement
			suifu_a_regl[3] = 1;
			suifu_a_regl[4] = 7;
			suifu_a_regl[6] = 7;

			patch::writePatch(&suifu_a_talk[0], suifu_a_talk_evt, sizeof(suifu_a_talk_evt)); //Random Toad Speach

			patch::writePatch(&suifu_b_init[0], suifu_b_init_hook, sizeof(suifu_b_init_hook)); //Random Toad 2 Init
			patch::writePatch(&suifu_b_regl[0], odoodo_regl_evt, sizeof(odoodo_regl_evt)); //Random Toad 2 Movement
			patch::writePatch(&suifu_b_talk[0], suifu_b_talk_evt, sizeof(suifu_b_talk_evt)); //Random Toad 2 Speach

			suifu_d_init[1] = GSW(1705); //Random Toad 3 Init
			suifu_d_init[3] = 1; 
			suifu_d_init[4] = 7;
	}
}