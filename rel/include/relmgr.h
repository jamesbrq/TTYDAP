#pragma once

#include "ttyd/memory.h"
#include "ttyd/seq_mapchange.h"
#include "gc/OSModule.h"

#include <cstdint>

class RelMgr
{
   public:
    RelMgr() {}
    ~RelMgr() {}

    // For `relToLoad`, do not include the file path nor the extension
    bool loadRel(const char *relToLoad, int32_t heap = ttyd::memory::HeapType::HEAP_DEFAULT);

    // Loads and links custom.rel permanently on the default heap.
    // Not tracked by relPtr/bssPtr, so the temp rel lifecycle won't unload it.
    bool loadCustomRel();

    bool linkRel();
    bool unlinkRel();
    void unloadRel();
    void runInitRel();

    const char *getPrevArea() const { return &this->prevArea[0]; }

    void setPrevArea(const char *area)
    {
        // Cast to uint32_t to simplify getting/setting the value
        uint32_t *prevAreaPtr = reinterpret_cast<uint32_t *>(&this->prevArea[0]);
        const uint32_t *areaPtr = reinterpret_cast<const uint32_t *>(&area[0]);
        *prevAreaPtr = *areaPtr;
    }

    bool inNewArea(const char *area) const
    {
        // Cast to uint32_t to simplify comparison
        const uint32_t *prevAreaPtr = reinterpret_cast<const uint32_t *>(&this->prevArea[0]);
        const uint32_t *areaPtr = reinterpret_cast<const uint32_t *>(&area[0]);
        return *prevAreaPtr != *areaPtr;
    }

    void setHeap(int32_t newHeap) { this->heap = static_cast<uint8_t>(newHeap); }
    int32_t getHeap() const { return static_cast<int32_t>(this->heap); }

    bool relIsLoaded() const { return this->relPtr != nullptr; }

   private:
    OSModuleInfo *relPtr;
    void *bssPtr;
    char prevArea[5];
    uint8_t heap;
};

extern RelMgr relMgr;
