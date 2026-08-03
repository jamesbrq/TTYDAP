#include <ttyd/battle_unit.h>

namespace ttyd::battle_unit
{
    BattleStatRelValues g_battleStats[] = {{AJI, 9, 28},
                                           {BOM, 9, 26},
                                           {DOU, 8, 10},
                                           {EKI, 8, 23},
                                           {GON, 4, 8},
                                           {GRA, 6, 15},
                                           {HEI, 4, 7},
                                           {JIN, 6, 16},
                                           {LAS, 10, 30},
                                           {MOO, 9, 28},
                                           {MRI, 5, 11},
                                           {MUJ, 7, 20},
                                           {PIK, 8, 24},
                                           {TIK, 2, 6},
                                           {TOU, 6, 13},
                                           {TOU2, 6, 13},
                                           {WIN, 5, 10}};

    const int g_battleStatsCount = sizeof(g_battleStats) / sizeof(g_battleStats[0]);

    struct BossAtkEntry
    {
        int32_t unitType;
        int32_t atk;
    };

    static const BossAtkEntry g_bossAtk[] = {
        {0x06, 1},  {0x07, 3}, {0x08, 1}, {0x14, 1}, {0x17, 5}, {0x21, 1},
        {0x22, 2},  {0x3F, 3}, {0x40, 4}, {0x41, 4}, {0x4C, 4}, {0x4D, 4},
        {0x4F, 4},  {0x5D, 4}, {0x63, 3}, {0x6B, 5}, {0x79, 6}, {0x84, 8},
        {0x87, 6},  {0x90, 7}, {0x92, 7}, {0x94, 7}, {0x95, 7}, {0xAB, 8},
    };

    int32_t GetBossAtk(int32_t unit_type)
    {
        for (const auto &e : g_bossAtk)
            if (e.unitType == unit_type)
                return e.atk;
        return -1;
    }

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
        int origHp[17];
        int origLv[17];
        for (int i = 0; i < count; ++i)
        {
            origHp[i] = g_battleStats[i].base_hp;
            origLv[i] = g_battleStats[i].level;
        }
        int indices[17];
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