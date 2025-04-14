#include "subrel_gor.h"

#include <cstdint>

namespace mod
{
    void main()
    {
        ApplyGor00Patches();
        ApplyGor01Patches();
        ApplyGor02Patches();
        ApplyGor03Patches();
        ApplyGor04Patches();
        ApplyGorMiscPatches();
    }

    void exit() {}
} // namespace mod
