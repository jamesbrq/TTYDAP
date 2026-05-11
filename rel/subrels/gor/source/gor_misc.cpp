#include "AP/rel_patch_definitions.h"
#include "evt_cmd.h"
#include "patch.h"
#include "subrel_gor.h"
#include "ttyd/evt_cam.h"
#include "ttyd/evt_item.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_msg.h"
#include "ttyd/evt_npc.h"
#include "ttyd/evt_pouch.h"
#include "ttyd/evtmgr_cmd.h"
#include "ttyd/gor_02.h"
#include "ttyd/gor_irai.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/swdrv.h"

#include <cstdint>

using ttyd::gor_02::gor_follow;
using namespace mod;
using namespace ttyd;

extern int32_t gor_luigi_gor_first_talk[];
extern int32_t gor_10_init_evt[];
extern int32_t gor_12_init_evt[];
extern int32_t gor_11_init_evt[];
extern int32_t gor_christine_nakama[];
extern int32_t gor_irai_msg_exposition[];
extern int32_t gor_irai_main[];
extern int32_t gor_irai_tenshu_main[];

// Assembly
extern int32_t gor_irai_init_func[];
extern int32_t gor_irai_main_func[];
extern int32_t gor_irai_window_disp[];
extern int32_t gor_keijiban_data_make[];
extern int32_t gor_monosiri_check[];
extern int32_t gor_evt_exchange_msg_set[];
extern int32_t gor_exchange_ret_tbl_no[];
extern int32_t gor_exchange_ryokin_medal[];

extern "C" {
    void bIraiCmp1(); void bIraiCmp1Return();
    void bIraiCmp2(); void bIraiCmp2Return();
    void bIraiCmp3(); void bIraiCmp3Return();
    void bIraiCmp4(); void bIraiCmp4Return();
    void bIraiCmp5(); void bIraiCmp5Return();
    void bIraiCmp6(); void bIraiCmp6Return();
    void bIraiCmp7(); void bIraiCmp7Return();
    void bIraiCmp8(); void bIraiCmp8Return();
    void bIraiCmp9(); void bIraiCmp9Return();
    void bIraiCompact(); void bIraiCompactReturn();
    void bIraiRenderRedCheck(); void bIraiRenderRedCheckReturn();
    void bIraiRenderGreyCheck(); void bIraiRenderGreyCheckReturn();
    void bIraiMainCompleteCheck(); void bIraiMainCompleteCheckReturn();
}

const char goombella[] = "\x83\x4C\x83\x6D\x82\xB6\x82\xA2";

// Array of new story thresholds for each chapter
uint32_t new_thresholds[8] = {
    1, // Blooper
    8,  // Ch.1
    11, // Ch.2
    20, // Ch.3
    10, // Ch.5
    42, // Ch.6
    16, // Ch.7
    18  // Ch.8
};

EVT_DEFINE_USER_FUNC(checkChapterClears)
{
    (void)isFirstCall;

    int8_t count = 0;
    for (int i = 114; i <= 120; i++)
    {
        if (ttyd::mario_pouch::pouchCheckItem(i) > 0)
            count++;
    }
    ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], count);
    return 2;
}

EVT_DEFINE_USER_FUNC(iraiSetStartFlag)
{
    (void)isFirstCall;

    ttyd::swdrv::swByteSet(1730 + ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]), 1);
    return 2;
}

EVT_DEFINE_USER_FUNC(iraiGetStartFlag)
{
    (void)isFirstCall;
    ttyd::evtmgr_cmd::evtSetValue(evt,
                                  evt->evtArguments[1],
                                  ttyd::swdrv::swByteGet(1730 + ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0])));
    return 2;
}

EVT_DEFINE_USER_FUNC(iraiClearStartFlag)
{
    (void)isFirstCall;

    ttyd::swdrv::swByteSet(1730 + ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]), 0);
    return 2;
}

// clang-format off
EVT_BEGIN(party_evt)
	USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
	USER_FUNC(evt_item::evt_item_entry, PTR("item01"), LW(3), LW(0), LW(1), LW(2), 16, GSWF(6076), 0)
	USER_FUNC(evt_item::evt_item_get_item, PTR("item01"))
	WAIT_MSEC(800)
	SET(GSW(1700), 7)
	USER_FUNC(evt_npc::evt_npc_set_position, PTR(&goombella), 0, -1000, 0)
	USER_FUNC(evt_cam::evt_cam3d_evt_off, 500, 11)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
	RETURN()
EVT_END()

EVT_BEGIN(irai_cancel_check_evt)
    IF_EQUAL(LW(0), -1)
        SET(LW(15), 1)
        RETURN()
    END_IF()
    USER_FUNC(iraiGetStartFlag, LW(11), LW(15))
    IF_EQUAL(LW(15), 1)
        USER_FUNC(evt_msg::evt_msg_print, 1, PTR("Do you want to cancel\nthis request?\n<o>"), 0, 0)
        USER_FUNC(evt_msg::evt_msg_select, 1, PTR("<select 0 2 0 60>\nYes\nNo"))
        IF_NOT_EQUAL(LW(0), 0)
            USER_FUNC(evt_msg::evt_msg_continue)
        ELSE()
            USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("irai_09"))
            USER_FUNC(evt_msg::evt_msg_select, 0, PTR("irai_01"))
            IF_EQUAL(LW(0), 0)
                USER_FUNC(evt_pouch::evt_pouch_get_coin, LW(2))
                IF_LARGE_EQUAL(LW(2), 30)
                    USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("irai_10"))
                    USER_FUNC(evt_pouch::evt_pouch_add_coin, -30)
                    USER_FUNC(iraiClearStartFlag, LW(11))
                ELSE()
                    USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("irai_11"))
                END_IF()
            ELSE()
                USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("irai_12"))
            END_IF()
        END_IF()
    END_IF()
    SET(LW(0), LW(11))
    ADD(LW(0), -1)
    RETURN()
EVT_END()

EVT_BEGIN(irai_gswf_patch)
    USER_FUNC(iraiSetStartFlag, LW(11))
EVT_PATCH_END()

EVT_BEGIN(irai_tenshu_gswf_patch)
    USER_FUNC(iraiClearStartFlag, LW(11))
EVT_PATCH_END()

EVT_BEGIN(irai_cancel_check_hook)
    RUN_CHILD_EVT(irai_cancel_check_evt)
    IF_EQUAL(LW(15), 1)
        USER_FUNC(gor_irai::gor_irai_end_func)
        USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
        RETURN()
    END_IF()
    SET_READ(&gor_irai_msg_exposition)
EVT_PATCH_END()
// clang-format on

void ApplyGorMiscPatches()
{
    gor_christine_nakama[707] = EVT_HELPER_CMD(2, 50);
    gor_christine_nakama[708] = EVT_HELPER_OP(LW(3));
    patch::writePatch(&gor_christine_nakama[710], party_evt, sizeof(party_evt));

    gor_luigi_gor_first_talk[18] = GSW(1708);
    gor_luigi_gor_first_talk[19] = 17;
    gor_luigi_gor_first_talk[44] = GSW(1700);
    gor_luigi_gor_first_talk[46] = 1;
    gor_luigi_gor_first_talk[47] = 1;
    gor_luigi_gor_first_talk[52] = 2;
    gor_luigi_gor_first_talk[53] = 2;
    gor_luigi_gor_first_talk[58] = 3;
    gor_luigi_gor_first_talk[59] = 3;
    gor_luigi_gor_first_talk[64] = 4;
    gor_luigi_gor_first_talk[65] = 4;
    gor_luigi_gor_first_talk[70] = 5;
    gor_luigi_gor_first_talk[71] = 5;
    gor_luigi_gor_first_talk[76] = 6;
    gor_luigi_gor_first_talk[77] = 6;
    gor_luigi_gor_first_talk[82] = 7;
    gor_luigi_gor_first_talk[83] = 7;
    gor_luigi_gor_first_talk[88] = 8;

    gor_10_init_evt[19] = GSW(1700);

    gor_11_init_evt[19] = GSW(1708);
    gor_11_init_evt[20] = 18;
    gor_12_init_evt[24] = GSW(1708);
    gor_12_init_evt[25] = 18;

    patch::writePatch(&gor_irai_main[39], irai_cancel_check_hook, sizeof(irai_cancel_check_hook));
    patch::writePatch(&gor_irai_main[106], irai_gswf_patch, sizeof(irai_gswf_patch));

    patch::writePatch(&gor_irai_tenshu_main[215], irai_tenshu_gswf_patch, sizeof(irai_tenshu_gswf_patch));

    patch::writeBranchPair(&gor_irai_init_func[43],
                           reinterpret_cast<void *>(bIraiCmp1),
                           reinterpret_cast<void *>(bIraiCmp1Return));
    patch::writeBranchPair(&gor_irai_init_func[56],
                           reinterpret_cast<void *>(bIraiCmp2),
                           reinterpret_cast<void *>(bIraiCmp2Return));
    patch::writeBranchPair(&gor_irai_init_func[64],
                           reinterpret_cast<void *>(bIraiCmp3),
                           reinterpret_cast<void *>(bIraiCmp3Return));
    patch::writeBranchPair(&gor_irai_init_func[72],
                           reinterpret_cast<void *>(bIraiCmp4),
                           reinterpret_cast<void *>(bIraiCmp4Return));
    patch::writeBranchPair(&gor_irai_init_func[80],
                           reinterpret_cast<void *>(bIraiCmp5),
                           reinterpret_cast<void *>(bIraiCmp5Return));
    patch::writeBranchPair(&gor_irai_init_func[86],
                           reinterpret_cast<void *>(bIraiCmp6),
                           reinterpret_cast<void *>(bIraiCmp6Return));
    patch::writeBranchPair(&gor_irai_init_func[90],
                           reinterpret_cast<void *>(bIraiCmp7),
                           reinterpret_cast<void *>(bIraiCmp7Return));
    patch::writeBranchPair(&gor_irai_init_func[98],
                           reinterpret_cast<void *>(bIraiCmp8),
                           reinterpret_cast<void *>(bIraiCmp8Return));
    patch::writeBranchPair(&gor_irai_init_func[106],
                           reinterpret_cast<void *>(bIraiCmp9),
                           reinterpret_cast<void *>(bIraiCmp9Return));

    patch::writeBranchPair(&gor_irai_init_func[114],
                           reinterpret_cast<void *>(bIraiCompact),
                           reinterpret_cast<void *>(bIraiCompactReturn));

    patch::writeIntWithCache(&gor_irai_main_func[90], 0x60000000);  // NOP

    patch::writeBranchPair(&gor_irai_main_func[95],
                           &gor_irai_main_func[97],
                           reinterpret_cast<void *>(bIraiMainCompleteCheck),
                           reinterpret_cast<void *>(bIraiMainCompleteCheckReturn));

    patch::writeIntWithCache(&gor_irai_main_func[97], 0x2C030002); // cmpwi r3, 0x2
    patch::writeIntWithCache(&gor_irai_main_func[98], 0x41820018); // beq +0x18

    patch::writeBranchPair(&gor_irai_window_disp[415],
                           &gor_irai_window_disp[417],
                           reinterpret_cast<void *>(bIraiRenderGreyCheck),
                           reinterpret_cast<void *>(bIraiRenderGreyCheckReturn));

    patch::writeIntWithCache(&gor_irai_window_disp[417], 0x2C030002); // cmpwi r3, 0x2
    patch::writeIntWithCache(&gor_irai_window_disp[418], 0x4082001C); // bne +0x1C

    patch::writeBranchPair(&gor_irai_window_disp[430],
                           &gor_irai_window_disp[433],
                           reinterpret_cast<void *>(bIraiRenderRedCheck),
                           reinterpret_cast<void *>(bIraiRenderRedCheckReturn));

    patch::writeBranchBL(&gor_keijiban_data_make[11], reinterpret_cast<void *>(bJohoyaSeqAddition));

    
    for (int i = 0; i < 8; i++)
    {
        gor_follow[i].story_threshold = new_thresholds[i];
    }

    patch::writeBranchPair(&gor_monosiri_check[14],
                           reinterpret_cast<void *>(bMonosiriBucket),
                           reinterpret_cast<void *>(bMonosiriBucketReturn));

    patch::writeIntWithCache(&gor_monosiri_check[16], 0x41800068); // blt +0x68
    patch::writeIntWithCache(&gor_monosiri_check[19], 0x60000000); // NOP
    patch::writeIntWithCache(&gor_monosiri_check[21], 0x60000000); // NOP
    patch::writeIntWithCache(&gor_monosiri_check[33], 0x60000000); // NOP
    patch::writeIntWithCache(&gor_monosiri_check[41], 0x60000000); // NOP
    patch::writeIntWithCache(&gor_monosiri_check[44], 0x281C0017); // cmplwi r28, 0x17

    patch::writeBranchBL(&gor_monosiri_check[51], reinterpret_cast<void *>(bMonosiriBucketExtra));
    patch::writeBranchBL(&gor_monosiri_check[69], reinterpret_cast<void *>(bMonosiriBucketExtra));
    patch::writeBranchBL(&gor_monosiri_check[87], reinterpret_cast<void *>(bMonosiriBucketExtra));
}
