#pragma once

#include "ttyd/evtmgr.h"

#include <cstdint>

namespace ttyd::battle
{
    extern "C"
    {
        extern void *_battleWorkPtr;
        void *BattleGetPartyPtr(void *ptr);
        void *BattleGetUnitPtr(void *ptr, int32_t index);
        int32_t BattleTransID(ttyd::evtmgr::EvtEntry *evt, int32_t unit_id);
        void _ExecAllUnitBattleEndEvent();

    }
} // namespace ttyd::battle