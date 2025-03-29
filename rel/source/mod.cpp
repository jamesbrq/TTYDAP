#include "errorHandling.h"
#include "gamePatches.h"
#include "mod.h"
#include "patch.h"
#include <cstdio>
#include <cstring>
#include <ttyd/animdrv.h>
#include <ttyd/dispdrv.h>
#include <ttyd/fontmgr.h>
#include <ttyd/npcdrv.h>
#include <ttyd/system.h>

ModInitFunction *ModInitFunction::sFirst = nullptr;
ModUpdateFunction *ModUpdateFunction::sFirst = nullptr;

namespace mod
{
    Mod *gMod = nullptr;

    void main()
    {
        Mod *mod = new Mod();
        mod->init();
    }

    Mod::Mod()
    {
    }

    void Mod::init()
    {
        gMod = this;

        mPFN_makeKey_trampoline = patch::hookFunction(ttyd::system::makeKey, []() { gMod->updateEarly(); });

        g_npcNameToPtr_trampoline = patch::hookFunction(ttyd::npcdrv::npcNameToPtr, checkForNpcNameToPtrError);
        g_animPoseMain_trampoline = patch::hookFunction(ttyd::animdrv::animPoseMain, preventAnimPoseMainCrash);

        applyGameFixes();
        applyVariousGamePatches();

        // Initialize typesetting early
        ttyd::fontmgr::fontmgrTexSetup();
        patch::hookFunction(ttyd::fontmgr::fontmgrTexSetup, []() {});

        mConsole.init();

        // Run spread initializations
        for (ModInitFunction *p = ModInitFunction::sFirst; p; p = p->next)
        {
            p->initFunction();
        }

        owr_mod_.Init();
    }

    void Mod::updateEarly()
    {
        // Check the game heaps for errors
        checkHeaps();

        owr_mod_.Update();
        // Run spread update functions
        for (ModUpdateFunction *p = ModUpdateFunction::sFirst; p; p = p->next)
        {
            p->updateFunction();
        }

        // Register draw command
        ttyd::dispdrv::dispEntry(
            ttyd::dispdrv::CameraId::kDebug3d,
            1,
            0.f,
            [](ttyd::dispdrv::CameraId layerId, void *user)
            {
                (void)layerId;
                reinterpret_cast<Mod *>(user)->draw();
            },
            this);

        mConsole.update();

        // Call original function
        mPFN_makeKey_trampoline();
    }

    void Mod::draw()
    {
        // Draw any error messages that occured this frame
        drawErrorMessages();
    }
} // namespace mod
