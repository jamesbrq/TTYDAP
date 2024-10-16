#include <AP/gor_misc.h>
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

extern int32_t first_evt[];
extern int32_t gonbaba_evt[];
extern int32_t first_evt_dokan[];
extern int32_t dokan_evt[];
extern int32_t hei_00_koopa_evt[];
extern int32_t hei_00_init_evt[];
extern int32_t bridge_evt[];
extern int32_t nokotaro_get[];
extern int32_t nokotaro_nakama[];
extern int32_t nokotarou_talk_2[];
extern int32_t evt_nokorin[];
extern int32_t hei_peach_mail[];
extern int32_t hei_01_init_evt[];
extern int32_t door_open_03[];
extern int32_t hei_03_init_evt[];
extern int32_t door_open_05[];
extern int32_t hei_05_init_evt[];
extern int32_t a_sekizou_1[];
extern int32_t hei_07_init_evt[];
extern int32_t evt_golden[];
extern int32_t hei_10_init_evt[];

void ApplyHeiPatches(OSModuleInfo* module_info)
{
	first_evt[187] = GSW(1701); //First cutscene
	first_evt[188] = 1;

	gonbaba_evt[424] = GSW(1701); //Hooktail flying cutscene
	gonbaba_evt[425] = 1;
	gonbaba_evt[434] = GSW(1701);
	gonbaba_evt[435] = 2;

	first_evt_dokan[1] = GSW(1701); //??? cutscene
	first_evt_dokan[2] = 1;

	dokan_evt[324] = GSW(1711); //After opening castle pipe
	dokan_evt[325] = 1;

	hei_00_koopa_evt[197] = GSW(1702); //Bowser intermission
	hei_00_koopa_evt[198] = 99; //Unknown

	hei_00_init_evt[1] = GSW(1702); //Room 0 Init
	hei_00_init_evt[2] = 99; //Unknown
	hei_00_init_evt[25] = GSW(1701);
	hei_00_init_evt[26] = 1;
	hei_00_init_evt[31] = GSW(1701);
	hei_00_init_evt[32] = 2;
	hei_00_init_evt[37] = GSW(1711);
	hei_00_init_evt[38] = 1;
	hei_00_init_evt[107] = GSW(1711);
	hei_00_init_evt[108] = 1;
	hei_00_init_evt[246] = GSW(1702);
	hei_00_init_evt[247] = 99; //Unknown
	hei_00_init_evt[249] = GSW(1708);
	hei_00_init_evt[250] = 99; //Unknown
	hei_00_init_evt[283] = GSW(1708);
	hei_00_init_evt[284] = 99; //Unknown

	bridge_evt[21] = GSWF(2001); //Bridge cutscene
	bridge_evt[22] = 1;
	
	nokotaro_get[69] = GSW(1701); //Koops get
	nokotaro_get[70] = 11;

	nokotaro_nakama[11] = GSW(1701); //Koops init
	nokotaro_nakama[12] = 10;

	nokotarou_talk_2[19] = GSW(1701); //Koops talk
	nokotarou_talk_2[20] = 11;

	evt_nokorin[10] = GSW(1701); //Koopie koo evt???
	evt_nokorin[11] = 11;

	hei_peach_mail[277] = GSW(1701); //Peach mail
	hei_peach_mail[278] = 99; //Unknown

	hei_01_init_evt[81] = GSWF(2001); //Room 1 Init
	hei_01_init_evt[82] = 1;
	hei_01_init_evt[117] = GSW(1701);
	hei_01_init_evt[118] = 99; //Unknown
	hei_01_init_evt[137] = GSWF(2001);
	hei_01_init_evt[138] = 1;

	door_open_03[89] = GSW(1701); //Door open 3
	door_open_03[90] = 6;

	hei_03_init_evt[33] = GSW(1701); //Room 3 Init
	hei_03_init_evt[34] = 6;

	door_open_05[1] = GSW(1701); //Door open 5
	door_open_05[2] = 7;
	door_open_05[90] = GSW(1701);
	door_open_05[91] = 7;


	hei_05_init_evt[27] = GSW(1701); //Room 5 Init
	hei_05_init_evt[28] = 7;

	a_sekizou_1[579] = GSW(1701); //Trivia
	a_sekizou_1[580] = 8;
	a_sekizou_1[991] = GSW(1701);
	a_sekizou_1[992] = 8;

	hei_07_init_evt[56] = GSW(1701); //Room 7 Init
	hei_07_init_evt[57] = 8;

	evt_golden[267] = GSW(1701); //Golden fella
	evt_golden[268] = 9;

	hei_10_init_evt[41] = GSW(1701); //Room 10 Init
	hei_10_init_evt[42] = 9;
}