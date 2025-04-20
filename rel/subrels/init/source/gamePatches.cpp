#include "gamePatches.h"
#include "mod.h"
#include "patch.h"
#include "assembly.h"
#include "ttyd/seq_title.h"

#include <cstdint>

void applyGameFixes()
{
    // Fix a memory leak from occuring when starting a new file. The leak is that memory for the inventory is re-allocated
    // without checking if it's already allocated.
#ifdef TTYD_US
    constexpr uint32_t pouchInitMemoryLeakAddress = 0x800D59DC;
#elif defined TTYD_JP
    constexpr uint32_t pouchInitMemoryLeakAddress = 0x800D17A8;
#elif defined TTYD_EU
    constexpr uint32_t pouchInitMemoryLeakAddress = 0x800D67E4;
#endif

    mod::patch::writeBranchBL(pouchInitMemoryLeakAddress, fixPouchInitMemoryLeak);

    // Fix the game not properly allocating enough memory for text in textboxes. The vanilla code does not allocate memory for
    // the null terminator.
#ifdef TTYD_US
    constexpr uint32_t msgWindowMrAddress = 0x800816F4;
#elif defined TTYD_JP
    constexpr uint32_t msgWindowMrAddress = 0x80080B6C;
#elif defined TTYD_EU
    constexpr uint32_t msgWindowMrAddress = 0x800829B0;
#endif

    mod::patch::writePatch(reinterpret_cast<void *>(msgWindowMrAddress), 0x38830001); // addi r4,r3,1

    // Fix a crash that can occur when defeating Blooper with an attack that causes both the body and the left tentacle to be
    // defeated at roughly the same time
#ifdef TTYD_US
    constexpr uint32_t blooperCrash1Address = 0x8010F810;
    constexpr uint32_t blooperCrash2Address = 0x8010F87C;
#elif defined TTYD_JP
    constexpr uint32_t blooperCrash1Address = 0x8010A724;
    constexpr uint32_t blooperCrash2Address = 0x8010A790;
#elif defined TTYD_EU
    constexpr uint32_t blooperCrash1Address = 0x801106E8;
    constexpr uint32_t blooperCrash2Address = 0x80110754;
#endif

    mod::patch::writeBranchBL(blooperCrash1Address, asmFixBlooperCrash1);
    mod::patch::writeBranchBL(blooperCrash2Address, asmFixBlooperCrash2);

        // Fix crashes that can occur when evt_map_blend_set_flag is trying to apply effects to partners/followers when one is not
    // out
#ifdef TTYD_US
    constexpr uint32_t evtMapBlendSetFlagPartnerCrashAddress = 0x800389C4;
    constexpr uint32_t evtMapBlendSetFlagFollowerCrashAddress = 0x80038A0C;
#elif defined TTYD_JP
    constexpr uint32_t evtMapBlendSetFlagPartnerCrashAddress = 0x80038328;
    constexpr uint32_t evtMapBlendSetFlagFollowerCrashAddress = 0x80038370;
#elif defined TTYD_EU
    constexpr uint32_t evtMapBlendSetFlagPartnerCrashAddress = 0x80038AAC;
    constexpr uint32_t evtMapBlendSetFlagFollowerCrashAddress = 0x80038AF4;
#endif

    mod::patch::writeStandardBranches(reinterpret_cast<void *>(evtMapBlendSetFlagPartnerCrashAddress),
                          asmFixEvtMapBlendSetFlagPartnerCrashStart,
                          asmFixEvtMapBlendSetFlagPartnerCrashBranchBack);

    mod::patch::writeStandardBranches(reinterpret_cast<void *>(evtMapBlendSetFlagFollowerCrashAddress),
                          asmFixEvtMapBlendSetFlagFollowerCrashStart,
                          asmFixEvtMapBlendSetFlagFollowerCrashBranchBack);
}

void applyVariousGamePatches()
{
    // Enable debug mode by default
    // Use applyAssemblyPatch to apply the change and clear the cache, all in a single function call
    mod::patch::writePatch(&ttyd::seq_title::seqTitleWorkPtr2->debug, static_cast<uint32_t>(-1));

    // The debug mode is reset whenever the title screen is reached, so adjust it to always enable debug mode instead
#ifdef TTYD_US
    constexpr uint32_t debugModeInitializeAddress = 0x80009B2C;
#elif defined TTYD_JP
    constexpr uint32_t debugModeInitializeAddress = 0x8000999C;
#elif defined TTYD_EU
    constexpr uint32_t debugModeInitializeAddress = 0x80009CF0;
#endif

    mod::patch::writePatch(reinterpret_cast<void *>(debugModeInitializeAddress), 0x3800FFFF); // li r0,-1

    // Adjust the size of the text on the crash screen
#ifdef TTYD_US
    constexpr uint32_t crasheScreenTextSizeAddress = 0x80428BC0;
#elif defined TTYD_JP
    constexpr uint32_t crasheScreenTextSizeAddress = 0x80422618;
#elif defined TTYD_EU
    constexpr uint32_t crasheScreenTextSizeAddress = 0x804356C8;
#endif

    // Use writePatch to apply the change and clear the cache, all in a single function call
    // The float value will be initialized via lis and ori in the assembly when handled like this
#ifdef TTYD_JP
    constexpr float newTextSize = 0.7f;
#else
    constexpr float newTextSize = 0.66f;
#endif

    const uint32_t *valuePtr = reinterpret_cast<const uint32_t *>(&newTextSize);
    mod::patch::writePatch(reinterpret_cast<void *>(crasheScreenTextSizeAddress), *valuePtr);

    // Make the crash screen scroll and loop back around once it has gone offscreen
#ifdef TTYD_US
    constexpr uint32_t crashScreenPPCHaltBranchAddress = 0x8025E4A4;
    constexpr uint32_t crashScreenEndBranchAddress = 0x8025E4A8;
#elif defined TTYD_JP
    constexpr uint32_t crashScreenPosYValueAddress = 0x802582F8;
#elif defined TTYD_EU
    constexpr uint32_t crashScreenPPCHaltBranchAddress = 0x8026207C;
    constexpr uint32_t crashScreenEndBranchAddress = 0x80262080;
#endif

#ifdef TTYD_JP
    mod::patch::writeStandardBranches(crashScreenPosYValueAddress,
                                      asmCrashScreenDecrementYPosStart,
                                      asmCrashScreenDecrementYPosBranchBack);
#else
    mod::patch::writePatch(reinterpret_cast<void *>(crashScreenPPCHaltBranchAddress), 0x3B400000); // li r26,0
    mod::patch::writePatch(reinterpret_cast<void *>(crashScreenEndBranchAddress), 0x4BFFFDD4);     // b -0x22C
#endif

    // Prevent upgrade item cutscenes from occuring, and instead just give them like standard items. This is needed because the
    // upgrades could potentially be obtained in maps other than where they are intended to be obtained, which will cause a
    // crash since the data for their cutscenes will not be available.
#ifdef TTYD_US
    constexpr uint32_t preventUpgradeItemCutscenesAddress = 0x800ABD04;
#elif defined TTYD_JP
    constexpr uint32_t preventUpgradeItemCutscenesAddress = 0x800AA048;
#elif defined TTYD_EU
    constexpr uint32_t preventUpgradeItemCutscenesAddress = 0x800AD0D4;
#endif

    mod::patch::writePatch(reinterpret_cast<void *>(preventUpgradeItemCutscenesAddress), 0x60000000); // nop

    // Prevent the game from crashing if the player tries to read the diary while not on the Excess Express
#ifdef TTYD_US
    constexpr uint32_t preventDiaryTextboxSelectionAddress = 0x800D214C;
#elif defined TTYD_JP
    constexpr uint32_t preventDiaryTextboxSelectionAddress = 0x800CE01C;
#elif defined TTYD_EU
    constexpr uint32_t preventDiaryTextboxSelectionAddress = 0x800D2F44;
#endif

    mod::patch::writeBranchBL(preventDiaryTextboxSelectionAddress, asmPreventDiaryTextboxSelection);

    // Certain rooms need some things patched at a specific point during the screen transition, so hook an address to handle
    // that
#ifdef TTYD_US
    constexpr uint32_t fixMapProblemsAddress = 0x800080E4;
#elif defined TTYD_JP
    constexpr uint32_t fixMapProblemsAddress = 0x80008054;
#elif defined TTYD_EU
    constexpr uint32_t fixMapProblemsAddress = 0x800082BC;
#endif

    mod::patch::writeBranchBL(fixMapProblemsAddress, fixMapProblems);

    // Allow automashing through text if B is held
#ifdef TTYD_US
    constexpr uint32_t autoMashThroughTextAddress1 = 0x80080FCC;
    constexpr uint32_t autoMashThroughTextAddress2 = 0x80080FF0;
    constexpr uint32_t autoMashThroughTextAddress3 = 0x80084268;
#elif defined TTYD_JP
    constexpr uint32_t autoMashThroughTextAddress1 = 0x8008047C;
    constexpr uint32_t autoMashThroughTextAddress2 = 0x800804A0;
    constexpr uint32_t autoMashThroughTextAddress3 = 0x80083390;
#elif defined TTYD_EU
    constexpr uint32_t autoMashThroughTextAddress1 = 0x80082288;
    constexpr uint32_t autoMashThroughTextAddress2 = 0x800822AC;
    constexpr uint32_t autoMashThroughTextAddress3 = 0x800855BC;
#endif

    mod::patch::writeBranchBL(autoMashThroughTextAddress1, autoMashText);
    mod::patch::writeBranchBL(autoMashThroughTextAddress2, autoMashText);
    mod::patch::writeBranchBL(autoMashThroughTextAddress3, autoMashText);
}
