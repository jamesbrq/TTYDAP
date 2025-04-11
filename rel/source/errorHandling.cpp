#include "errorHandling.h"
#include "util.h"
#include "OWR.h"
#include "gc/os.h"
#include "ttyd/memory.h"
#include "ttyd/npcdrv.h"
#include "ttyd/system.h"

#include <cstdint>
#include <cstdio>
#include <cinttypes>

NpcNameToPtrErrorInfo npcNameToPtrErrorInfo;
AnimPoseMainErrorInfo animPoseMainErrorInfo;
HeapCorruptionInfo heapCorruptionInfo;
float errorTextPosY = ERROR_TEXT_DEFAULT_POS_Y;

NpcEntry *(*g_npcNameToPtr_trampoline)(const char *name) = nullptr;
void (*g_animPoseMain_trampoline)(int32_t poseId) = nullptr;

void HeapCorruptionInfo::drawBuffer()
{
    // Get the text to draw
    constexpr float scale = 0.6f;
    const char *text = this->getBufferPtr();

    // Draw the text
    const float posY = errorTextPosY;
    mod::owr::gSelf->DrawString(text, -233.f, errorTextPosY, 0xFFFFFFFF, scale);

    // Decrement errorTextPosY to account for how many lines are in the buffer
    errorTextPosY = posY - mod::util::getTextMultilineIncrement(text, scale, 0);

    // Done, so reset heapIndex and free the memory used by the buffer
    this->setBufferIndex(0);
    this->freeBuffer();
}

void NpcNameToPtrErrorInfo::drawErrorMessage()
{
    // Decrement the timer
    const uint32_t timer = this->getTimer() - 1;
    this->setTimer(timer);

    // Get the current counter value
    const uint32_t counter = this->getCounter();

    // If the timer is at 0, then reset the counter
    if (timer == 0)
    {
        this->resetCounter();
    }

    // Get the text
    char buf[64];
    snprintf(buf, sizeof(buf), "npcNameToPtr error occured x%" PRIu32, counter);

    // Draw the text
    constexpr float scale = 0.6f;
    const float posY = errorTextPosY;
    mod::owr::gSelf->DrawString(buf, -233.f, posY, 0xFFFFFFFF, scale);

    // Decrement errorTextPosY to go to the next line
    errorTextPosY = posY - (32.f * scale);
}

void AnimPoseMainErrorInfo::drawErrorMessage()
{
    // Decrement the timer
    const uint32_t timer = this->getTimer() - 1;
    this->setTimer(timer);

    // Get the current counter value
    const uint32_t counter = this->getCounter();

    // If the timer is at 0, then reset the counter
    if (timer == 0)
    {
        this->resetCounter();
    }

    // Get the text
    char buf[64];
    snprintf(buf, sizeof(buf), "animPoseMain error occured x%" PRIu32, counter);

    // Draw the text
    constexpr float scale = 0.6f;
    const float posY = errorTextPosY;
    mod::owr::gSelf->DrawString(buf, -233.f, posY, 0xFFFFFFFF, scale);

    // Decrement errorTextPosY to go to the next line
    errorTextPosY = posY - (32.f * scale);
}

static const void *checkIndividualStandardHeap(const gc::os::ChunkInfo *start)
{
    const gc::os::ChunkInfo *prevChunk = nullptr;
    for (const gc::os::ChunkInfo *currentChunk = start; currentChunk; currentChunk = currentChunk->next)
    {
        // Check pointer sanity
        if (!mod::util::ptrIsValid(const_cast<gc::os::ChunkInfo *>(currentChunk)))
        {
            return currentChunk;
        }

        // Sanity check size
        if (currentChunk->size >= 0x1800000)
        {
            return currentChunk;
        }

        // Check linked list integrity
        if (prevChunk != currentChunk->prev)
        {
            return currentChunk;
        }

        prevChunk = currentChunk;
    }

    return nullptr;
}

static const void *checkIndividualSmartHeap(const ttyd::memory::SmartAllocationData *start)
{
    const ttyd::memory::SmartAllocationData *prevChunk = nullptr;
    for (const ttyd::memory::SmartAllocationData *currentChunk = start; currentChunk; currentChunk = currentChunk->pNext)
    {
        // Check pointer sanity
        if (!mod::util::ptrIsValid(const_cast<ttyd::memory::SmartAllocationData *>(currentChunk)))
        {
            return currentChunk;
        }

        // Sanity check size
        if (currentChunk->usedSize >= 0x1800000)
        {
            return currentChunk;
        }

        // Check linked list integrity
        if (prevChunk != currentChunk->pPrev)
        {
            return currentChunk;
        }

        prevChunk = currentChunk;
    }

    return nullptr;
}

static const void *checkIndividualMapHeap(const ttyd::memory::MapAllocEntry *start)
{
    for (const ttyd::memory::MapAllocEntry *currentChunk = start; currentChunk; currentChunk = currentChunk->next)
    {
        // Check pointer sanity
        if (!mod::util::ptrIsValid(const_cast<ttyd::memory::MapAllocEntry *>(currentChunk)))
        {
            return currentChunk;
        }

        // Sanity check size
        if (currentChunk->size >= 0x1800000)
        {
            return currentChunk;
        }
    }

    return nullptr;
}

static void initStandardHeapError(const void *address, int32_t heapIndex, bool isUsedPortion)
{
    // Set up the text to be drawn
    // Make sure heapCorruptioBufferIndex is valid
    HeapCorruptionInfo *heapCorruptionInfoPtr = &heapCorruptionInfo;
    if (!heapCorruptionInfoPtr->verifyBufferIndex())
    {
        return;
    }

    // Get the used or free text
    const char *usedOrFreeString;
    if (isUsedPortion)
    {
        usedOrFreeString = "used";
    }
    else
    {
        usedOrFreeString = "free";
    }

    int32_t index = heapCorruptionInfoPtr->getBufferIndex();

    index += snprintf(heapCorruptionInfoPtr->initBufferEntry(index),
                      heapCorruptionInfoPtr->getBufferSize() - index,
                      "Main Heap %" PRId32 " (%s) corrupt at 0x%08" PRIX32 "\r\n",
                      heapIndex,
                      usedOrFreeString,
                      reinterpret_cast<uint32_t>(address));

    // Update the index
    heapCorruptionInfoPtr->setBufferIndex(index);
}

static void initSmartHeapError(const void *address, bool isUsedPortion)
{
    // Set up the text to be drawn
    // Make sure heapCorruptioBufferIndex is valid
    HeapCorruptionInfo *heapCorruptionInfoPtr = &heapCorruptionInfo;
    if (!heapCorruptionInfoPtr->verifyBufferIndex())
    {
        return;
    }

    // Get the used or free text
    const char *usedOrFreeString;
    if (isUsedPortion)
    {
        usedOrFreeString = "used";
    }
    else
    {
        usedOrFreeString = "free";
    }

    int32_t index = heapCorruptionInfoPtr->getBufferIndex();

    index += snprintf(heapCorruptionInfoPtr->initBufferEntry(index),
                      heapCorruptionInfoPtr->getBufferSize() - index,
                      "Smart Heap (%s) corrupt at 0x%08" PRIX32 "\n",
                      usedOrFreeString,
                      reinterpret_cast<uint32_t>(address));

    // Update the index
    heapCorruptionInfoPtr->setBufferIndex(index);
}

#ifdef TTYD_JP
static void initMapHeapError(const void *address, uint16_t inUse)
#else
static void initMapHeapError(const void *address, uint16_t inUse, bool isBattleHeap)
#endif
{
    // Set up the text to be drawn
    // Make sure heapCorruptioBufferIndex is valid
    HeapCorruptionInfo *heapCorruptionInfoPtr = &heapCorruptionInfo;
    if (!heapCorruptionInfoPtr->verifyBufferIndex())
    {
        return;
    }

    // Get the used or free text
    const char *usedOrFreeString;
    if (inUse)
    {
        usedOrFreeString = "used";
    }
    else
    {
        usedOrFreeString = "free";
    }

#ifdef TTYD_JP
    const char *format = "Map Heap (%s) corrupt at 0x%08" PRIX32 "\n";
#else
    // Get the battle text if checking the battle heap
    const char *currentHeap = nullptr;

    if (isBattleHeap)
    {
        currentHeap = "Battle ";
    }

    const char *format = "%sMap Heap (%s) corrupt at 0x%08" PRIX32 "\n";
#endif
    int32_t index = heapCorruptionInfoPtr->getBufferIndex();

    index += snprintf(heapCorruptionInfoPtr->initBufferEntry(index),
                      heapCorruptionInfoPtr->getBufferSize() - index,
                      format,
#ifndef TTYD_JP
                      currentHeap,
#endif
                      usedOrFreeString,
                      reinterpret_cast<uint32_t>(address));

    // Update the index
    heapCorruptionInfoPtr->setBufferIndex(index);
}

void checkHeaps()
{
    const gc::os::HeapInfo *heapArrayPtr = gc::os::OSAlloc_HeapArray;
    const void *addressWithError;

    // Check the standard heaps
    const int32_t numHeaps = gc::os::OSAlloc_NumHeaps;
    for (int32_t i = 0; i < numHeaps; i++)
    {
        const gc::os::HeapInfo *heapPtr = &heapArrayPtr[i];

        // Check the used entries
        const gc::os::ChunkInfo *tempChunk = heapPtr->firstUsed;
        addressWithError = checkIndividualStandardHeap(tempChunk);
        if (addressWithError)
        {
            initStandardHeapError(addressWithError, i, true);
        }

        // Check the free entries
        tempChunk = heapPtr->firstFree;
        addressWithError = checkIndividualStandardHeap(tempChunk);
        if (addressWithError)
        {
            initStandardHeapError(addressWithError, i, false);
        }
    }

    // Check the smart heap
    const ttyd::memory::SmartWork *smartWorkPtr = ttyd::memory::_smartWorkPtr;

    // Check the used entries
    const ttyd::memory::SmartAllocationData *tempChunk = smartWorkPtr->pFirstUsed;
    addressWithError = checkIndividualSmartHeap(tempChunk);
    if (addressWithError)
    {
        initSmartHeapError(addressWithError, true);
    }

    // Check the free entries
    tempChunk = smartWorkPtr->pFirstFree;
    addressWithError = checkIndividualSmartHeap(tempChunk);
    if (addressWithError)
    {
        initSmartHeapError(addressWithError, false);
    }

    // Check the map heap
    const ttyd::memory::MapAllocEntry *mapHeapPtr = ttyd::memory::mapalloc_base_ptr;
    addressWithError = checkIndividualMapHeap(mapHeapPtr);

    if (addressWithError)
    {
#ifdef TTYD_JP
        initMapHeapError(addressWithError, static_cast<const ttyd::memory::MapAllocEntry *>(addressWithError)->inUse);
    }
#else
        initMapHeapError(addressWithError, static_cast<const ttyd::memory::MapAllocEntry *>(addressWithError)->inUse, false);
    }

    // Check the battle map heap
    mapHeapPtr = ttyd::memory::R_battlemapalloc_base_ptr;
    addressWithError = checkIndividualMapHeap(mapHeapPtr);
    if (addressWithError)
    {
        initMapHeapError(addressWithError, static_cast<const ttyd::memory::MapAllocEntry *>(addressWithError)->inUse, true);
    }
#endif
}

void drawErrorMessages()
{
    // Draw any heap corruption errors that occured this frame
    HeapCorruptionInfo *heapCorruptionInfoPtr = &heapCorruptionInfo;
    if (heapCorruptionInfoPtr->shouldDrawBuffer())
    {
        heapCorruptionInfoPtr->drawBuffer();
    }

    // Draw error text if npcNameToPtr returned an invalid pointer
    NpcNameToPtrErrorInfo *npcNameToPtrErrorInfoPtr = &npcNameToPtrErrorInfo;
    if (npcNameToPtrErrorInfoPtr->getTimer() > 0)
    {
        npcNameToPtrErrorInfoPtr->drawErrorMessage();
    }

    // Draw error text if animPoseMain would cause a crash
    AnimPoseMainErrorInfo *animPoseMainErrorInfoPtr = &animPoseMainErrorInfo;
    if (animPoseMainErrorInfoPtr->getTimer() > 0)
    {
        animPoseMainErrorInfoPtr->drawErrorMessage();
    }

    // Reset errorTextPosY for use on the next frame
    errorTextPosY = ERROR_TEXT_DEFAULT_POS_Y;
}

NpcEntry *checkForNpcNameToPtrError(const char *name)
{
    // Call the original function immediately, as its result is needed
    NpcEntry *ret = g_npcNameToPtr_trampoline(name);

    // Check if the returned pointer is valid
    const NpcWork *workPtr = npcGetWorkPtr();
    if (ret >= &workPtr->entries[workPtr->npcMaxCount])
    {
        // Didn't find the correct NPC, so print error text
        NpcNameToPtrErrorInfo *npcNameToPtrErrorInfoPtr = &npcNameToPtrErrorInfo;
        npcNameToPtrErrorInfoPtr->setTimer(ttyd::system::sysMsec2Frame(5000));
        npcNameToPtrErrorInfoPtr->incrementCounter();
    }

    return ret;
}

void preventAnimPoseMainCrash(int32_t poseId)
{
    // Make sure poseId is valid
    if (poseId < 0)
    {
        // poseId is invalid, so print error text
        AnimPoseMainErrorInfo *animPoseMainErrorInfoPtr = &animPoseMainErrorInfo;
        animPoseMainErrorInfoPtr->setTimer(ttyd::system::sysMsec2Frame(5000));
        animPoseMainErrorInfoPtr->incrementCounter();
        return;
    }

    // Call the original function
    return g_animPoseMain_trampoline(poseId);
}
