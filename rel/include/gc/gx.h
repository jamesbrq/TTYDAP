#pragma once

#include <cstdint>

namespace gc::gx
{
    struct GXColor
    {
        uint8_t r, g, b, a;

        GXColor() {}

        GXColor(uint8_t newR, uint8_t newG, uint8_t newB, uint8_t newA)
        {
            this->r = newR;
            this->g = newG;
            this->b = newB;
            this->a = newA;
        }
    };

    extern "C"
    {
        void GXSetFog(uint8_t type, float startz, float endz, float nearz, float farz, GXColor *color);
    }
} // namespace gc::gx
