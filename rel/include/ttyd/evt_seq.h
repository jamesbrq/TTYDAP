#pragma once

#include <cstdint>

#include "evtmgr.h"

namespace ttyd::evt_seq
{
    extern "C"
    {
        EVT_DECLARE_USER_FUNC(evt_seq_wait, 1)
    }
} // namespace ttyd::evt_seq
