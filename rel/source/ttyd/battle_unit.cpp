#include <ttyd/battle_unit.h>

namespace ttyd::battle_unit
{
    BattleStatRelValues g_battleStats[] = {{AJI, 10, 26},
                                           {BOM, 10, 25},
                                           {DOU, 8, 19},
                                           {EKI, 8, 5},
                                           {GON, 4, 8},
                                           {GRA, 6, 15},
                                           {HEI, 4, 7},
                                           {JIN, 6, 16},
                                           {MOO, 10, 25},
                                           {MRI, 5, 11},
                                           {MUJ, 7, 18},
                                           {TIK, 2, 6},
                                           {TOU, 6, 13},
                                           {TOU2, 6, 13},
                                           {WIN, 5, 10}};

    const int g_battleStatsCount = sizeof(g_battleStats) / sizeof(g_battleStats[0]);

    const BattleStatRelValues *GetBattleStats(RelId rel)
    {
        for (int i = 0; i < g_battleStatsCount; ++i)
        {
            if (g_battleStats[i].rel == rel)
                return &g_battleStats[i];
        }
        return nullptr;
    }

    void ShuffleBattleStats()
    {
        const int count = g_battleStatsCount;
        uint32_t old = ttyd::system::main_next;
        ttyd::system::main_next = *(uint32_t *)0x80003244;
        int origHp[16];
        int origLv[16];
        for (int i = 0; i < count; ++i)
        {
            origHp[i] = g_battleStats[i].base_hp;
            origLv[i] = g_battleStats[i].level;
        }
        int indices[16];
        for (int i = 0; i < count; ++i) indices[i] = i;
        for (int i = count - 1; i > 0; --i)
        {
            int j = irand(i + 1);
            int temp = indices[i];
            indices[i] = indices[j];
            indices[j] = temp;
        }
        for (int i = 0; i < count; ++i)
        {
            g_battleStats[i].base_hp = origHp[indices[i]];
            g_battleStats[i].level = origLv[indices[i]];
        }
        ttyd::system::main_next = old;
    }
} // namespace ttyd::battle_unit