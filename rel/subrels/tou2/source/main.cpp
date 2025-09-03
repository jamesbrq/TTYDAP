#include "AP/rel_patch_definitions.h"
#include "evt_cmd.h"
#include "OWR.h"
#include "patch.h"
#include "subrel_tou2.h"
#include "ttyd/evt_npc.h"
#include "ttyd/evtmgr_cmd.h"

#include <cstdint>

using namespace ttyd;
using namespace mod;

extern int32_t tou2_init_kinosikowa[];
extern int32_t tou2_npc_entry[];
extern int32_t tou2_evt_match[];
extern int32_t tou2_evt_kurikuri_lose[];
extern int32_t tou2_evt_shinemon_lose[];
extern int32_t tou2_evt_champ_win[];
extern int32_t tou2_evt_champ_lose[];
extern int32_t tou2_evt_default_opening[];
extern int32_t tou2_evt_default_lose[];
extern int32_t tou2_evt_boss[];
extern int32_t tou2_evt_watch_match[];
extern int32_t tou2_evt_kyaku_1[];
extern int32_t tou2_evt_kyaku_2[];
extern int32_t tou2_evt_kyaku_3[];
extern int32_t tou2_evt_kyaku_4[];
extern int32_t tou2_evt_kyaku_5[];
extern int32_t tou2_evt_kyaku_6[];
extern int32_t tou2_evt_kyaku_7[];
extern int32_t tou2_evt_kyaku_8[];
extern int32_t tou_03_init_evt[];
extern int32_t tou2_entry_event[];
extern int32_t tou2_dead_event[];
extern int32_t tou2_phase_event[];

// Assembly
extern int32_t tou2_disp_proc[];
extern int32_t tou2_evt_tou_get_fighter_battle_msg[];
extern int32_t tou2_evt_tou_get_fighter_info[];
extern int32_t tou2_evt_tou_get_fighter_name2[];
extern int32_t tou2_evt_tou_get_rule_info[];
extern int32_t tou2_evt_tou_get_rule_msg2[];
extern int32_t tou2_rankingInit[];
extern int32_t tou2_screen_init[];

EVT_DECLARE_USER_FUNC(tou2_condtions_check, 1)
EVT_DEFINE_USER_FUNC(tou2_condtions_check) 
{
    (void)isFirstCall;
    evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], owr::gState->apSettings->touConditions);
    return 2;
}

// clang-format off
EVT_BEGIN(tou2_evt_match_evt)
    USER_FUNC(tou2_condtions_check, LW(0))
    IF_EQUAL(LW(0), 1)
        USER_FUNC(evt_npc::evt_npc_get_battle_result, LW(0))
        IF_EQUAL(LW(0), 1)
            SET(GSWF(2443), 1)
        END_IF()
    END_IF()
    SET(GSWF(2388), 0)
    RETURN()
EVT_END()


EVT_BEGIN(tou2_evt_match_hook)
    RUN_CHILD_EVT(tou2_evt_match_evt)
EVT_PATCH_END()
// clang-format on


namespace mod
{
    void main()
    {
        tou2_init_kinosikowa[1] = GSW(1703);
        tou2_init_kinosikowa[2] = 28;

        tou2_npc_entry[1] = GSW(1703);
        tou2_npc_entry[2] = 2;
        tou2_npc_entry[55] = GSW(1703);
        tou2_npc_entry[57] = 2;
        tou2_npc_entry[58] = 18;
        tou2_npc_entry[63] = 20;

        tou2_evt_match[55] = GSW(1703);
        tou2_evt_match[56] = 28;
        patch::writePatch(&tou2_evt_match[113], tou2_evt_match_hook, sizeof(tou2_evt_match_hook));
        tou2_evt_match[139] = GSW(1703);
        tou2_evt_match[140] = 28;
        tou2_evt_match[235] = GSW(1703);
        tou2_evt_match[236] = 28;
        tou2_evt_match[238] = GSW(1703);
        tou2_evt_match[239] = 14;
        tou2_evt_match[245] = GSW(1703);
        tou2_evt_match[246] = 12;
        tou2_evt_match[334] = GSW(1703);
        tou2_evt_match[335] = 7;
        tou2_evt_match[337] = GSW(1703);
        tou2_evt_match[338] = 7;
        tou2_evt_match[341] = GSW(1703);
        tou2_evt_match[342] = 28;

        tou2_evt_kurikuri_lose[104] = GSW(1703);
        tou2_evt_kurikuri_lose[105] = 28;

        tou2_evt_shinemon_lose[32] = GSW(1703);
        tou2_evt_shinemon_lose[33] = 28;

        tou2_evt_champ_win[4] = GSW(1703);
        tou2_evt_champ_win[5] = 28;
        tou2_evt_champ_win[219] = GSW(1703);
        tou2_evt_champ_win[220] = 28;

        tou2_evt_champ_lose[37] = GSW(1703);
        tou2_evt_champ_lose[38] = 28;
        tou2_evt_champ_lose[72] = GSW(1703);
        tou2_evt_champ_lose[73] = 28;

        tou2_evt_default_opening[7] = GSW(1703);
        tou2_evt_default_opening[8] = 28;
        tou2_evt_default_opening[157] = GSW(1703);
        tou2_evt_default_opening[158] = 14;
        tou2_evt_default_opening[420] = GSW(1703);
        tou2_evt_default_opening[421] = 28;
        tou2_evt_default_opening[438] = GSW(1703);
        tou2_evt_default_opening[439] = 28;
        tou2_evt_default_opening[451] = GSW(1703);
        tou2_evt_default_opening[452] = 28;
        tou2_evt_default_opening[470] = GSW(1703);
        tou2_evt_default_opening[471] = 28;
        tou2_evt_default_opening[518] = GSW(1703);
        tou2_evt_default_opening[519] = 28;
        tou2_evt_default_opening[614] = GSW(1703);
        tou2_evt_default_opening[616] = 12;
        tou2_evt_default_opening[617] = 28;
        tou2_evt_default_opening[622] = 28;
        tou2_evt_default_opening[634] = GSW(1703);
        tou2_evt_default_opening[636] = 28;
        tou2_evt_default_opening[675] = GSW(1703);
        tou2_evt_default_opening[676] = 28;

        tou2_evt_default_lose[49] = GSW(1703);
        tou2_evt_default_lose[50] = 28;
        tou2_evt_default_lose[110] = GSW(1703);
        tou2_evt_default_lose[112] = 12;
        tou2_evt_default_lose[113] = 28;
        tou2_evt_default_lose[118] = 28;

        tou2_evt_boss[1900] = GSW(1703);
        tou2_evt_boss[1901] = 20;

        tou2_evt_watch_match[1] = GSW(1703);
        tou2_evt_watch_match[2] = 28;

        tou2_evt_kyaku_1[11] = GSW(1703);
        tou2_evt_kyaku_1[13] = 5;

        tou2_evt_kyaku_2[11] = GSW(1703);
        tou2_evt_kyaku_2[13] = 5;

        tou2_evt_kyaku_3[11] = GSW(1703);
        tou2_evt_kyaku_3[13] = 5;

        tou2_evt_kyaku_4[11] = GSW(1703);
        tou2_evt_kyaku_4[13] = 5;

        tou2_evt_kyaku_5[11] = GSW(1703);
        tou2_evt_kyaku_5[13] = 5;

        tou2_evt_kyaku_6[11] = GSW(1703);
        tou2_evt_kyaku_6[13] = 5;

        tou2_evt_kyaku_7[11] = GSW(1703);
        tou2_evt_kyaku_7[13] = 5;

        tou2_evt_kyaku_8[11] = GSW(1703);
        tou2_evt_kyaku_8[13] = 5;

        tou_03_init_evt[1] = GSW(1703);
        tou_03_init_evt[3] = 19;
        tou_03_init_evt[52] = GSW(1703);
        tou_03_init_evt[53] = 19;
        tou_03_init_evt[108] = GSW(1703);
        tou_03_init_evt[110] = 2;
        tou_03_init_evt[128] = GSW(1703);
        tou_03_init_evt[130] = 2;
        tou_03_init_evt[142] = 19;
        tou_03_init_evt[178] = GSW(1703);
        tou_03_init_evt[179] = 28;
        tou_03_init_evt[250] = GSW(1703);
        tou_03_init_evt[252] = 2;
        tou_03_init_evt[253] = 18;
        tou_03_init_evt[265] = 20;
        tou_03_init_evt[280] = GSW(1703);
        tou_03_init_evt[281] = 19;

        tou2_entry_event[4] = GSW(1703);
        tou2_entry_event[5] = 14;
        tou2_entry_event[201] = GSW(1703);
        tou2_entry_event[202] = 28;

        tou2_dead_event[25] = GSW(1703);
        tou2_dead_event[26] = 14;

        tou2_phase_event[69] = GSW(1703);
        tou2_phase_event[70] = 14;
        tou2_phase_event[141] = GSW(1703);
        tou2_phase_event[142] = 14;
        tou2_phase_event[219] = GSW(1703);
        tou2_phase_event[220] = 14;
        tou2_phase_event[372] = GSW(1703);
        tou2_phase_event[373] = 14;

        // Assembly
        tou2_disp_proc[18] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou2_disp_proc[20] = 0x2C03001C; // cmpwi r3, 0x1C

        tou2_evt_tou_get_fighter_battle_msg[10] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou2_evt_tou_get_fighter_battle_msg[13] = 0x2C03001C; // cmpwi r3, 0x1C

        tou2_evt_tou_get_fighter_info[10] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou2_evt_tou_get_fighter_info[13] = 0x2C03001C; // cmpwi r3, 0x1C

        tou2_evt_tou_get_fighter_name2[10] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou2_evt_tou_get_fighter_name2[13] = 0x2C03001C; // cmpwi r3, 0x1C

        tou2_evt_tou_get_rule_info[10] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou2_evt_tou_get_rule_info[13] = 0x2C03001C; // cmpwi r3, 0x1C

        tou2_evt_tou_get_rule_msg2[10] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou2_evt_tou_get_rule_msg2[13] = 0x2C03001C; // cmpwi r3, 0x1C

        tou2_rankingInit[12] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou2_rankingInit[14] = 0x2C03001C; // cmpwi r3, 0x1C
        tou2_rankingInit[95] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou2_rankingInit[97] = 0x2C03001C; // cmpwi r3, 0x1C

        tou2_screen_init[72] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou2_screen_init[76] = 0x2C03001C; // cmpwi r3, 0x1C
    }

    void exit() {}
} // namespace mod
