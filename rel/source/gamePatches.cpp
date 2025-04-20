#include "gamePatches.h"
#include "util.h"
#include "patch.h"
#include "assembly.h"
#include "visibility.h"
#include "gc/pad.h"
#include "ttyd/system.h"
#include "ttyd/party.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/memory.h"
#include "ttyd/swdrv.h"
#include "ttyd/seq_mapchange.h"
#include "ttyd/npcdrv.h"
#include "ttyd/evtmgr.h"
#include "ttyd/mariost.h"
#include "ttyd/seqdrv.h"

#include <cstdint>
#include <cstring>

using namespace mod::util;

KEEP_FUNC uint32_t autoMashText(gc::pad::PadId controllerId)
{
    if (mod::util::checkButtonComboEveryFrame(gc::pad::PadInput::PAD_B))
    {
        // Return the value for B to make sure the text is being mashed through as fast as possible
        return gc::pad::PadInput::PAD_B;
    }

    // Return the intended value
    return ttyd::system::keyGetButtonTrg(controllerId);
}

KEEP_FUNC uint32_t cFixBlooperCrash1(uint32_t unkValue, void *battleUnitPtr)
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
        *reinterpret_cast<uint32_t *>(reinterpret_cast<uint32_t>(battleUnitPtr) + offset) = unkValue;
    }

    // Restore the value that was in r3
    return 2;
}

#ifdef TTYD_JP
KEEP_FUNC int32_t cCrashScreenDecrementYPos()
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

KEEP_FUNC void *fixPouchInitMemoryLeak(int32_t heap, uint32_t size)
{
    // Check if the memory has already been allocated or not
    ttyd::mario_pouch::PouchData *pouchPtr = ttyd::mario_pouch::pouchGetPtr();
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

KEEP_FUNC ttyd::party::PartyEntry *cFixEvtMapSetFlagCrash(ttyd::party::PartyEntry *partyPtr, bool shouldSpawnPartner)
{
    // Bring out a partner/follower if no partner/follower is currently out
    if (partyPtr)
    {
        return partyPtr;
    }

    return ttyd::party::partyGetPtr(spawnFailsafePartnerOrFollower(shouldSpawnPartner));
}

KEEP_FUNC void cPreventDiaryTextboxOptionSelection(const char *currentText, int32_t *storeAddress, int32_t selectedOption)
{
    enum DiaryTextboxOption
    {
        FirstOption = 0,
        SecondOption,
        ThirdOption,
    };

    // Only need to check if trying to read the diary
    if (strcmp(currentText, "stg6_rsh_diary_01_yn") == 0)
    {
        // Prevent the first option from being selected, so that the game does not crash when reading the diary
        // Only needs to run when not on the train
        if (strcmp(ttyd::seq_mapchange::_next_area, "rsh") != 0)
        {
            selectedOption = DiaryTextboxOption::SecondOption;
        }
    }

    // Restore the overwritten instruction
    *storeAddress = selectedOption;
}

KEEP_FUNC void *fixMapProblems()
{
    const int32_t sequencePosition = ttyd::swdrv::swByteGet(0);
    const char *nextMapPtr = &ttyd::seq_mapchange::_next_map[0];

    if (strcmp(nextMapPtr, "nok_00") == 0)
    {
        // Prevent the game from crashing if the player enters the intro cutscene after interacting with an NPC that is past
        // slot 10
        // Check if the cutscene is going to play
        if (sequencePosition < 26)
        {
            // Clear the pointer used to check which animation Mario should use when greeting the Koopa
            fbatGetPointer()->wHitNpc = nullptr; // Mario will do no animation when the pointer is not set
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
        ttyd::evtmgr::EvtWork *eventWork = ttyd::evtmgr::evtGetWork();
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
    return _globalWorkPtr;
}
