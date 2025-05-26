#pragma once

#include "gc/types.h"

namespace ttyd::mapdrv
{
    extern "C"
    {
        void mapPlayAnimationLv(const char *animName, int loopType, int flags);
    }
} // namespace ttyd::mapdrv
