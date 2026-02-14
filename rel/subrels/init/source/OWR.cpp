#include "evt_cmd.h"
#include "OWR.h"
#include "patch.h"
#include "ttyd/evt_bero.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_memcard.h"
#include "ttyd/evt_party.h"
#include "ttyd/evtmgr_cmd.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/msgdrv.h"
#include "ttyd/pmario_sound.h"
#include "ttyd/seq_logo.h"
#include "ttyd/seq_mapchange.h"
#include "ttyd/seq_game.h"
#include "ttyd/seqdrv.h"
#include "ttyd/statuswindow.h"
#include "ttyd/swdrv.h"
#include <AP/rel_patch_definitions.h>
#include <string.h>
#include <ttyd/common_types.h>
#include <ttyd/icondrv.h>
#include <ttyd/item_data.h>
#include <ttyd/system.h>
#include <ttyd/win_item.h>
#include <ttyd/win_log.h>

#include <cstdint>

// Assembly References
extern int32_t statusWinDisp[];
extern int32_t main_winRootMain[];
extern int32_t main_compare_func3_r[];
extern int32_t main_compare_func3[];
extern int32_t main_mapGX[];
extern int32_t main_winGetMapTplName[];
extern int32_t main_winLogMain[];
extern int32_t main_winLogInit[];
extern int32_t main_monoshiriGX[];
extern int32_t main_monosiri_disp[];
extern int32_t main_winMain[];
extern int32_t main_irai_init_func[];
extern int32_t main_rsh_prolog[];
extern int32_t main_BattleInformationSetDropMaterial[];
extern int32_t main_rule_disp[];
extern int32_t main_tou_gamen_screen_tev_init[];
extern int32_t main_bom1000_jump[];
extern int32_t main_jump_minnnanokoe[];
extern int32_t main_setupDataLoad[];
extern int32_t main_badgeShop_bargainGeneration[];
extern int32_t main_johoya_data_make[];
extern int32_t main_keijiban_data_make[];
extern int32_t main_evt_shop_main_func[];
extern int32_t main_uranaisi_data_make_next[];
extern int32_t main_uranaisi_data_make_starpiece[];
extern int32_t main_uranaisi_data_make_supercoin[];
extern int32_t main_winMgrSelectEntry[];
extern int32_t main_evt_badgeShop_starmaniac_get_kind_cnt[];
extern int32_t main_badgeShop_get[];
extern int32_t main_badgeShop_add[];
extern int32_t main_evt_mobj_kururing_floor[];
extern int32_t main_mobj_kururing_floor[];
extern int32_t main_mobj_powerupblk[];
extern int32_t evt_mobj_powerupblk[];
extern int32_t main_breakfast[];
extern int32_t main_evt_shop_setup[];
extern int32_t loadDraw[];
extern int32_t main_battle_exit[];
extern int32_t main_seq_battleInit[];
extern int32_t main__psndSFXOn[];
extern int32_t main_psndBGMOff_f_d[];
extern int32_t main_psndBGMOn_f_d[];
extern int32_t main_battleCheckUnitMonosiriFlag[];
extern int32_t main_BattleDrawEnemyHPBar[];
extern int32_t btlseqEnd[];
extern int32_t evt_mobj_brick[];
extern int32_t help_disp[];
extern int32_t _mapLoad[];
extern int32_t itemMain[];
extern int32_t stone_ry[];
extern int32_t stone_bg[];
// End of Assembly References

// Script References
extern int32_t main_mail_evt_nok_01[];
extern int32_t main_mail_evt_gor_03[];
extern int32_t main_mail_evt_gor_02[];
extern int32_t main_mail_evt_gor_01[];
extern int32_t main_mail_evt_gor_04[];
extern int32_t main_mail_evt_rsh_03_a[];
extern int32_t main_mail_evt_hom_00[];
extern int32_t main_mail_evt_gor_02_2[];
extern int32_t main_mail_evt_bom_01[];
extern int32_t main_mail_evt_tik_00[];
extern int32_t main_mail_evt_win_01[];
extern int32_t main_mail_evt_tou_01[];
extern int32_t main_mail_evt_usu_01[];
extern int32_t main_mail_evt_gor_01_2[];
extern int32_t main_mail_evt_rsh_03_a_2[];
extern int32_t main_mail_evt_pik_00[];
extern int32_t main_buy_evt[];
extern int32_t main_buy_evt_evt[];
extern int32_t evt_sub_starstone[];
extern int32_t main_evt_sub_starstone_evt[];
extern int32_t main_init_param_80137004[];
extern int32_t main_battleSetUnitMonosiriFlag[];
extern int32_t main_partyChristineAttack_Monosiri[];
extern int32_t main_partyChristineAttack_Monosiri_evt[];
extern int32_t starstone_end_evt[];
extern int32_t bero_las_deny[];
extern int32_t starstone_cam_z[];
extern int32_t starstone_eff_z[];
extern int32_t starstone_interpolate_angle[];
extern int32_t starstone_item_handler[];
extern int32_t starstone_item_z[];

extern int32_t main_mobj_save_blk_sysevt[];
extern int32_t main_init_event[];
extern int32_t evt_lecture_msg[];
extern int32_t evt_msg_print_party[];
extern int32_t evt_msg_print_party_add[];
extern int32_t main_preventDiaryTextboxSelectionAddress[];

extern char starstone_current_map[32];
extern uint32_t main_next;
extern ttyd::pmario_sound::BGMListEntry main_psbgmlist[262];

using ttyd::seq_mapchange::_next_area;
using ttyd::seq_mapchange::_next_map;
using ttyd::system::irand;
using namespace ttyd;
using namespace mod::patch;
using namespace mod::owr;

uint32_t invalidParams[] = {0x802CC81C, 0x802CC8BC, 0x802CD500, 0x802CD668, 0x802CD694, 0x802CD77C, 0x802CD7B0, 0x802CD7E4,
                            0x802CD810, 0x802CD944, 0x802CD9D4, 0x802CDA00, 0x802CDAEC, 0x802CDB48, 0x802CDB78, 0x802CDBA8,
                            0x802CDC78, 0x802CDCA8, 0x802CDD6C, 0x802CDE0C, 0x802CE1CC, 0x802CE1FC, 0x802CE22C, 0x802CE25C,
                            0x802CE28C, 0x802CE2C0, 0x802CE2F0, 0x802CE33C, 0x802CE370, 0x802CE3A4, 0x802CE3D8, 0x802CE408,
                            0x802CE438, 0x802CE47C, 0x802CE4AC, 0x802CE4D8, 0x802CE504, 0x802CE530, 0x802CE598, 0x802CE5D0,
                            0x802CE600, 0x802CE62C, 0x802CE660, 0x802CE690, 0x802CE6C0, 0x802CE6F4, 0x802CE724, 0x802CE7E4, 0x804218C8};

// clang-format off
EVT_BEGIN(main_buy_evt_hook)
    RUN_CHILD_EVT(main_buy_evt_evt)
    RETURN()
EVT_END()

EVT_BEGIN(main_evt_sub_starstone_hook)
    RUN_CHILD_EVT(main_evt_sub_starstone_evt)
    IF_EQUAL(LW(1), 0)
        GOTO(&evt_sub_starstone[831])
    END_IF()
    RETURN()
EVT_PATCH_END()

EVT_BEGIN(main_partyChristineAttack_Monosiri_hook)
    RUN_CHILD_EVT(main_partyChristineAttack_Monosiri_evt)
    GOTO(&main_partyChristineAttack_Monosiri[392])
EVT_PATCH_END()

EVT_BEGIN(main_evt_sub_starstone_end_hook)
    RUN_CHILD_EVT(starstone_end_evt)
    IF_EQUAL(GSWF(6119), 1)
        SET(GSWF(6119), 0)
        RETURN()
    END_IF()
EVT_PATCH_END()

EVT_BEGIN(starstone_cam_z_hook)
    RUN_CHILD_EVT(starstone_cam_z)
EVT_PATCH_END()

EVT_BEGIN(starstone_eff_z_hook)
    RUN_CHILD_EVT(starstone_eff_z)
EVT_PATCH_END()

EVT_BEGIN(starstone_interpolate_hook)
    RUN_CHILD_EVT(starstone_interpolate_angle)
EVT_PATCH_END()

EVT_BEGIN(starstone_item_handler_hook)
    RUN_CHILD_EVT(starstone_item_handler)
    GOTO(LW(2))
EVT_PATCH_END()

EVT_BEGIN(starstone_item_z_hook)
    RUN_CHILD_EVT(starstone_item_z)
EVT_PATCH_END()

EVT_BEGIN(bero_las_deny_hook)
    RUN_CHILD_EVT(bero_las_deny)
    IF_EQUAL(LW(7), 1)
        RETURN()
    END_IF()
EVT_PATCH_END()
// clang-format on

namespace mod::owr
{
    void ApplyMainAssemblyPatches()
    {
        writeIntWithCache(&loadDraw[777], 0x480000C4); // li r3, 0x6A4 (GSW(1700))

        writeIntWithCache(&statusWinDisp[425], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
        writeIntWithCache(&statusWinDisp[487], 0x386006A4); // li r3, 0x6A4 (GSW(1700))

        writeIntWithCache(&main_winRootMain[40], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
        writeIntWithCache(&main_winRootMain[82], 0x386006A4); // li r3, 0x6A4 (GSW(1700))

        writeIntWithCache(&main_compare_func3_r[30], 0x38840833); // addi r4, r4, 0x833 GSW(1715)
        writeIntWithCache(&main_compare_func3_r[32], 0x2C030005); // cmpwi r3, 0x5
        writeIntWithCache(&main_compare_func3_r[52], 0x38840833); // addi r4, r4, 0x833 GSW(1715)
        writeIntWithCache(&main_compare_func3_r[54], 0x2C030005); // cmpwi r3, 0x5

        writeIntWithCache(&main_compare_func3[30], 0x38840833); // addi r4, r4, 0x833 GSW(1715)
        writeIntWithCache(&main_compare_func3[32], 0x2C030005); // cmpwi r3, 0x5
        writeIntWithCache(&main_compare_func3[52], 0x38840833); // addi r4, r4, 0x833 GSW(1715)
        writeIntWithCache(&main_compare_func3[54], 0x2C030005); // cmpwi r3, 0x5

        writeIntWithCache(&main_mapGX[238], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
        writeIntWithCache(&main_mapGX[240], 0x2C030010); // cmpwi r3, 0xF

        // setup ptr to win_log_mapGX_arr before loop
        patch::writeBranchPair(&main_mapGX[90],
                               reinterpret_cast<void *>(bMapGXArrInject),
                               reinterpret_cast<void *>(bMapGXArrInjectReturn));

        // use ptr to win_log_mapGX_arr for sequence checks inside loop
        patch::writeBranchPair(&main_mapGX[91],
                               &main_mapGX[94],
                               reinterpret_cast<void *>(bMapGXArrFlagCheck),
                               reinterpret_cast<void *>(bMapGXArrFlagCheckReturn));

        writeIntWithCache(&main_mapGX[94], 0x2C030001); // cmpwi r3, 0x1

        // increment ptr to win_log_mapGX_arr at end of loop
        patch::writeBranchPair(&main_mapGX[235],
                               reinterpret_cast<void *>(bMapGXArrIncrement),
                               reinterpret_cast<void *>(bMapGXArrIncrementReturn));

        writeIntWithCache(&main_mapGX[239], 0x60000000); // NOP

        writeIntWithCache(&main_winGetMapTplName[3], 0x386006A5);  // li r3, 0x6A5 (GSW(1701))
        writeIntWithCache(&main_winGetMapTplName[8], 0x2C030001);  // cmpwi r3, 0x1
        writeIntWithCache(&main_winGetMapTplName[12], 0x386006A6); // li r3, 0x6A6 (GSW(1702))
        writeIntWithCache(&main_winGetMapTplName[14], 0x2C030001); // cmpwi r3, 0x1
        writeIntWithCache(&main_winGetMapTplName[18], 0x386006A7); // li r3, 0x6A7 (GSW(1703))
        writeIntWithCache(&main_winGetMapTplName[20], 0x2C030001); // cmpwi r3, 0x1
        writeIntWithCache(&main_winGetMapTplName[24], 0x386006A8); // li r3, 0x6A8 (GSW(1704))
        writeIntWithCache(&main_winGetMapTplName[26], 0x2C030001); // cmpwi r3, 0x2
        writeIntWithCache(&main_winGetMapTplName[30], 0x386006A9); // li r3, 0x6A9 (GSW(1705))
        writeIntWithCache(&main_winGetMapTplName[32], 0x2C03000A); // cmpwi r3, 0xA
        writeIntWithCache(&main_winGetMapTplName[36], 0x386006AA); // li r3, 0x6AA (GSW(1706))
        writeIntWithCache(&main_winGetMapTplName[38], 0x2C030002); // cmpwi r3, 0x2
        writeIntWithCache(&main_winGetMapTplName[42], 0x386006AB); // li r3, 0x6AB (GSW(1707))
        writeIntWithCache(&main_winGetMapTplName[44], 0x2C030001); // cmpwi r3, 0x1

        writeIntWithCache(&main_winLogMain[196], 0x386006A5); // li r3, 0x6A5 (GSW(1701))
        writeIntWithCache(&main_winLogMain[198], 0x2C030001); // cmpwi r3, 0x1
        writeIntWithCache(&main_winLogMain[202], 0x386006A6); // li r3, 0x6A6 (GSW(1702))
        writeIntWithCache(&main_winLogMain[204], 0x2C030001); // cmpwi r3, 0x1
        writeIntWithCache(&main_winLogMain[208], 0x386006A7); // li r3, 0x6A7 (GSW(1703))
        writeIntWithCache(&main_winLogMain[210], 0x2C030001); // cmpwi r3, 0x1
        writeIntWithCache(&main_winLogMain[214], 0x386006A8); // li r3, 0x6A8 (GSW(1704))
        writeIntWithCache(&main_winLogMain[216], 0x2C030001); // cmpwi r3, 0x1
        writeIntWithCache(&main_winLogMain[220], 0x386006A9); // li r3, 0x6A9 (GSW(1705))
        writeIntWithCache(&main_winLogMain[222], 0x2C03000A); // cmpwi r3, 0xA
        writeIntWithCache(&main_winLogMain[226], 0x386006AA); // li r3, 0x6AA (GSW(1706))
        writeIntWithCache(&main_winLogMain[228], 0x2C030002); // cmpwi r3, 0x2
        writeIntWithCache(&main_winLogMain[232], 0x386006AB); // li r3, 0x6AB (GSW(1707))
        writeIntWithCache(&main_winLogMain[234], 0x2C030001); // cmpwi r3, 0x1
        writeIntWithCache(&main_winLogMain[259], 0x386006A5); // li r3, 0x6A5 (GSW(1701))
        writeIntWithCache(&main_winLogMain[261], 0x2C030001); // cmpwi r3, 0x1
        writeIntWithCache(&main_winLogMain[265], 0x386006A6); // li r3, 0x6A6 (GSW(1702))
        writeIntWithCache(&main_winLogMain[267], 0x2C030001); // cmpwi r3, 0x1
        writeIntWithCache(&main_winLogMain[271], 0x386006A7); // li r3, 0x6A7 (GSW(1703))
        writeIntWithCache(&main_winLogMain[273], 0x2C030001); // cmpwi r3, 0x1
        writeIntWithCache(&main_winLogMain[277], 0x386006A8); // li r3, 0x6A8 (GSW(1704))
        writeIntWithCache(&main_winLogMain[279], 0x2C030001); // cmpwi r3, 0x1
        writeIntWithCache(&main_winLogMain[283], 0x386006A9); // li r3, 0x6A9 (GSW(1705))
        writeIntWithCache(&main_winLogMain[285], 0x2C03000A); // cmpwi r3, 0xA
        writeIntWithCache(&main_winLogMain[289], 0x386006AA); // li r3, 0x6AA (GSW(1706))
        writeIntWithCache(&main_winLogMain[291], 0x2C030002); // cmpwi r3, 0x2
        writeIntWithCache(&main_winLogMain[295], 0x386006AB); // li r3, 0x6AB (GSW(1707))
        writeIntWithCache(&main_winLogMain[297], 0x2C030001); // cmpwi r3, 0x1

        // setup ptr to win_log_mapGX_arr before loop
        patch::writeBranchPair(&main_winLogMain[431],
                               reinterpret_cast<void *>(bWinLogArrInject),
                               reinterpret_cast<void *>(bWinLogArrInjectReturn));

        // use ptr to win_log_mapGX_arr for sequence checks inside loop
        patch::writeBranchPair(&main_winLogMain[432],
                               &main_winLogMain[435],
                               reinterpret_cast<void *>(bWinLogArrFlagCheck),
                               reinterpret_cast<void *>(bWinLogArrFlagCheckReturn));

        writeIntWithCache(&main_winLogMain[435], 0x2C030001); // cmpwi r3, 0x1

        // increment ptr to win_log_mapGX_arr at end of loop
        patch::writeBranchPair(&main_winLogMain[505],
                               reinterpret_cast<void *>(bWinLogArrIncrement),
                               reinterpret_cast<void *>(bWinLogArrIncrementReturn));

        writeIntWithCache(&main_winLogInit[53], 0x386006A4);  // li r3, 0x6A4 (GSW(1700))
        writeIntWithCache(&main_winLogInit[55], 0x2C030000);  // cmpwi r3, 0x0
        writeIntWithCache(&main_winLogInit[102], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
        writeIntWithCache(&main_winLogInit[104], 0x2C030000); // cmpwi r3, 0x0
        writeIntWithCache(&main_winLogInit[151], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
        writeIntWithCache(&main_winLogInit[153], 0x2C030000); // cmpwi r3, 0x0
        writeIntWithCache(&main_winLogInit[356], 0x386006B8); // li r3, 0x6B8 (GSW(1720))
        writeIntWithCache(&main_winLogInit[358], 0x2C030001); // cmpwi r3, 0x1
        writeIntWithCache(&main_winLogInit[362], 0x386006AA); // li r3, 0x6AA (GSW(1706))
        writeIntWithCache(&main_winLogInit[364], 0x2C030026); // cmpwi r3, 0x26
        writeIntWithCache(&main_winLogInit[670], 0x3884082C); // addi r4, r4, 0x82C GSW(1708)
        writeIntWithCache(&main_winLogInit[672], 0x2C030009); // cmpwi r3, 0x9
        writeIntWithCache(&main_winLogInit[687], 0x3884082C); // addi r4, r4, 0x82C GSW(1708)
        writeIntWithCache(&main_winLogInit[689], 0x2C030010); // cmpwi r3, 0x10

        writeIntWithCache(&main_monoshiriGX[231], 0x38840833); // addi r4, r4, 0x833 GSW(1715)
        writeIntWithCache(&main_monoshiriGX[233], 0x2C030005); // cmpwi r3, 0x5

        writeIntWithCache(&main_monosiri_disp[376], 0x38840833); // addi r4, r4, 0x833 GSW(1715)
        writeIntWithCache(&main_monosiri_disp[378], 0x2C030005); // cmpwi r3, 0x5

        writeIntWithCache(&main_winMain[184], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
        writeIntWithCache(&main_winMain[186], 0x2C030000); // cmpwi r3, 0x0
        writeIntWithCache(&main_winMain[205], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
        writeIntWithCache(&main_winMain[207], 0x2C030000); // cmpwi r3, 0x0
        writeIntWithCache(&main_winMain[232], 0x38840824); // addi r4, r4, 0x824 GSW(1700)
        writeIntWithCache(&main_winMain[234], 0x2C030000); // cmpwi r3, 0x0

        writeIntWithCache(&main_BattleInformationSetDropMaterial[84], 0x38830824); // addi r4, r3, 0x824 GSW(1700)
        writeIntWithCache(&main_BattleInformationSetDropMaterial[89], 0x2C030000); // cmpwi r3, 0x0

        writeIntWithCache(&main_rule_disp[12], 0x38840827); // addi r4, r4, 0x827 GSW(1703)
        writeIntWithCache(&main_rule_disp[14], 0x2C03001C); // cmpwi r3, 0x1C

        writeIntWithCache(&main_tou_gamen_screen_tev_init[27], 0x38840827); // addi r4, r4, 0x827 GSW(1703)
        writeIntWithCache(&main_tou_gamen_screen_tev_init[48], 0x2C030013); // cmpwi r3, 0x13

        writeIntWithCache(&main_setupDataLoad[23], 0x38840824); // addi r4, r4, 0x824 GSW(1700)

        patch::writeBranchBL(&main_badgeShop_bargainGeneration[7], reinterpret_cast<void *>(bChapterClearCheck));
        writeIntWithCache(&main_badgeShop_bargainGeneration[8], 0x2C030001); // cmpwi r3, 0x1
        patch::writeBranchBL(&main_badgeShop_bargainGeneration[34], reinterpret_cast<void *>(bChapterClearCheck));
        writeIntWithCache(&main_badgeShop_bargainGeneration[35], 0x2C030002); // cmpwi r3, 0x2
        patch::writeBranchBL(&main_badgeShop_bargainGeneration[61], reinterpret_cast<void *>(bChapterClearCheck));
        writeIntWithCache(&main_badgeShop_bargainGeneration[62], 0x2C030003); // cmpwi r3, 0x3
        patch::writeBranchBL(&main_badgeShop_bargainGeneration[88], reinterpret_cast<void *>(bChapterClearCheck));
        writeIntWithCache(&main_badgeShop_bargainGeneration[89], 0x2C030004); // cmpwi r3, 0x4
        patch::writeBranchBL(&main_badgeShop_bargainGeneration[115], reinterpret_cast<void *>(bChapterClearCheck));
        writeIntWithCache(&main_badgeShop_bargainGeneration[116], 0x2C030005); // cmpwi r3, 0x5
        patch::writeBranchBL(&main_badgeShop_bargainGeneration[142], reinterpret_cast<void *>(bChapterClearCheck));
        writeIntWithCache(&main_badgeShop_bargainGeneration[143], 0x2C030006); // cmpwi r3, 0x6
        patch::writeBranchBL(&main_badgeShop_bargainGeneration[169], reinterpret_cast<void *>(bChapterClearCheck));
        writeIntWithCache(&main_badgeShop_bargainGeneration[170], 0x2C030007); // cmpwi r3, 0x7
        writeIntWithCache(&main_badgeShop_bargainGeneration[184], 0x386006AC); // li r3, 0x6AC (GSW(1708))
        writeIntWithCache(&main_badgeShop_bargainGeneration[187], 0x2C030011); // cmpwi r3, 0x11

        patch::writeBranchBL(&main_johoya_data_make[11], reinterpret_cast<void *>(bJohoyaSeqAddition));

        writeIntWithCache(&main_evt_shop_main_func[41], 0x3884082A); // addi r4, r4, 0x82C GSW(1706)
        writeIntWithCache(&main_evt_shop_main_func[43], 0x2C030020); // cmpwi r3, 0x20
        writeIntWithCache(&main_evt_shop_main_func[47], 0x3884082A); // addi r4, r4, 0x82C GSW(1706)
        writeIntWithCache(&main_evt_shop_main_func[49], 0x2C030024); // cmpwi r3, 0x24

        patch::writeBranchBL(&main_uranaisi_data_make_next[11], reinterpret_cast<void *>(bJohoyaSeqAddition));

        patch::writeBranchBL(&main_uranaisi_data_make_starpiece[11], reinterpret_cast<void *>(bJohoyaSeqAddition));

        patch::writeBranchBL(&main_uranaisi_data_make_supercoin[11], reinterpret_cast<void *>(bJohoyaSeqAddition));

        patch::writeBranchPair(&evt_msg_print_party[30],
                               reinterpret_cast<void *>(bPrintPartyErrorFix),
                               reinterpret_cast<void *>(bPrintPartyErrorFixReturn));

        patch::writeBranchPair(&evt_msg_print_party_add[32],
                               reinterpret_cast<void *>(bPrintPartyAddErrorFix),
                               reinterpret_cast<void *>(bPrintPartyAddErrorFixReturn));

        patch::writeBranchPair(&main_evt_mobj_kururing_floor[39],
                               reinterpret_cast<void *>(bKururingFloorCapture),
                               reinterpret_cast<void *>(bKururingFloorCaptureReturn));

        patch::writeBranchPair(&main_evt_mobj_kururing_floor[60],
                               reinterpret_cast<void *>(bKururingFloorRelease),
                               reinterpret_cast<void *>(bKururingFloorReleaseReturn));

        patch::writeBranchPair(&evt_mobj_powerupblk[24],
                               reinterpret_cast<void *>(bPowerupblkCapture),
                               reinterpret_cast<void *>(bPowerupblkCaptureReturn));

        patch::writeBranchPair(&evt_mobj_powerupblk[52],
                               reinterpret_cast<void *>(bPowerupblkRelease),
                               reinterpret_cast<void *>(bPowerupblkReleaseReturn));

        patch::writeBranchPair(&main_mobj_powerupblk[79],
                               reinterpret_cast<void *>(bMobjStarstoneRotation),
                               reinterpret_cast<void *>(bMobjStarstoneRotationReturn));

        writeIntWithCache(&main_mobj_kururing_floor[188], 0x808301BB); // lwz r4, 0x1BA(r3)

        writeIntWithCache(&main_mobj_powerupblk[73], 0x809F01D8);  // lwz r4, 0x1D8(r31)
        writeIntWithCache(&main_mobj_powerupblk[110], 0x809F01D8); // lwz r4, 0x1D8(r31)

        patch::writeBranchPair(&main_breakfast[22],
                               reinterpret_cast<void *>(bPeachPointer),
                               reinterpret_cast<void *>(bPeachReturn));

        writeIntWithCache(&main_evt_shop_setup[82], 0x2C000000); // cmpwi r0, 0x0
        writeIntWithCache(&main_evt_shop_setup[83], 0x418200FC); // beq +0xFC

        patch::writeBranchPair(&main_evt_shop_setup[84],
                               reinterpret_cast<void *>(bShopFlagCheck),
                               reinterpret_cast<void *>(bShopFlagCheckReturn));

        writeIntWithCache(&main_evt_shop_setup[85], 0x480000F4); // b +0xF4

        writeIntWithCache(&main_battle_exit[76], 0x38840831); // addi r4, r4, 0x831 GSW(1713)
        writeIntWithCache(&main_battle_exit[78], 0x2C030008); // cmpwi r3, 0x8
        writeIntWithCache(&main_battle_exit[82], 0x38840831); // addi r4, r4, 0x831 GSW(1713)
        writeIntWithCache(&main_battle_exit[84], 0x2C03000A); // cmpwi r3, 0xA
        writeIntWithCache(&main_battle_exit[94], 0x3884082B); // addi r4, r4, 0x82B GSW(1707)
        writeIntWithCache(&main_battle_exit[96], 0x2C030012); // cmpwi r3, 0x12

        writeIntWithCache(&main_seq_battleInit[10], 0x38830831); // addi r4, r3, 0x831 GSW(1713)
        writeIntWithCache(&main_seq_battleInit[13], 0x2C030008); // cmpwi r3, 0x8
        writeIntWithCache(&main_seq_battleInit[17], 0x38840831); // addi r4, r4, 0x831 GSW(1713)
        writeIntWithCache(&main_seq_battleInit[19], 0x2C03000A); // cmpwi r3, 0xA
        writeIntWithCache(&main_seq_battleInit[23], 0x3884082B); // addi r4, r4, 0x82B GSW(1707)
        writeIntWithCache(&main_seq_battleInit[25], 0x2C030012); // cmpwi r3, 0x12

        writeIntWithCache(&main__psndSFXOn[71], 0x38840824); // addi r4, r4, 0x824 GSW(1700)

        writeIntWithCache(&main_psndBGMOff_f_d[28], 0x38840831); // addi r4, r4, 0x831 GSW(1713)
        writeIntWithCache(&main_psndBGMOff_f_d[30], 0x2C030008); // cmpwi r3, 0x8
        writeIntWithCache(&main_psndBGMOff_f_d[34], 0x38840831); // addi r4, r4, 0x831 GSW(1713)
        writeIntWithCache(&main_psndBGMOff_f_d[36], 0x2C03000A); // cmpwi r3, 0xA

        patch::writeBranchPair(&main_psndBGMOff_f_d[83],
                               reinterpret_cast<void *>(bStarstoneBgmKeep),
                               reinterpret_cast<void *>(bStarstoneBgmKeepReturn));
        
        patch::writeBranchPair(&main_psndBGMOff_f_d[121],
                               reinterpret_cast<void *>(bStarstoneBgmKeepFinal),
                               reinterpret_cast<void *>(bStarstoneBgmKeepFinalReturn));

        writeIntWithCache(&main_psndBGMOn_f_d[71], 0x38840835); // addi r4, r4, 0x835 GSW(1717)
        writeIntWithCache(&main_psndBGMOn_f_d[73], 0x2C03000E); // cmpwi r3, 0xE
        writeIntWithCache(&main_psndBGMOn_f_d[77], 0x38840835); // addi r4, r4, 0x835 GSW(1717)
        writeIntWithCache(&main_psndBGMOn_f_d[79], 0x2C030011); // cmpwi r3, 0x11
        writeIntWithCache(&main_psndBGMOn_f_d[88], 0x38840831); // addi r4, r4, 0x831 GSW(1713)
        writeIntWithCache(&main_psndBGMOn_f_d[90], 0x2C030008); // cmpwi r3, 0x8
        writeIntWithCache(&main_psndBGMOn_f_d[94], 0x38840831); // addi r4, r4, 0x831 GSW(1713)
        writeIntWithCache(&main_psndBGMOn_f_d[96], 0x2C03000A); // cmpwi r3, 0xA

        // Expand msgSearch to check 4 tables
        uint32_t* msgSearchPtr = (uint32_t*)ttyd::msgdrv::msgSearch;
        writeIntWithCache(&msgSearchPtr[44], 0x2C1B0004); // cmpwi r27, 0x4

        uint32_t *msgLoadPtr = (uint32_t *)ttyd::msgdrv::msgLoad;
        writeIntWithCache(&msgLoadPtr[85], 0x60000000); // NOP

        uint32_t *msgWindow_DispPtr = (uint32_t *)0x80080260;
        writeIntWithCache(&msgWindow_DispPtr[407], 0x80630040); // lwz r3, 0x40(r3)
        writeIntWithCache(&msgWindow_DispPtr[420], 0x80630040); // lwz r3, 0x40(r3)

        msgdrv::msgw = gState->state_msgWork;

        patch::writeBranchPair(&help_disp[21], 
                               reinterpret_cast<void *>(bShopDesc), 
                               reinterpret_cast<void *>(bShopDescReturn));

        if (mod::owr::gState->apSettings->peekaboo)
            writeIntWithCache(&main_battleCheckUnitMonosiriFlag[10], 0x60000000); // NOP

        if (gState->apSettings->runFill)
        {
            patch::writeBranchPair(&main_init_param_80137004[126],
                                   reinterpret_cast<void *>(bGreenZoneMaxInit),
                                   reinterpret_cast<void *>(bGreenZoneMaxInitReturn));
        }

        patch::writeBranchPair(&main_battleSetUnitMonosiriFlag[6],
                               reinterpret_cast<void *>(bMonosiriItemCheck),
                               reinterpret_cast<void *>(bMonosiriItemCheckReturn));

        patch::writeBranchPair(&main_BattleDrawEnemyHPBar[22],
                               reinterpret_cast<void *>(bHpColorBack),
                               reinterpret_cast<void *>(bHpColorBackReturn));

        patch::writeBranchPair(&main_BattleDrawEnemyHPBar[90],
                               reinterpret_cast<void *>(bHpColorFront),
                               reinterpret_cast<void *>(bHpColorFrontReturn));

        patch::writeBranchPair(&btlseqEnd[354],
                               reinterpret_cast<void *>(bExpMultiplier),
                               reinterpret_cast<void *>(bExpMultiplierReturn));
        
        patch::writeBranchPair(&evt_mobj_brick[30],
                               reinterpret_cast<void *>(bBlockVisibility),
                               reinterpret_cast<void *>(bBlockVisibilityReturn));

        patch::writeBranchPair(&itemMain[216],
                               reinterpret_cast<void *>(bItemStarstoneCheck),
                               reinterpret_cast<void *>(bItemStarstoneCheckReturn));

        patch::writeBranchPair(&stone_ry[23],
                               reinterpret_cast<void *>(bStoneRotationPointerCheck),
                               reinterpret_cast<void *>(bStoneRotationPointerCheckReturn));

        patch::writeBranchPair(&stone_ry[28],
                               reinterpret_cast<void *>(bStoneRotationPointerCheck2),
                               reinterpret_cast<void *>(bStoneRotationPointerCheckReturn2));

        patch::writeBranchPair(&stone_ry[33],
                               reinterpret_cast<void *>(bStoneRotationPointerCheck3),
                               reinterpret_cast<void *>(bStoneRotationPointerCheckReturn3));

        patch::writeBranchPair(&stone_ry[35],
                               reinterpret_cast<void *>(bStoneRotationPointerCheck4),
                               reinterpret_cast<void *>(bStoneRotationPointerCheckReturn4));

        patch::writeBranchPair(&stone_ry[41],
                               reinterpret_cast<void *>(bStoneRotationPointerCheck5),
                               reinterpret_cast<void *>(bStoneRotationPointerCheckReturn5));

        patch::writeBranchPair(&stone_bg[26],
                               reinterpret_cast<void *>(bStoneBgPointerCheck),
                               reinterpret_cast<void *>(bStoneBgPointerCheckReturn));

        patch::writeBranchPair(&stone_bg[28],
                               reinterpret_cast<void *>(bStoneBgPointerCheck2),
                               reinterpret_cast<void *>(bStoneBgPointerCheckReturn2));

        if (gState->apSettings->music == 2)
        {
            uint32_t old = main_next;
            main_next = *(uint32_t *)0x80003244;

            int validIndices[259];
            int validCount = 0;

            for (int i = 3; i < 262; i++)
            {
                for (unsigned int j = 0; j < sizeof(invalidParams) / sizeof(uint32_t); j++)
                {
                    if (reinterpret_cast<uint32_t>(main_psbgmlist[i].filename) == invalidParams[j])
                    {
                        break;
                    }
                    if (j == (sizeof(invalidParams) / sizeof(uint32_t)) - 1)
                    {
                        validIndices[validCount] = i;
                        validCount++;
                    }
                }
            }

            if (validCount > 1)
            {
                const char *namePointers[validCount];
                for (int i = 0; i < validCount; i++)
                {
                    namePointers[i] = main_psbgmlist[validIndices[i]].name;
                }

                for (int i = validCount - 1; i > 0; i--)
                {
                    int j = irand(i + 1);
                    const char *temp = namePointers[i];
                    namePointers[i] = namePointers[j];
                    namePointers[j] = temp;
                }

                for (int i = 0; i < validCount; i++)
                {
                    main_psbgmlist[validIndices[i]].name = namePointers[i];
                }
            }

            main_next = old;
        }

        // Add map markers for fast travel
        win_log::mapMarkers[64].map_prefix = rshNode;
        win_log::mapMarkers[64].isLocation = 1;
        win_log::mapMarkers[64].unk_0x05 = 0;

        int16_t tempPos = win_log::mapMarkers[92].y_pos;
        win_log::mapMarkers[92].y_pos = win_log::mapMarkers[91].y_pos;

        win_log::mapMarkers[91].map_prefix = las_09Node;
        win_log::mapMarkers[91].isLocation = 1;
        win_log::mapMarkers[91].unk_0x05 = 0;
        win_log::mapMarkers[91].y_pos = tempPos - 12;
    }

    void ApplyMainScriptPatches()
    {
        main_mail_evt_nok_01[40] = GSW(1701);
        main_mail_evt_nok_01[41] = 4;

        main_mail_evt_gor_03[41] = GSW(1709);
        main_mail_evt_gor_03[42] = 3;

        main_mail_evt_gor_02[41] = GSW(1703);
        main_mail_evt_gor_02[42] = 31;

        main_mail_evt_gor_01[40] = GSW(1705);
        main_mail_evt_gor_01[41] = 6;

        main_mail_evt_gor_04[51] = GSW(1709);
        main_mail_evt_gor_04[52] = 11;

        main_mail_evt_rsh_03_a[40] = GSW(1706);
        main_mail_evt_rsh_03_a[41] = 32;

        main_mail_evt_hom_00[40] = GSW(1720);
        main_mail_evt_hom_00[41] = 8;

        main_mail_evt_gor_02_2[40] = GSW(1706);
        main_mail_evt_gor_02_2[41] = 52;

        main_mail_evt_bom_01[40] = GSW(1707);
        main_mail_evt_bom_01[41] = 3;

        main_mail_evt_tik_00[51] = GSW(1707);
        main_mail_evt_tik_00[52] = 21;

        main_mail_evt_win_01[40] = GSW(1702);
        main_mail_evt_win_01[41] = 14;

        main_mail_evt_tou_01[40] = GSW(1703);
        main_mail_evt_tou_01[41] = 11;

        main_mail_evt_usu_01[40] = GSWF(6043);
        main_mail_evt_usu_01[41] = 1;

        main_mail_evt_gor_01_2[41] = GSW(1717);
        main_mail_evt_gor_01_2[42] = 29;

        main_mail_evt_rsh_03_a_2[40] = GSW(1706);
        main_mail_evt_rsh_03_a_2[41] = 23;

        main_mail_evt_pik_00[40] = GSW(1707);
        main_mail_evt_pik_00[41] = 3;

        main_mobj_save_blk_sysevt[25] = GSW(1700);
        main_mobj_save_blk_sysevt[26] = 7;

        main_init_event[61] = GSW(1715);
        main_init_event[62] = 3;
        main_init_event[64] = GSW(1715);
        main_init_event[65] = 7;

        evt_lecture_msg[107] = GSW(1700);
        evt_lecture_msg[108] = 17;

        patch::writePatch(&main_buy_evt[352], main_buy_evt_hook, sizeof(main_buy_evt_hook));

        patch::writePatch(&evt_sub_starstone[0], starstone_item_handler_hook, sizeof(starstone_item_handler_hook));
        evt_sub_starstone[4] = 0;

        patch::writePatch(&evt_sub_starstone[235], starstone_cam_z_hook, sizeof(starstone_cam_z_hook));
        evt_sub_starstone[238] = 0;
        evt_sub_starstone[239] = 0;
        evt_sub_starstone[241] = 0;
        evt_sub_starstone[242] = 0;
        evt_sub_starstone[243] = 0;
        evt_sub_starstone[244] = 0;

        patch::writePatch(&evt_sub_starstone[259], starstone_interpolate_hook, sizeof(starstone_interpolate_hook));
        evt_sub_starstone[261] = 0;
        evt_sub_starstone[266] = LW(12);

        patch::writePatch(&evt_sub_starstone[439], starstone_item_z_hook, sizeof(starstone_item_z_hook));
        evt_sub_starstone[441] = 0;

        patch::writePatch(&evt_sub_starstone[420], starstone_eff_z_hook, sizeof(starstone_eff_z_hook));
        evt_sub_starstone[422] = 0;

        patch::writePatch(&evt_sub_starstone[535], main_evt_sub_starstone_end_hook, sizeof(main_evt_sub_starstone_end_hook));
        evt_sub_starstone[545] = 0;

        patch::writePatch(&evt_sub_starstone[821], main_evt_sub_starstone_hook, sizeof(main_evt_sub_starstone_hook));

        patch::writePatch(&main_partyChristineAttack_Monosiri[388],
                          main_partyChristineAttack_Monosiri_hook,
                          sizeof(main_partyChristineAttack_Monosiri_hook));

        patch::writePatch(&evt_bero::bero_case_exec_door[0], bero_las_deny_hook, sizeof(bero_las_deny_hook));
        evt_bero::bero_case_exec_door[7] = 0;
    }

    void ApplyItemDataTablePatches()
    {
        using item_data::itemDataTable;
        namespace ItemId = ::common::ItemId;
        namespace IconType = ::icondrv::IconType;

        itemDataTable[ItemId::SUPER_LUIGI].name = goombellaName; // Goombella
        itemDataTable[ItemId::SUPER_LUIGI].description = goombellaDescription; // A pro at Headbonking! She looks up enemy stats and HP.
        itemDataTable[ItemId::SUPER_LUIGI].icon_id = IconType::GOOMBELLA; 
        itemDataTable[ItemId::SUPER_LUIGI_2].name = koopsName; // Koops
        itemDataTable[ItemId::SUPER_LUIGI_2].description = koopsDescription; // A shell-attack master! He has a Defense of 1.
        itemDataTable[ItemId::SUPER_LUIGI_2].icon_id = IconType::KOOPS;
        itemDataTable[ItemId::SUPER_LUIGI_3].name = flurrieName; // Flurrie
        itemDataTable[ItemId::SUPER_LUIGI_3].description = flurrieDescription; // A double threat with her bulk and gale-force breath!
        itemDataTable[ItemId::SUPER_LUIGI_3].icon_id = IconType::FLURRIE;
        itemDataTable[ItemId::SUPER_LUIGI_4].name = yoshiName; // Yoshi
        itemDataTable[ItemId::SUPER_LUIGI_4].description = yoshiDescription; // A hotshot at multiple attacks that each deal low damage.
        itemDataTable[ItemId::SUPER_LUIGI_4].icon_id = IconType::YOSHI_GREEN + gState->apSettings->yoshiColor;
        itemDataTable[ItemId::SUPER_LUIGI_5].name = vivianName; // Vivian
        itemDataTable[ItemId::SUPER_LUIGI_5].description = vivianDescription; // An expert at fire attacks and dodge moves!
        itemDataTable[ItemId::SUPER_LUIGI_5].icon_id = IconType::VIVIAN;
        itemDataTable[ItemId::INVALID_ITEM_006F].name = bobberyName; // Bobbery
        itemDataTable[ItemId::INVALID_ITEM_006F].description = bobberyDescription; // A demolitions expert with a taste for massive explosions!
        itemDataTable[ItemId::INVALID_ITEM_006F].icon_id = IconType::BOBBERY;
        itemDataTable[ItemId::INVALID_ITEM_0070].name = mowzName; // Ms. Mowz
        itemDataTable[ItemId::INVALID_ITEM_0070].description = mowzDescription; // A master thief who can steal anything from anyone!
        itemDataTable[ItemId::INVALID_ITEM_0070].icon_id = IconType::MS_MOWZ;
        itemDataTable[ItemId::INVALID_ITEM_0071].name = apItemName;
        itemDataTable[ItemId::INVALID_ITEM_0071].description = apItemDescription;
        itemDataTable[ItemId::INVALID_ITEM_0071].icon_id = IconType::AP_ITEM;
        itemDataTable[ItemId::INVALID_ITEM_PAPER_0053].name = tenCoinsName;
        itemDataTable[ItemId::INVALID_ITEM_PAPER_0053].description = tenCoinsDescription;
        itemDataTable[ItemId::INVALID_ITEM_PAPER_0053].icon_id = IconType::COIN;
        itemDataTable[ItemId::INVALID_ITEM_PAPER_0054].name = returnPipeName;
        itemDataTable[ItemId::INVALID_ITEM_PAPER_0054].description = returnPipeDescription;
        itemDataTable[ItemId::INVALID_ITEM_PAPER_0054].icon_id = IconType::RETURN_PIPE;
        itemDataTable[ItemId::INVALID_ITEM_PAPER_0054].type_sort_order = 1;
        itemDataTable[ItemId::INVALID_ITEM_PLANE_MODE_ICON].name = planeModeName;
        itemDataTable[ItemId::INVALID_ITEM_PLANE_MODE_ICON].description = planeModeDescription;
        itemDataTable[ItemId::INVALID_ITEM_PAPER_MODE_ICON].name = paperModeName;
        itemDataTable[ItemId::INVALID_ITEM_PAPER_MODE_ICON].description = paperModeDescription;
        itemDataTable[ItemId::INVALID_ITEM_TUBE_MODE_ICON].name = tubeModeName;
        itemDataTable[ItemId::INVALID_ITEM_TUBE_MODE_ICON].description = tubeModeDescription;
        itemDataTable[ItemId::INVALID_ITEM_BOAT_MODE_ICON].name = boatModeName;
        itemDataTable[ItemId::INVALID_ITEM_BOAT_MODE_ICON].description = boatModeDescription;
        itemDataTable[ItemId::WHACKA_BUMP].sell_price = 30;

        // Buy/Sell Prices
        for (int i = ItemId::POWER_JUMP; i < ItemId::MAX_ITEM_ID; i++)
        {
            int divisor = (itemDataTable[i].sell_price >= 250) ? 6 : (itemDataTable[i].sell_price >= 100) ? 4 : 2;
            itemDataTable[i].sell_price = ((itemDataTable[i].sell_price / divisor + 2) / 5) * 5;
            itemDataTable[i].buy_price = itemDataTable[i].discount_price = itemDataTable[i].sell_price * 2;
        }
      
        if (gState->apSettings->firstAttack)
            itemDataTable[ItemId::FIRST_ATTACK].bp_cost = 0;
      
        // Key Renames
        itemDataTable[ItemId::ELEVATOR_KEY_001A].name = elevatorKeyStationName;
        itemDataTable[ItemId::CARD_KEY_001D].name = cardKey1Name;
        itemDataTable[ItemId::CARD_KEY_001E].name = cardKey2Name;
        itemDataTable[ItemId::CARD_KEY_001F].name = cardKey3Name;
        itemDataTable[ItemId::CARD_KEY_0020].name = cardKey4Name;
        itemDataTable[ItemId::PALACE_KEY_0026].name = palaceKeyTowerName;
        itemDataTable[ItemId::ELEVATOR_KEY_001B].name = elevatorKey1Name;
        itemDataTable[ItemId::ELEVATOR_KEY_001C].name = elevatorKey2Name;
        itemDataTable[ItemId::STATION_KEY_0018].name = stationKey1Name;
        itemDataTable[ItemId::STATION_KEY_0019].name = stationKey2Name;
        itemDataTable[ItemId::STORAGE_KEY_0012].name = storageKey1Name;
        itemDataTable[ItemId::STORAGE_KEY_0013].name = storageKey2Name;
        itemDataTable[ItemId::BLACK_KEY_0021].name = blackKeyPlaneName;
        itemDataTable[ItemId::BLACK_KEY_0022].name = blackKeyPaperName;
        itemDataTable[ItemId::BLACK_KEY_0023].name = blackKeyTubeName;
        itemDataTable[ItemId::BLACK_KEY_0024].name = blackKeyBoatName;

        //Progressive Renames
        itemDataTable[ItemId::BOOTS].name = progressiveBootsName;
        itemDataTable[ItemId::HAMMER].name = progressiveHammerName;
    }

    OWR::OWR()
    {
        gSelf = this;
        gState = &gSelf->state;

        ApplyMainAssemblyPatches();
        ApplyMainScriptPatches();
        ApplyItemDataTablePatches();

        g_OSLink_trampoline = patch::hookFunction(OSLink, OSLinkHook);
        g_seq_logoMain_trampoline = patch::hookFunction(seq_logo::seq_logoMain, logoSkip);
        g_seq_gameInit_trampoline = patch::hookFunction(seq_game::seq_gameInit, seq_gameInitHook);
        g_seqSetSeq_trampoline = patch::hookFunction(seqdrv::seqSetSeq, seqSetSeqHook);
        g_msgSearch_trampoline = patch::hookFunction(msgdrv::msgSearch, msgSearchHook);
        g_pouchGetItem_trampoline = patch::hookFunction(mario_pouch::pouchGetItem, pouchGetItemHook);
        g_partySetForceMove_trampoline = patch::hookFunction(party::partySetForceMove, partySetForceMoveHook);
        g_evt_mario_set_pose_trampoline = patch::hookFunction(evt_mario::evt_mario_set_pose, evtMarioSetPoseHook);
        g_statusWinDisp_trampoline = patch::hookFunction(statuswindow::statusWinDisp, DisplayStarPowerNumber);
        g_pouchGetStarstone_trampoline = patch::hookFunction(mario_pouch::pouchGetStarStone, SetMaxSP);
        g_winItemMain_trampoline = patch::hookFunction(win_item::winItemMain, WinItemMainHook);
        g_winLogMain_trampoline = patch::hookFunction(win_log::main_winLogMain, WinLogMainHook);
        g_msgAnalize_trampoline = patch::hookFunction(msgdrv::msgAnalize, MsgAnalizeHook);
        g_msgWindow_Entry_trampoline = patch::hookFunction(msgdrv::msgWindow_Entry, msgWindow_Entry_Hook);
        g__load_trampoline = patch::hookFunction(seq_mapchange::_load, _load_Hook);
        g_BtlUnit_Entry_trampoline = patch::hookFunction(battle_unit::BtlUnit_Entry, BtlUnit_Entry_Hook);

        // Hook gaugeDisp with a standard branch since the original function does not need to be called
        patch::writeBranch(statuswindow::gaugeDisp, DisplayStarPowerOrbs);
    }
} // namespace mod::owr
