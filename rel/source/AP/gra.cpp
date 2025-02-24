#include <AP/gra.h>
#include <ttyd/evt_cam.h>
#include <ttyd/evt_npc.h>
#include <ttyd/evt_nannpc.h>
#include <ttyd/evt_msg.h>
#include <ttyd/evt_map.h>
#include <ttyd/evt_hit.h>
#include <ttyd/evt_mario.h>
#include <ttyd/evt_party.h>
#include <ttyd/evt_bero.h>
#include <ttyd/evt_snd.h>
#include <ttyd/evt_urouro.h>
#include "evt_cmd.h"
#include "common_types.h"
#include "patch.h"

using namespace mod;
using namespace ttyd;

extern int32_t evt_machibuse[];
extern int32_t gra_00_init_evt[];
extern int32_t gra_01_init_evt[];
extern int32_t shopmaster_pig_init[];
extern int32_t shopmaster_talk[];
extern int32_t gra02_get_key[];
extern int32_t gra02_straw_blow_evt[];
extern int32_t gra_02_init_evt[];
extern int32_t gra_04_tree_open_evt[];
extern int32_t gra_04_init_evt[];
extern int32_t gra_05_futa_open_evt[];
extern int32_t evt_gra_05iwa_move_init[];
extern int32_t evt_gra_05iwa_move_evt[];
extern int32_t evt_break_floor[];
extern int32_t gra_05_init_evt[];
extern int32_t first_gra06jin[];
extern int32_t gra_06_init_evt[];
extern int32_t evt_usu_kagemario_party_kill_gra[];

//Assembly
extern int32_t gra_evt_kagemario_init[];

const char doopliss[] = "\x82\xC9\x82\xB9\x83\x7D\x83\x8A\x83\x49";

EVT_BEGIN(evt_machibuse_evt1)
	IF_SMALL(GSWF(6044), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_gra_03"), 0, PTR(doopliss))
	ELSE()
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_gra_09"), 0, PTR(doopliss))
	END_IF()
	GOTO(90)
EVT_END()

void ApplyGraPatches(OSModuleInfo* module_info)
{
	evt_machibuse[78] = 0; //Unused
	evt_machibuse[291] = GSWF(6044);
	evt_machibuse[292] = 1;
	patch::writePatch(&evt_machibuse[358], evt_machibuse_evt1, sizeof(evt_machibuse_evt1));
	evt_machibuse[421] = EVT_HELPER_CMD(1, 3);
	evt_machibuse[422] = 90;
	evt_machibuse[426] = 0; //Unused
	evt_machibuse[476] = GSW(1715);
	evt_machibuse[477] = 210;
	evt_machibuse[538] = GSWF(6044);
	evt_machibuse[540] = 1;
	evt_machibuse[544] = EVT_HELPER_CMD(0, 49);
	evt_machibuse[545] = 0;
	evt_machibuse[546] = 0;
	evt_machibuse[547] = 0;
	evt_machibuse[548] = 0;
	evt_machibuse[549] = 0;
	evt_machibuse[550] = 0;
	evt_machibuse[551] = 0;

	gra_00_init_evt[65] = GSW(1715);
	gra_00_init_evt[67] = 202;
	gra_00_init_evt[68] = 209;

	gra_01_init_evt[85] = 500; //Unused

	shopmaster_pig_init[1] = GSW(1715);
	shopmaster_pig_init[3] = 200;

	shopmaster_talk[1] = GSW(1715);
	shopmaster_talk[3] = 200;

	gra02_get_key[1] = GSWF(6041);
	gra02_get_key[2] = 1;

	gra02_straw_blow_evt[19] = GSW(1714);
	gra02_straw_blow_evt[20] = 1;

	gra_02_init_evt[43] = GSWF(6041);
	gra_02_init_evt[44] = 1;
	gra_02_init_evt[57] = GSW(1714);
	gra_02_init_evt[58] = 1;

	gra_04_tree_open_evt[37] = GSW(1714);
	gra_04_tree_open_evt[38] = 2;

	gra_04_init_evt[20] = GSW(1714);
	gra_04_init_evt[21] = 2;

	gra_05_futa_open_evt[19] = GSWF(6047);
	gra_05_futa_open_evt[20] = 3;

	evt_gra_05iwa_move_init[5] = GSW(1714);
	evt_gra_05iwa_move_init[6] = 5;

	evt_gra_05iwa_move_evt[176] = GSW(1714);
	evt_gra_05iwa_move_evt[177] = 5;

	evt_break_floor[1] = GSW(1714);
	evt_break_floor[2] = 4;

	gra_05_init_evt[39] = GSW(1714);
	gra_05_init_evt[40] = 3;
	gra_05_init_evt[81] = GSW(1714);
	gra_05_init_evt[82] = 4;
	gra_05_init_evt[98] = GSW(1714);
	gra_05_init_evt[99] = 5;

	first_gra06jin[129] = GSW(1714);
	first_gra06jin[130] = 6;

	gra_06_init_evt[36] = GSW(1714);
	gra_06_init_evt[37] = 6;
	gra_06_init_evt[132] = GSW(1714);
	gra_06_init_evt[133] = 6;

	evt_usu_kagemario_party_kill_gra[1] = GSW(1714); //Unused

	gra_evt_kagemario_init[2] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
	gra_evt_kagemario_init[6] = 0x2C030003; // cmpwi r3, 0x3
	gra_evt_kagemario_init[8] = 0x2C030007; // cmpwi r3, 0x7
}