#include "errorHandling.h"
#include "mod.h"
#include "patch.h"
#include "relmgr.h"
#include "visibility.h"
#include "ttyd/dispdrv.h"

#include <cstdio>
#include <cstring>

namespace mod
{
    KEEP_VAR Mod *gMod = nullptr;
    KEEP_VAR void (*mPFN_marioStMain_trampoline)() = nullptr;

    void main()
    {
        // Load and link custom.rel permanently
        relMgr.loadCustomRel();

        // Run the init rel to handle function hooks/patches/etc
        relMgr.runInitRel();
    }

    void exit() {}

    KEEP_FUNC void updateEarly()
    {
        // Check the game heaps for errors
        checkHeaps();

        gMod->owr_mod_.Update();

        // Register draw command
        ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::kDebug3d, 1, 0.f, draw, nullptr);

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
