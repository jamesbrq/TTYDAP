#include "subrel_jon.h"
#include "evt_cmd.h"
#include "OWR.h"
#include "patch.h"
#include "AP/rel_patch_definitions.h"
#include "ttyd/evt_bero.h"

#include <cstdint>

using namespace ttyd;

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
    }

    void exit() {}
} // namespace mod
