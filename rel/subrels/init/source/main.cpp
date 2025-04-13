#include "subrel_main.h"
#include "mod.h"
#include "cxx.h"
#include "patch.h"
#include "errorHandling.h"
#include "gamePatches.h"
#include "ttyd/mariost.h"
#include "ttyd/npcdrv.h"
#include "ttyd/animdrv.h"
#include "ttyd/fontmgr.h"

#include <cstdint>

namespace mod
{
    void main()
    {
        Mod *mod = new Mod();
        gMod = mod;

        npcNameToPtrErrorInfoPtr = &mod->npcNameToPtrErrorInfo;
        animPoseMainErrorInfoPtr = &mod->animPoseMainErrorInfo;
        heapCorruptionInfoPtr = &mod->heapCorruptionInfo;
        errorTextPosY = ERROR_TEXT_DEFAULT_POS_Y;

        mPFN_marioStMain_trampoline = patch::hookFunction(marioStMain, updateEarly);
        g_npcNameToPtr_trampoline = patch::hookFunction(npcNameToPtr, checkForNpcNameToPtrError);
        g_animPoseMain_trampoline = patch::hookFunction(ttyd::animdrv::animPoseMain, preventAnimPoseMainCrash);

        applyGameFixes();
        applyVariousGamePatches();

        // Initialize typesetting early
        ttyd::fontmgr::fontmgrTexSetup();

        // Prevent fontmgrTexSetup from running again
        patch::writePatch(reinterpret_cast<void *>(ttyd::fontmgr::fontmgrTexSetup), 0x4E800020); // blr
    }

    void exit() {}
} // namespace mod
