#include "vm_prefetch.h"
#include "vm_ttyd.h"
#include "vm_unit_spans.h"

#include "gc/OSModule.h"

#include <cstdint>

namespace
{
    constexpr uint32_t PAGE_SIZE = 0x1000;

    // REL section-table entry: byte offset from module base + size. Offset bit 0
    // is the executable flag; mask it off for the address.
    struct OSSectionInfo
    {
        uint32_t offset;
        uint32_t size;
    };

    // CSV section ids we track, in a fixed order. sec5 (the BattleUnitKind data)
    // is index 2, which VM_PrefetchForKind uses for kind->unit identification.
    constexpr int kUsedSecs[4] = {1, 4, 5, 6};

    uint32_t g_secBase[8]; // window VA of each section (0 = absent)
    uint32_t g_secSize[8];
    uint32_t g_winLo = 0;
    uint32_t g_winHi = 0;
    bool g_ready = false;

    struct UnitWin
    {
        uint32_t lo[4];
        uint32_t hi[4];
    };
    UnitWin g_units[mod::vm::kVmUnitCount];

    // After Link, OSModuleInfo offset fields are relocated to absolute window
    // pointers (e.g. sectionInfoOffset reads back as 0x7F00004C, not 0x4C).
    // Relative offsets are always tiny (< image size << window base), so treat
    // any value already at/above the window base as absolute.
    inline uint32_t toWindowAddr(uint32_t v, uint32_t win) { return (v >= win) ? v : (win + v); }
}

namespace mod::vm
{
    void VM_PrefetchInit(OSModuleInfo *mod)
    {
        g_ready = false;
        for (int i = 0; i < 8; i++)
        {
            g_secBase[i] = 0;
            g_secSize[i] = 0;
        }

        const uint32_t win = VM_WindowBase();
        const uint32_t imgBytes = VM_DbgNumVpages() * PAGE_SIZE;
        const uint32_t nsec = mod->numSections;
        const uint32_t secOff = mod->sectionInfoOffset;
        const uint32_t secAddr = toWindowAddr(secOff, win); // relocated -> absolute

        // Guard: never read the section table outside the mapped window.
        if (nsec == 0 || nsec > 32 || secAddr < win || secAddr + 8u * nsec > win + imgBytes)
        {
            return;
        }

        const OSSectionInfo *sec = reinterpret_cast<const OSSectionInfo *>(secAddr);

        for (int k = 0; k < 4; k++)
        {
            const uint32_t s = static_cast<uint32_t>(kUsedSecs[k]);
            if (s < nsec)
            {
                uint32_t raw = sec[s].offset & ~3u; // clear exec flag / align
                if (raw != 0)
                    g_secBase[s] = toWindowAddr(raw, win);
                g_secSize[s] = sec[s].size;
            }
        }

        g_winLo = win;
        g_winHi = win + imgBytes;

        for (int i = 0; i < kVmUnitCount; i++)
        {
            const VmUnitSpans &u = kVmUnits[i];
            const uint32_t off[4] = {u.off1, u.off4, u.off5, u.off6};
            const uint32_t end[4] = {u.end1, u.end4, u.end5, u.end6};
            for (int k = 0; k < 4; k++)
            {
                const uint32_t s = static_cast<uint32_t>(kUsedSecs[k]);
                if (off[k] != end[k] && g_secBase[s] != 0)
                {
                    g_units[i].lo[k] = g_secBase[s] + off[k];
                    g_units[i].hi[k] = g_secBase[s] + end[k];
                }
                else
                {
                    g_units[i].lo[k] = 0;
                    g_units[i].hi[k] = 0;
                }
            }
        }

        g_ready = true;
    }

    void VM_PrefetchForKind(uint32_t kindAddr, bool lock)
    {
        constexpr bool kDisablePrefetch = false;
        if (kDisablePrefetch)
            return;

        if (!g_ready)
            return;
        if (kindAddr < g_winLo || kindAddr >= g_winHi)
            return; // DOL kind (Mario/party/system) or out of range

        for (int i = 0; i < kVmUnitCount; i++)
        {
            const uint32_t lo5 = g_units[i].lo[2];
            const uint32_t hi5 = g_units[i].hi[2];
            if (lo5 != 0 && kindAddr >= lo5 && kindAddr < hi5)
            {
                for (int k = 0; k < 4; k++)
                    if (g_units[i].lo[k] != g_units[i].hi[k])
                    {
                        const uint32_t ea = g_units[i].lo[k];
                        const uint32_t len = g_units[i].hi[k] - g_units[i].lo[k];
                        if (lock)
                            VM_PrefetchLocked(ea, len);
                        else
                            VM_Prefetch(ea, len);
                    }
                return;
            }
        }
    }
}
