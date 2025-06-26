#pragma once

namespace ttyd::tou
{
    struct RankingEntry
    {
        uint16_t flags;   // +0x00: Flags
        uint16_t padding; // +0x02: Padding
        int player_id;    // +0x04: Player ID
        int16_t score;    // +0x08: Score value
        int16_t padding2; // +0x0A: Padding
    };

    struct RankingData
    {
        int count;  // Number of entries
        RankingEntry *entries; // Array of ranking entries
    };

    const uint16_t FLAG_WIN = 0x01;      // Bit 0: Win flag
    const uint16_t FLAG_INACTIVE = 0x02; // Bit 1: Inactive/skip entry
    const uint16_t FLAG_STOP = 0x04;     // Bit 2: Stop counting flag

    #define tou_rank_wp (*(RankingData **)0x805cc0d0)

    extern "C"
    {
        void tou_rankingControll();
    }
    } // namespace ttyd::tou
