#include "gamePatches.h"
#include "util.h"
#include "patch.h"
#include "assembly.h"
#include "gc/pad.h"
#include "ttyd/system.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/memory.h"
#include "ttyd/swdrv.h"
#include "ttyd/seq_mapchange.h"
#include "ttyd/npcdrv.h"
#include "ttyd/evtmgr.h"
#include "ttyd/mariost.h"
#include "ttyd/seq_title.h"
#include "ttyd/seqdrv.h"

#include <cstdint>
#include <cstring>

using namespace ::ttyd::seq_mapchange;
using namespace ::ttyd::seqdrv;

static uint32_t autoMashText(gc::pad::PadId controllerId)
{
    if (mod::util::checkButtonComboEveryFrame(gc::pad::PadInput::PAD_B))
    {
        // Return the value for B to make sure the text is being mashed through as fast as possible
        return gc::pad::PadInput::PAD_B;
    }

    // Return the intended value
    return ttyd::system::keyGetButtonTrg(controllerId);
}

uint32_t cFixBlooperCrash1(uint32_t unkValue, void* battleUnitPtr)
{
    if (mod::util::ptrIsValid(battleUnitPtr))
    {
#ifdef TTYD_US
        uint32_t offset = 0x218;
#elif defined TTYD_JP
        uint32_t offset = 0x214;
#elif defined TTYD_EU
        uint32_t offset = 0x218;
#endif
        * reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(battleUnitPtr) + offset) = unkValue;
    }

    // Restore the value that was in r3
    return 2;
}

#ifdef TTYD_JP
int32_t cCrashScreenDecrementYPos()
{
    static int32_t decrementCount = 0;
    int32_t tempDecrementCount = decrementCount;

    constexpr int32_t kPosYIncrementAmount = 50;
    int32_t textPosY = tempDecrementCount + kPosYIncrementAmount;

    if (--tempDecrementCount < -960)
    {
        // Simulate incrementing exactly once to match the US/PAL code
        decrementCount = -1;
        return kPosYIncrementAmount;
    }
    else
    {
        decrementCount = tempDecrementCount;
        return textPosY;
    }
}
#endif

static void* fixPouchInitMemoryLeak(int32_t heap, uint32_t size)
{
    // Check if the memory has already been allocated or not
    ttyd::mario_pouch::PouchData* pouchPtr = ttyd::mario_pouch::pouchGetPtr();
    if (pouchPtr)
    {
        // The memory has already been allocated
        return pouchPtr;
    }
    else
    {
        // The memory has not been allocated, so allocate it
        return ttyd::memory::__memAlloc(heap, size);
    }
}

void cPreventDiaryTextboxOptionSelection(const char *currentText, int32_t *storeAddress, int32_t selectedOption)
{
    enum DiaryTextboxOption
    {
        FirstOption = 0,
        SecondOption,
        ThirdOption,
    };

    int32_t newOption = selectedOption;

    // Only need to check if trying to read the diary
    if (strcmp(currentText, "stg6_rsh_diary_01_yn") == 0)
    {
        // Prevent the first option from being selected, so that the game does not crash when reading the diary
        // Only needs to run when not on the train
        if (strcmp(_next_area, "rsh") != 0)
        {
            if (selectedOption == DiaryTextboxOption::ThirdOption)
            {
                seqSetSeq(SeqIndex::kGameOver, nullptr, nullptr);
            }

            newOption = DiaryTextboxOption::SecondOption;
        }
    }

    // Restore the overwritten instruction
    *storeAddress = newOption;
}

static void* fixMapProblems()
{
    const int32_t sequencePosition = ttyd::swdrv::swByteGet(0);
    const char* nextMapPtr = &ttyd::seq_mapchange::_next_map[0];

    if (strcmp(nextMapPtr, "nok_00") == 0)
    {
        // Prevent the game from crashing if the player enters the intro cutscene after interacting with an NPC that is past
        // slot 10
        // Check if the cutscene is going to play
        if (sequencePosition < 26)
        {
            // Clear the pointer used to check which animation Mario should use when greeting the Koopa
            ttyd::npcdrv::fbatGetPointer()->wHitNpc = nullptr; // Mario will do no animation when the pointer is not set
        }
    }
    else if (strcmp(nextMapPtr, "rsh_05_a") == 0)
    {
        // Prevent the game from crashing if the player enters rsh_05_a with the sequence past 338
        if (sequencePosition > 338)
        {
            // Set the sequence to 338 to prevent the crash
            ttyd::swdrv::swByteSet(0, 338);
        }
    }
    else if (strcmp(nextMapPtr, "aji_13") == 0)
    {
        // Prevent the game from crashing if the conveyor belt has not been activated
        // Set GW(11) to 0 upon entering the room to prevent the crash
        ttyd::evtmgr::EvtWork* eventWork = ttyd::evtmgr::evtGetWork();
        eventWork->gwData[11] = 0;
    }
    else if (strcmp(nextMapPtr, "las_08") == 0)
    {
        // Prevent the game from crashing if the player entered las_08 with the sequence at 385 and GSW(1121) at 7
        if (sequencePosition == 385)
        {
            // Check if GSW(1121) is currently higher than 6
            if (ttyd::swdrv::swByteGet(1121) > 6)
            {
                // Lower the value to 6 to prevent the game from crashing
                ttyd::swdrv::swByteSet(1121, 6);
            }
        }
    }

    // The overwritten instruction sets r3 to the global work pointer, so return that
    return ttyd::mariost::_globalWorkPtr;
}

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

    mod::patch::writePatch(reinterpret_cast<void*>(msgWindowMrAddress), 0x38830001); // addi r4,r3,1

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

    mod::patch::writePatch(reinterpret_cast<void*>(debugModeInitializeAddress), 0x3800FFFF); // li r0,-1

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

    const uint32_t* valuePtr = reinterpret_cast<const uint32_t*>(&newTextSize);
    mod::patch::writePatch(reinterpret_cast<void*>(crasheScreenTextSizeAddress), *valuePtr);

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
        asmCrashScreenDecrementYPosStart, asmCrashScreenDecrementYPosBranchBack);
#else
    mod::patch::writePatch(reinterpret_cast<void*>(crashScreenPPCHaltBranchAddress), 0x3B400000); // li r26,0
    mod::patch::writePatch(reinterpret_cast<void*>(crashScreenEndBranchAddress), 0x4BFFFDD4);     // b -0x22C
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

    mod::patch::writePatch(reinterpret_cast<void*>(preventUpgradeItemCutscenesAddress), 0x60000000); // nop

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
