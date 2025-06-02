#pragma once

#include <ttyd/dispdrv.h>

#include <cstdint>

namespace ttyd::win_root
{
    struct WinPauseMenu;
} // namespace ttyd::win_root

namespace ttyd::win_log
{

    extern "C"
    {
        struct MapMarker
        {
            // 0,0 is at the middle of the map
            int16_t x_pos;
            int16_t y_pos;
            uint8_t isLocation; // trail marker if 0
            uint8_t unk_0x05;   // if required_sequence isn't met, used to conditionally show anyway. can take the values [0-4]
            uint16_t required_sequence;
            const char *map_prefix; // the three letter string E.g. "gor"
        };
        static_assert(sizeof(MapMarker) == 0xC);

        extern MapMarker mapMarkers[0x5D];
    }

} // namespace ttyd::win_log