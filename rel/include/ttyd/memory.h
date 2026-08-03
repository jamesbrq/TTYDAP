#pragma once

#include <cstdint>

namespace ttyd::memory
{
    struct SmartAllocationData
    {
        void *pMemory;
        uint32_t usedSize;
        void *pFileInfo; // FileEntryData *pFileInfo;
        uint16_t bUsed;
        uint8_t group;
        uint8_t pad_f;
        uint32_t unusedSize;
        SmartAllocationData *pNext;
        SmartAllocationData *pPrev;
    } __attribute__((__packed__));

    struct SmartWork
    {
        void *pArena;
        SmartAllocationData allocations[2048];
        uint32_t unallocatedArenaStartSize;
        SmartAllocationData *pFirstUsed;
        SmartAllocationData *pLastUsed;
        SmartAllocationData *pFirstFree;
        SmartAllocationData *pLastFree;
        uint32_t lastFrameFreeCount;
    } __attribute__((__packed__));

    struct MapAllocEntry
    {
        MapAllocEntry *next;
        uint32_t size;
        uint16_t inUse;
        uint16_t currentId;
        uint8_t unk[0x14];
    } __attribute__((__packed__));

    enum HeapType : int32_t
    {
        HEAP_DEFAULT = 0,
        HEAP_MAP,
        HEAP_EXT,
        HEAP_EFFECT,
#ifdef TTYD_JP
        HEAP_BATTLE,
#endif
        HEAP_SMART,
    };

    struct HeapEnd
    {
        void *pHeapDefault;
        void *pHeapMap;
        void *pHeapExt;
        void *pHeapEffect;
#ifdef TTYD_JP
        void *pHeapBattle;
#endif
        void *pHeapSmart;
    } __attribute__((__packed__));

    struct HeapStart
    {
        void *pHeapDefault;
        void *pHeapMap;
        void *pHeapExt;
        void *pHeapEffect;
#ifdef TTYD_JP
        void *pHeapBattle;
#endif
        void *pHeapSmart;
    } __attribute__((__packed__));

    static_assert(sizeof(SmartAllocationData) == 0x1C);
    static_assert(sizeof(SmartWork) == 0xE01C);
    static_assert(sizeof(MapAllocEntry) == 0x20);

    #ifdef TTYD_JP
    static_assert(sizeof(HeapEnd) == 0x18);
    static_assert(sizeof(HeapStart) == 0x18);
#else
    static_assert(sizeof(HeapEnd) == 0x14);
    static_assert(sizeof(HeapStart) == 0x14);
#endif

    extern "C"
    {
#ifndef TTYD_JP
        extern MapAllocEntry *R_battlemapalloc_base_ptr;
        extern uint32_t R_battlemapalloc_size;
#endif
        extern SmartWork *_smartWorkPtr; // wp
        extern uint32_t mapalloc_size;
        extern MapAllocEntry *mapalloc_base_ptr;

        extern HeapEnd heapEnd;
        extern HeapStart heapStart;

        // memInit
        // memClear
        void *__memAlloc(int32_t heap, uint32_t size);
        void __memFree(int32_t heap, void *ptr);
        // _mapAlloc
        // _mapAllocTail
        // _mapFree
        // smartInit
        // smartReInit
        // smartAutoFree
        // smartFree
        // smartAlloc
        // smartGarbage
        // smartTexObj
    }
} // namespace ttyd::memory
