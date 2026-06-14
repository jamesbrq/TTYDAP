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

    // Diagnostics at 0x80003B00 (see layout comments in VM_PrefetchInit).
    volatile uint32_t *const PF = reinterpret_cast<volatile uint32_t *>(0x80003B00);

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
        PF[15] = 0x51;
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
        PF[16] = nsec;
        PF[17] = secOff;
        PF[18] = imgBytes;
        PF[20] = secAddr;
        PF[15] = 0x52;

        // Guard: never read the section table outside the mapped window.
        if (nsec == 0 || nsec > 32 || secAddr < win || secAddr + 8u * nsec > win + imgBytes)
        {
            PF[9] = 0;
            PF[19] = 0x0BAD5EC5; // marker: section table rejected
            PF[15] = 0x56;
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
        PF[15] = 0x53;

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
        PF[15] = 0x54;

        PF[0] = g_secBase[1]; PF[1] = g_secSize[1];
        PF[2] = g_secBase[4]; PF[3] = g_secSize[4];
        PF[4] = g_secBase[5]; PF[5] = g_secSize[5];
        PF[6] = g_secBase[6]; PF[7] = g_secSize[6];
        uint32_t maxEnd5 = 0;
        for (int i = 0; i < kVmUnitCount; i++)
            if (kVmUnits[i].end5 > maxEnd5)
                maxEnd5 = kVmUnits[i].end5;
        PF[8] = maxEnd5;
        PF[9] = (maxEnd5 <= g_secSize[5]) ? 1u : 0u;
        PF[12] = 0; // prefetch-hit count
        PF[13] = 0; // window-kind-without-unit count

        g_ready = true;
        PF[15] = 0x55;
    }

    void VM_PrefetchForKind(uint32_t kindAddr, bool lock)
    {
        // Prefetch re-enabled. The earlier wrong-data symptoms were NOT a
        // delivery problem -- prefetch loads the correct ARAM bytes. They were
        // the sound/anim resolvers misreading positive window pointers as
        // indices, fixed by moving VM_WINDOW into the negative range. Set true
        // to force pure demand-paging for diagnostics.
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
                PF[10] = kindAddr;
                PF[11] = static_cast<uint32_t>(i); // last unit prefetched
                PF[12] = PF[12] + 1;
                return;
            }
        }

        PF[13] = PF[13] + 1; // window kind matched no unit (unexpected)
        PF[14] = kindAddr;
    }
}
