#pragma once
#include <cstdint>

// Loads custom.rel into the ARAM-backed virtual window (0x81800000) instead of
// keeping it resident in MEM1. Returns true on success. See vm_customrel.cpp.
//
// cacheBytes          : steady-state working-set cache size (paged mode).
// prefetchAllForTest  : if true, page in the whole relocated image immediately
//                       (requires cacheBytes >= fixed size) for a fault-free
//                       Stage-3 correctness test.
bool LoadCustomRelVM(const char *relName = "custom", uint32_t cacheBytes = 0x20000); // 32 pages: 20 lockable + 12 free; sweep down from here, watch FAULTCNT (0x80003B58)

// Loads TWO rels packed into the same VM window (A at page 0, B right after).
// Shares one ARAM region/HTAB/cache. Falls back to LoadCustomRelVM(nameA) if
// the second rel is absent on disc. Returns true on success.
bool LoadBothCustomRelsVM(const char *nameA = "custom", const char *nameB = "custom2", uint32_t cacheBytes = 0x20000);
