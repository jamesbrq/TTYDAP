#include "SmartHeap.h"
#include "patch.h"
#include "visibility.h"
#include "gc/os.h"
#include "ttyd/memory.h"
#include "ttyd/seq_mapchange.h"
#include <cstdint>
#include <cstring>

extern "C" void SmartAllocFailure(), SmartAllocFailureReturn();

namespace mod::smart_heap {
using namespace ttyd::memory;
namespace {
using AllocFn = SmartAllocationData *(*)(uint32_t, uint32_t);
using FreeFn = void (*)(SmartAllocationData *);
AllocFn originalAlloc;
FreeFn originalFree;
uint32_t owners[2048];
uint32_t serial;
uint32_t allocDepth;
struct Recent { uint32_t caller, size, group, node; } recent[32];
struct Scan {
    uint8_t seen[512] = {};
    uint8_t Member(int i) const { return (seen[i / 4] >> (2 * (i % 4))) & 3; }
    void Mark(int i, uint8_t value) { seen[i / 4] |= value << (2 * (i % 4)); }
    uint32_t used = 0, free = 0, orphan = 0, bytes = 0, slack = 0;
    uint16_t groups[256] = {};
    bool valid = true;
};
int Index(SmartAllocationData *node) {
    auto *work = _smartWorkPtr;
    if (!work) return -1;
    uintptr_t first = reinterpret_cast<uintptr_t>(work->allocations);
    uintptr_t address = reinterpret_cast<uintptr_t>(node);
    if (address < first || address >= first + sizeof(work->allocations) ||
        (address - first) % sizeof(SmartAllocationData)) return -1;
    return (address - first) / sizeof(SmartAllocationData);
}
bool InMri() { return std::strncmp(ttyd::seq_mapchange::_next_map, "mri_", 4) == 0; }
void Walk(Scan &scan, SmartAllocationData *node, SmartAllocationData *tail, bool used) {
    SmartAllocationData *previous = nullptr;
    while (node) {
        int i = Index(node);
        if (i < 0 || scan.Member(i) || node->pPrev != previous || (!!node->bUsed != used)) {
            gc::os::OSReport("SMART bad-list used=%u node=%p prev=%p index=%d\n", used, node, previous, i);
            scan.valid = false;
            return;
        }
        scan.Mark(i, used ? 1 : 2);
        if (used) {
            uintptr_t expected = previous ? reinterpret_cast<uintptr_t>(previous->pMemory) +
                previous->usedSize + previous->unusedSize :
                reinterpret_cast<uintptr_t>(_smartWorkPtr->pArena) + _smartWorkPtr->unallocatedArenaStartSize;
            if (reinterpret_cast<uintptr_t>(node->pMemory) != expected) {
                gc::os::OSReport("SMART bad-gap node=%p memory=%p expected=%08x\n", node, node->pMemory,
                                 static_cast<uint32_t>(expected));
                scan.valid = false;
            }
            ++scan.used; ++scan.groups[node->group];
            scan.bytes += node->usedSize; scan.slack += node->unusedSize;
            uintptr_t lo = reinterpret_cast<uintptr_t>(heapStart.pHeapSmart);
            uintptr_t hi = reinterpret_cast<uintptr_t>(heapEnd.pHeapSmart);
            uintptr_t address = reinterpret_cast<uintptr_t>(node->pMemory);
            if (address < lo || address >= hi || node->usedSize > hi - address ||
                node->unusedSize > hi - address - node->usedSize) {
                gc::os::OSReport("SMART bad-range node=%p memory=%p bytes=%u owner=%08x\n",
                                 node, node->pMemory, node->usedSize, owners[i]);
                scan.valid = false;
            }
        } else ++scan.free;
        previous = node; node = node->pNext;
    }
    if (previous != tail) scan.valid = false;
}
Scan Inspect() {
    Scan scan;
    if (!_smartWorkPtr) { scan.valid = false; return scan; }
    Walk(scan, _smartWorkPtr->pFirstUsed, _smartWorkPtr->pLastUsed, true);
    Walk(scan, _smartWorkPtr->pFirstFree, _smartWorkPtr->pLastFree, false);
    scan.slack += _smartWorkPtr->unallocatedArenaStartSize;
    for (int i = 0; i < 2048; ++i) if (!scan.Member(i)) ++scan.orphan;
    return scan;
}
void Report(const char *reason, const Scan &s) {
    gc::os::OSReport("SMART %s map=%s serial=%u used=%u free=%u orphan=%u bytes=%u slack=%u valid=%u\n",
        reason, ttyd::seq_mapchange::_next_map, serial, s.used, s.free, s.orphan, s.bytes, s.slack, s.valid);
    for (int g = 0; g < 256; ++g) if (s.groups[g])
        gc::os::OSReport("SMART group=%d records=%u\n", g, s.groups[g]);
}
void Stop(const char *reason, const Scan &s) {
    Report(reason, s);
    for (uint32_t n = serial > 32 ? serial - 32 : 0; n < serial; ++n) {
        const auto &r = recent[n % 32];
        gc::os::OSReport("SMART recent=%u caller=%08x size=%u group=%u node=%08x\n",
                         n, r.caller, r.size, r.group, r.node);
    }
    for (int i = 0; i < 2048; ++i) if (s.Member(i) == 1 || !s.Member(i)) {
        const auto &node = _smartWorkPtr->allocations[i];
        gc::os::OSReport("SMART record=%d membership=%u used=%u group=%u size=%u file=%p owner=%08x next=%p prev=%p\n",
            i, s.Member(i), node.bUsed, node.group, node.usedSize, node.pFileInfo, owners[i], node.pNext, node.pPrev);
    }
    // SDK OSPanic takes source file and line before the format string.
    reinterpret_cast<void (*)(const char *, int, const char *, ...)>(gc::os::OSPanic)(
        __FILE__, __LINE__, "Smart heap: %s in %s", reason, ttyd::seq_mapchange::_next_map);
    for (;;) {}
}
KEEP_FUNC SmartAllocationData *Allocate(uint32_t size, uint32_t group) {
    uint32_t caller = reinterpret_cast<uint32_t>(__builtin_return_address(0));
    if (_smartWorkPtr && (InMri() || !_smartWorkPtr->pFirstFree)) {
        Scan before = Inspect();
        if (!before.valid || before.orphan || !before.free)
            gc::os::OSReport("SMART request caller=%08x size=%u group=%u\n", caller, size, group);
        if (!before.valid || before.orphan) Stop("corruption before allocation", before);
        if (!before.free) {
            Report("record exhaustion before cache collection", before);
#ifdef TTYD_US
            // Mirror native transient cleanup before forcing file-cache collection.
            reinterpret_cast<void (*)(int)>(0x8002f9cc)(3);
            // Native smartAlloc only collects files after failing to find bytes;
            // collect unused cached files BEFORE it dereferences an empty list.
            reinterpret_cast<void (*)(int)>(0x8018a6e4)(0);
#endif
            before = Inspect();
            if (!before.valid || before.orphan || !before.free) Stop("record exhaustion", before);
            Report("records recovered from file cache", before);
        }
    }
    ++allocDepth;
    auto *node = originalAlloc(size, group);
    --allocDepth;
    recent[serial % 32] = {caller, size, group, reinterpret_cast<uint32_t>(node)};
    ++serial;
    int index = Index(node);
    if (index >= 0) owners[index] = caller;
    if (!node) {
        Scan after = Inspect();
        Report("allocation failed after returning reserved record", after);
        if (!after.valid || after.orphan) Stop("corruption on allocation failure", after);
        if (InMri()) Stop("allocation failed", after);
        return nullptr;
    }
    if (InMri()) {
        Scan after = Inspect();
        if (!after.valid || after.orphan) Stop("corruption after allocation", after);
        if (serial % 128 == 0 || after.free < 128) Report("allocation checkpoint", after);
    }
    return node;
}
KEEP_FUNC void Free(SmartAllocationData *node) {
    if (!node) return;
    int index = Index(node);
    if (index < 0 || !node->bUsed) {
        gc::os::OSReport("SMART free caller=%p node=%p\n", __builtin_return_address(0), node);
        Stop("invalid or repeated free", Inspect());
    }
    if (InMri()) {
        Scan before = Inspect();
        if (!before.valid || before.Member(index) != 1 || (before.orphan && !allocDepth)) Stop("corruption before free", before);
    }
    originalFree(node);
    if (!node->bUsed) owners[index] = 0;
    if (InMri()) {
        Scan after = Inspect();
        if (!after.valid || (after.orphan && !allocDepth)) Stop("corruption after free", after);
    }
}
}
KEEP_FUNC void Checkpoint(const char *reason) {
#ifdef TTYD_US
    Scan scan = Inspect();
    Report(reason, scan);
    if (!scan.valid || scan.orphan) Stop("checkpoint corruption", scan);
#else
    (void)reason;
#endif
}
extern "C" KEEP_FUNC void SmartReturnFailedRecord(SmartAllocationData *node) {
    Scan scan = Inspect();
    int index = Index(node);
    if (!scan.valid || index < 0 || scan.Member(index) || !node->bUsed || scan.orphan != 1)
        Stop("invalid failed-allocation record", scan);
    node->bUsed = 0; node->usedSize = 0; node->unusedSize = 0;
    node->pMemory = nullptr; node->pFileInfo = nullptr; node->group = 0;
    node->pNext = nullptr; node->pPrev = _smartWorkPtr->pLastFree;
    if (node->pPrev) node->pPrev->pNext = node;
    else _smartWorkPtr->pFirstFree = node;
    _smartWorkPtr->pLastFree = node;
    owners[index] = 0;
    Report("returned failed-allocation record", Inspect());
}
KEEP_FUNC void Init() {
#ifdef TTYD_US
    if (originalAlloc) return;
    originalAlloc = patch::hookFunction(reinterpret_cast<AllocFn>(0x8002f540), Allocate);
    originalFree = patch::hookFunction(reinterpret_cast<FreeFn>(0x8002f878), Free);

    patch::writeBranchPair(reinterpret_cast<void *>(0x8002f858),
                          reinterpret_cast<void *>(SmartAllocFailure), reinterpret_cast<void *>(SmartAllocFailureReturn));
#endif
}
}
