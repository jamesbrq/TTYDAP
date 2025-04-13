#include "subrel_aaa.h"
#include "evt_cmd.h"
#include "AP/rel_patch_definitions.h"

#include <cstdint>

extern int32_t aaa_evt_prologue2[];
extern int32_t aaa_epilogue_evt2[];
extern int32_t aaa_00_init_evt[];

namespace mod
{
    void main()
    {
        aaa_evt_prologue2[188] = GSW(1700);
        aaa_evt_prologue2[189] = 2;

        aaa_epilogue_evt2[485] = GSW(1708);
        aaa_epilogue_evt2[486] = 19;

        aaa_00_init_evt[49] = GSW(1708);
        aaa_00_init_evt[50] = 18;
    }

    void exit() {}
} // namespace mod
