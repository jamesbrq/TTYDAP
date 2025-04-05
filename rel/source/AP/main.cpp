#include "patch.h"
#include "ttyd/seq_mapchange.h"
#include "ttyd/seqdrv.h"
#include <AP/rel_patch_definitions.h>
#include <ttyd/common_types.h>
#include <ttyd/evt_shop.h>
#include <ttyd/evtmgr_cmd.h>
#include <ttyd/icondrv.h>
#include <ttyd/item_data.h>
#include <ttyd/swdrv.h>

#include "evt_cmd.h"
#include <cstdint>
#include <cstring>

using namespace ttyd::seq_mapchange;
using namespace ttyd::evtmgr_cmd;
using namespace ttyd::swdrv;
using namespace ttyd;

using namespace mod;
using namespace mod::patch;

using ttyd::item_data::itemDataTable;
namespace ItemId = ::ttyd::common::ItemId;
namespace IconType = ::ttyd::icondrv::IconType;

// Assembly References
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
extern int32_t winMgrSelectEntry[];
extern int32_t evt_badgeShop_starmaniac_get_kind_cnt[];
extern int32_t badgeShop_get[];
extern int32_t badgeShop_add[];
extern int32_t evt_mobj_kururing_floor[];
extern int32_t mobj_kururing_floor[];
extern int32_t mobj_powerupblk[];
extern int32_t evt_mobj_powerupblk[];
extern int32_t breakfast[];
extern int32_t evt_shop_setup[];
// End of Assembly References

// Script References
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
extern int32_t main_buy_evt[];

uintptr_t goods[] = {
    0x805f162c, 
    0x80612510,
    0x805c8738,
    0x805be5b4,
    0x805fb948,
    0x805d5874,
    0x805d0588,
    0x805dc5b8,
    0x805de110, 
    0x805cbb14
};

extern int32_t mobj_save_blk_sysevt[];
extern int32_t init_event[];
extern int32_t evt_lecture_msg[];
extern int32_t evt_msg_print_party[];
extern int32_t evt_msg_print_party_add[];
extern int32_t preventDiaryTextboxSelectionAddress[];

uintptr_t shopWorkPtr = 0x8041EB60;

EVT_DEFINE_USER_FUNC(setShopFlags)  
{  
   (void)isFirstCall;  
   char *shopWork = reinterpret_cast<char *>(evtGetValue(evt, evt->evtArguments[0]));

   uint32_t *itemIds = *reinterpret_cast<uint32_t **>(shopWork + 0x08);
   uint16_t *itemFlags = reinterpret_cast<uint16_t *>(shopWork + 0x14);
   uint32_t selectedIndex = *reinterpret_cast<uint32_t *>(shopWork + 0x2C);

   int gswfBase = 6200;
   uintptr_t itemIdsAddress = reinterpret_cast<uintptr_t>(itemIds);
   int loopCount = static_cast<int>(sizeof(goods) / sizeof(uintptr_t));
   for (int i = 0; i < loopCount; i++)
   {
       if (itemIdsAddress != goods[i])
       {
           if (i == loopCount - 1)
               return 2;
           gswfBase += 6;
           continue;
       }
       break;
   }
   swSet(gswfBase + selectedIndex);
   if (itemIds[selectedIndex * 2] > 120)
       return 2;
   itemFlags[selectedIndex] |= 1;
   return 2;  
}

void checkShopFlag(uint32_t item, uint32_t index)
{
    if (item > 120)
        return;
    char *shopWork = *reinterpret_cast<char **>(shopWorkPtr);
    uint32_t *itemIds = *reinterpret_cast<uint32_t **>(shopWork + 0x08);
    uint16_t *itemFlags = reinterpret_cast<uint16_t *>(shopWork + 0x14);

    int gswfBase = 6200;
    uintptr_t itemIdsAddress = reinterpret_cast<uintptr_t>(itemIds);
    int loopCount = static_cast<int>(sizeof(goods) / sizeof(uintptr_t));
    for (int i = 0; i < loopCount; i++)
    {
        if (itemIdsAddress != goods[i])
        {
            if (i == loopCount - 1)
                return;
            gswfBase += 6;
            continue;
        }
        break;
    }
    if (swGet(gswfBase + index))
        itemFlags[index] |= 1;
}

// clang-format off
EVT_BEGIN(main_buy_evt_evt)
    USER_FUNC(evt_shop::get_ptr, LW(0))
    USER_FUNC(setShopFlags, LW(0))
    USER_FUNC(evt_shop::get_buy_evt, LW(1))
    IF_NOT_EQUAL(LW(1), 0)
        SET(LW(0), LW(12))
        RUN_CHILD_EVT(LW(1))
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN(main_buy_evt_hook)
    RUN_CHILD_EVT(main_buy_evt_evt)
    RETURN()
EVT_END()
// clang-format on

void ApplyMainAssemblyPatches()
{
    writeIntWithCache(&statusWinDisp[425], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
    writeIntWithCache(&statusWinDisp[487], 0x386006A4); // li r3, 0x6A4 (GSW(1700))

    writeIntWithCache(&winRootMain[40], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
    writeIntWithCache(&winRootMain[82], 0x386006A4); // li r3, 0x6A4 (GSW(1700))

    writeIntWithCache(&compare_func3_r[30], 0x38840833); // addi r4, r4, 0x833 GSW(1715)
    writeIntWithCache(&compare_func3_r[32], 0x2C030005); // cmpwi r3, 0x5
    writeIntWithCache(&compare_func3_r[52], 0x38840833); // addi r4, r4, 0x833 GSW(1715)
    writeIntWithCache(&compare_func3_r[54], 0x2C030005); // cmpwi r3, 0x5

    writeIntWithCache(&compare_func3[30], 0x38840833); // addi r4, r4, 0x833 GSW(1715)
    writeIntWithCache(&compare_func3[32], 0x2C030005); // cmpwi r3, 0x5
    writeIntWithCache(&compare_func3[52], 0x38840833); // addi r4, r4, 0x833 GSW(1715)
    writeIntWithCache(&compare_func3[54], 0x2C030005); // cmpwi r3, 0x5

    writeIntWithCache(&mapGX[238], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
    writeIntWithCache(&mapGX[240], 0x2C030010); // cmpwi r3, 0xF

    patch::writeBranchPair(&mapGX[91],
                           reinterpret_cast<void *>(bMapGXArrInject),
                           reinterpret_cast<void *>(bMapGXArrInjectReturn));
    patch::writeBranchPair(&mapGX[235],
                           reinterpret_cast<void *>(bMapGXArrIncrement),
                           reinterpret_cast<void *>(bMapGXArrIncrementReturn));
    patch::writeBranchPair(&mapGX[243], reinterpret_cast<void *>(bMapGXChSplit), reinterpret_cast<void *>(bMapGXChSplitReturn));

    writeIntWithCache(&winGetMapTplName[3], 0x386006A5);  // li r3, 0x6A5 (GSW(1701))
    writeIntWithCache(&winGetMapTplName[8], 0x2C030001);  // cmpwi r3, 0x1
    writeIntWithCache(&winGetMapTplName[12], 0x386006A6); // li r3, 0x6A6 (GSW(1702))
    writeIntWithCache(&winGetMapTplName[14], 0x2C030001); // cmpwi r3, 0x1
    writeIntWithCache(&winGetMapTplName[18], 0x386006A7); // li r3, 0x6A7 (GSW(1703))
    writeIntWithCache(&winGetMapTplName[20], 0x2C030001); // cmpwi r3, 0x1
    writeIntWithCache(&winGetMapTplName[24], 0x386006A8); // li r3, 0x6A8 (GSW(1704))
    writeIntWithCache(&winGetMapTplName[26], 0x2C030001); // cmpwi r3, 0x2
    writeIntWithCache(&winGetMapTplName[30], 0x386006A9); // li r3, 0x6A9 (GSW(1705))
    writeIntWithCache(&winGetMapTplName[32], 0x2C03000A); // cmpwi r3, 0xA
    writeIntWithCache(&winGetMapTplName[36], 0x386006AA); // li r3, 0x6AA (GSW(1706))
    writeIntWithCache(&winGetMapTplName[38], 0x2C030002); // cmpwi r3, 0x2
    writeIntWithCache(&winGetMapTplName[42], 0x386006AB); // li r3, 0x6AB (GSW(1707))
    writeIntWithCache(&winGetMapTplName[44], 0x2C030001); // cmpwi r3, 0x1

    writeIntWithCache(&winLogMain[196], 0x386006A5); // li r3, 0x6A5 (GSW(1701))
    writeIntWithCache(&winLogMain[198], 0x2C030001); // cmpwi r3, 0x1
    writeIntWithCache(&winLogMain[202], 0x386006A6); // li r3, 0x6A6 (GSW(1702))
    writeIntWithCache(&winLogMain[204], 0x2C030001); // cmpwi r3, 0x1
    writeIntWithCache(&winLogMain[208], 0x386006A7); // li r3, 0x6A7 (GSW(1703))
    writeIntWithCache(&winLogMain[210], 0x2C030001); // cmpwi r3, 0x1
    writeIntWithCache(&winLogMain[214], 0x386006A8); // li r3, 0x6A8 (GSW(1704))
    writeIntWithCache(&winLogMain[216], 0x2C030001); // cmpwi r3, 0x1
    writeIntWithCache(&winLogMain[220], 0x386006A9); // li r3, 0x6A9 (GSW(1705))
    writeIntWithCache(&winLogMain[222], 0x2C03000A); // cmpwi r3, 0xA
    writeIntWithCache(&winLogMain[226], 0x386006AA); // li r3, 0x6AA (GSW(1706))
    writeIntWithCache(&winLogMain[228], 0x2C030002); // cmpwi r3, 0x2
    writeIntWithCache(&winLogMain[232], 0x386006AB); // li r3, 0x6AB (GSW(1707))
    writeIntWithCache(&winLogMain[234], 0x2C030001); // cmpwi r3, 0x1
    writeIntWithCache(&winLogMain[259], 0x386006A5); // li r3, 0x6A5 (GSW(1701))
    writeIntWithCache(&winLogMain[261], 0x2C030001); // cmpwi r3, 0x1
    writeIntWithCache(&winLogMain[265], 0x386006A6); // li r3, 0x6A6 (GSW(1702))
    writeIntWithCache(&winLogMain[267], 0x2C030001); // cmpwi r3, 0x1
    writeIntWithCache(&winLogMain[271], 0x386006A7); // li r3, 0x6A7 (GSW(1703))
    writeIntWithCache(&winLogMain[273], 0x2C030001); // cmpwi r3, 0x1
    writeIntWithCache(&winLogMain[277], 0x386006A8); // li r3, 0x6A8 (GSW(1704))
    writeIntWithCache(&winLogMain[279], 0x2C030001); // cmpwi r3, 0x1
    writeIntWithCache(&winLogMain[283], 0x386006A9); // li r3, 0x6A9 (GSW(1705))
    writeIntWithCache(&winLogMain[285], 0x2C03000A); // cmpwi r3, 0xA
    writeIntWithCache(&winLogMain[289], 0x386006AA); // li r3, 0x6AA (GSW(1706))
    writeIntWithCache(&winLogMain[291], 0x2C030002); // cmpwi r3, 0x2
    writeIntWithCache(&winLogMain[295], 0x386006AB); // li r3, 0x6AB (GSW(1707))
    writeIntWithCache(&winLogMain[297], 0x2C030001); // cmpwi r3, 0x1

    patch::writeBranchPair(&winLogMain[432],
                           reinterpret_cast<void *>(bWinLogArrInject),
                           reinterpret_cast<void *>(bWinLogArrInjectReturn));
    patch::writeBranchPair(&winLogMain[505],
                           reinterpret_cast<void *>(bWinLogArrIncrement),
                           reinterpret_cast<void *>(bWinLogArrIncrementReturn));

    writeIntWithCache(&winLogInit[53], 0x386006A4);  // li r3, 0x6A4 (GSW(1700))
    writeIntWithCache(&winLogInit[55], 0x2C030000);  // cmpwi r3, 0x0
    writeIntWithCache(&winLogInit[102], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
    writeIntWithCache(&winLogInit[104], 0x2C030000); // cmpwi r3, 0x0
    writeIntWithCache(&winLogInit[151], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
    writeIntWithCache(&winLogInit[153], 0x2C030000); // cmpwi r3, 0x0
    writeIntWithCache(&winLogInit[356], 0x386006B8); // li r3, 0x6B8 (GSW(1720))
    writeIntWithCache(&winLogInit[358], 0x2C030001); // cmpwi r3, 0x1
    writeIntWithCache(&winLogInit[362], 0x386006AA); // li r3, 0x6AA (GSW(1706))
    writeIntWithCache(&winLogInit[364], 0x2C030026); // cmpwi r3, 0x26
    writeIntWithCache(&winLogInit[670], 0x3884082C); // addi r4, r4, 0x82C GSW(1708)
    writeIntWithCache(&winLogInit[672], 0x2C030009); // cmpwi r3, 0x9
    writeIntWithCache(&winLogInit[687], 0x3884082C); // addi r4, r4, 0x82C GSW(1708)
    writeIntWithCache(&winLogInit[689], 0x2C030010); // cmpwi r3, 0x10

    writeIntWithCache(&monoshiriGX[231], 0x38840833); // addi r4, r4, 0x833 GSW(1715)
    writeIntWithCache(&monoshiriGX[233], 0x2C030005); // cmpwi r3, 0x5

    writeIntWithCache(&monosiri_disp[376], 0x38840833); // addi r4, r4, 0x833 GSW(1715)
    writeIntWithCache(&monosiri_disp[378], 0x2C030005); // cmpwi r3, 0x5

    writeIntWithCache(&winMain[184], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
    writeIntWithCache(&winMain[186], 0x2C030000); // cmpwi r3, 0x0
    writeIntWithCache(&winMain[205], 0x386006A4); // li r3, 0x6A4 (GSW(1700))
    writeIntWithCache(&winMain[207], 0x2C030000); // cmpwi r3, 0x0
    writeIntWithCache(&winMain[232], 0x38840824); // addi r4, r4, 0x824 GSW(1700)
    writeIntWithCache(&winMain[234], 0x2C030000); // cmpwi r3, 0x0

    writeIntWithCache(&BattleInformationSetDropMaterial[84], 0x38830824); // addi r4, r3, 0x824 GSW(1700)
    writeIntWithCache(&BattleInformationSetDropMaterial[89], 0x2C030000); // cmpwi r3, 0x0

    writeIntWithCache(&_rule_disp[12], 0x38840827); // addi r4, r4, 0x827 GSW(1703)
    writeIntWithCache(&_rule_disp[14], 0x2C03001C); // cmpwi r3, 0x1C

    writeIntWithCache(&tou_gamen_screen_tev_init[27], 0x38840827); // addi r4, r4, 0x827 GSW(1703)
    writeIntWithCache(&tou_gamen_screen_tev_init[48], 0x2C030013); // cmpwi r3, 0x13

    writeIntWithCache(&setupDataLoad[23], 0x38840824); // addi r4, r4, 0x824 GSW(1700)

    patch::writeBranchBL(&badgeShop_bargainGeneration[7], reinterpret_cast<void *>(bChapterClearCheck));
    writeIntWithCache(&badgeShop_bargainGeneration[8], 0x2C030001); // cmpwi r3, 0x1
    patch::writeBranchBL(&badgeShop_bargainGeneration[34], reinterpret_cast<void *>(bChapterClearCheck));
    writeIntWithCache(&badgeShop_bargainGeneration[35], 0x2C030002); // cmpwi r3, 0x2
    patch::writeBranchBL(&badgeShop_bargainGeneration[61], reinterpret_cast<void *>(bChapterClearCheck));
    writeIntWithCache(&badgeShop_bargainGeneration[62], 0x2C030003); // cmpwi r3, 0x3
    patch::writeBranchBL(&badgeShop_bargainGeneration[88], reinterpret_cast<void *>(bChapterClearCheck));
    writeIntWithCache(&badgeShop_bargainGeneration[89], 0x2C030004); // cmpwi r3, 0x4
    patch::writeBranchBL(&badgeShop_bargainGeneration[115], reinterpret_cast<void *>(bChapterClearCheck));
    writeIntWithCache(&badgeShop_bargainGeneration[116], 0x2C030005); // cmpwi r3, 0x5
    patch::writeBranchBL(&badgeShop_bargainGeneration[142], reinterpret_cast<void *>(bChapterClearCheck));
    writeIntWithCache(&badgeShop_bargainGeneration[143], 0x2C030006); // cmpwi r3, 0x6
    patch::writeBranchBL(&badgeShop_bargainGeneration[169], reinterpret_cast<void *>(bChapterClearCheck));
    writeIntWithCache(&badgeShop_bargainGeneration[170], 0x2C030007); // cmpwi r3, 0x7
    writeIntWithCache(&badgeShop_bargainGeneration[184], 0x386006AC); // li r3, 0x6AC (GSW(1708))
    writeIntWithCache(&badgeShop_bargainGeneration[187], 0x2C030011); // cmpwi r3, 0x11

    patch::writeBranchBL(&johoya_data_make[11], reinterpret_cast<void *>(bJohoyaSeqAddition));

    writeIntWithCache(&evt_shop_main_func[41], 0x3884082A); // addi r4, r4, 0x82C GSW(1706)
    writeIntWithCache(&evt_shop_main_func[43], 0x2C030020); // cmpwi r3, 0x20
    writeIntWithCache(&evt_shop_main_func[47], 0x3884082A); // addi r4, r4, 0x82C GSW(1706)
    writeIntWithCache(&evt_shop_main_func[49], 0x2C030024); // cmpwi r3, 0x24

    patch::writeBranchBL(&uranaisi_data_make_next[11], reinterpret_cast<void *>(bJohoyaSeqAddition));

    patch::writeBranchBL(&uranaisi_data_make_starpiece[11], reinterpret_cast<void *>(bJohoyaSeqAddition));

    patch::writeBranchBL(&uranaisi_data_make_supercoin[11], reinterpret_cast<void *>(bJohoyaSeqAddition));

    patch::writeBranchPair(&evt_msg_print_party[30],
                           reinterpret_cast<void *>(bPrintPartyErrorFix),
                           reinterpret_cast<void *>(bPrintPartyErrorFixReturn));

    patch::writeBranchPair(&evt_msg_print_party_add[32],
                           reinterpret_cast<void *>(bPrintPartyAddErrorFix),
                           reinterpret_cast<void *>(bPrintPartyAddErrorFixReturn));

    patch::writeBranchPair(&evt_mobj_kururing_floor[39],
                           reinterpret_cast<void *>(bKururingFloorCapture),
                           reinterpret_cast<void *>(bKururingFloorCaptureReturn));
    patch::writeBranchPair(&evt_mobj_kururing_floor[60],
                           reinterpret_cast<void *>(bKururingFloorRelease),
                           reinterpret_cast<void *>(bKururingFloorReleaseReturn));
    patch::writeBranchPair(&evt_mobj_powerupblk[24],
                           reinterpret_cast<void *>(bPowerupblkCapture),
                           reinterpret_cast<void *>(bPowerupblkCaptureReturn));
    patch::writeBranchPair(&evt_mobj_powerupblk[52],
                           reinterpret_cast<void *>(bPowerupblkRelease),
                           reinterpret_cast<void *>(bPowerupblkReleaseReturn));

    writeIntWithCache(&mobj_kururing_floor[188], 0x808301BB); // lwz r4, 0x1BA(r3)

    writeIntWithCache(&mobj_powerupblk[73], 0x809F01D8);  // lwz r4, 0x1D8(r31)
    writeIntWithCache(&mobj_powerupblk[110], 0x809F01D8); // lwz r4, 0x1D8(r31)

    patch::writeBranchPair(&breakfast[22], reinterpret_cast<void *>(bPeachPointer), reinterpret_cast<void *>(bPeachReturn));

    writeIntWithCache(&evt_shop_setup[82], 0x2C000079); // cmpwi r0, 0x79
    writeIntWithCache(&evt_shop_setup[83], 0x418100FC); // blt +0xFC
    patch::writeBranchPair(&evt_shop_setup[84],
                           reinterpret_cast<void *>(bShopFlagCheck),
                           reinterpret_cast<void *>(bShopFlagCheckReturn));
    writeIntWithCache(&evt_shop_setup[85], 0x480000F4); // b +0xF4

    // sys_prolog[25] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
    // sys_prolog[28] = 0x2C030002; // cmpwi r3, 0x2
    // sys_prolog[25] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
    // sys_prolog[28] = 0x2C030002; // cmpwi r3, 0x2
    // sys_prolog[25] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
    // sys_prolog[28] = 0x2C030002; // cmpwi r3, 0x2
    // sys_prolog[25] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
    // sys_prolog[28] = 0x2C030002; // cmpwi r3, 0x2
    //  FINISH ^
}

void ApplyMainScriptPatches()
{
    mail_evt_nok_01[40] = GSW(1701);
    mail_evt_nok_01[41] = 4;

    mail_evt_gor_03[41] = GSW(1709);
    mail_evt_gor_03[42] = 3;

    mail_evt_gor_02[41] = GSW(1703);
    mail_evt_gor_02[42] = 31;

    mail_evt_gor_01[40] = GSW(1705);
    mail_evt_gor_01[41] = 6;

    mail_evt_gor_04[51] = GSW(1709);
    mail_evt_gor_04[52] = 11;

    mail_evt_rsh_03_a[40] = GSW(1706);
    mail_evt_rsh_03_a[41] = 32;

    mail_evt_hom_00[40] = GSW(1720);
    mail_evt_hom_00[41] = 8;

    mail_evt_gor_02_2[40] = GSW(1706);
    mail_evt_gor_02_2[41] = 52;

    mail_evt_bom_01[40] = GSW(1707);
    mail_evt_bom_01[41] = 3;

    mail_evt_tik_00[51] = GSW(1707);
    mail_evt_tik_00[52] = 21;

    mail_evt_win_01[40] = GSW(1702);
    mail_evt_win_01[41] = 14;

    mail_evt_tou_01[40] = GSW(1703);
    mail_evt_tou_01[41] = 11;

    mail_evt_usu_01[40] = GSWF(6043);
    mail_evt_usu_01[41] = 1;

    mail_evt_gor_01_2[41] = GSW(1717);
    mail_evt_gor_01_2[42] = 29;

    mail_evt_rsh_03_a_2[40] = GSW(1706);
    mail_evt_rsh_03_a_2[41] = 23;

    mail_evt_pik_00[40] = GSW(1707);
    mail_evt_pik_00[41] = 3;

    mobj_save_blk_sysevt[25] = GSW(1700);
    mobj_save_blk_sysevt[26] = 7;

    init_event[61] = GSW(1715);
    init_event[62] = 3;
    init_event[64] = GSW(1715);
    init_event[65] = 7;

    evt_lecture_msg[107] = GSW(1700);
    evt_lecture_msg[108] = 17;

    patch::writePatch(&main_buy_evt[352], main_buy_evt_hook, sizeof(main_buy_evt_hook));
}

void ApplyItemDataTablePatches()
{
    itemDataTable[ItemId::SUPER_LUIGI].name = "goombella";
    itemDataTable[ItemId::SUPER_LUIGI].description = "goombella_desc";
    itemDataTable[ItemId::SUPER_LUIGI].icon_id = IconType::GOOMBELLA;
    itemDataTable[ItemId::SUPER_LUIGI_2].name = "koops";
    itemDataTable[ItemId::SUPER_LUIGI_2].description = "koops_desc";
    itemDataTable[ItemId::SUPER_LUIGI_2].icon_id = IconType::KOOPS;
    itemDataTable[ItemId::SUPER_LUIGI_3].name = "flurrie";
    itemDataTable[ItemId::SUPER_LUIGI_3].description = "flurrie_desc";
    itemDataTable[ItemId::SUPER_LUIGI_3].icon_id = IconType::FLURRIE;
    itemDataTable[ItemId::SUPER_LUIGI_4].name = "yoshi";
    itemDataTable[ItemId::SUPER_LUIGI_4].description = "yoshi_desc";
    itemDataTable[ItemId::SUPER_LUIGI_4].icon_id = IconType::YOSHI_GREEN;
    itemDataTable[ItemId::SUPER_LUIGI_5].name = "vivian";
    itemDataTable[ItemId::SUPER_LUIGI_5].description = "vivian_desc";
    itemDataTable[ItemId::SUPER_LUIGI_5].icon_id = IconType::VIVIAN;
    itemDataTable[ItemId::INVALID_ITEM_006F].name = "bobbery";
    itemDataTable[ItemId::INVALID_ITEM_006F].description = "bobbery_desc";
    itemDataTable[ItemId::INVALID_ITEM_006F].icon_id = IconType::BOBBERY;
    itemDataTable[ItemId::INVALID_ITEM_0070].name = "mowz";
    itemDataTable[ItemId::INVALID_ITEM_0070].description = "mowz_desc";
    itemDataTable[ItemId::INVALID_ITEM_0070].icon_id = IconType::MS_MOWZ;
    itemDataTable[ItemId::INVALID_ITEM_0071].name = "ap_item";
    itemDataTable[ItemId::INVALID_ITEM_0071].description = "ap_item";
    itemDataTable[ItemId::INVALID_ITEM_0071].icon_id = IconType::AP_ITEM;
    itemDataTable[ItemId::INVALID_ITEM_PAPER_0053].name = "10_coins";
    itemDataTable[ItemId::INVALID_ITEM_PAPER_0053].description = "10_coins";
    itemDataTable[ItemId::INVALID_ITEM_PAPER_0053].icon_id = IconType::COIN;
    itemDataTable[ItemId::INVALID_ITEM_PLANE_MODE_ICON].name = "plane_mode";
    itemDataTable[ItemId::INVALID_ITEM_PLANE_MODE_ICON].description = "plane_mode";
    itemDataTable[ItemId::INVALID_ITEM_PAPER_MODE_ICON].name = "paper_mode";
    itemDataTable[ItemId::INVALID_ITEM_PAPER_MODE_ICON].description = "paper_mode";
    itemDataTable[ItemId::INVALID_ITEM_TUBE_MODE_ICON].name = "tube_mode";
    itemDataTable[ItemId::INVALID_ITEM_TUBE_MODE_ICON].description = "tube_mode";
    itemDataTable[ItemId::INVALID_ITEM_BOAT_MODE_ICON].name = "boat_mode";
    itemDataTable[ItemId::INVALID_ITEM_BOAT_MODE_ICON].description = "boat_mode";
}
