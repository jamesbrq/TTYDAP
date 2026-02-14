#include "subrel_jon.h"
#include "evt_cmd.h"
#include "OWR.h"
#include "patch.h"
#include "AP/rel_patch_definitions.h"
#include "ttyd/battle_unit.h"
#include "ttyd/battle_database_common.h"
#include "ttyd/evt_bero.h"

#include <cstdint>

using namespace ttyd;
using namespace mod::owr;
using namespace ttyd::battle_unit;
using namespace ttyd::battle_database_common;

extern int32_t jon_zonbaba_first_event[];

// clang-format off
EVT_BEGIN(jon_zonbaba_first_event_evt)
    USER_FUNC(evt_bero::evt_bero_mapchange, PTR("end_00"), 0)
    RETURN()
EVT_END()

EVT_BEGIN(jon_zonbaba_first_event_hook)
    RUN_CHILD_EVT(jon_zonbaba_first_event_evt)
EVT_PATCH_END()
// clang-format on

namespace mod
{
    void main()
    {
        if (mod::owr::gState->apSettings->goal == 3)
        {
            patch::writePatch(&jon_zonbaba_first_event[672], jon_zonbaba_first_event_hook, sizeof(jon_zonbaba_first_event_hook));
            jon_zonbaba_first_event[674] = 0;
        }

        for (int i = kBtlGrpRange_jon_jon.start; i <= kBtlGrpRange_jon_jon.end; i++)
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
