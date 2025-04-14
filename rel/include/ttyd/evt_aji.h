#pragma once

#include <cstdint>

#include "evtmgr.h"

namespace ttyd::evt_aji
{
    extern "C"
    {
        EVT_DECLARE_USER_FUNC(aji_peach_evt_toumei, 1)
    }
} // namespace ttyd::evt_aji
