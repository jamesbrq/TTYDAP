#pragma once

#include <cstdint>

void *allocMemory(std::size_t size);
void freeMemory(void *ptr);

inline void *operator new(std::size_t size)
{
    return allocMemory(size);
}

inline void *operator new[](std::size_t size)
{
    return allocMemory(size);
}

inline void operator delete(void *ptr)
{
    return freeMemory(ptr);
}

inline void operator delete[](void *ptr)
{
    return freeMemory(ptr);
}

inline void operator delete(void *ptr, std::size_t size)
{
    (void)size;
    return freeMemory(ptr);
}

inline void operator delete[](void *ptr, std::size_t size)
{
    (void)size;
    return freeMemory(ptr);
}
