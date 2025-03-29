#pragma once

#include <cstdint>

namespace gc
{
    struct vec3
    {
        float x, y, z;
    };

    struct mat3x4
    {
        float a[12];
    };

    struct mat4x4
    {
        float a[16];
    };

    struct color4
    {
        uint8_t r, g, b, a;
    };
} // namespace gc
