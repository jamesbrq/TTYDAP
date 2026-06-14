#pragma once
#include <cstdint>

// ARAM-backed demand-paged virtual window for TTYD (US, G8ME01).
// MMU math ported verbatim from tueidj's vm.c (GC/Wii VM lib);
// platform I/O retargeted to the game's SDK symbols.
//
// Window: 0x81800000 (segment 8), the first address past the 24MB of real
// MEM1. The base is doubly constrained: it must be >= 0x80000000 (TTYD's
// __psndSFXOn / animPoseSetAnim treat a value < 0x80000000 as a numeric index
// rather than a name pointer), AND within +-32MB of the DOL (rel .text holds
// ~1600 REL24 bl's into the DOL that OSLink would otherwise truncate). TTYD
// already maps the 24MB of MEM1 as a 16MB+8MB IBAT/DBAT pair ending at
// 0x817FFFFF, so 0x81800000+ is already BAT-free and faults into the pager;
// bring-up only verifies this (verifyWindowBatFree in vm_ttyd.cpp).

namespace mod::vm
{
    typedef uint8_t vm_page[0x1000];

    // PowerPC 32-bit PTE (8 bytes). Big-endian bitfields pack MSB-first.
    typedef union
    {
        uint32_t data[2];
        struct
        {
            uint32_t valid : 1;
            uint32_t VSID  : 24;
            uint32_t hash  : 1;
            uint32_t API   : 6;
            uint32_t RPN   : 20;
            uint32_t resv0 : 3;
            uint32_t R     : 1;
            uint32_t C     : 1;
            uint32_t WIMG  : 4;
            uint32_t resv1 : 1;
            uint32_t PP    : 2;
        };
    } PTE;

    typedef PTE *PTEG;

    typedef struct
    {
        uint16_t page_index; // virtual page currently resident here (0xFFFF = none)
        uint16_t pte_index;  // index of its PTE within HTABORG
        uint8_t valid : 1;
        uint8_t locked : 1;
        uint8_t dirty : 1;
    } p_map;

    typedef struct
    {
        uint16_t p_map_index; // hosting cache slot, or kNoPhys
        uint8_t committed : 1; // content exists in ARAM backing
    } vm_map;

    // Returns the window base (0x7F000000) on success, nullptr on failure.
    void *VM_Init(uint32_t imageSize, uint32_t cacheBytes);

    // Link-phase entry: map the window 1:1 onto a page-aligned MEM1 buffer
    // holding the raw REL bytes, so the game's Link() (called with
    // module = VM_WindowBase()) relocates the module for window addresses.
    // Returns the window base, or nullptr on failure.
    void *VM_BeginLink(void *linkBufPageAligned, uint32_t imageSize);

    // Provide an explicit ARAM backing region (base + byte count), used instead
    // of ARAlloc. Call before VM_BeginLink. Lets us place the image above
    // musyx's sample heap (aramTop) when ARAlloc has no room.
    void VM_SetAramRegion(uint32_t base, uint32_t bytes);

    // Persist phase: DMA the relocated [0, residentSize) region into ARAM and
    // drop the 1:1 mapping. Does NOT allocate the paging cache, so the caller
    // can free the link buffer first. residentSize = module->fixSize - VM_WindowBase().
    bool VM_Persist(uint32_t residentSize);

    // Allocate the MEM1 paging cache and arm paged mode (after VM_Persist).
    bool VM_StartPaging(uint32_t cacheBytes);

    // Convenience wrapper for Persist+StartPaging (holds the link buffer across both).
    bool VM_EndLink(uint32_t residentSize, uint32_t cacheBytes);

    // One-time: copy an already-VM_Base-relocated image from MEM into ARAM.
    bool VM_LoadImage(const void *src, uint32_t size);

    // Explicitly page-in (and PTE-map) every page covering [ea, ea+len).
    // Normal context only; guards with interrupt-disable internally.
    void VM_Prefetch(uint32_t ea, uint32_t len);

    // Like VM_Prefetch but pins the covered pages so they are never evicted.
    // Used for the OSModuleInfo header (page 0), which the OS reads during
    // module link/unlink on every map transition.
    void VM_PrefetchLocked(uint32_t ea, uint32_t len);

    // Clear every frame lock (call at battle start) and re-pin page 0. Keeps
    // battle-scoped VM_PrefetchLocked pins from accumulating across battles.
    void VM_UnlockAll();

    // Tear down a partially/fully configured VM (neutralizes SDR1/SR, clears
    // the page table) so the caller can fall back to a normal resident load.
    void VM_Abort();

    // Diagnostics.
    uint32_t VM_DbgAramBase();
    uint32_t VM_DbgNumVpages();

    // Stage 4: install the DSI handler for transparent demand paging.
    void VM_EnableDemandPaging();

    // The fault worker (called from the DSI stub). Returns 1 if handled.
    int VM_HandleFault(uint32_t dsisr, uint32_t dar);

    uint32_t VM_WindowBase();
}
