#include "StateManager.h"
#include "OWR.h"
#include "gc/dvd.h"
#include "ttyd/memory.h"
#include "string.h"

#include <cstdint>
#include <cstdio>

using namespace mod::owr;
using namespace gc::dvd;
using namespace ttyd::memory;

StateManager::StateManager()
{
    gState = this;
    gState->Init();
    for (int i = 0; i < 8; i++) gState->state_msgWork[i] = ttyd::msgdrv::msgWork[i]; // msgWork.entries[0] && msgWork.entries[1]
    gState->state_msgWork[16] = ttyd::msgdrv::msgWork[8]; // msgWork.animBase
    gState->LoadEnemyData();
}

void StateManager::Init()
{
    gState->apSettings = reinterpret_cast<APSettings *>(0x80003220);
    gState->tattleItems = reinterpret_cast<uint16_t *>(0x80003B00);
    gState->maniacItems = reinterpret_cast<uint16_t *>(0x80003400);
}

size_t StateManager::LoadEntranceData()
{
    // Set up the filepath
    char buf[64];
    snprintf(buf, sizeof(buf), "/msg/US/warp.txt");
    // Open the file
    DVDFileInfo fileInfo;
    if (!DVDOpen(buf, &fileInfo))
    {
        return 0;
    }
    // Get the size of the file
    uint32_t size = fileInfo.length;
    // Make sure the size is a multiple of DVD_READ_SIZE
    uint32_t alignedSize = (size + DVD_READ_SIZE - 1) & ~(DVD_READ_SIZE - 1);
    // Allocate temporary memory for the file
    uint8_t *buffer = reinterpret_cast<uint8_t *>(__memAlloc(HeapType::HEAP_DEFAULT, alignedSize));
    if (!buffer)
    {
        DVDClose(&fileInfo);
        return 0;
    }
    // Read the file from the disc
    const int32_t result = DVDReadPrio(&fileInfo, buffer, alignedSize, 0, 0);
    // Close the file
    DVDClose(&fileInfo);
    // Make sure the file was successfully read
    if (result <= 0)
    {
        __memFree(HeapType::HEAP_DEFAULT, buffer);
        return 0;
    }
    // Parse the buffer into the entrance data array
    const char *ptr = reinterpret_cast<const char *>(buffer);
    const char *end = ptr + size;
    size_t entriesLoaded = 0;
    while (ptr < end)
    {
        // Read srcMap
        size_t len = strlen(ptr);
        if (ptr + len >= end)
            break;
        size_t copyLen = (len < NAME_LEN) ? len + 1 : NAME_LEN;
        memcpy(entranceData[entriesLoaded].srcMap, ptr, copyLen);
        if (len >= NAME_LEN)
            entranceData[entriesLoaded].srcMap[NAME_LEN - 1] = '\0';
        ptr += len + 1;
        // Read srcBero
        len = strlen(ptr);
        if (ptr + len >= end)
            break;
        copyLen = (len < NAME_LEN) ? len + 1 : NAME_LEN;
        memcpy(entranceData[entriesLoaded].srcBero, ptr, copyLen);
        if (len >= NAME_LEN)
            entranceData[entriesLoaded].srcBero[NAME_LEN - 1] = '\0';
        ptr += len + 1;
        // Read destMap
        len = strlen(ptr);
        if (ptr + len >= end)
            break;
        copyLen = (len < NAME_LEN) ? len + 1 : NAME_LEN;
        memcpy(entranceData[entriesLoaded].destMap, ptr, copyLen);
        if (len >= NAME_LEN)
            entranceData[entriesLoaded].destMap[NAME_LEN - 1] = '\0';
        ptr += len + 1;
        // Read destBero
        len = strlen(ptr);
        if (ptr + len >= end)
            break;
        copyLen = (len < NAME_LEN) ? len + 1 : NAME_LEN;
        memcpy(entranceData[entriesLoaded].destBero, ptr, copyLen);
        if (len >= NAME_LEN)
            entranceData[entriesLoaded].destBero[NAME_LEN - 1] = '\0';
        ptr += len + 1;
        entriesLoaded++;
    }
    // Free the temporary buffer
    __memFree(HeapType::HEAP_DEFAULT, buffer);
    return entriesLoaded;
}

size_t StateManager::LoadEnemyData()
{
    char buf[64];
    snprintf(buf, sizeof(buf), "/mod/enemies.bin");

    DVDFileInfo fileInfo;
    if (!DVDOpen(buf, &fileInfo))
        return 0;

    uint32_t size = fileInfo.length;
    uint32_t alignedSize = (size + DVD_READ_SIZE - 1) & ~(DVD_READ_SIZE - 1);

    uint8_t *buffer = reinterpret_cast<uint8_t *>(__memAlloc(HeapType::HEAP_DEFAULT, alignedSize));
    if (!buffer)
    {
        DVDClose(&fileInfo);
        return 0;
    }

    int32_t result = DVDReadPrio(&fileInfo, buffer, alignedSize, 0, 0);
    DVDClose(&fileInfo);

    if (result <= 0)
    {
        __memFree(HeapType::HEAP_DEFAULT, buffer);
        return 0;
    }

    const uint8_t *ptr = buffer;
    uint16_t numEntries = (ptr[0] << 8) | ptr[1];
    ptr += 2;

    for (uint16_t i = 0; i < numEntries && i < NUM_BATTLE_GROUPS; i++)
    {
        uint8_t enemyCount = *ptr++;
        enemyLoadouts[i].enemyCount = enemyCount;

        for (uint8_t j = 0; j < 5; j++)
        {
            enemyLoadouts[i].enemyIds[j] = 0;
        }

        for (uint8_t j = 0; j < enemyCount; j++)
        {
            enemyLoadouts[i].enemyIds[j] = *ptr++;
        }
    }

    __memFree(HeapType::HEAP_DEFAULT, buffer);
    return numEntries;
}
