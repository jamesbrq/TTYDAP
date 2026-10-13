#include <ttyd/battle_unit.h>

namespace ttyd::battle_unit
{
    const BattleStatRelValues g_battleStats[] = {{AJI, 9, 28},
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

    static const BossAtkEntry g_enemyAtk[] = {
        {0x01, 1}, {0x02, 1}, {0x03, 2}, {0x04, 1},
        {0x05, 1}, {0x07, 3}, {0x0B, 4}, {0x0C, 4},
        {0x0E, 2}, {0x0F, 2}, {0x10, 1}, {0x11, 2},
        {0x12, 1}, {0x13, 1}, {0x14, 1}, {0x15, 1},
        {0x16, 3}, {0x18, 2}, {0x19, 2}, {0x1A, 2},
        {0x1B, 2}, {0x1C, 3}, {0x1D, 2}, {0x24, 1},
        {0x25, 2}, {0x26, 2}, {0x27, 3}, {0x28, 2},
        {0x29, 3}, {0x2A, 2}, {0x2B, 2}, {0x2C, 2},
        {0x2D, 3}, {0x2E, 3}, {0x2F, 3}, {0x30, 3},
        {0x31, 4}, {0x33, 4}, {0x35, 4}, {0x37, 6},
        {0x38, 4}, {0x39, 2}, {0x3A, 3}, {0x3B, 5},
        {0x3C, 5}, {0x3D, 4}, {0x3E, 4}, {0x42, 2},
        {0x43, 2}, {0x44, 3}, {0x45, 2}, {0x46, 2},
        {0x47, 3}, {0x48, 3}, {0x49, 3}, {0x4A, 3},
        {0x4B, 3}, {0x54, 3}, {0x55, 4}, {0x56, 3},
        {0x57, 3}, {0x58, 3}, {0x59, 3}, {0x5B, 4},
        {0x5C, 2}, {0x67, 4}, {0x68, 4}, {0x69, 3},
        {0x6A, 5}, {0x70, 4}, {0x71, 5}, {0x72, 5},
        {0x73, 4}, {0x75, 3}, {0x76, 0}, {0x77, 4},
        {0x78, 5}, {0x7B, 4}, {0x7C, 5}, {0x7E, 6},
        {0x7F, 6}, {0x80, 5}, {0x81, 5}, {0x82, 5},
        {0x83, 5}, {0x99, 3}, {0x9A, 3}, {0x9B, 4},
        {0x9C, 4}, {0x9D, 4}, {0x9E, 5}, {0x9F, 5},
        {0xA0, 6}, {0xA1, 6}, {0xA2, 9}, {0xA3, 7},
        {0xA4, 7}, {0xA5, 8}, {0xA6, 8}, {0xA7, 6},
        {0xA8, 4}, {0xA9, 8},
    };

    int32_t GetEnemyAtk(int32_t unit_type)
    {
        for (const auto &e : g_enemyAtk)
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

} // namespace ttyd::battle_unit