#pragma once

#include <cstdint>

#include "evtmgr.h"

namespace ttyd::mri_puni
{
    extern "C"
    {
        EVT_DECLARE_USER_FUNC(evt_puni_set_mode, 2);
        EVT_DECLARE_USER_FUNC(evt_puni_flag_onoff, 3);
    }
} // namespace ttyd::mri_puni
