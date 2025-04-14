#include "cxx.h"
#include "ttyd/memory.h"

#include <cstdint>

void *allocMemory(std::size_t size)
{
    return ttyd::memory::__memAlloc(ttyd::memory::HeapType::HEAP_DEFAULT, size);
}

void freeMemory(void *ptr)
{
    return ttyd::memory::__memFree(ttyd::memory::HeapType::HEAP_DEFAULT, ptr);
}
