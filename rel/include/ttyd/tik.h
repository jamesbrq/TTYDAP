#pragma once

#include <cstdint>

#include "evtmgr_cmd.h"

namespace ttyd::tik
{
    extern "C"
    {
        EVT_DECLARE_USER_FUNC(unk_tik_000005c0, 0);
        EVT_DECLARE_USER_FUNC(unk_tik_00000598, 0);
    }
} // namespace ttyd::tik