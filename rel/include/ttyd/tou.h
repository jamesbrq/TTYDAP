#pragma once

#include <cstdint>

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
        int count;             // Number of entries
        RankingEntry *entries; // Array of ranking entries
    };

#define TOU_FLAG_WIN 0x01;      // Bit 0: Win flag
#define TOU_FLAG_INACTIVE 0x02; // Bit 1: Inactive/skip entry
#define TOU_FLAG_STOP 0x04;     // Bit 2: Stop counting flag

    extern "C"
    {
        extern RankingData *tou_rank_wp;

        void tou_rankingControll();
    }
} // namespace ttyd::tou
