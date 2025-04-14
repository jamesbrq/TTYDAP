#pragma once

#include <cstdint>

namespace ttyd::countdown
{
    extern "C"
    {
        void countDownStop();
        void countDownEnd();
    }
} // namespace ttyd::countdown
