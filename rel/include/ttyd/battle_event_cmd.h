#pragma once

#include "evtmgr.h"

namespace ttyd::battle_event_cmd
{
    extern "C"
    {
        EVT_DECLARE_USER_FUNC(btlevtcmd_InviteApInfoReport, 0);
        EVT_DECLARE_USER_FUNC(btlevtcmd_WaitAttackEnd, 0);
        EVT_DECLARE_USER_FUNC(btlevtcmd_GetMaxHp, 2);
        EVT_DECLARE_USER_FUNC(btlevtcmd_GetHp, 2);
        EVT_DECLARE_USER_FUNC(btlevtcmd_SpawnUnit, 3);
        EVT_DECLARE_USER_FUNC(btlevtcmd_SetUnitWork, 3);
    }
} // namespace ttyd::battle_event_cmd