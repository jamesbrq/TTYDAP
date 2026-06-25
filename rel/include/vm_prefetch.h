#pragma once
#include <cstdint>

struct OSModuleInfo;

namespace mod::vm
{
    // Build the kind->unit->span table from the linked module's section table.
    // Must be called after Link while the window is still 1:1 mapped (before
    // VM_Persist), because it reads the section table through the window.
    void VM_PrefetchInit(OSModuleInfo *mod);

    // If kindAddr points into the window (an enemy BattleUnitKind), page in that
    // unit's full closure (every REL section span it occupies). No-op for DOL
    // kinds (Mario/party/system) or when the VM is not active. Call from the
    // BtlUnit_Entry hook before the kind is dereferenced.
    void VM_PrefetchForKind(uint32_t kindAddr, bool lock = false);
}
