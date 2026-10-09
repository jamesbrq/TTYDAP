#include "BossPreview.h"
#include "subrel_las.h"
#include "evt_cmd.h"
#include "OWR.h"
#include "patch.h"
#include "AP/rel_patch_definitions.h"
#include "ttyd/battle_unit.h"
#include "ttyd/battle_database_common.h"
#include "ttyd/evt_case.h"
#include "ttyd/evt_item.h"
#include "ttyd/evt_map.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_msg.h"
#include "ttyd/evt_npc.h"
#include "ttyd/evtmgr_cmd.h"
#include "ttyd/swdrv.h"

#include <cstdint>

using namespace ttyd;
using namespace mod::owr;
using namespace ttyd::battle_unit;
using namespace ttyd::battle_database_common;

extern int32_t las_first_evt_00[];
extern int32_t las_00_init_evt[];
extern int32_t las_senkaron_event[];
extern int32_t las_kurokaron_init[];
extern int32_t las_kurokaron_talk[];
extern int32_t las_key_evt_05[];
extern int32_t las_unlock_evt_05[];
extern int32_t las_05_init_evt[];
extern int32_t las_door_matigai_evt[];
extern int32_t las_door_seikai_evt[];
extern int32_t las_mugen_kairou_init[];
extern int32_t las_first_evt_09[];
extern int32_t las_majyorin_evt_main[];
extern int32_t las_09_init_evt[];
extern int32_t las_tenkyugi_evt2[];
extern int32_t las_kagi_8_on[];
extern int32_t las_kagi_8_init[];
extern int32_t las_daiza_evt[];
extern int32_t las_10_init_evt[];
extern int32_t las_cloud_evt[];
extern int32_t las_19_init_evt[];
extern int32_t las_hosi_sw_check[];
extern int32_t las_hosi_init[];
extern int32_t las_21_init_evt[];
extern int32_t las_key_evt_22[];
extern int32_t las_unlock_evt_22[];
extern int32_t las_22_init_evt[];
extern int32_t las_23_init_evt[];
extern int32_t las_break_floor_evt[];
extern int32_t las_box_evt[];
extern int32_t las_24_init_evt[];
extern int32_t las_unlock_evt_25[];
extern int32_t las_25_init_evt[];
extern int32_t las_bonbaba_evt[];
extern int32_t las_bonbaba_init[];
extern int32_t las_26_init_evt[];
extern int32_t las_syuryo_init[];
extern int32_t las_first_evt_28[];
extern int32_t las_syuryo_evt[];
extern int32_t las_koopa_evt[];
extern int32_t las_shuryolight_init_28[];
extern int32_t las_28_init_evt[];
extern int32_t las_last_evt_3[];
extern int32_t las_last_evt_3_1[];
extern int32_t las_last_evt_3_2[];
extern int32_t las_last_evt_4[];
extern int32_t las_shuryolight_init_29[];
extern int32_t las_29_init_evt[];
extern int32_t las_bero_entry_data_30[];
extern int32_t las_30_init_evt[];
extern int32_t las_key_check_evt_22[];
extern int32_t las_key_check_evt_25[];
extern int32_t las_key_tbl_05[];
extern int32_t las_key_tbl_22[];
extern int32_t las_key_tbl_25[];

// clang-format off
EVT_BEGIN(stairs_revert)
    IF_LARGE_EQUAL(GSW(1708), 8)
        USER_FUNC(evt_mario::evt_mario_key_onoff, 0)
        IF_EQUAL(GSWF(6112), 1)
            USER_FUNC(evt_msg::evt_msg_print, 0, PTR("raise_text"), 0, 0)
            USER_FUNC(evt_msg::evt_msg_select, 0, PTR("raise_text_yn"))
            IF_EQUAL(LW(0), 0)
                SET(GSWF(6112), 0)
                USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("raise_text2"))
            ELSE()
                USER_FUNC(evt_msg::evt_msg_continue)
            END_IF()
        ELSE()
            USER_FUNC(evt_msg::evt_msg_print, 0, PTR("lower_text"), 0, 0)
            USER_FUNC(evt_msg::evt_msg_select, 0, PTR("lower_text_yn"))
            IF_EQUAL(LW(0), 0)
                SET(GSWF(6112), 1)
                USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("lower_text2"))
            ELSE()
                USER_FUNC(evt_msg::evt_msg_continue)
            END_IF()
        END_IF()
        USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN(las_10_init_evt_evt)
    USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 1, PTR("hosi_off"), 1)
    USER_FUNC(evt_case::evt_run_case_evt, 9, 1, PTR("A_daiza1"), 0, PTR(&stairs_revert), 0)
    RETURN()
EVT_END()

EVT_BEGIN(las_10_init_evt_hook)
    RUN_CHILD_EVT(las_10_init_evt_evt)
    GOTO(&las_10_init_evt[85])
EVT_PATCH_END()

EVT_BEGIN(tenkyugi_evt2_evt)
    USER_FUNC(evt_msg::evt_msg_print_party, PTR("stg8_las_35"))
    SET(GSW(1708), 8)
    SET(GSWF(6112), 1)
    RETURN()
EVT_END()

EVT_BEGIN(tenkyugi_evt2_hook)
    RUN_CHILD_EVT(tenkyugi_evt2_evt)
    GOTO(&las_tenkyugi_evt2[113])
EVT_PATCH_END()

EVT_BEGIN(las_05_init_evt_evt)
    USER_FUNC(evt_item::evt_item_entry, PTR("key"), 46, 0, 0, 0, 0, -1, PTR(&las_key_evt_05))
    RETURN()
EVT_END()

EVT_BEGIN(las_05_init_evt_hook)
    IF_EQUAL(GSWF(6123), 1)
        IF_EQUAL(GSWF(6071), 0)
            RUN_CHILD_EVT(las_05_init_evt_evt)
        END_IF()
    END_IF()
EVT_PATCH_END()
// clang-format on

EVT_DECLARE_USER_FUNC(sqPhase1BattleResult, 1)
EVT_DEFINE_USER_FUNC(sqPhase1BattleResult)
{
    const int32_t ret = ttyd::evt_npc::evt_npc_get_battle_result(evt, isFirstCall);
    if (ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]) == 1)
        ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 5);
    return ret;
}

namespace mod
{
    static int32_t GrodusDialogue(evtmgr::EvtEntry *evt, bool firstCall)
    {
        if (firstCall) boss_preview::SetPalaceEncounter(12);
        return boss_preview::SceneDialogue(evt, firstCall);
    }

    static int32_t BowserReveal(evtmgr::EvtEntry *evt, bool firstCall)
    {
        if (firstCall) boss_preview::SetPalaceEncounter(11);
        return evt_npc::evt_npc_set_position(evt, firstCall);
    }

    static int32_t Queen1Reveal(evtmgr::EvtEntry *evt, bool firstCall)
    {
        if (firstCall) boss_preview::SetPalaceEncounter(13);
        return evt_npc::evt_npc_set_position(evt, firstCall);
    }

    static int32_t Queen2Reveal(evtmgr::EvtEntry *evt, bool firstCall)
    {
        if (firstCall) boss_preview::SetPalaceEncounter(14);
        return evt_npc::evt_npc_set_position(evt, firstCall);
    }

    static int32_t QueenDefeated(evtmgr::EvtEntry *evt, bool firstCall)
    {
        if (firstCall) boss_preview::SetPalaceEncounter(-1);
        return evt_npc::evt_npc_set_position(evt, firstCall);
    }

    void main()
    {
        las_first_evt_00[160] = GSW(1708);
        las_first_evt_00[161] = 2;

        las_00_init_evt[17] = GSW(1708);
        las_00_init_evt[18] = 2;
        las_00_init_evt[23] = GSW(1708);
        las_00_init_evt[24] = 2;

        las_senkaron_event[1] = GSWF(6123);
        las_senkaron_event[2] = 0;
        las_senkaron_event[843] = GSWF(6123);
        las_senkaron_event[844] = 1;

        las_kurokaron_init[1] = GSWF(6123);
        las_kurokaron_init[2] = 0;

        las_kurokaron_talk[3] = GSWF(6123);
        las_kurokaron_talk[4] = 0;

        las_key_evt_05[1] = GSWF(6071);
        las_key_evt_05[2] = 1;

        las_unlock_evt_05[1] = GSW(1708);
        las_unlock_evt_05[2] = 4;

        las_05_init_evt[19] = GSWF(6123);
        las_05_init_evt[20] = 0;
        patch::writePatch(&las_05_init_evt[25], las_05_init_evt_hook, sizeof(las_05_init_evt_hook));
        las_05_init_evt[36] = 0;
        las_05_init_evt[37] = 0;
        las_05_init_evt[38] = 0;
        las_05_init_evt[40] = GSW(1708);
        las_05_init_evt[41] = 4;

        las_door_matigai_evt[1] = GSW(1708);
        las_door_matigai_evt[2] = 4;

        las_door_seikai_evt[1] = GSW(1708);
        las_door_seikai_evt[2] = 4;
        las_door_seikai_evt[10] = GSW(1708);
        las_door_seikai_evt[11] = 5;

        las_mugen_kairou_init[3] = GSW(1708);
        las_mugen_kairou_init[4] = 4;

        las_first_evt_09[195] = GSW(1708);
        las_first_evt_09[196] = 6;

        las_majyorin_evt_main[54] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_majyorin_evt_main[65] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_majyorin_evt_main[135] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_majyorin_evt_main[158] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_majyorin_evt_main[172] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_majyorin_evt_main[238] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_majyorin_evt_main[247] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_majyorin_evt_main[253] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_majyorin_evt_main[259] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_majyorin_evt_main[273] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_majyorin_evt_main[279] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_majyorin_evt_main[306] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_majyorin_evt_main[318] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_majyorin_evt_main[324] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_majyorin_evt_main[330] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_majyorin_evt_main[336] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_bonbaba_evt[102] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_bonbaba_evt[211] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_bonbaba_evt[241] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_bonbaba_evt[271] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_bonbaba_evt[301] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_bonbaba_evt[313] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_bonbaba_evt[343] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_bonbaba_evt[349] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_syuryo_evt[83] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_syuryo_evt[89] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_syuryo_evt[107] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_syuryo_evt[113] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_syuryo_evt[127] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_syuryo_evt[168] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_syuryo_evt[182] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_syuryo_evt[188] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_syuryo_evt[208] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_syuryo_evt[234] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_syuryo_evt[248] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_syuryo_evt[254] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_syuryo_evt[268] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_syuryo_evt[296] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_syuryo_evt[310] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_syuryo_evt[372] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_syuryo_evt[395] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_syuryo_evt[401] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_koopa_evt[167] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_koopa_evt[625] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_koopa_evt[642] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_koopa_evt[672] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_koopa_evt[700] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_koopa_evt[951] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_koopa_evt[963] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_koopa_evt[1060] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_koopa_evt[1072] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_koopa_evt[1121] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_koopa_evt[1135] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_koopa_evt[1141] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3[90] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3[170] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3[213] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3[234] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3[577] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3[734] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3[752] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3[885] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3[904] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3[932] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3[958] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3[1000] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3[1055] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3[1078] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3[1112] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3[1129] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3[1176] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3[1228] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3[1238] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3[1261] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3[1278] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_1[91] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_1[134] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3_1[187] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_1[201] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3_2[84] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_2[420] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_2[437] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3_2[531] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_2[608] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3_2[672] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_2[689] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3_2[753] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_2[770] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3_2[867] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_2[882] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3_2[1149] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_2[1251] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3_2[1257] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_2[1623] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_2[1638] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3_2[1721] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_2[1738] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3_2[1744] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_last_evt_3_2[1759] = reinterpret_cast<int32_t>(boss_preview::SceneDialogue);
        las_last_evt_3_2[1765] = reinterpret_cast<int32_t>(boss_preview::SceneCamera);
        las_syuryo_evt[83] = reinterpret_cast<int32_t>(GrodusDialogue);
        las_koopa_evt[152] = reinterpret_cast<int32_t>(BowserReveal);
        las_last_evt_3[45] = reinterpret_cast<int32_t>(Queen1Reveal);
        las_last_evt_3_1[45] = reinterpret_cast<int32_t>(Queen2Reveal);
        las_last_evt_3_2[38] = reinterpret_cast<int32_t>(Queen2Reveal);
        las_last_evt_4[31] = reinterpret_cast<int32_t>(QueenDefeated);

        las_majyorin_evt_main[632] = GSW(1708);
        las_majyorin_evt_main[633] = 9;

        las_09_init_evt[16] = GSW(1708);
        las_09_init_evt[17] = 5;
        las_09_init_evt[52] = GSW(1708);
        las_09_init_evt[53] = 5;
        las_09_init_evt[202] = GSW(1708);
        las_09_init_evt[203] = 8;
        las_09_init_evt[249] = GSWF(6072);
        las_09_init_evt[250] = 1;

        patch::writePatch(&las_tenkyugi_evt2[107], tenkyugi_evt2_hook, sizeof(tenkyugi_evt2_hook));
        las_tenkyugi_evt2[111] = 0;
        las_tenkyugi_evt2[112] = 0;

        las_kagi_8_on[1] = GSW(1708);
        las_kagi_8_on[2] = 7;

        las_kagi_8_init[1] = GSW(1708);
        las_kagi_8_init[2] = 7;

        las_daiza_evt[48] = GSW(1708);
        las_daiza_evt[49] = 7;

        patch::writePatch(&las_10_init_evt[78], las_10_init_evt_hook, sizeof(las_10_init_evt_hook));
        las_10_init_evt[82] = 0;
        las_10_init_evt[83] = 0;
        las_10_init_evt[1] = GSW(1708);
        las_10_init_evt[2] = 7;
        las_10_init_evt[61] = GSW(1708);
        las_10_init_evt[62] = 7;
        las_10_init_evt[88] = GSW(1708);
        las_10_init_evt[89] = 7;
        las_10_init_evt[128] = GSW(1708);
        las_10_init_evt[129] = 8;

        las_cloud_evt[1] = GSW(1708);
        las_cloud_evt[2] = 10;

        las_19_init_evt[11] = GSW(1708);
        las_19_init_evt[12] = 8;
        las_19_init_evt[37] = GSW(1708);
        las_19_init_evt[38] = 10;
        las_19_init_evt[76] = GSWF(6112);
        las_19_init_evt[77] = 1;
        las_19_init_evt[144] = GSWF(6112);
        las_19_init_evt[145] = 1;

        las_hosi_sw_check[291] = GSW(1708);
        las_hosi_sw_check[292] = 12;

        las_hosi_init[1] = GSW(1708);
        las_hosi_init[2] = 12;

        las_21_init_evt[1] = GSW(1708);
        las_21_init_evt[2] = 8;
        las_21_init_evt[28] = GSWF(6112);
        las_21_init_evt[29] = 1;
        las_21_init_evt[77] = GSWF(6112);
        las_21_init_evt[78] = 1;

        las_key_evt_22[1] = GSWF(6073);
        las_key_evt_22[2] = 1;

        las_unlock_evt_22[1] = GSW(1708);
        las_unlock_evt_22[2] = 11;

        las_key_check_evt_22[2] = 46;

        las_22_init_evt[19] = GSW(1708);
        las_22_init_evt[20] = 8;
        las_22_init_evt[26] = GSW(1708);
        las_22_init_evt[27] = 12;
        las_22_init_evt[115] = GSWF(6073);
        las_22_init_evt[116] = 1;
        las_22_init_evt[129] = GSW(1708);
        las_22_init_evt[130] = 11;
        las_22_init_evt[134] = 46;

        las_23_init_evt[1] = GSW(1708);
        las_23_init_evt[2] = 8;
        las_23_init_evt[28] = GSWF(6112);
        las_23_init_evt[29] = 1;

        las_break_floor_evt[1] = GSW(1708);
        las_break_floor_evt[2] = 13;

        las_box_evt[26] = GSWF(6074);
        las_box_evt[27] = 1;

        las_24_init_evt[44] = GSW(1708);
        las_24_init_evt[45] = 13;

        las_key_check_evt_25[2] = 46;

        las_unlock_evt_25[1] = GSW(1708);
        las_unlock_evt_25[2] = 14;

        las_25_init_evt[1] = GSW(1708);
        las_25_init_evt[2] = 8;
        las_25_init_evt[28] = GSWF(6112);
        las_25_init_evt[29] = 1;
        las_25_init_evt[51] = GSWF(6112);
        las_25_init_evt[52] = 1;
        las_25_init_evt[112] = 46;

        las_bonbaba_evt[795] = GSWF(6072);
        las_bonbaba_evt[796] = 1;

        las_bonbaba_init[1] = GSWF(6072);
        las_bonbaba_init[3] = 0;
        las_bonbaba_init[25] = 1;

        las_26_init_evt[1] = GSWF(6072);
        las_26_init_evt[2] = 0;
        las_26_init_evt[15] = GSW(1708);
        las_26_init_evt[16] = 16;
        las_26_init_evt[84] = GSW(1708);
        las_26_init_evt[85] = 16;
        las_26_init_evt[111] = GSWF(6072);
        las_26_init_evt[112] = 0;
        las_26_init_evt[120] = GSWF(6072);
        las_26_init_evt[122] = 0;
        las_26_init_evt[129] = 1;
        las_26_init_evt[153] = GSWF(6072);
        las_26_init_evt[154] = 1;
        las_26_init_evt[167] = GSW(1708);
        las_26_init_evt[168] = 16;

        las_syuryo_init[6] = GSW(1708);
        las_syuryo_init[7] = 14;
        las_syuryo_init[9] = GSW(1708);
        las_syuryo_init[10] = 15;

        las_first_evt_28[152] = GSW(1708);
        las_first_evt_28[153] = 15;

        las_koopa_evt[1661] = GSW(1708);
        las_koopa_evt[1662] = 16;

        las_shuryolight_init_28[1] = GSW(1708);
        las_shuryolight_init_28[2] = 14;
        las_shuryolight_init_28[4] = GSW(1708);
        las_shuryolight_init_28[5] = 16;

        las_28_init_evt[1] = GSW(1708);
        las_28_init_evt[2] = 16;
        las_28_init_evt[28] = GSW(1708);
        las_28_init_evt[29] = 14;
        las_28_init_evt[43] = GSW(1708);
        las_28_init_evt[44] = 16;
        las_28_init_evt[93] = GSW(1708);
        las_28_init_evt[94] = 14;
        las_28_init_evt[99] = GSW(1708);
        las_28_init_evt[100] = 15;

        las_last_evt_3[1339] = GSW(1708);
        las_last_evt_3[1340] = 17;

        las_last_evt_3[1330] = reinterpret_cast<int32_t>(&sqPhase1BattleResult);

        las_last_evt_3_2[1822] = GSW(1708);
        las_last_evt_3_2[1823] = 17;

        las_last_evt_3_2[137] = 5;
        las_last_evt_3_2[181] = 400;

        las_last_evt_4[1821] = GSW(1708);
        las_last_evt_4[1822] = 18;

        las_shuryolight_init_29[1] = GSW(1708);
        las_shuryolight_init_29[2] = 16;

        las_29_init_evt[11] = GSW(1708);
        las_29_init_evt[13] = 16;
        las_29_init_evt[121] = 17;

        las_30_init_evt[1] = GSW(1708);
        las_30_init_evt[2] = 16;

        las_key_tbl_05[0] = 46;
        las_key_tbl_22[0] = 46;
        las_key_tbl_25[0] = 46;

        if (mod::owr::gState->apSettings->cutsceneSkip)
        {
            las_bero_entry_data_30[26] = PTR("sekai_yami2");
            las_last_evt_3_1[517] = PTR("las_29");
            las_last_evt_3_1[518] = PTR("minnnanokoe");
        }

        // Skip the Rogueport epilogue after the final boss: the ending event's
        // evt_bero_mapchange goes straight to the credits instead of gor_11.
        // Word index byte-verified: cmd at last_evt_4+0x1C98 (callc, funcptr,
        // "gor_11", 0) -> map string is word 1832.
        if (mod::owr::gState->apSettings->epilogueSkip)
            las_last_evt_4[1832] = PTR("end_00");

        ApplyEnemyGroups(battleGroupList, kBtlGrpRange_las_las);

        ApplyBossGroups(kBossGrpRange_las_las);
    }

    void exit() {}
} // namespace mod
