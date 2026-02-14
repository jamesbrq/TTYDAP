#include "AP/rel_patch_definitions.h"
#include "evt_cmd.h"
#include "OWR.h"
#include "patch.h"
#include "subrel_moo.h"
#include "ttyd/battle_unit.h"
#include "ttyd/battle_database_common.h"
#include "ttyd/evt_cam.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_party.h"

#include <cstdint>

using namespace ttyd;
using namespace mod::owr;
using namespace ttyd::battle_unit;
using namespace ttyd::battle_database_common;

extern int32_t moo_evt_first_00[];
extern int32_t moo_00_init_evt[];
extern int32_t moo_dokan_evt_02[];
extern int32_t moo_02_init_evt[];
extern int32_t moo_mario_fly[];
extern int32_t moo_kanbu_fly[];
extern int32_t moo_epilogue_evt[];
extern int32_t moo_04_init_evt[];

// clang-format off
EVT_BEGIN(moo_04_init_evt_evt)
    IF_EQUAL(GSW(1708), 18)
        USER_FUNC(evt_mario::evt_mario_kill_party, 0)
        RUN_EVT(&moo_epilogue_evt)
        SET(LW(4), 1)
        RETURN()
    END_IF()
    SWITCH(GSW(1707))
        CASE_SMALL(15)
            USER_FUNC(evt_cam::evt_cam3d_evt_set, 0, 134, 533, 0, 38, -8, 0, 11)
            RUN_EVT(&moo_mario_fly)
            WAIT_FRM(1)
            SET(LW(4), 1)
            RETURN()
        CASE_END()
        CASE_SMALL(16)
            USER_FUNC(evt_cam::evt_cam3d_evt_set, 20, 200, 533, 20, 104, -8, 0, 11)
            USER_FUNC(evt_mario::evt_mario_cont_onoff, 0)
            USER_FUNC(evt_party::evt_party_cont_onoff, 0, 0)
            USER_FUNC(evt_mario::evt_mario_set_pos, 0, -1000, 0)
            USER_FUNC(evt_party::evt_party_set_pos, 0, 0, -1000, 0)
            WAIT_FRM(1)
            RUN_EVT(&moo_kanbu_fly)
            SET(LW(4), 1)
            RETURN()
        CASE_END()
    END_SWITCH()
    SET(LW(4), 0)
    RETURN()
EVT_END()

EVT_BEGIN(moo_04_init_evt_hook)
    RUN_CHILD_EVT(moo_04_init_evt_evt)
    IF_EQUAL(LW(4), 1)
        RETURN()
    END_IF()
    GOTO(&moo_04_init_evt[74])
EVT_PATCH_END()
// clang-format on

namespace mod
{
    void main()
    {
        moo_evt_first_00[518] = GSW(1707);
        moo_evt_first_00[519] = 6;

        moo_00_init_evt[1] = GSW(1707);
        moo_00_init_evt[3] = 6;
        moo_00_init_evt[52] = GSW(1707);
        moo_00_init_evt[54] = 6;

        moo_dokan_evt_02[1] = GSW(1707);
        moo_dokan_evt_02[2] = 7;

        moo_02_init_evt[49] = GSW(1707);
        moo_02_init_evt[50] = 7;

        moo_04_init_evt[1] = GSW(1708);
        moo_04_init_evt[2] = 18;
        patch::writePatch(&moo_04_init_evt[15], moo_04_init_evt_hook, sizeof(moo_04_init_evt_hook));
        moo_04_init_evt[25] = 0;
        moo_04_init_evt[26] = 0;
        moo_04_init_evt[28] = 0;
        moo_04_init_evt[74] = EVT_HELPER_CMD(0, 49);
        moo_04_init_evt[217] = 0;

        for (int i = kBtlGrpRange_moo_moo.start; i <= kBtlGrpRange_moo_moo.end; i++)
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
