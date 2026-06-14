#include "errorHandling.h"
#include "mod.h"
#include "GhostPeers.h"
#include "patch.h"
#include "relmgr.h"
#include "visibility.h"
#include "ttyd/dispdrv.h"
#include "ttyd/memory.h"
#include "vm_probe.h"
#include "vm_customrel.h"

#include <gc/os.h>
#include <cstdio>
#include <cstring>

namespace mod
{
    KEEP_VAR Mod *gMod = nullptr;
    KEEP_VAR void (*mPFN_marioStMain_trampoline)() = nullptr;

    void heapProbe();

    void main()
    {
        LoadCustomRelVM();

        // Run the init rel to handle function hooks/patches/etc
        relMgr.runInitRel();

        // Heap census at 0x80003D00 (clear of the VM status block at 0x80003C00,
        // MMU dump at 0x80003C40, ARAM probe at 0x80003CA0). Read as big-endian
        // u32s; legend in heapProbe below.
        heapProbe();
    }

    void exit() {}

    KEEP_FUNC void heapProbe()
    {
        volatile uint32_t *d = reinterpret_cast<volatile uint32_t *>(0x80003D00);

        // OSAlloc heaps in fixed order: DEFAULT, MAP, EXT, EFFECT, SMART.
        // Pairs of (capacity, free-now) at d0..d9.
        const int32_t kHeaps[5] = {
            ttyd::memory::HeapType::HEAP_DEFAULT,
            ttyd::memory::HeapType::HEAP_MAP,
            ttyd::memory::HeapType::HEAP_EXT,
            ttyd::memory::HeapType::HEAP_EFFECT,
            ttyd::memory::HeapType::HEAP_SMART,
        };
        for (int i = 0; i < 5; ++i)
        {
            gc::os::HeapInfo &h = gc::os::OSAlloc_HeapArray[kHeaps[i]];
            uint32_t freeNow = 0;
            for (gc::os::ChunkInfo *c = h.firstFree; c; c = c->next)
                freeNow += c->size;
            d[i * 2] = h.capacity;
            d[i * 2 + 1] = freeNow;
        }

        // Smart (costume) arena lives inside HEAP_SMART but is carved by the
        // smart allocator, not OSAlloc. d9 (HEAP_SMART OSAlloc free) vs d12
        // (smart-arena free) shows whether __memAlloc(HEAP_SMART) has room
        // outside the arena.
        const uint32_t smartStart = reinterpret_cast<uint32_t>(ttyd::memory::heapStart.pHeapSmart);
        const uint32_t smartEnd = reinterpret_cast<uint32_t>(ttyd::memory::heapEnd.pHeapSmart);
        const uint32_t smartTotal = (smartEnd > smartStart + 0x20) ? (smartEnd - smartStart - 0x20) : 0;
        uint32_t smartUsed = 0;
        for (const ttyd::memory::SmartAllocationData *c = ttyd::memory::_smartWorkPtr->pFirstUsed; c; c = c->pNext)
            smartUsed += c->usedSize;

        d[10] = smartTotal;
        d[11] = smartUsed;
        d[12] = (smartTotal > smartUsed) ? (smartTotal - smartUsed) : 0;
        d[13] = static_cast<uint32_t>(gc::os::OSAlloc_NumHeaps);
        d[14] = 0x48454150; // 'HEAP' sanity marker
        d[15] = 0;
    }

    KEEP_FUNC void updateEarly()
    {
        // Check the game heaps for errors
        checkHeaps();

        gMod->owr_mod_.Update();

        ghosts::UpdateAll();

        // Register draw command
        ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::kDebug3d, 1, 0.f, draw, nullptr);
        
        ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::k3d, 1, 0.f, ghosts::DrawAll, nullptr);
        ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::kDebug3d, 1, 100.0f, ghosts::DrawNameTagsAll, nullptr);

        // Call the original function
        mPFN_marioStMain_trampoline();
    }

    KEEP_FUNC void draw(ttyd::dispdrv::CameraId layerId, void *user)
    {
        (void)layerId;
        (void)user;

        // Draw any error messages that occured this frame
        drawErrorMessages();
    }
} // namespace mod
