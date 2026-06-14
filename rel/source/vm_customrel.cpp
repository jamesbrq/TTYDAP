#include "vm_ttyd.h"
#include "vm_customrel.h"
#include "vm_prefetch.h"

#include "gc/dvd.h"
#include "gc/OSModule.h"
#include "ttyd/memory.h"

#include <cstdint>

using namespace gc::dvd;
using namespace ttyd::memory;

extern "C"
{
    uint32_t ARGetBaseAddress(void);         // 0x802a8e00
    uint32_t ARGetSize(void);                // 0x802a8e08
    uint32_t aramGetFirstUserAddress(void);  // 0x80291bd4 (musyx)
    uint32_t aramGetUserBytes(void);         // 0x80291bf8 (musyx)

    // Smart (costume) arena allocator. Returns the bookkeeping node; the usable
    // buffer is node->pMemory. smartFree takes the node. At boot the arena has
    // ~5MB contiguous free, so this hits the fast path (no garbage collection).
    SmartAllocationData *smartAlloc(uint32_t size, uint32_t group); // 0x8002f540
    void smartFree(SmartAllocationData *node);                      // 0x8002f878
}

namespace
{
    constexpr uint32_t PAGE_SIZE = 0x1000;

    // Group tag for our transient link buffer. Freed manually within this
    // function before any map/costume loads, so it is never reached by a
    // smartAutoFree sweep; the tag value is immaterial as long as it is not a
    // group the game collects during our synchronous window.
    constexpr uint32_t kSmartGroupTransient = 0;

    // Boot-time status block, readable in the Dolphin memory editor as
    // big-endian u32s. d[0] is the last milestone reached, so if the rel
    // crashes mid-bring-up the value pinpoints where.
    volatile uint32_t *const DBG = reinterpret_cast<volatile uint32_t *>(0x80003C00);

    inline uint32_t alignUp(uint32_t v, uint32_t a) { return (v + a - 1) & ~(a - 1); }
    inline void stage(uint32_t s) { DBG[0] = s; }
}

bool LoadCustomRelVM(uint32_t cacheBytes)
{
    for (int i = 0; i < 16; i++)
        DBG[i] = 0;
    stage(1); // entered

    // ARAM layout probe (pristine, before our own ARAlloc). Read at 0x80003CA0.
    {
        volatile uint32_t *AP = reinterpret_cast<volatile uint32_t *>(0x80003CA0);
        AP[4] = *reinterpret_cast<volatile uint32_t *>(0x8041F324); // __AR_Size
        AP[5] = *reinterpret_cast<volatile uint32_t *>(0x8041F330); // __AR_StackPointer
        AP[6] = *reinterpret_cast<volatile uint32_t *>(0x8041EFD8); // musyx aramTop
        AP[7] = *reinterpret_cast<volatile uint32_t *>(0x8041F33C); // __AR_init_flag
        AP[0] = ARGetBaseAddress();
        AP[1] = ARGetSize();
        AP[2] = aramGetFirstUserAddress();
        AP[3] = aramGetUserBytes();
    }

    DVDFileInfo fileInfo;
    if (!DVDOpen("/mod/custom.rel", &fileInfo))
    {
        stage(0xE1);
        return false;
    }

    uint32_t fileSize = alignUp(fileInfo.length, DVD_READ_SIZE);
    DBG[1] = fileInfo.length;
    DBG[2] = fileSize;
    stage(2); // opened

    // Transient link buffer comes from the smart (costume) arena, not
    // HEAP_DEFAULT, so HEAP_DEFAULT's bump can shrink to just the persistent
    // cache/HTAB. The buffer is freed right after VM_Persist, before any map or
    // costume loads, so it never collides with costume allocations.
    //
    // +2 pages of slack: one to page-align linkBuf within the allocation, one
    // so the full-page-rounded persist DMA (alignUp(imageSize, PAGE)) never
    // reads past the buffer.
    SmartAllocationData *linkNode = smartAlloc(fileSize + 2 * PAGE_SIZE, kSmartGroupTransient);
    uint8_t *rawBuf = linkNode ? static_cast<uint8_t *>(linkNode->pMemory) : nullptr;
    if (!rawBuf)
    {
        stage(0xE2);
        DVDClose(&fileInfo);
        return false;
    }
    uint8_t *linkBuf = reinterpret_cast<uint8_t *>(
        (reinterpret_cast<uint32_t>(rawBuf) + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1));

    const int32_t result = DVDReadPrio(&fileInfo, linkBuf, fileSize, 0, 0);
    DVDClose(&fileInfo);
    if (result <= 0)
    {
        stage(0xE3);
        smartFree(linkNode);
        return false;
    }
    stage(3); // dvd read ok

    // Reserve the top slice of the game's aramMgr map/battle ARAM heap. init
    // already ran (ARAlloc'd the full 0x3FC000), so the SDK already treats the
    // whole block as reserved; we only lower aramMgr's own usage cap so its
    // bump allocator never climbs into our slice, then park the image there.
    {
        volatile uint32_t *mgrTop = reinterpret_cast<volatile uint32_t *>(0x803E5EF0);  // work+0x20
        volatile uint32_t *mgrBase = reinterpret_cast<volatile uint32_t *>(0x803E5EF4); // work+0x24
        volatile uint32_t *mgrSize = reinterpret_cast<volatile uint32_t *>(0x803E5EF8); // work+0x28

        uint32_t reserve = alignUp(fileSize, PAGE_SIZE) + 0x2000;
        uint32_t oldSize = *mgrSize;
        uint32_t newSize = (oldSize > reserve) ? (oldSize - reserve) : 0;
        uint32_t aramBase = *mgrBase + newSize;

        if (newSize != 0 && *mgrTop > aramBase)
            reinterpret_cast<void (*)()>(0x801528A0)(); // aramMgrGarbage

        DBG[10] = *mgrTop;
        DBG[11] = aramBase;

        if (newSize == 0 || *mgrTop > aramBase)
        {
            stage(0xE9); // reserve failed: aramMgr heap too full at load time
            smartFree(linkNode);
            return false;
        }

        *mgrSize = newSize;
        mod::vm::VM_SetAramRegion(aramBase, reserve);
    }

    if (!mod::vm::VM_BeginLink(linkBuf, fileSize))
    {
        stage(0xE4);
        smartFree(linkNode);
        return false;
    }
    DBG[5] = mod::vm::VM_DbgAramBase();
    DBG[6] = mod::vm::VM_DbgNumVpages();
    mod::vm::VM_DebugDumpMmu(reinterpret_cast<volatile uint32_t *>(0x80003C40));
    stage(4); // window mapped 1:1, MMU live

    OSModuleInfo *relWindow = reinterpret_cast<OSModuleInfo *>(mod::vm::VM_WindowBase());
    DBG[8] = relWindow->id;       // module id, read through the window
    DBG[9] = *reinterpret_cast<volatile uint32_t *>(mod::vm::VM_WindowBase()); // first word

    // BSS stays in HEAP_DEFAULT: it is tiny (~0x144) and persistent.
    constexpr int32_t kResidentHeap = HeapType::HEAP_DEFAULT;
    uint32_t bssSize = relWindow->bssSize;
    if (bssSize == 0)
        bssSize = 1;
    DBG[7] = bssSize;
    uint8_t *bssArea = static_cast<uint8_t *>(__memAlloc(kResidentHeap, bssSize));
    if (!bssArea)
    {
        stage(0xE5);
        mod::vm::VM_Abort();
        smartFree(linkNode);
        return false;
    }

    if (!Link(relWindow, bssArea, false))
    {
        stage(0xE6);
        OSUnlink(relWindow);
        mod::vm::VM_Abort();
        __memFree(kResidentHeap, bssArea);
        smartFree(linkNode);
        return false;
    }
    DBG[3] = relWindow->fixSize; // absolute end addr post-link; expect 0x7Fxxxxxx
    stage(5);                    // relocated for window

    // MakeRelFixed-equivalent (see rel_shrink.cpp). Boot relocations are now
    // fully applied, so drop the self(40)/DOL(0) imp-table entries. Otherwise
    // the OS re-walks our rel table during later module link/unlink: OSLink is
    // hooked by the framework and fires on every area-rel load (map
    // transitions), and that path runs Link->Relocate over our already-
    // relocated module, re-applying fixups on top of absolute data and
    // corrupting a pointer -> ISI at ~0x05000000. The framework's own
    // loadCustomRel() calls MakeRelFixed for exactly this reason; the VM path
    // must too. We need only the impSize truncation (not its ShrinkAllocation
    // -- our image lives in ARAM, not a heap chunk), and we do it while the
    // window is still 1:1 mapped so it persists into the ARAM copy. impOffset
    // is absolute post-link, matching MakeRelFixed's own pointer use.
    {
        OSModuleImpSection *impTable =
            reinterpret_cast<OSModuleImpSection *>(relWindow->impOffset);
        uint32_t impCount = relWindow->impSize / sizeof(OSModuleImpSection);
        for (uint32_t i = 0; i < impCount; i++)
        {
            int32_t mid = impTable[i].moduleId;
            if (mid == 0 || mid == static_cast<int32_t>(relWindow->id))
            {
                relWindow->impSize = i * sizeof(OSModuleImpSection);
                break;
            }
        }
        DBG[14] = 0xF1000000u | (relWindow->impSize & 0x00FFFFFFu); // confirm truncation ran
    }

    uint32_t residentSize = relWindow->fixSize - mod::vm::VM_WindowBase();
    DBG[4] = residentSize;

    // Commit the ENTIRE image, not just [0, fixSize). Belt-and-suspenders with
    // the imp-table truncation above: even though the OS should no longer walk
    // our rel table, backing the whole file means any stray read past fixSize
    // returns real bytes rather than a zero-filled page. Dirty-page write-back
    // (locate_oldest) keeps any window write from being lost on eviction. Once
    // the truncation is confirmed to stop all 0x7Fxxxxxx rel-table reads, this
    // can be trimmed back to residentSize to reclaim the extra ARAM.
    uint32_t imageSize = mod::vm::VM_DbgNumVpages() * PAGE_SIZE;

    // Build the kind->unit->span prefetch table from the section table while the
    // window is still 1:1 mapped (it reads the section table through the window).
    mod::vm::VM_PrefetchInit(relWindow);

    // Persist to ARAM and drop the 1:1 map. Frees nothing yet.
    if (!mod::vm::VM_Persist(imageSize))
    {
        stage(0xE7);
        mod::vm::VM_Abort();
        __memFree(kResidentHeap, bssArea);
        smartFree(linkNode);
        return false;
    }
    stage(6); // image in ARAM, window unmapped

    // Release the transient link buffer back to the smart arena BEFORE the
    // paging cache claims MEM1.
    smartFree(linkNode);
    linkNode = nullptr;

    DBG[13] = cacheBytes;
    if (!mod::vm::VM_StartPaging(cacheBytes))
    {
        stage(0xE8);
        mod::vm::VM_Abort();
        __memFree(kResidentHeap, bssArea);
        return false;
    }
    stage(7);

    // Pin page 0 (OSModuleInfo header + section table) so the OS can always read
    // it during module link/unlink, and so the re-read below is safe in paged
    // mode where nothing else is resident yet.
    mod::vm::VM_PrefetchLocked(mod::vm::VM_WindowBase(), PAGE_SIZE);

    // Arm the DSI demand-paging net. Prefetch warms the cache for battle units;
    // the net catches every other window access (field enemy setup, etc.) that
    // prefetch doesn't cover, paging it in instead of faulting to a halt.
    mod::vm::VM_EnableDemandPaging();

    DBG[12] = *reinterpret_cast<volatile uint32_t *>(mod::vm::VM_WindowBase()); // re-read via paged window
    DBG[15] = 1; // success
    stage(8);
    return true;
}
