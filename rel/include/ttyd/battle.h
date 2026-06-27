#pragma once

namespace ttyd::battle
{
    extern "C"
    {
        extern void *_battleWorkPtr;
        void *BattleGetPartyPtr(void *ptr);
        void *BattleGetUnitPtr(void *ptr, int32_t index);
        void _ExecAllUnitBattleEndEvent();

    }
} // namespace ttyd::battle