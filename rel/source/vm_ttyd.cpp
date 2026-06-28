#include "vm_ttyd.h"
#include "gc/os.h"
#include "ttyd/memory.h"
#include <cstring>

namespace
{
    // --- SDK symbols (resolve via ttyd.us.lst at the noted US addresses) ---
    extern "C"
    {
        void ARStartDMA(uint32_t type, uint32_t mram, uint32_t aram, uint32_t len); // 0x802a8bdc
        uint32_t ARGetDMAStatus(void);                                              // 0x802a8ba0
        uint32_t ARAlloc(uint32_t len);                                             // 0x802a8ccc
        uint32_t ARGetSize(void);                                                   // 0x802a8e08
        void ARQPostRequest(void *req, uint32_t owner, uint32_t type, uint32_t prio,
                            uint32_t src, uint32_t dst, uint32_t len, void *cb);    // 0x802aa8ec
        void *__OSSetExceptionHandler(uint32_t except, void *handler);              // 0x80295aec
        void *__OSGetExceptionHandler(uint32_t except);                             // 0x80295b08
        int OSDisableInterrupts(void);                                              // 0x80299ad4
        void OSRestoreInterrupts(int);                                              // 0x80299afc
        void DCFlushRange(void *p, uint32_t len);                                   // 0x80296a54
        void DCInvalidateRange(void *p, uint32_t len);                              // 0x80296a28
        void ICInvalidateRange(void *p, uint32_t len);                              // 0x80296b0c
        void OSReport(const char *fmt, ...);                                        // 0x80297814
    }

    constexpr uint32_t AR_MRAMTOARAM = 0;
    constexpr uint32_t AR_ARAMTOMRAM = 1;
    constexpr uint32_t kDsiException = 2; // confirm against gc/os.h OSException enum
    constexpr uint32_t kIsiException = 3; // instruction-fetch fault (window code)

    constexpr uint32_t PAGE_SIZE = 0x1000;
    constexpr uint32_t PAGE_MASK = 0xFFFFF000;
    constexpr uint32_t PTE_SIZE = 0x10000; // 64KB page table (HTABMASK = 0)
    constexpr uint32_t HTABMASK = 0x0;
    constexpr uint32_t VM_VSID = 0x00000007;
    // Window placement is doubly constrained:
    //  (1) >= 0x80000000, or TTYD's __psndSFXOn / animPoseSetAnim read a rel
    //      name pointer as a numeric index (sign-bit tag) and misresolve it
    //      (the 0x7F000000 failure: garbage sound, anim clamped to 0, no fault).
    //  (2) within +-32MB of the DOL: rel .text holds 1627 REL24 bl's into the
    //      DOL (targets 0x8000519c..0x802ba2fc); OSLink mask-truncates any
    //      out-of-range one into a wild branch. 0x90000000 (256MB out) breaks
    //      every one -- only data-pointer (ADDR32) enemies survived there.
    // 0x81800000 satisfies both: negative; ~24MB above the lowest DOL target;
    // and the first address past the 24MB of real MEM1. TTYD already maps that
    // 24MB as a 16MB+8MB IBAT/DBAT pair ending at 0x817FFFFF, so 0x81800000 is
    // already BAT-free and faults into the pager -- no reshape needed; bring-up
    // only verifies it (verifyWindowBatFree). Confirm via DebugDumpMmu m[18]
    // (reads the real IBAT/DBAT0-3 -- must be 0 over the window).
    constexpr uint32_t VM_WINDOW = 0x81800000;
    constexpr uint16_t kNoPhys = 0xFFFF;
    constexpr uint16_t kNoVirt = 0xFFFF;
    constexpr uint16_t kLinear = 0xFFFE; // vpage mapped 1:1 during link phase
    constexpr uint32_t kOSContextSize = 0x2c8; // sizeof(OSContext): GPRs+FPRs+SRR0/1+...

    constexpr int MAX_VPAGES = 640;
    constexpr int MAX_PPAGES = 64;  // production cache only (0x40000 ceiling); test residency path removed
    constexpr uint16_t kMinFreeFrames = 12; // never lock so many frames that fewer than this stay evictable

    inline uint32_t v2p(const void *x)
    {
        return reinterpret_cast<uint32_t>(x) & 0x3FFFFFFF;
    }
    inline uint32_t p2k0(uint32_t x)
    {
        return x | 0x80000000;
    }

    // Force MSR[EE]=0 for the rest of a fault-path page-in. The SDK exception
    // dispatcher can enter the handler with interrupts ENABLED; if the page-in
    // then re-enables them (OSRestoreInterrupts inside aramDmaBlocking), a timer
    // or audio IRQ fires mid-fault, the scheduler context-switches, and the live
    // exception OSContext is overwritten with another thread's registers --
    // which OSLoadContext then restores, producing a wild branch. We clear EE
    // before touching anything and do NOT restore it: the exception return
    // (OSLoadContext -> rfi from ctx->srr1, EE=0) sets the final MSR.
    inline void faultDisableEE()
    {
        uint32_t msr;
        asm volatile("mfmsr %0" : "=r"(msr));
        asm volatile("mtmsr %0" ::"r"(msr & ~0x8000u));
        asm volatile("isync" ::: "memory");
    }

    using mod::vm::p_map;
    using mod::vm::PTE;
    using mod::vm::PTEG;
    using mod::vm::vm_map;
    using mod::vm::vm_page;
    using namespace gc::os;
    using namespace ttyd::memory;

    PTE *HTABORG = nullptr;
    vm_page *VM_Base = reinterpret_cast<vm_page *>(VM_WINDOW);
    vm_page *MEM_Base = nullptr;

    p_map phys_map[MAX_PPAGES];
    vm_map virt_map[MAX_VPAGES];
    uint16_t pmap_max = 0;
    uint16_t pmap_head = 0;
    uint16_t g_freePhys = 0;
    uint16_t g_lockedFrames = 0;
    uint32_t g_numVpages = 0;
    uint32_t g_aramBase = 0;
    uint32_t g_extAramBase = 0;
    uint32_t g_extAramBytes = 0;
    bool g_useExtAram = false;
    bool vm_initialized = false;
    bool g_linkMode = false;
    void *g_linkBuf = nullptr;

    void *g_pairLinkA = nullptr;
    void *g_pairLinkB = nullptr;
    uint32_t g_pairPagesA = 0;
    uint32_t g_pairPagesB = 0;
    uint32_t g_secondHeaderVpage = 0; // window vpage of module B's header (0 = single-rel)

    // ARQ-routed DMA (normal context only). When g_dmaUseArq is set AND interrupts
    // are enabled, aramDmaBlocking posts through ARQPostRequest so VM transfers
    // queue behind in-flight musyx/aramMgr requests on the shared AR engine
    // instead of stomping it with a raw ARStartDMA. Only the prefetch warming
    // sets this; the fault path (EE off) and boot persist/load stay raw.
    bool g_dmaUseArq = false;
    constexpr uint32_t kVmArqOwner = 0x564D4151; // 'VMAQ'
    volatile uint32_t g_arqDone = 0;
    uint32_t g_arqReq[8]; // SDK ARQRequest (8 words); ARQPostRequest fills it

    void *g_savedDsi = nullptr;
    void *g_savedIsi = nullptr;

    // Defined under "helpers" below; forward-declared so locate_oldest can write
    // a dirty page back to ARAM before reusing its frame.
    void aramDmaBlocking(uint32_t type, uint32_t mram, uint32_t aram, uint32_t len);

    inline void tlbie(const void *p)
    {
        asm volatile("tlbie %0" ::"r"(p));
    }

    void tlbia(void)
    {
        for (int i = 0; i < 64; i++) asm volatile("tlbie %0" ::"r"(i * PAGE_SIZE));
        asm volatile("sync" ::: "memory");
    }

    // --- verbatim MMU math from tueidj vm.c ---

    PTEG CalcPTEG(uint32_t virt, int secondary)
    {
        uint32_t segment_index = (virt >> 12) & 0xFFFF;
        uint32_t ptr = v2p(HTABORG);
        uint32_t hash = segment_index ^ VM_VSID;

        if (secondary)
            hash = ~hash;

        hash &= (HTABMASK << 10) | 0x3FF;
        ptr |= hash << 6;

        return reinterpret_cast<PTEG>(p2k0(ptr));
    }

    PTE *StorePTE(PTEG pteg, uint32_t virt, uint32_t physical, uint8_t WIMG, uint8_t PP, int secondary)
    {
        PTE p = {{0}};

        p.valid = 1;
        p.VSID = VM_VSID;
        p.hash = secondary ? 1 : 0;
        p.API = virt >> 22;
        p.RPN = physical >> 12;
        p.WIMG = WIMG;
        p.PP = PP;

        for (int i = 0; i < 8; i++)
        {
            if (pteg[i].valid)
                continue;

            pteg[i].data[1] = p.data[1];
            pteg[i].data[0] = p.data[0];
            DCFlushRange(&pteg[i], sizeof(PTE)); // tablewalk reads RAM
            asm volatile("sync" ::: "memory");
            asm volatile("tlbie %0" ::"r"(virt));
            asm volatile("sync" ::: "memory");
            return pteg + i;
        }

        return nullptr;
    }

    PTE *insert_pte(uint16_t index, uint32_t physical, uint8_t WIMG, uint8_t PP)
    {
        uint32_t virt = reinterpret_cast<uint32_t>(VM_Base + index);

        for (int i = 0; i < 2; i++)
        {
            PTEG pteg = CalcPTEG(virt, i);
            PTE *pte = StorePTE(pteg, virt, physical, WIMG, PP, i);
            if (pte)
                return pte;
        }

        return nullptr;
    }

    uint16_t locate_oldest(void)
    {
        uint16_t head = pmap_head;

        for (;; ++head)
        {
            if (head >= pmap_max)
                head = 0;

            if (!phys_map[head].valid || phys_map[head].locked)
                continue;

            PTE *p = HTABORG + phys_map[head].pte_index;
            tlbie(VM_Base + phys_map[head].page_index);

            if (p->C)
            {
                p->C = 0;
                phys_map[head].dirty = 1;
                continue;
            }
            if (p->R)
            {
                p->R = 0;
                continue;
            }

            // Write the frame back to ARAM if it was ever stored to (e.g. the OS
            // applying a deferred relocation into one of our sections). Without
            // this the next page-in of that vpage would reload the pre-write
            // bytes from ARAM and silently revert the change.
            if (phys_map[head].dirty)
            {
                uint16_t v = phys_map[head].page_index;
                void *src = MEM_Base + head;
                DCFlushRange(src, PAGE_SIZE);
                aramDmaBlocking(AR_MRAMTOARAM, reinterpret_cast<uint32_t>(src), g_aramBase + v * PAGE_SIZE, PAGE_SIZE);
                phys_map[head].dirty = 0;
            }

            p->data[0] = 0; // invalidate

            pmap_head = head + 1;
            return head;
        }
    }

    // --- helpers ---

    // Raw blocking AR DMA: interrupts-off, hand-managed ARINT. Used only in fault
    // context (EE already off) and at boot, where ARQ's interrupt-driven callback
    // can't run. This collides with musyx/aramMgr ARQ traffic, so it is NOT used
    // during the battle-setup warming -- see the dispatcher below.
    void aramDmaRaw(uint32_t type, uint32_t mram, uint32_t aram, uint32_t len)
    {
        volatile uint16_t *const kDspCsr = reinterpret_cast<volatile uint16_t *>(0xCC00500A);
        constexpr uint16_t kAidInt = 0x0008, kArInt = 0x0020, kDspInt = 0x0080;

        int cookie = OSDisableInterrupts();
        while (ARGetDMAStatus());
        bool arWasPending = (*kDspCsr & kArInt) != 0; // real ARQ completion; leave it for musyx
        ARStartDMA(type, mram, aram, len);
        while (ARGetDMAStatus());
        if (!arWasPending) // our transfer set ARINT; clear only it (w1c), keep AID/DSP + masks
            *kDspCsr = static_cast<uint16_t>((*kDspCsr & ~(kAidInt | kArInt | kDspInt)) | kArInt);
        OSRestoreInterrupts(cookie);
    }

    // ARQ completion callback. Runs on the AR interrupt; r3 = the ARQRequest* we
    // posted (ignored -- only one transfer is ever outstanding). Releases the
    // blocking poll below.
    extern "C" void vmArqCallback(uint32_t) { g_arqDone = 1; }

    // ARQ-routed blocking AR DMA. Posts via ARQPostRequest so the transfer queues
    // behind any in-flight musyx/aramMgr requests on the same engine, then spins
    // until the callback fires. REQUIRES interrupts enabled (else the callback
    // never runs and this hangs); the dispatcher guarantees that. Cache ops are
    // the caller's job, matching aramDmaRaw.
    void aramDmaArq(uint32_t type, uint32_t mram, uint32_t aram, uint32_t len)
    {
        uint32_t source = (type == AR_ARAMTOMRAM) ? aram : mram;
        uint32_t dest = (type == AR_ARAMTOMRAM) ? mram : aram;
        g_arqDone = 0;
        ARQPostRequest(g_arqReq, kVmArqOwner, type, 0, source, dest, len,
                       reinterpret_cast<void *>(&vmArqCallback));
        while (!g_arqDone) { }
    }

    // Dispatcher. Normal-context prefetch (g_dmaUseArq set, EE on) routes through
    // ARQ to avoid corrupting the shared AR engine while the game streams battle
    // banks; everything else (faults with EE off, boot) takes the raw path. The
    // EE check is a safety net: if ever entered with interrupts off, fall back to
    // raw rather than hang on a callback that can't fire.
    void aramDmaBlocking(uint32_t type, uint32_t mram, uint32_t aram, uint32_t len)
    {
        uint32_t msr;
        asm volatile("mfmsr %0" : "=r"(msr));
        if (g_dmaUseArq && (msr & 0x8000u))
            aramDmaArq(type, mram, aram, len);
        else
            aramDmaRaw(type, mram, aram, len);
    }

    uint16_t allocPhys(void)
    {
        if (g_freePhys < pmap_max)
            return g_freePhys++;

        uint16_t victim = locate_oldest();
        uint16_t oldV = phys_map[victim].page_index;
        if (oldV != kNoVirt && oldV < g_numVpages)
            virt_map[oldV].p_map_index = kNoPhys;
        return victim;
    }

    void mapPage(uint16_t v)
    {
        if (v >= g_numVpages)
            return;
        if (virt_map[v].p_map_index != kNoPhys)
            return; // already resident

        uint16_t pidx = allocPhys();
        void *dst = MEM_Base + pidx;

        if (virt_map[v].committed)
        {
            DCInvalidateRange(dst, PAGE_SIZE);
            aramDmaBlocking(AR_ARAMTOMRAM, reinterpret_cast<uint32_t>(dst), g_aramBase + v * PAGE_SIZE, PAGE_SIZE);
            ICInvalidateRange(dst, PAGE_SIZE); // page may hold executable code
        }
        else
        {
            memset(dst, 0, PAGE_SIZE);
            DCFlushRange(dst, PAGE_SIZE);
        }

        PTE *pte = insert_pte(v, v2p(dst), 0, 0b10);
        phys_map[pidx].valid = 1;
        phys_map[pidx].locked = 0;
        phys_map[pidx].dirty = 0;
        phys_map[pidx].page_index = v;
        phys_map[pidx].pte_index = static_cast<uint16_t>(pte - HTABORG);
        virt_map[v].p_map_index = pidx;
    }

    // --- window BAT-coverage check -------------------------------------------
    // The window must NOT be covered by any IBAT/DBAT, or accesses are
    // BAT-translated and never fault to the pager. TTYD already maps the 24MB of
    // MEM1 as a 16MB (slot 0) + 8MB (slot 2) pair ending at 0x817FFFFF, so
    // 0x81800000 is already BAT-free -- no reshape needed. This just verifies it
    // and bails (resident fallback) if some config unexpectedly covers the
    // window.
    inline bool batCoversWindow(uint32_t u)
    {
        if ((u & 0x3u) == 0)
            return false;
        uint32_t bepi = u & 0xFFFE0000u;
        uint32_t sz = (((u >> 2) & 0x7FFu) + 1u) << 17;
        return VM_WINDOW >= bepi && VM_WINDOW < bepi + sz;
    }

    bool verifyWindowBatFree()
    {
        uint32_t iu[4], du[4];
        asm volatile("mfspr %0, 528" : "=r"(iu[0])); // IBAT0U
        asm volatile("mfspr %0, 530" : "=r"(iu[1])); // IBAT1U
        asm volatile("mfspr %0, 532" : "=r"(iu[2])); // IBAT2U
        asm volatile("mfspr %0, 534" : "=r"(iu[3])); // IBAT3U
        asm volatile("mfspr %0, 536" : "=r"(du[0])); // DBAT0U
        asm volatile("mfspr %0, 538" : "=r"(du[1])); // DBAT1U
        asm volatile("mfspr %0, 540" : "=r"(du[2])); // DBAT2U
        asm volatile("mfspr %0, 542" : "=r"(du[3])); // DBAT3U

        bool covered = false;
        for (int i = 0; i < 4; i++)
        {
            if (batCoversWindow(iu[i]) || batCoversWindow(du[i]))
                covered = true;
        }
        if (covered)
            OSReport("VMBAT: window %08x is BAT-covered; aborting to resident\n", VM_WINDOW);
        return !covered;
    }

    // Common MMU bring-up: HTAB + ARAM backing + SDR1/SR. No cache, no PTEs.
    bool bringUpMmu(uint32_t imageSize)
    {
        uint32_t imgPages = (imageSize + PAGE_SIZE - 1) / PAGE_SIZE;
        if (imgPages == 0 || imgPages > MAX_VPAGES)
            return false;
        g_numVpages = imgPages;

        uint8_t *rawH = static_cast<uint8_t *>(__memAlloc(0, PTE_SIZE + 0x10000));
        if (!rawH)
            return false;
        HTABORG = reinterpret_cast<PTE *>((reinterpret_cast<uint32_t>(rawH) + 0xFFFF) & ~0xFFFF);
        memset(HTABORG, 0, PTE_SIZE);
        DCFlushRange(HTABORG, PTE_SIZE);

        uint32_t need = (imgPages * PAGE_SIZE + 0x1F) & ~0x1F;
        if (g_useExtAram)
        {
            if (need > g_extAramBytes)
                return false;
            g_aramBase = g_extAramBase;
        }
        else
        {
            g_aramBase = ARAlloc(need);
            if (g_aramBase == 0)
                return false;
        }

        for (uint32_t v = 0; v < imgPages; v++)
        {
            virt_map[v].committed = 0;
            virt_map[v].p_map_index = kNoPhys;
        }

        tlbia();
        asm volatile("mtspr 25, %0" ::"r"(v2p(HTABORG) | HTABMASK));
        asm volatile("mtsrin %0, %1" ::"r"(VM_VSID), "r"(VM_WINDOW));
        asm volatile("sync; isync" ::: "memory");

        if (!verifyWindowBatFree())
            return false; // window is BAT-covered; caller falls back to resident

        g_savedDsi = __OSGetExceptionHandler(kDsiException);
        g_savedIsi = __OSGetExceptionHandler(kIsiException);
        return true;
    }

    inline bool rangesOverlap(uint32_t a0, uint32_t a1, uint32_t b0, uint32_t b1)
    {
        return a0 < b1 && b0 < a1;
    }

    // Returns a bitmask of live regions the candidate cache span [lo,hi) would
    // alias. 0 == safe. A non-zero placement means a page-in DMA (which writes
    // a full 4KB frame at MEM_Base + pidx*0x1000) would stomp memory that is
    // then read back as something else -- the class of bug that corrupted the
    // saved OSContext and produced the wild blr out of npcSetupBattleInfo.
    uint32_t cacheConflictMask(uint32_t lo, uint32_t hi)
    {
        uint32_t mask = 0;

        // 0x01 - VM page table: the hardware tablewalk reads it every fault.
        if (HTABORG)
        {
            uint32_t h = reinterpret_cast<uint32_t>(HTABORG);
            if (rangesOverlap(lo, hi, h, h + PTE_SIZE))
                mask |= 0x01;
        }
        // 0x02 / 0x04 - VM bookkeeping arrays: read/written on every fault+evict.
        {
            uint32_t p = reinterpret_cast<uint32_t>(phys_map);
            if (rangesOverlap(lo, hi, p, p + sizeof(phys_map)))
                mask |= 0x02;
        }
        {
            uint32_t p = reinterpret_cast<uint32_t>(virt_map);
            if (rangesOverlap(lo, hi, p, p + sizeof(virt_map)))
                mask |= 0x04;
        }
        // 0x08 - the live exception/thread OSContext. A page-in landing here
        // overwrites the saved GPRs, so OSLoadContext resumes the faulting
        // instruction with garbage registers (the observed crash).
        {
            uint32_t c = reinterpret_cast<uint32_t>(OSGetCurrentContext());
            if (c && rangesOverlap(lo, hi, c, c + kOSContextSize))
                mask |= 0x08;
        }
        // 0x10 - the active stack (grows down): guard a window around live SP.
        {
            uint32_t sp;
            asm volatile("mr %0, 1" : "=r"(sp));
            if (rangesOverlap(lo, hi, sp - 0x2000u, sp + 0x10000u))
                mask |= 0x10;
        }
        // 0x20 - OS-reserved low memory.   0x40 - our own diagnostic blocks.
        if (rangesOverlap(lo, hi, 0x80000000u, 0x80003100u))
            mask |= 0x20;
        if (rangesOverlap(lo, hi, 0x80003B00u, 0x80003F00u))
            mask |= 0x40;

        return mask;
    }

    bool setupPagedCache(uint32_t cacheBytes)
    {
        uint32_t cachePages = cacheBytes / PAGE_SIZE;
        if (cachePages < 4 || cachePages > MAX_PPAGES)
            return false;

        const uint32_t need = cacheBytes + PAGE_SIZE;
        constexpr int kMaxTries = 6;

        // Validated allocation. __memAlloc(0) can hand back a region that aliases
        // live state (OSContext / stack / our tables / the page table). We
        // allocate, validate the page-aligned span, and on a conflict we *hold*
        // the offending block so the next __memAlloc advances past it, then
        // retry. Held blocks are returned to the heap once a clean region wins.
        uint8_t *rejected[kMaxTries] = {};
        int nrej = 0;
        uint8_t *goodRaw = nullptr;
        uint32_t lo = 0, hi = 0, mask = 0xFFFFFFFFu;
        int tries = 0;

        for (; tries < kMaxTries; ++tries)
        {
            uint8_t *raw = static_cast<uint8_t *>(__memAlloc(0, need));
            if (!raw)
                break;
            uint32_t base = (reinterpret_cast<uint32_t>(raw) + (PAGE_SIZE - 1)) & PAGE_MASK;
            lo = base;
            hi = base + cachePages * PAGE_SIZE;
            mask = cacheConflictMask(lo, hi);
            if (mask == 0)
            {
                goodRaw = raw;
                break;
            }
            rejected[nrej++] = raw; // keep allocated to push the bump past the conflict
        }

        // Return the rejected blocks (the kept good block, higher up, stays live).
        for (int i = 0; i < nrej; ++i)
            __memFree(0, rejected[i]);

        if (!goodRaw)
        {
            // Refuse to arm paged mode on a cache that aliases live memory:
            // every fault would silently corrupt it and crash far away. Surface
            // it here, at bring-up, instead of as a random battle crash.
            uint32_t sp;
            asm volatile("mr %0, 1" : "=r"(sp));
            const uint32_t ctxAddr = reinterpret_cast<uint32_t>(OSGetCurrentContext());
            OSReport("VMCACHE FATAL: no conflict-free cache after %d tries\n", tries);
            OSReport("  cand=%08x..%08x mask=%08x (1=htab 2=physmap 4=virtmap 8=ctx 10=stack 20=lowos 40=diag)\n",
                     lo, hi, mask);
            OSReport("  ctx=%08x..%08x sp=%08x htab=%08x physmap=%08x virtmap=%08x\n",
                     ctxAddr, ctxAddr ? ctxAddr + kOSContextSize : 0, sp,
                     reinterpret_cast<uint32_t>(HTABORG),
                     reinterpret_cast<uint32_t>(phys_map),
                     reinterpret_cast<uint32_t>(virt_map));
            for (;;)
            {
            } // halt
        }

        pmap_max = static_cast<uint16_t>(cachePages);
        MEM_Base = reinterpret_cast<vm_page *>(lo);
        memset(MEM_Base, 0, cachePages * PAGE_SIZE);
        DCFlushRange(MEM_Base, cachePages * PAGE_SIZE);

        for (uint32_t i = 0; i < cachePages; i++)
        {
            phys_map[i].valid = 1;
            phys_map[i].locked = 0;
            phys_map[i].dirty = 0;
            phys_map[i].page_index = kNoVirt;
            phys_map[i].pte_index = 0;
        }
        g_freePhys = 0;
        pmap_head = 0;
        return true;
    }
} // namespace

extern "C" void *vm_saved_dsi;
extern "C" void *vm_saved_isi;
extern "C" void vm_dsi_stub(); // defined in vm_dsi.cpp (or weak stub below)

namespace mod::vm
{
    uint32_t VM_WindowBase()
    {
        return VM_WINDOW;
    }

    void *VM_Init(uint32_t imageSize, uint32_t cacheBytes)
    {
        if (vm_initialized)
            return VM_Base;
        if (cacheBytes >= imageSize)
            return nullptr;
        if (!bringUpMmu(imageSize))
            return nullptr;
        if (!setupPagedCache(cacheBytes))
            return nullptr;
        vm_saved_dsi = g_savedDsi;
        vm_saved_isi = g_savedIsi;
        vm_initialized = true;
        return VM_Base;
    }

    // Link phase: map the window 1:1 onto a page-aligned MEM1 buffer that
    // already holds the raw REL bytes. After this returns, the caller links
    // the module with module = VM_WindowBase() so it relocates for the window,
    // runs its prolog (fully resident), then calls VM_EndLink.
    void *VM_BeginLink(void *linkBufPageAligned, uint32_t imageSize)
    {
        if (vm_initialized)
            return nullptr;
        if ((reinterpret_cast<uint32_t>(linkBufPageAligned) & (PAGE_SIZE - 1)) != 0)
            return nullptr;
        if (!bringUpMmu(imageSize))
            return nullptr;

        g_linkBuf = linkBufPageAligned;
        uint8_t *base = static_cast<uint8_t *>(linkBufPageAligned);

        int cookie = OSDisableInterrupts();
        for (uint32_t v = 0; v < g_numVpages; v++)
        {
            insert_pte(static_cast<uint16_t>(v), v2p(base + v * PAGE_SIZE), 0, 0b10);
            virt_map[v].p_map_index = kLinear;
            virt_map[v].committed = 0;
        }
        DCFlushRange(HTABORG, PTE_SIZE); // make PTEs visible to the HW tablewalk
        asm volatile("sync; isync" ::: "memory");
        OSRestoreInterrupts(cookie);

        vm_saved_dsi = g_savedDsi;
        vm_saved_isi = g_savedIsi;
        g_linkMode = true;
        vm_initialized = true;
        return VM_Base;
    }

    void VM_SetAramRegion(uint32_t base, uint32_t bytes)
    {
        g_extAramBase = base;
        g_extAramBytes = bytes;
        g_useExtAram = true;
    }

    // Persist phase: DMA the relocated resident region to ARAM, drop the 1:1
    // mapping, and mark resident pages committed. Does NOT allocate the paging
    // cache, so the caller can free the (large) link buffer before
    // VM_StartPaging claims MEM1.
    //
    // The DMA length is rounded up to a whole PAGE, not 0x20: mapPage faults in
    // full 4KB pages from ARAM, so the tail of the last committed page must be
    // backed by real bytes. Callers that pass the full image size also get the
    // imp/rel tables backed -- required so the OS can re-walk them for deferred
    // relocations on later map transitions.
    bool VM_Persist(uint32_t residentSize)
    {
        if (!vm_initialized || !g_linkMode)
            return false;

        uint32_t aligned = (residentSize + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
        uint32_t aramSize = ARGetSize();
        if (aramSize != 0 && g_aramBase + aligned > aramSize)
            return false; // would overflow ARAM; caller falls back to resident load
        DCFlushRange(g_linkBuf, aligned);
        aramDmaBlocking(AR_MRAMTOARAM, reinterpret_cast<uint32_t>(g_linkBuf), g_aramBase, aligned);

        int cookie = OSDisableInterrupts();
        memset(HTABORG, 0, PTE_SIZE);
        DCFlushRange(HTABORG, PTE_SIZE);
        tlbia();
        asm volatile("sync; isync" ::: "memory");
        OSRestoreInterrupts(cookie);

        uint32_t residentPages = (residentSize + PAGE_SIZE - 1) / PAGE_SIZE;
        if (residentPages > g_numVpages)
            residentPages = g_numVpages;
        for (uint32_t v = 0; v < g_numVpages; v++)
        {
            virt_map[v].committed = (v < residentPages) ? 1 : 0;
            virt_map[v].p_map_index = kNoPhys;
        }

        g_linkMode = false;
        g_linkBuf = nullptr;
        return true;
    }

    // Allocate the MEM1 paging cache and arm paged mode. Call after VM_Persist
    // (and after the caller has freed the link buffer).
    bool VM_StartPaging(uint32_t cacheBytes)
    {
        if (!vm_initialized || g_linkMode)
            return false;
        return setupPagedCache(cacheBytes);
    }

    // Convenience wrapper (does not free the link buffer between phases).
    bool VM_EndLink(uint32_t residentSize, uint32_t cacheBytes)
    {
        return VM_Persist(residentSize) && VM_StartPaging(cacheBytes);
    }

    void *VM_BeginLinkPair(void *linkBufA, uint32_t bytesA, void *linkBufB, uint32_t bytesB, void **outWindowB)
    {
        if (vm_initialized)
            return nullptr;
        if ((reinterpret_cast<uint32_t>(linkBufA) & (PAGE_SIZE - 1)) != 0)
            return nullptr;
        if ((reinterpret_cast<uint32_t>(linkBufB) & (PAGE_SIZE - 1)) != 0)
            return nullptr;

        uint32_t pagesA = (bytesA + PAGE_SIZE - 1) / PAGE_SIZE;
        uint32_t pagesB = (bytesB + PAGE_SIZE - 1) / PAGE_SIZE;
        if (pagesA == 0 || pagesB == 0 || (pagesA + pagesB) > MAX_VPAGES)
            return nullptr;

        if (!bringUpMmu((pagesA + pagesB) * PAGE_SIZE))
            return nullptr;

        uint8_t *baseA = static_cast<uint8_t *>(linkBufA);
        uint8_t *baseB = static_cast<uint8_t *>(linkBufB);

        int cookie = OSDisableInterrupts();
        for (uint32_t v = 0; v < pagesA; v++)
        {
            insert_pte(static_cast<uint16_t>(v), v2p(baseA + v * PAGE_SIZE), 0, 0b10);
            virt_map[v].p_map_index = kLinear;
            virt_map[v].committed = 0;
        }
        for (uint32_t v = 0; v < pagesB; v++)
        {
            uint32_t vv = pagesA + v;
            insert_pte(static_cast<uint16_t>(vv), v2p(baseB + v * PAGE_SIZE), 0, 0b10);
            virt_map[vv].p_map_index = kLinear;
            virt_map[vv].committed = 0;
        }
        DCFlushRange(HTABORG, PTE_SIZE); // make PTEs visible to the HW tablewalk
        asm volatile("sync; isync" ::: "memory");
        OSRestoreInterrupts(cookie);

        vm_saved_dsi = g_savedDsi;
        vm_saved_isi = g_savedIsi;
        g_pairLinkA = linkBufA;
        g_pairLinkB = linkBufB;
        g_pairPagesA = pagesA;
        g_pairPagesB = pagesB;
        g_secondHeaderVpage = pagesA;
        g_linkMode = true;
        vm_initialized = true;

        if (outWindowB)
            *outWindowB = reinterpret_cast<void *>(VM_WINDOW + pagesA * PAGE_SIZE);
        return VM_Base;
    }

    bool VM_PersistPair()
    {
        if (!vm_initialized || !g_linkMode)
            return false;

        uint32_t alignedA = g_pairPagesA * PAGE_SIZE;
        uint32_t alignedB = g_pairPagesB * PAGE_SIZE;
        uint32_t aramSize = ARGetSize();
        if (aramSize != 0 && g_aramBase + alignedA + alignedB > aramSize)
            return false; // would overflow ARAM

        DCFlushRange(g_pairLinkA, alignedA);
        aramDmaBlocking(AR_MRAMTOARAM, reinterpret_cast<uint32_t>(g_pairLinkA), g_aramBase, alignedA);
        DCFlushRange(g_pairLinkB, alignedB);
        aramDmaBlocking(AR_MRAMTOARAM, reinterpret_cast<uint32_t>(g_pairLinkB), g_aramBase + alignedA, alignedB);

        int cookie = OSDisableInterrupts();
        memset(HTABORG, 0, PTE_SIZE);
        DCFlushRange(HTABORG, PTE_SIZE);
        tlbia();
        asm volatile("sync; isync" ::: "memory");
        OSRestoreInterrupts(cookie);

        for (uint32_t v = 0; v < g_numVpages; v++)
        {
            virt_map[v].committed = 1;
            virt_map[v].p_map_index = kNoPhys;
        }

        g_linkMode = false;
        g_pairLinkA = nullptr;
        g_pairLinkB = nullptr;
        return true;
    }

    bool VM_LoadImage(const void *src, uint32_t size)
    {
        if (!vm_initialized || g_linkMode)
            return false;
        uint32_t aligned = (size + 0x1F) & ~0x1F;
        DCFlushRange(const_cast<void *>(src), aligned);
        aramDmaBlocking(AR_MRAMTOARAM, reinterpret_cast<uint32_t>(src), g_aramBase, aligned);

        uint32_t pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
        if (pages > g_numVpages)
            pages = g_numVpages;
        for (uint32_t v = 0; v < pages; v++) virt_map[v].committed = 1;
        return true;
    }

    void VM_Prefetch(uint32_t ea, uint32_t len)
    {
        if (!vm_initialized || g_linkMode || len == 0)
            return;
        if (ea < VM_WINDOW)
            return;

        uint32_t first = (ea - VM_WINDOW) / PAGE_SIZE;
        uint32_t last = (ea - VM_WINDOW + len - 1) / PAGE_SIZE;

        // Interrupts stay ENABLED so the ARQ-routed page-ins can complete (their
        // callback fires on the AR interrupt). Safe here: warming runs on the main
        // thread, which isn't executing window code, and no interrupt handler
        // touches the window -- so no reentrant fault into mapPage.
        g_dmaUseArq = true;
        for (uint32_t v = first; v <= last && v < g_numVpages; v++) mapPage(static_cast<uint16_t>(v));
        g_dmaUseArq = false;
        asm volatile("sync; isync" ::: "memory");
    }

    // Like VM_Prefetch but pins the covered pages so locate_oldest never evicts
    // them. Used for the OSModuleInfo header (page 0), which the OS reads during
    // module link/unlink on every map transition.
    void VM_PrefetchLocked(uint32_t ea, uint32_t len)
    {
        if (!vm_initialized || g_linkMode || len == 0)
            return;
        if (ea < VM_WINDOW)
            return;

        uint32_t first = (ea - VM_WINDOW) / PAGE_SIZE;
        uint32_t last = (ea - VM_WINDOW + len - 1) / PAGE_SIZE;

        // Interrupts stay ENABLED for ARQ completion (see VM_Prefetch).
        g_dmaUseArq = true;
        for (uint32_t v = first; v <= last && v < g_numVpages; v++)
        {
            mapPage(static_cast<uint16_t>(v));
            uint16_t pidx = virt_map[v].p_map_index;
            if (pidx != kNoPhys && !phys_map[pidx].locked &&
                g_lockedFrames + kMinFreeFrames < pmap_max)
            {
                phys_map[pidx].locked = 1;
                g_lockedFrames++;
            }
        }
        g_dmaUseArq = false;
        asm volatile("sync; isync" ::: "memory");
    }

    void VM_UnlockAll()
    {
        if (!vm_initialized || g_linkMode)
            return;
        int cookie = OSDisableInterrupts();
        for (uint16_t i = 0; i < pmap_max; i++)
            phys_map[i].locked = 0;
        g_lockedFrames = 0;
        OSRestoreInterrupts(cookie);
        VM_PrefetchLocked(VM_WINDOW, PAGE_SIZE); // keep module header (page 0) pinned
        if (g_secondHeaderVpage != 0)
            VM_PrefetchLocked(VM_WINDOW + g_secondHeaderVpage * PAGE_SIZE, PAGE_SIZE); // module B header
    }

    int VM_HandleFault(uint32_t dsisr, uint32_t dar)
    {
        if (!vm_initialized || g_linkMode)
            return 0;
        if (dar < VM_WINDOW || dar >= (VM_WINDOW + g_numVpages * PAGE_SIZE))
            return 0;
        if ((dsisr & ~0x02000000) != 0x40000000)
            return 0;

        uint16_t v = static_cast<uint16_t>(((dar & ~0xFFFu) - VM_WINDOW) / PAGE_SIZE);
        faultDisableEE(); // keep the page-in atomic vs interrupts/reschedule
        mapPage(v);
        asm volatile("sync; isync" ::: "memory");
        return 1;
    }

    // ISI: faulting addr is SRR0, not DAR; resident -> not a paging fault, chain.
    int VM_HandleFaultISI(uint32_t addr)
    {
        if (!vm_initialized || g_linkMode)
            return 0;
        if (addr < VM_WINDOW || addr >= (VM_WINDOW + g_numVpages * PAGE_SIZE))
            return 0;

        uint16_t v = static_cast<uint16_t>(((addr & ~0xFFFu) - VM_WINDOW) / PAGE_SIZE);
        if (virt_map[v].p_map_index != kNoPhys)
            return 0; // already resident -> not a demand-paging fault

        faultDisableEE(); // keep the page-in atomic vs interrupts/reschedule
        mapPage(v);
        asm volatile("sync; isync" ::: "memory");
        return 1;
    }

    void VM_Abort()
    {
        if (!vm_initialized)
            return;
        int cookie = OSDisableInterrupts();
        if (HTABORG)
        {
            memset(HTABORG, 0, PTE_SIZE);
            DCFlushRange(HTABORG, PTE_SIZE);
        }
        tlbia();
        asm volatile("mtspr 25, %0" ::"r"(0));
        asm volatile("mtsrin %0, %1" ::"r"(0x80000000), "r"(VM_WINDOW));
        asm volatile("sync; isync" ::: "memory");
        OSRestoreInterrupts(cookie);
        vm_initialized = false;
        g_linkMode = false;
        g_linkBuf = nullptr;
        g_pairLinkA = nullptr;
        g_pairLinkB = nullptr;
        g_secondHeaderVpage = 0;
    }

    uint32_t VM_DbgAramBase()
    {
        return g_aramBase;
    }
    uint32_t VM_DbgNumVpages()
    {
        return g_numVpages;
    }

    void VM_EnableDemandPaging()
    {
        if (!vm_initialized || g_linkMode)
            return;
        __OSSetExceptionHandler(kDsiException, reinterpret_cast<void *>(vm_dsi_stub));
        __OSSetExceptionHandler(kIsiException, reinterpret_cast<void *>(vm_dsi_stub));
    }
} // namespace mod::vm

// --- extern "C" bridges for the asm/C DSI stub ---
extern "C"
{
    void *vm_saved_dsi = nullptr;
    void *vm_saved_isi = nullptr;
}

extern "C" int vm_fault_c(uint32_t dsisr, uint32_t dar)
{
    return mod::vm::VM_HandleFault(dsisr, dar);
}

extern "C" int vm_ifault_c(uint32_t addr)
{
    return mod::vm::VM_HandleFaultISI(addr);
}

// Weak placeholder, overridden by the real handler in vm_dsi.cpp. Never
// installed unless VM_EnableDemandPaging() runs, so the empty body is harmless
// if vm_dsi.cpp is ever excluded from the build.
extern "C" __attribute__((weak)) void vm_dsi_stub() {}