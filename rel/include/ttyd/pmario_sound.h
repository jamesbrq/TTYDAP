#pragma once

namespace ttyd::pmario_sound
{
    extern "C"
    {
        int psndBGMOff(int channelFlags);
        int psndSFXOn(int channelFlags);
    }
}