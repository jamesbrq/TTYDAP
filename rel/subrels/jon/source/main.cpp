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

extern int32_t jon_talk_idouya[];
extern int32_t jon_iri_12_dokan_in[];
extern int32_t jon_iri_12_enemy_dead_event[];
extern int32_t jon_iri_12_makkino_talk[];
extern int32_t jon_iri_12_makkino_fall_return[];
extern int32_t jon_iri_12_init[];
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
        jon_talk_idouya[86] = GSW(1742);
        jon_talk_idouya[87] = 1;
        jon_talk_idouya[89] = GSW(1772);
        jon_talk_idouya[90] = 2;
        jon_talk_idouya[92] = GSW(1772);
        jon_talk_idouya[93] = 3;

        jon_iri_12_dokan_in[1] = GSW(1742);
        jon_iri_12_dokan_in[2] = 1;
        jon_iri_12_dokan_in[4] = GSW(1772);
        jon_iri_12_dokan_in[5] = 2;
        jon_iri_12_dokan_in[7] = GSW(1772);
        jon_iri_12_dokan_in[8] = 2;

        jon_iri_12_enemy_dead_event[1] = GSW(1742);
        jon_iri_12_enemy_dead_event[2] = 1;
        jon_iri_12_enemy_dead_event[4] = GSW(1772);
        jon_iri_12_enemy_dead_event[5] = 2;
        jon_iri_12_enemy_dead_event[7] = GSW(1772);
        jon_iri_12_enemy_dead_event[8] = 2;

        jon_iri_12_makkino_talk[8] = GSW(1772);
        jon_iri_12_makkino_talk[9] = 1;
        jon_iri_12_makkino_talk[17] = GSW(1772);
        jon_iri_12_makkino_talk[18] = 2;
        jon_iri_12_makkino_talk[36] = GSW(1772);
        jon_iri_12_makkino_talk[37] = 1;

        jon_iri_12_makkino_fall_return[44] = GSW(1772);
        jon_iri_12_makkino_fall_return[45] = 3;
        jon_iri_12_makkino_fall_return[63] = GSW(1772);
        jon_iri_12_makkino_fall_return[64] = 3;

        jon_iri_12_init[1] = GSW(1742);
        jon_iri_12_init[2] = 1;
        jon_iri_12_init[8] = GSW(1772);
        jon_iri_12_init[9] = 1;
        jon_iri_12_init[52] = GSW(1772);
        jon_iri_12_init[53] = 2;
        jon_iri_12_init[55] = GSW(1772);
        jon_iri_12_init[66] = 2;

        jon_zonbaba_first_event[642] = GSW(1742);
        jon_zonbaba_first_event[643] = 1;
        jon_zonbaba_first_event[645] = GSW(1772);
        jon_zonbaba_first_event[646] = 2;
        jon_zonbaba_first_event[648] = GSW(1772);
        jon_zonbaba_first_event[649] = 2;
        jon_zonbaba_first_event[667] = GSW(1772);
        jon_zonbaba_first_event[668] = 3;

        if (mod::owr::gState->apSettings->goal == 3)
        {
            patch::writePatch(&jon_zonbaba_first_event[672], jon_zonbaba_first_event_hook, sizeof(jon_zonbaba_first_event_hook));
            jon_zonbaba_first_event[674] = 0;
        }

        if (gState->apSettings->enemyRandomizer)
        {
            for (int i = kBtlGrpRange_jon_jon.start; i <= kBtlGrpRange_jon_jon.end; i++)
            {
                BattleGroupSetup *battleGroup = battleGroupList[i];
                EnemyLoadout &loadout = gState->enemyLoadouts[i];
                for (int32_t j = 0; j < battleGroup->num_enemies; j++)
                {
                    BattleUnitSetup &unit = battleGroup->enemy_data[j];
                    unit.position.y = GetEnemyYPosition(loadout.enemyIds[j]);
                    unit.unit_kind_params = GetUnitKindById(loadout.enemyIds[j]);
                }
            }
        }
    }

    void exit() {}
} // namespace mod
