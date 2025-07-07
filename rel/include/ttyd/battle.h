#pragma once

namespace ttyd::battle
{
    extern "C"
    {
        extern void *_battleWorkPtr;
        void *BattleGetPartyPtr(void *ptr);

    }
} // namespace ttyd::battle