#pragma once

#include "evtmgr.h"

namespace ttyd::evt_audience
{
    extern "C"
    {
        EVT_DECLARE_USER_FUNC(evt_audience_ap_recovery, 0);
    }
} // namespace ttyd::evt_audience