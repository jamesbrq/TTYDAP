#include "evt_cmd.h"
#include "patch.h"
#include <AP/rel_patch_definitions.h>
#include <ttyd/evt_msg.h>
#include <ttyd/evt_npc.h>

using namespace mod;
using namespace ttyd;

extern int32_t badgemaster_init[];
extern int32_t badgemaster_talk[];
extern int32_t kinoji_talk[];
extern int32_t marco_init_01[];
extern int32_t marco_talk_01[];
extern int32_t master_init[];
extern int32_t master_talk[];
extern int32_t shoptender_init[];
extern int32_t shoptender_talk[];
extern int32_t roten1_talk[];
extern int32_t roten2_talk[];
extern int32_t kuribo1_talk[];
extern int32_t kuribo2_talk[];
extern int32_t kuribo3_talk[];
extern int32_t chusan1_talk[];
extern int32_t chusan2_talk[];
extern int32_t bomhei_talk[];
extern int32_t bottakuru_init[];
extern int32_t luigi_init_01[];
extern int32_t luigi_npcEnt_01[];
extern int32_t kick2_init[];
extern int32_t thief_init[];
extern int32_t thief_talk[];
extern int32_t christine_nakama_before[];
extern int32_t luigi_f1_fade_check[];
extern int32_t luigi_f2_fade_check[];
extern int32_t marco_hitorigoto[];
extern int32_t goods_list;
extern int32_t gor_01_shop_setup[];
extern int32_t christine_nakama[];
extern int32_t gor_01_koopa_evt[];
extern int32_t gor_01_init_evt[];
extern int32_t cooking_evt[];

// clang-format off
EVT_BEGIN(badgemaster_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_024_00"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(marco_init_01_evt)
	IF_LARGE_EQUAL(GSW(1705), 1)
		IF_SMALL_EQUAL(GSW(1717), 26)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(marco_init_01_hook)
	RUN_CHILD_EVT(marco_init_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(bottakuru_init_evt)
	USER_FUNC(checkChapterClears, LW(0))
	IF_SMALL(LW(0), 1)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_npc::evt_npc_flag_onoff, 1, PTR("me"), 1073741824)
	USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
	SET(GSWF(1206), 0)
	RETURN()
EVT_END()

EVT_BEGIN(bottakuru_init_hook)
	RUN_CHILD_EVT(bottakuru_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(kinoji_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_019_02"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(shoptender_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_015_02"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(roten1_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_025_01"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(roten2_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_025_06_2"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(kuribo1_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_026_06"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(kuribo2_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_027_06"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(kuribo3_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_028_08"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(chusan1_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_029_04"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(chusan2_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_030_03"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(bomhei_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_031_05"), 0, PTR("me"))
	RETURN()
EVT_END()
// clang-format on

void ApplyGor01Patches()
{
    badgemaster_init[21] = GSW(1703);
    badgemaster_init[22] = 1;
    badgemaster_init[24] = GSW(1703);
    badgemaster_init[25] = 13;

    patch::writePatch(&badgemaster_talk[0], badgemaster_talk_evt, sizeof(badgemaster_talk_evt));

    patch::writePatch(&kinoji_talk[0], kinoji_talk_evt, sizeof(kinoji_talk_evt));

    patch::writePatch(&marco_init_01[0], marco_init_01_hook, sizeof(marco_init_01_hook));

    marco_talk_01[6] = GSW(1705);
    marco_talk_01[7] = EVT_HELPER_CMD(1, 39);
    marco_talk_01[8] = 99; // Never should be reachable
    marco_talk_01[22] = 0;
    marco_talk_01[483] = GSW(1705);
    marco_talk_01[484] = 1;
    marco_talk_01[485] = EVT_HELPER_CMD(0, 42);
    marco_talk_01[486] = EVT_HELPER_CMD(0, 0);
    marco_talk_01[493] = EVT_HELPER_CMD(0, 49);
    marco_talk_01[494] = EVT_HELPER_CMD(0, 2);
    marco_talk_01[495] = EVT_HELPER_CMD(0, 1);

    master_init[1] = GSW(1705);
    master_init[2] = 1;

    master_talk[99] = GSW(1705);
    master_talk[101] = 1;
    master_talk[109] = 2;
    master_talk[120] = 3;
    master_talk[483] = GSWF(6099);
    master_talk[544] = GSW(1705);
    master_talk[545] = 4;
    master_talk[556] = 4;
    master_talk[563] = EVT_HELPER_CMD(0, 42);
    master_talk[564] = EVT_HELPER_CMD(0, 0);
    master_talk[571] = EVT_HELPER_CMD(0, 49);
    master_talk[572] = EVT_HELPER_CMD(0, 2);
    master_talk[573] = EVT_HELPER_CMD(0, 1);

    shoptender_init[0] = EVT_HELPER_CMD(0, 0);
    shoptender_init[1] = EVT_HELPER_CMD(0, 0);
    shoptender_init[2] = EVT_HELPER_CMD(0, 0);

    patch::writePatch(&shoptender_talk[189], shoptender_talk_evt, sizeof(shoptender_talk_evt));

    patch::writePatch(&roten1_talk[0], roten1_talk_evt, sizeof(roten1_talk_evt));
    patch::writePatch(&roten2_talk[0], roten2_talk_evt, sizeof(roten2_talk_evt));
    patch::writePatch(&kuribo1_talk[25], kuribo1_talk_evt, sizeof(kuribo1_talk_evt));
    patch::writePatch(&kuribo2_talk[0], kuribo2_talk_evt, sizeof(kuribo2_talk_evt));
    patch::writePatch(&kuribo3_talk[68], kuribo3_talk_evt, sizeof(kuribo3_talk_evt));
    patch::writePatch(&chusan1_talk[104], chusan1_talk_evt, sizeof(chusan1_talk_evt));
    patch::writePatch(&chusan2_talk[0], chusan2_talk_evt, sizeof(chusan2_talk_evt));
    patch::writePatch(&bomhei_talk[0], bomhei_talk_evt, sizeof(bomhei_talk_evt));

    patch::writePatch(&bottakuru_init[0], bottakuru_init_hook, sizeof(bottakuru_init_hook));

    luigi_init_01[1] = GSW(1710);
    luigi_init_01[2] = EVT_HELPER_CMD(1, 36);
    luigi_init_01[3] = 2;
    luigi_init_01[4] = EVT_HELPER_CMD(0, 0);
    luigi_init_01[15] = EVT_HELPER_CMD(1, 36);
    luigi_init_01[16] = 4;
    luigi_init_01[17] = EVT_HELPER_CMD(0, 0);
    luigi_init_01[28] = EVT_HELPER_CMD(1, 41);
    luigi_init_01[29] = 7;

    luigi_npcEnt_01[1] = GSW(1710);
    luigi_npcEnt_01[2] = EVT_HELPER_CMD(1, 36);
    luigi_npcEnt_01[3] = 2;
    luigi_npcEnt_01[4] = EVT_HELPER_CMD(0, 0);
    luigi_npcEnt_01[16] = EVT_HELPER_CMD(1, 36);
    luigi_npcEnt_01[17] = 4;
    luigi_npcEnt_01[18] = EVT_HELPER_CMD(0, 0);
    luigi_npcEnt_01[30] = EVT_HELPER_CMD(1, 41);
    luigi_npcEnt_01[31] = 7;

    kick2_init[1] = GSW(1710);
    kick2_init[2] = EVT_HELPER_CMD(1, 36);
    kick2_init[3] = 7;
    kick2_init[4] = EVT_HELPER_CMD(0, 0);

    thief_init[1] = GSW(1700);

    thief_talk[1] = GSW(1700); // Thief Speach
    thief_talk[4] = 17;        // Thief Speach
    thief_talk[89] = EVT_HELPER_CMD(0, 42);
    thief_talk[90] = EVT_HELPER_CMD(0, 0);
    thief_talk[99] = EVT_HELPER_CMD(0, 49);
    thief_talk[100] = EVT_HELPER_CMD(0, 2);
    thief_talk[101] = EVT_HELPER_CMD(0, 1);

    christine_nakama_before[1] = GSW(1700);

    luigi_f1_fade_check[1] = GSW(1710); // Luigi color??
    luigi_f1_fade_check[3] = 2;
    luigi_f1_fade_check[4] = 3;
    luigi_f1_fade_check[6] = 4;
    luigi_f1_fade_check[7] = 5;
    luigi_f1_fade_check[23] = 7;

    luigi_f1_fade_check[1] = GSW(1710); // Luigi color 2??
    luigi_f1_fade_check[3] = 2;
    luigi_f1_fade_check[4] = 3;
    luigi_f1_fade_check[6] = 4;
    luigi_f1_fade_check[7] = 5;
    luigi_f1_fade_check[9] = 7;

    marco_hitorigoto[1] = GSW(1705);
    marco_hitorigoto[2] = 1;

    gor_01_shop_setup[124] = PTR(&goods_list);
    gor_01_shop_setup[129] = PTR(&goods_list);
    gor_01_shop_setup[134] = PTR(&goods_list);
    gor_01_shop_setup[139] = PTR(&goods_list);
    gor_01_shop_setup[144] = PTR(&goods_list);

    gor_01_koopa_evt[573] = GSW(1706);
    gor_01_koopa_evt[574] = 46;

    gor_01_init_evt[4] = GSW(1706);
    gor_01_init_evt[5] = 45;
    gor_01_init_evt[14] = GSW(1708);
    gor_01_init_evt[15] = 16;
    gor_01_init_evt[35] = GSW(1708);
    gor_01_init_evt[36] = 18;
    gor_01_init_evt[116] = GSW(1700);
    gor_01_init_evt[122] = GSW(1700);
    gor_01_init_evt[138] = GSW(1710);
    gor_01_init_evt[139] = 1;
    gor_01_init_evt[150] = GSW(1710);
    gor_01_init_evt[151] = 1;
    gor_01_init_evt[255] = GSW(1705);
    gor_01_init_evt[256] = 1;
    gor_01_init_evt[308] = GSW(1705);
    gor_01_init_evt[309] = 1;
    gor_01_init_evt[321] = GSW(1700);
    gor_01_init_evt[523] = GSW(1708);
    gor_01_init_evt[524] = 18;

    cooking_evt[412] = GSW(1715);
    cooking_evt[413] = 4;
}
