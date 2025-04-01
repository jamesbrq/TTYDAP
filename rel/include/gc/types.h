#ifndef GC_TYPES_H
#define GC_TYPES_H

#include <cstdint>

namespace gc
{
    typedef int32_t BOOL;
    typedef int32_t OSHeapHandle;

    typedef int64_t OSTime;
    typedef uint32_t OSTick;

    typedef float mtx34[3][4];
    typedef float mtx44[4][4];
    typedef float mtxUnk4[][4];

    struct vec3
    {
        float x, y, z;
    };

    static_assert(sizeof(vec3) == 0xC);

    struct Vec3
    {
        float x, y, z;
    } __attribute__((__packed__));

    static_assert(sizeof(Vec3) == 0xC);

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
#endif

