#pragma once

namespace ttyd::battle_audience
{
    extern "C"
    {
        void BattleAudience_SetPresentItemNo(int item_id);
        void BattleAudience_SetPresentTargetUnitId(int unit_id);
        void BattleAudience_SetPresentItemType(int item_type);
    }
}