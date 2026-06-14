#include <cstdint>

// Stage-0 MMU safety probe (removed). The window's BAT-free placement is now
// verified at runtime by verifyWindowBatFree() in vm_ttyd.cpp.

namespace mod::vmprobe
{
    void Probe()
    {
        // Stage-0 MMU safety probe removed: it only surfaced read-only
        // diagnostics to low RAM (0x80003C00) for the memory editor during
        // bring-up. The window placement it validated is now verified at
        // runtime by verifyWindowBatFree() in vm_ttyd.cpp.
    }
}
