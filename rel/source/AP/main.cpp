#include <AP/main.h>
#include "patch.h"

using namespace mod;

extern "C" {
	void bMapGXArrInject();
	void bMapGXArrInjectReturn();
	void bMapGXArrIncrement();
	void bMapGXArrIncrementReturn();
	void bMapGXChSplit();
	void bMapGXChSplitReturn();
	void bWinLogArrInject();
	void bWinLogArrInjectReturn();
	void bWinLogArrIncrement();
	void bWinLogArrIncrementReturn();
	void bChapterClearCheck();
	void bChapterClearCheckReturn();
	void bJohoyaSeqAddition();
}

//Assembly References
extern int32_t statusWinDisp[];
extern int32_t winRootMain[];
extern int32_t compare_func3_r[];
extern int32_t compare_func3[];
extern int32_t mapGX[];
extern int32_t winGetMapTplName[];
extern int32_t winLogMain[];
extern int32_t winLogInit[];
extern int32_t monoshiriGX[];
extern int32_t monosiri_disp[];
extern int32_t winMain[];
extern int32_t irai_init_func[];
extern int32_t gra_evt_kagemario_init[];
extern int32_t jin_evt_kagemario_init[];
extern int32_t usu_evt_kagemario_init[];
extern int32_t rsh_prolog[];
extern int32_t check_kou_evt[];
extern int32_t mario_chk[];
extern int32_t BattleInformationSetDropMaterial[];
extern int32_t _rule_disp[];
extern int32_t tou_gamen_screen_tev_init[];
extern int32_t bom1000_jump[];
extern int32_t jump_minnnanokoe[];
extern int32_t setupDataLoad[];
extern int32_t badgeShop_bargainGeneration[];
extern int32_t johoya_data_make[];
extern int32_t keijiban_data_make[];
extern int32_t evt_shop_main_func[];
extern int32_t uranaisi_data_make_next[];
extern int32_t uranaisi_data_make_starpiece[];
extern int32_t uranaisi_data_make_supercoin[];
extern int32_t sys_prolog[];
//End of Assekmbly References

//Script References
extern int32_t mail_evt_nok_01[];
extern int32_t mail_evt_gor_03[];
extern int32_t mail_evt_gor_02[];
extern int32_t mail_evt_gor_01[];
extern int32_t mail_evt_gor_04[];
extern int32_t mail_evt_rsh_03_a[];
extern int32_t mail_evt_hom_00[];
extern int32_t mail_evt_gor_02_2[];
extern int32_t mail_evt_bom_01[];
extern int32_t mail_evt_tik_00[];
extern int32_t mail_evt_win_01[];
extern int32_t mail_evt_tou_01[];
extern int32_t mail_evt_usu_01[];
extern int32_t mail_evt_gor_01_2[];
extern int32_t mail_evt_rsh_03_a_2[];
extern int32_t mail_evt_pik_00[];
extern int32_t mobj_save_blk_sysevt[];
extern int32_t init_event[];
extern int32_t evt_lecture_msg[];

void ApplyMainAssemblyPatches()
{
	statusWinDisp[425] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	statusWinDisp[487] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))

	winRootMain[40] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	winRootMain[82] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))

	compare_func3_r[29] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
	compare_func3_r[32] = 0x2C030005; // cmpwi r3, 0x5
	compare_func3_r[51] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
	compare_func3_r[54] = 0x2C030005; // cmpwi r3, 0x5

	compare_func3[29] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
	compare_func3[32] = 0x2C030005; // cmpwi r3, 0x5
	compare_func3[51] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
	compare_func3[54] = 0x2C030005; // cmpwi r3, 0x5

	mapGX[238] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	mapGX[240] = 0x2C03000F; // cmpwi r3, 0xF
	patch::writeBranchPair(&mapGX[91], bMapGXArrInject, bMapGXArrInjectReturn);
	patch::writeBranchPair(&mapGX[235], bMapGXArrIncrement, bMapGXArrIncrementReturn);
	patch::writeBranchPair(&mapGX[243], bMapGXChSplit, bMapGXChSplitReturn);

	winGetMapTplName[3] = 0x386006A5; // li r3, 0x6A5 (GSW(1701))
	winGetMapTplName[8] = 0x2C030001; // cmpwi r3, 0x1
	winGetMapTplName[12] = 0x386006A6; // li r3, 0x6A6 (GSW(1702))
	winGetMapTplName[14] = 0x2C030001; // cmpwi r3, 0x1
	winGetMapTplName[18] = 0x386006A7; // li r3, 0x6A7 (GSW(1703))
	winGetMapTplName[20] = 0x2C030001; // cmpwi r3, 0x1
	winGetMapTplName[24] = 0x386006A8; // li r3, 0x6A8 (GSW(1704))
	winGetMapTplName[26] = 0x2C030001; // cmpwi r3, 0x2
	winGetMapTplName[30] = 0x386006A9; // li r3, 0x6A9 (GSW(1705))
	winGetMapTplName[32] = 0x2C03000A; // cmpwi r3, 0xA
	winGetMapTplName[36] = 0x386006AA; // li r3, 0x6AA (GSW(1706))
	winGetMapTplName[38] = 0x2C030002; // cmpwi r3, 0x2
	winGetMapTplName[42] = 0x386006AB; // li r3, 0x6AB (GSW(1707))
	winGetMapTplName[44] = 0x2C030001; // cmpwi r3, 0x1

	winLogMain[196] = 0x386006A5; // li r3, 0x6A5 (GSW(1701))
	winLogMain[198] = 0x2C030001; // cmpwi r3, 0x1
	winLogMain[202] = 0x386006A6; // li r3, 0x6A6 (GSW(1702))
	winLogMain[204] = 0x2C030001; // cmpwi r3, 0x1
	winLogMain[208] = 0x386006A7; // li r3, 0x6A7 (GSW(1703))
	winLogMain[210] = 0x2C030001; // cmpwi r3, 0x1
	winLogMain[214] = 0x386006A8; // li r3, 0x6A8 (GSW(1704))
	winLogMain[216] = 0x2C030001; // cmpwi r3, 0x1
	winLogMain[220] = 0x386006A9; // li r3, 0x6A9 (GSW(1705))
	winLogMain[222] = 0x2C03000A; // cmpwi r3, 0xA
	winLogMain[226] = 0x386006AA; // li r3, 0x6AA (GSW(1706))
	winLogMain[228] = 0x2C030002; // cmpwi r3, 0x2
	winLogMain[232] = 0x386006AB; // li r3, 0x6AB (GSW(1707))
	winLogMain[234] = 0x2C030001; // cmpwi r3, 0x1
	winLogMain[259] = 0x386006A5; // li r3, 0x6A5 (GSW(1701))
	winLogMain[261] = 0x2C030001; // cmpwi r3, 0x1
	winLogMain[265] = 0x386006A6; // li r3, 0x6A6 (GSW(1702))
	winLogMain[267] = 0x2C030001; // cmpwi r3, 0x1
	winLogMain[271] = 0x386006A7; // li r3, 0x6A7 (GSW(1703))
	winLogMain[273] = 0x2C030001; // cmpwi r3, 0x1
	winLogMain[277] = 0x386006A8; // li r3, 0x6A8 (GSW(1704))
	winLogMain[279] = 0x2C030001; // cmpwi r3, 0x1
	winLogMain[283] = 0x386006A9; // li r3, 0x6A9 (GSW(1705))
	winLogMain[285] = 0x2C03000A; // cmpwi r3, 0xA
	winLogMain[289] = 0x386006AA; // li r3, 0x6AA (GSW(1706))
	winLogMain[291] = 0x2C030002; // cmpwi r3, 0x2
	winLogMain[295] = 0x386006AB; // li r3, 0x6AB (GSW(1707))
	winLogMain[297] = 0x2C030001; // cmpwi r3, 0x1

	patch::writeBranchPair(&winLogMain[432], bWinLogArrInject, bWinLogArrInjectReturn);
	patch::writeBranchPair(&winLogMain[235], bWinLogArrIncrement, bWinLogArrIncrementReturn);

	winLogInit[53] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	winLogInit[55] = 0x2C030000; // cmpwi r3, 0x0
	winLogInit[102] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	winLogInit[104] = 0x2C030000; // cmpwi r3, 0x0
	winLogInit[151] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	winLogInit[153] = 0x2C030000; // cmpwi r3, 0x0
	winLogInit[356] = 0x386006B8; // li r3, 0x6B8 (GSW(1720))
	winLogInit[358] = 0x2C030001; // cmpwi r3, 0x1
	winLogInit[362] = 0x386006AA; // li r3, 0x6AA (GSW(1706))
	winLogInit[364] = 0x2C030026; // cmpwi r3, 0x26
	winLogInit[669] = 0x386006AC; // li r3, 0x6AC (GSW(1708))
	winLogInit[672] = 0x2C030009; // cmpwi r3, 0x9
	winLogInit[686] = 0x386006AC; // li r3, 0x6AC (GSW(1708))
	winLogInit[689] = 0x2C030010; // cmpwi r3, 0x10

	monoshiriGX[230] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
	monoshiriGX[233] = 0x2C030005; // cmpwi r3, 0x5

	monosiri_disp[375] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
	monosiri_disp[378] = 0x2C030005; // cmpwi r3, 0x5

	winMain[184] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	winMain[186] = 0x2C030000; // cmpwi r3, 0x0
	winMain[205] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	winMain[207] = 0x2C030000; // cmpwi r3, 0x0
	winMain[231] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	winMain[234] = 0x2C030000; // cmpwi r3, 0x0

	irai_init_func[33] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	irai_init_func[43] = 0x2C030000; // cmpwi r3, 0x0
	irai_init_func[56] = 0x2C030000; // cmpwi r3, 0x0
	irai_init_func[64] = 0x2C030000; // cmpwi r3, 0x0
	irai_init_func[72] = 0x2C030000; // cmpwi r3, 0x0
	irai_init_func[80] = 0x2C030000; // cmpwi r3, 0x0
	irai_init_func[86] = 0x2C030000; // cmpwi r3, 0x0
	irai_init_func[90] = 0x2C030000; // cmpwi r3, 0x0
	irai_init_func[98] = 0x2C030000; // cmpwi r3, 0x0
	irai_init_func[106] = 0x2C030000; // cmpwi r3, 0x0

	gra_evt_kagemario_init[2] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
	gra_evt_kagemario_init[6] = 0x2C030003; // cmpwi r3, 0x3
	gra_evt_kagemario_init[8] = 0x2C030007; // cmpwi r3, 0x7

	jin_evt_kagemario_init[2] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
	jin_evt_kagemario_init[6] = 0x2C030003; // cmpwi r3, 0x3
	jin_evt_kagemario_init[8] = 0x2C030007; // cmpwi r3, 0x7

	usu_evt_kagemario_init[2] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
	usu_evt_kagemario_init[6] = 0x2C030003; // cmpwi r3, 0x3
	usu_evt_kagemario_init[8] = 0x2C030007; // cmpwi r3, 0x7

	rsh_prolog[14] = 0x386006A4; // li r3, 0x6AA (GSW(1706))
	rsh_prolog[16] = 0x2C03002B; // cmpwi r3, 0x2B

	check_kou_evt[15] = 0x386006A7; // li r3, 0x6A7 (GSW(1703))
	check_kou_evt[17] = 0x2C030016; // cmpwi r3, 0x16
	check_kou_evt[27] = 0x386006A7; // li r3, 0x6A7 (GSW(1703))
	check_kou_evt[30] = 0x2C030016; // cmpwi r3, 0x18

	mario_chk[172] = 0x386006AB; // li r3, 0x6AB (GSW(1707))
	mario_chk[175] = 0x2C03000C; // cmpwi r3, 0xC

	BattleInformationSetDropMaterial[87] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	BattleInformationSetDropMaterial[89] = 0x2C030000; // cmpwi r3, 0x0

	_rule_disp[9] = 0x386006A7; // li r3, 0x6A7 (GSW(1703))
	_rule_disp[14] = 0x2C03001C; // cmpwi r3, 0x1C

	tou_gamen_screen_tev_init[25] = 0x386006A7; // li r3, 0x6A7 (GSW(1703))
	tou_gamen_screen_tev_init[48] = 0x2C030013; // cmpwi r3, 0x13

	bom1000_jump[62] = 0x386006AC; // li r3, 0x6AC (GSW(1708))
	bom1000_jump[90] = 0x2C030011; // cmpwi r3, 0x11

	jump_minnnanokoe[48] = 0x386006AC; // li r3, 0x6AC (GSW(1708))
	jump_minnnanokoe[76] = 0x2C030011; // cmpwi r3, 0x11

	setupDataLoad[22] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	setupDataLoad[25] = 0x2C030002; // cmpwi r3, 0x2

	patch::writeBranchBL(&badgeShop_bargainGeneration[7], bChapterClearCheck);
	badgeShop_bargainGeneration[8] = 0x2C030001; // cmpwi r3, 0x1
	patch::writeBranchBL(&badgeShop_bargainGeneration[34], bChapterClearCheck);
	badgeShop_bargainGeneration[35] = 0x2C030002; // cmpwi r3, 0x2
	patch::writeBranchBL(&badgeShop_bargainGeneration[61], bChapterClearCheck);
	badgeShop_bargainGeneration[62] = 0x2C030003; // cmpwi r3, 0x3
	patch::writeBranchBL(&badgeShop_bargainGeneration[88], bChapterClearCheck);
	badgeShop_bargainGeneration[89] = 0x2C030004; // cmpwi r3, 0x4
	patch::writeBranchBL(&badgeShop_bargainGeneration[115], bChapterClearCheck);
	badgeShop_bargainGeneration[116] = 0x2C030005; // cmpwi r3, 0x5
	patch::writeBranchBL(&badgeShop_bargainGeneration[142], bChapterClearCheck);
	badgeShop_bargainGeneration[143] = 0x2C030006; // cmpwi r3, 0x6
	patch::writeBranchBL(&badgeShop_bargainGeneration[169], bChapterClearCheck);
	badgeShop_bargainGeneration[170] = 0x2C030007; // cmpwi r3, 0x7
	badgeShop_bargainGeneration[184] = 0x386006AC; // li r3, 0x6AC (GSW(1708))
	badgeShop_bargainGeneration[187] = 0x2C030011; // cmpwi r3, 0x11

	patch::writeBranchBL(&johoya_data_make[11], bJohoyaSeqAddition);

	patch::writeBranchBL(&keijiban_data_make[11], bJohoyaSeqAddition);

	evt_shop_main_func[40] = 0x386006A4; // li r3, 0x6AA (GSW(1706))
	evt_shop_main_func[43] = 0x2C030020; // cmpwi r3, 0x20
	evt_shop_main_func[46] = 0x386006A4; // li r3, 0x6AA (GSW(1706))
	evt_shop_main_func[49] = 0x2C030024; // cmpwi r3, 0x24

	patch::writeBranchBL(&uranaisi_data_make_next[11], bJohoyaSeqAddition);

	patch::writeBranchBL(&uranaisi_data_make_starpiece[11], bJohoyaSeqAddition);

	patch::writeBranchBL(&uranaisi_data_make_supercoin[11], bJohoyaSeqAddition);

	sys_prolog[25] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	sys_prolog[28] = 0x2C030002; // cmpwi r3, 0x2
	sys_prolog[25] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	sys_prolog[28] = 0x2C030002; // cmpwi r3, 0x2
	sys_prolog[25] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	sys_prolog[28] = 0x2C030002; // cmpwi r3, 0x2
	sys_prolog[25] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	sys_prolog[28] = 0x2C030002; // cmpwi r3, 0x2
	// FINISH ^
}

void ApplyMainScriptPatches()
{

}