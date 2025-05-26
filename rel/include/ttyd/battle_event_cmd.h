#pragma once

#include "evtmgr.h"

namespace ttyd::battle_event_cmd
{
    extern "C"
    {
        EVT_DECLARE_USER_FUNC(btlevtcmd_InviteApInfoReport, 0);
    }
} // namespace ttyd::battle_event_cmd