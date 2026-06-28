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
        {
            LoadBothCustomRelsVM();
        }
        else
        {
            relMgr.loadCustomRel("custom");
            relMgr.loadCustomRel("custom2"); // no-op if absent; links after custom so it can import it
        }

        // Run the init rel to handle function hooks/patches/etc
        relMgr.runInitRel();
    }

    void exit() {}

    KEEP_FUNC void updateEarly()
    {
        // Check the game heaps for errors
        checkHeaps();

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