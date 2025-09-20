#pragma once

namespace ttyd::pmario_sound
{
    struct BGMListEntry
    {
        const char *name;     // +0x00 - Pointer to BGM name string
        uint32_t flags;       // +0x04 - Control flags (bit 31 = SS vs Song)
        const char *filename; // +0x08 - Pointer to audio file path
        uint32_t audioParams; // +0x0C - Packed audio parameters
    };

    extern "C"
    {
        int psndBGMOff(int channelFlags);
        int psndBGMOff_f_d(uint32_t bgmSlot, uint16_t fadeTime, uint32_t mode);
        int psndSFXOn(int channelFlags);

        void *psbgm;
    }
}