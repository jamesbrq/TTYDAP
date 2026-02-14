#include "AP/rel_patch_definitions.h"
#include "common_types.h"
#include "evt_cmd.h"
#include "OWR.h"
#include "patch.h"
#include "subrel_dou.h"
#include "ttyd/battle_unit.h"
#include "ttyd/battle_database_common.h"
#include "ttyd/evt_cam.h"
#include "ttyd/evt_item.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_mobj.h"
#include "ttyd/evt_npc.h"

#include <cstdint>

using namespace ttyd;
using namespace mod::owr;
using namespace ttyd::battle_unit;
using namespace ttyd::battle_database_common;

extern int32_t dou_bero_in_event_00[];
extern int32_t dou_totsunyu_event[];
extern int32_t dou_korutesu_event_01[];
extern int32_t dou_01_init_evt[];
extern int32_t dou_watergate_handle_init[];
extern int32_t dou_watergate_handle[];
extern int32_t dou_03_init_evt[];
extern int32_t dou_bigdoor_event_init[];
extern int32_t dou_bigdoor_event[];
extern int32_t dou_peramajin_before[];
extern int32_t dou_peramajin_after[];
extern int32_t dou_09_init_evt[];
extern int32_t dou_namiyoke_check[];
extern int32_t dou_suifu_c_init_11[];
extern int32_t dou_suifu_c_talk_11[];
extern int32_t dou_suifu_d_init_11[];
extern int32_t dou_suifu_d_talk_11[];
extern int32_t dou_suifu_e_init_11[];
extern int32_t dou_suifu_e_talk_11[];
extern int32_t dou_mony_init_11[];
extern int32_t dou_mony_talk_11[];
extern int32_t dou_peton_init_11[];
extern int32_t dou_peton_talk_11[];
extern int32_t dou_watarikiri_suifu_init[];
extern int32_t dou_suifu_orei[];
extern int32_t dou_escapeway_event[];
extern int32_t dou_bombwall_bomb[];
extern int32_t dou_bombwall_init[];
extern int32_t dou_11_init_evt[];

// clang-format off
EVT_BEGIN(dou_peramajin_after_item)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 0)
	USER_FUNC(evt_mobj::evt_mobj_wait_animation_end, PTR("mobj_box00"))
	USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
	USER_FUNC(evt_item::evt_item_entry, PTR("item01"), LW(3), LW(0), LW(1), LW(2), 16, -1, 0)
	USER_FUNC(evt_item::evt_item_get_item, PTR("item01"))
	WAIT_MSEC(800)
	SET(GSWF(6052), 1)
	USER_FUNC(evt_cam::evt_cam3d_evt_off, 500, 11)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
	RETURN()
EVT_END()
// clang-format on

namespace mod
{
    void main()
    {
        dou_bero_in_event_00[1] = GSW(1717);
        dou_bero_in_event_00[2] = 1;

        dou_totsunyu_event[34] = GSW(1717);
        dou_totsunyu_event[35] = 2;

        dou_korutesu_event_01[67] = GSW(1717);
        dou_korutesu_event_01[68] = 3;

        dou_01_init_evt[41] = GSW(1717);
        dou_01_init_evt[42] = 2;

        dou_watergate_handle_init[1] = GSW(1717);
        dou_watergate_handle_init[2] = 5;

        dou_watergate_handle[1] = GSW(1717);
        dou_watergate_handle[2] = 5;
        dou_watergate_handle[224] = GSW(1717);
        dou_watergate_handle[225] = 5;

        dou_03_init_evt[53] = GSWF(6113);
        dou_03_init_evt[54] = 1;
        dou_03_init_evt[69] = GSW(1717);
        dou_03_init_evt[70] = 5;

        dou_bigdoor_event_init[1] = GSWF(6113);
        dou_bigdoor_event_init[2] = 1;

        dou_bigdoor_event[141] = GSWF(6113);
        dou_bigdoor_event[142] = 1;

        dou_peramajin_after[0] = EVT_HELPER_CMD(2, 50);
        dou_peramajin_after[1] = EVT_HELPER_OP(LW(3));
        patch::writePatch(&dou_peramajin_after[3], dou_peramajin_after_item, sizeof(dou_peramajin_after_item));

        dou_09_init_evt[31] = GSW(1717);
        dou_09_init_evt[32] = 5;
        dou_09_init_evt[204] = 12;

        dou_namiyoke_check[1] = GSW(1717);
        dou_namiyoke_check[2] = 5;
        dou_namiyoke_check[4] = GSW(1717);
        dou_namiyoke_check[5] = 6;

        dou_suifu_c_init_11[1] = GSW(1717);
        dou_suifu_c_init_11[3] = 7;
        dou_suifu_c_init_11[4] = 12;
        dou_suifu_c_init_11[13] = GSW(1717);
        dou_suifu_c_init_11[14] = 11;

        dou_suifu_c_talk_11[1] = GSW(1717);
        dou_suifu_c_talk_11[3] = 7;
        dou_suifu_c_talk_11[4] = 11;
        dou_suifu_c_talk_11[12] = 12;

        dou_suifu_d_init_11[1] = GSW(1717);
        dou_suifu_d_init_11[3] = 7;
        dou_suifu_d_init_11[4] = 12;
        dou_suifu_d_init_11[13] = GSW(1717);
        dou_suifu_d_init_11[14] = 11;

        dou_suifu_d_talk_11[1] = GSW(1717);
        dou_suifu_d_talk_11[3] = 7;
        dou_suifu_d_talk_11[4] = 11;
        dou_suifu_d_talk_11[12] = 12;

        dou_suifu_e_init_11[1] = GSW(1717);
        dou_suifu_e_init_11[3] = 7;
        dou_suifu_e_init_11[4] = 12;
        dou_suifu_e_init_11[13] = GSW(1717);
        dou_suifu_e_init_11[14] = 11;

        dou_suifu_e_talk_11[1] = GSW(1717);
        dou_suifu_e_talk_11[3] = 7;
        dou_suifu_e_talk_11[4] = 11;
        dou_suifu_e_talk_11[12] = 12;

        dou_mony_init_11[1] = GSW(1717);
        dou_mony_init_11[3] = 7;
        dou_mony_init_11[4] = 12;

        dou_mony_talk_11[1] = GSW(1717);
        dou_mony_talk_11[3] = 7;
        dou_mony_talk_11[4] = 11;
        dou_mony_talk_11[22] = 12;

        dou_peton_init_11[1] = GSW(1717);
        dou_peton_init_11[3] = 7;
        dou_peton_init_11[4] = 12;

        dou_peton_talk_11[1] = GSW(1717);
        dou_peton_talk_11[3] = 7;
        dou_peton_talk_11[4] = 11;
        dou_peton_talk_11[30] = 12;

        dou_watarikiri_suifu_init[77] = GSW(1717);
        dou_watarikiri_suifu_init[78] = 11;

        dou_suifu_orei[329] = GSW(1717);
        dou_suifu_orei[330] = 7;

        dou_escapeway_event[91] = GSW(1717);
        dou_escapeway_event[92] = 12;

        dou_bombwall_bomb[176] = GSW(1717);
        dou_bombwall_bomb[177] = 13;

        dou_bombwall_init[31] = GSW(1717);
        dou_bombwall_init[32] = 10;
        dou_bombwall_init[58] = 11;
        dou_bombwall_init[59] = 12;
        dou_bombwall_init[101] = 13;

        dou_11_init_evt[42] = GSW(1717);
        dou_11_init_evt[44] = 6;
        dou_11_init_evt[60] = 1;
        dou_11_init_evt[74] = GSW(1717);
        dou_11_init_evt[75] = 6;
        dou_11_init_evt[109] = GSW(1717);
        dou_11_init_evt[110] = 7;
        dou_11_init_evt[112] = GSW(1717);
        dou_11_init_evt[113] = 12;
        dou_11_init_evt[141] = GSW(1717);
        dou_11_init_evt[142] = 10;
        dou_11_init_evt[174] = GSW(1717);
        dou_11_init_evt[175] = 7;
        dou_11_init_evt[291] = GSW(1717);
        dou_11_init_evt[292] = 14;
        dou_11_init_evt[294] = GSW(1717);
        dou_11_init_evt[295] = 15;

        for (int i = kBtlGrpRange_dou_dou.start; i <= kBtlGrpRange_dou_dou.end; i++)
        {
            if (gState->apSettings->enemyRandomizer == 0)
                break;
            BattleGroupSetup *battleGroup = battleGroupList[i];
            EnemyLoadout &loadout = gState->enemyLoadouts[i];
            for (int32_t j = 0; j < battleGroup->num_enemies; j++)
            {
                BattleUnitSetup &unit = battleGroup->enemy_data[j];
                unit.unit_kind_params = GetUnitKindById(loadout.enemyIds[j]);
                unit.position.y = GetEnemyYPosition(loadout.enemyIds[j]);
            }
        }
    }

    void exit() {}
} // namespace mod
