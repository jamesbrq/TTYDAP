#pragma once

#include <cstdint>

namespace ttyd::gor_02
{
    struct ChapterBucketList
    {
        uint32_t story_threshold; // Story position threshold for this chapter
        uint32_t *enemy_ids;      // Pointer to array of enemy IDs with missable tattles
        uint32_t enemy_count;     // Number of enemies in this chapter's list
    };

    extern "C"
    {
        extern ChapterBucketList gor_follow[8];
    }
} // namespace ttyd::gor_02