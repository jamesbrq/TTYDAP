#pragma once

#include "ttyd/dispdrv.h"
#include "ttyd/win_root.h"

#include <cstdint>

namespace ttyd::win_log
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

    extern "C"
    {
        extern MapMarker mapMarkers[93];   // Internally named `tubuDt`
        extern char main_win_log_name[64]; // Internally named `name$763`

        int32_t main_winLogMain(ttyd::win_root::WinPauseMenu *menu);
    }

} // namespace ttyd::win_log