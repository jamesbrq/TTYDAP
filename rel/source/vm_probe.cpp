#include <cstdint>

// Stage 0: read-only MMU safety probe for the paged-VM plan.
// Confirms (a) SDR1 is unused, (b) no DBAT maps the VM window,
// (c) the window's segment register, before we ever write MMU state.
// Results are surfaced to 0x80003C00 (big-endian u32s) for the
// Dolphin memory editor. Nothing here writes any SPR.

namespace mod::vmprobe
{
    namespace
    {
        // Candidate virtual window base. MUST match VM_WINDOW in vm_ttyd.cpp.
        // 0x81800000 -> segment 8, first address past the 24MB of real MEM1.
        // Negative (so TTYD's sign-based name-vs-index resolvers treat rel
        // pointers as pointers) and within +-32MB of the DOL (so rel .text's
        // REL24 bl's still encode). TTYD already maps MEM1 as a 16MB+8MB BAT pair
        // ending at 0x817FFFFF, so 0x81800000 is already BAT-free and faults into
        // the pager; see vm_ttyd.cpp.
        constexpr uint32_t kVmBase = 0x81800000u;

        inline uint32_t spr_sdr1()
        {
            uint32_t v;
            asm volatile("mfspr %0, 25" : "=r"(v));
            return v;
        }
        inline uint32_t spr_hid4()
        {
            uint32_t v;
            asm volatile("mfspr %0, 1011" : "=r"(v));
            return v;
        }
        inline uint32_t srin(uint32_t ea)
        {
            uint32_t v;
            asm volatile("mfsrin %0, %1" : "=r"(v) : "r"(ea));
            return v;
        }

        inline uint32_t dbatU(int i)
        {
            uint32_t v = 0;
            switch (i)
            {
                case 0: asm volatile("mfspr %0, 536" : "=r"(v)); break;
                case 1: asm volatile("mfspr %0, 538" : "=r"(v)); break;
                case 2: asm volatile("mfspr %0, 540" : "=r"(v)); break;
                case 3: asm volatile("mfspr %0, 542" : "=r"(v)); break;
                case 4: asm volatile("mfspr %0, 568" : "=r"(v)); break; // Gekko ext
                case 5: asm volatile("mfspr %0, 570" : "=r"(v)); break;
                case 6: asm volatile("mfspr %0, 572" : "=r"(v)); break;
                case 7: asm volatile("mfspr %0, 574" : "=r"(v)); break;
            }
            return v;
        }
        inline uint32_t dbatL(int i)
        {
            uint32_t v = 0;
            switch (i)
            {
                case 0: asm volatile("mfspr %0, 537" : "=r"(v)); break;
                case 1: asm volatile("mfspr %0, 539" : "=r"(v)); break;
                case 2: asm volatile("mfspr %0, 541" : "=r"(v)); break;
                case 3: asm volatile("mfspr %0, 543" : "=r"(v)); break;
                case 4: asm volatile("mfspr %0, 569" : "=r"(v)); break;
                case 5: asm volatile("mfspr %0, 571" : "=r"(v)); break;
                case 6: asm volatile("mfspr %0, 573" : "=r"(v)); break;
                case 7: asm volatile("mfspr %0, 575" : "=r"(v)); break;
            }
            return v;
        }

        // Does this DBAT (upper word) cover ea? Vs(bit30)|Vp(bit31) = valid.
        bool batCovers(uint32_t batu, uint32_t ea)
        {
            if ((batu & 0x3u) == 0) return false; // neither Vs nor Vp
            const uint32_t bepi = batu & 0xFFFE0000u;
            const uint32_t bl = (batu >> 2) & 0x7FFu;        // bits 19-29
            const uint32_t size = (bl + 1u) << 17;           // 128KB units
            return (ea >= bepi) && (ea < (bepi + size));
        }
    }

    void Probe()
    {
        volatile uint32_t *d = reinterpret_cast<volatile uint32_t *>(0x80003C00);

        const uint32_t sdr1 = spr_sdr1();
        const uint32_t srWin = srin(kVmBase);
        const uint32_t hid4 = spr_hid4();

        uint32_t coverMask = 0;
        uint32_t validMask = 0;
        for (int i = 0; i < 8; ++i)
        {
            const uint32_t u = dbatU(i);
            if ((u & 0x3u) != 0) validMask |= (1u << i);
            if (batCovers(u, kVmBase)) coverMask |= (1u << i);
        }

        d[0] = sdr1;       // 3C00: want 0x00000000 (page table unused)
        d[1] = srWin;      // 3C04: segment reg for the window (0x81800000 -> SR8)
        d[2] = coverMask;  // 3C08: want 0 (no I/D BAT maps the window post-reshape)
        d[3] = validMask;  // 3C0C: which DBATs are valid (reference)
        d[4] = hid4;       // 3C10: HID4 (bit for ext-BAT enable)
        d[5] = dbatU(0);   // 3C14
        d[6] = dbatL(0);   // 3C18
        d[7] = dbatU(1);   // 3C1C
        d[8] = dbatL(1);   // 3C20
        d[9] = dbatU(2);   // 3C24
        d[10] = dbatL(2);  // 3C28
        d[11] = dbatU(3);  // 3C2C
        d[12] = dbatL(3);  // 3C30
    }
}
