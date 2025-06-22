#pragma once

#include <cstdint>

namespace gc::gx
{
    struct GXColor
    {
        uint8_t r, g, b, a;
    };

    extern "C"
    {
        void GXSetFog(uint8_t type, float startz, float endz, float nearz, float farz, GXColor *color);
    }
} // namespace gc::demo
