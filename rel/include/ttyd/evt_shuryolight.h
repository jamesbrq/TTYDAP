#pragma once

#include <cstdint>

#include "evtmgr.h"

namespace ttyd::evt_shuryolight
{
    extern "C"
    {
        EVT_DECLARE_USER_FUNC(aji_evt_shuryolight_init, 1)
        EVT_DECLARE_USER_FUNC(aji_evt_shuryolight_run_evt, 3)
    }
} // namespace ttyd::evt_shuryolight
