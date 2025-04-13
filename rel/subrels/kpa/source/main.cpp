#include "subrel_kpa.h"
#include "evt_cmd.h"
#include "AP/rel_patch_definitions.h"

#include <cstdint>

extern int32_t kpa_goal_00[];
extern int32_t kpa_00_fly_evt[];
extern int32_t kpa_00_init_evt[];
extern int32_t kpa_01_touzyo_evt[];
extern int32_t kpa_01_init_evt[];
extern int32_t kpa_goal_01[];
extern int32_t kpa_goal_02[];
extern int32_t kpa_07_evt[];
extern int32_t kpa_07_init_evt[];

namespace mod
{
    void main()
    {
        kpa_goal_00[4] = GSW(1713);
        kpa_goal_00[5] = 15;

        kpa_00_fly_evt[335] = GSW(1711);
        kpa_00_fly_evt[336] = 13;

        kpa_00_init_evt[11] = GSW(1711);
        kpa_00_init_evt[13] = 11;
        kpa_00_init_evt[17] = 12;

        kpa_01_touzyo_evt[204] = GSW(1711);
        kpa_01_touzyo_evt[205] = 12;

        kpa_01_init_evt[33] = GSW(1711);
        kpa_01_init_evt[35] = 11;
        kpa_01_init_evt[41] = 12;

        kpa_goal_01[4] = GSW(1706);
        kpa_goal_01[5] = 47;

        kpa_goal_02[4] = GSW(1715);
        kpa_goal_02[5] = 12;

        kpa_07_evt[1180] = GSW(1706);
        kpa_07_evt[1181] = 48;

        kpa_07_init_evt[45] = GSW(1706);
        kpa_07_init_evt[46] = 47;
    }

    void exit() {}
} // namespace mod
