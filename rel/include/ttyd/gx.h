#pragma once

#include <cstdint>

namespace ttyd::gx
{
    extern "C"
    {
        void GXSetFog(int32_t type, void *fogData, float f1, float f2, float f3, float f4);
    }
} // namespace ttyd::gx
