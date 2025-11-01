#include "AP/rel_patch_definitions.h"
#include "evt_cmd.h"
#include "patch.h"
#include "subrel_end.h"

#include <cstdint>

extern int32_t unk_evt_end_00004654[];
extern int32_t unk_evt_end_00004718[];

// clang-format off
EVT_BEGIN(end_credits_evt)
    IF_LARGE_EQUAL(GSW(1708), 18)
        SET(GSW(1708), 20)
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN(end_credits_hook)
    RUN_CHILD_EVT(end_credits_evt)
EVT_PATCH_END()
// clang-format on

namespace mod
{
    void main()
    {
        patch::writePatch(&unk_evt_end_00004654[16], end_credits_hook, sizeof(end_credits_hook));
        unk_evt_end_00004654[18] = 0;

        patch::writePatch(&unk_evt_end_00004718[58], end_credits_hook, sizeof(end_credits_hook));
        unk_evt_end_00004718[60] = 0;
    }

    void exit() {}
} // namespace mod
