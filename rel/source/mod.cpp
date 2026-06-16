#include "errorHandling.h"
#include "mod.h"
#include "GhostPeers.h"
#include "patch.h"
#include "relmgr.h"
#include "visibility.h"
#include "ttyd/dispdrv.h"
#include "ttyd/memory.h"
#include "vm_customrel.h"

#include <gc/os.h>
#include <cstdio>
#include <cstring>

namespace mod
{
    KEEP_VAR Mod *gMod = nullptr;
    KEEP_VAR void (*mPFN_marioStMain_trampoline)() = nullptr;

    void main()
    {
        if (platformIsConsole())
            LoadCustomRelVM();
        else
            relMgr.loadCustomRel();

        // Run the init rel to handle function hooks/patches/etc
        relMgr.runInitRel();
    }

    void exit() {}

    // Temporary: publish heap sizes to the diagnostic scratch block so the AP
    // client (/heaps) can read them. Layout at 0x80003C00 (big-endian u32):
    //   [0] magic 'HEAP'  [1] default  [2] map  [3] ext  [4] effect  [5] smart
    //   [6] smart contiguous-free (unallocatedArenaStartSize)
    KEEP_FUNC void writeHeapStats()
    {
        volatile uint32_t *out = reinterpret_cast<volatile uint32_t *>(0x80003C00);
        out[0] = 0x48454150;
        void **starts = reinterpret_cast<void **>(&ttyd::memory::heapStart);
        void **ends = reinterpret_cast<void **>(&ttyd::memory::heapEnd);
        for (int i = 0; i < 5; i++)
            out[1 + i] = reinterpret_cast<uint32_t>(ends[i]) - reinterpret_cast<uint32_t>(starts[i]);
        ttyd::memory::SmartWork *sw = ttyd::memory::_smartWorkPtr;
        out[6] = sw ? sw->unallocatedArenaStartSize : 0;
    }

    KEEP_FUNC void updateEarly()
    {
        // Check the game heaps for errors
        checkHeaps();

        writeHeapStats();

        gMod->owr_mod_.Update();

        if (multiplayerEnabled())
            ghosts::UpdateAll();

        // Register draw command
        ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::kDebug3d, 1, 0.f, draw, nullptr);

        if (multiplayerEnabled())
        {
            ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::k3d, 1, 0.f, ghosts::DrawAll, nullptr);
            ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::kDebug3d, 1, 100.0f, ghosts::DrawNameTagsAll, nullptr);
        }

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
