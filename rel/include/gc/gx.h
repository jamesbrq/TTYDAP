#pragma once

#include <cstdint>

namespace gc::gx
{
    extern "C"
    {
        void GXSetFog(int32_t type, void *fogData);
    }
} // namespace gc::demo
