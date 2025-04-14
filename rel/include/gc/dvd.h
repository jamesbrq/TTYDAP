#pragma once

#include <cstdint>

#define DVD_STATE_FATAL_ERROR -1
#define DVD_STATE_END 0
#define DVD_STATE_BUSY 1
#define DVD_STATE_WAITING 2
#define DVD_STATE_COVER_CLOSED 3
#define DVD_STATE_NO_DISK 4
#define DVD_STATE_COVER_OPEN 5
#define DVD_STATE_WRONG_DISK 6
#define DVD_STATE_MOTOR_STOPPED 7
#define DVD_STATE_MOTOR_PAUSING 8
#define DVD_STATE_IGNORED 9
#define DVD_STATE_CANCELED 10
#define DVD_STATE_RETRY 11

#define DVD_READ_SIZE 32
#define DVD_OFFSET_SIZE 4

namespace gc::dvd
{
    struct DVDFileInfo;
    struct DVDCommandBlock;
    typedef void (*DVDCallback)(int32_t result, DVDFileInfo *info);
    typedef void (*DVDCBCallback)(int32_t result, DVDCommandBlock *block);

    struct DVDDiskID
    {
        char game_name[4];
        char company[2];
        uint8_t disk_number;
        uint8_t game_version;
        uint8_t is_streaming;
        uint8_t streaming_buffer_size;
        uint8_t padding[22];
    } __attribute__((__packed__));

    struct DVDCommandBlock
    {
        DVDCommandBlock *next;
        DVDCommandBlock *prev;
        uint32_t command;
        int32_t state;
        uint32_t offset;
        uint32_t length;
        void *buffer;
        uint32_t current_transfer_size;
        uint32_t transferred_size;
        DVDDiskID *disk_id;
        DVDCBCallback callback;
        void *user_data;
    } __attribute__((__packed__));

    struct DVDFileInfo
    {
        DVDCommandBlock block;
        uint32_t start_address;
        uint32_t length;
        DVDCallback callback;
    } __attribute__((__packed__));

    struct DVDDirectory
    {
        uint32_t entry_number;
        uint32_t location;
        uint32_t next;
    } __attribute__((__packed__));

    struct DVDDirectoryEntry
    {
        uint32_t entry_number;
        int32_t is_directory; // Handled as a bool
        const char *fileName;
    } __attribute__((__packed__));

    static_assert(sizeof(DVDDiskID) == 0x20);
    static_assert(sizeof(DVDCommandBlock) == 0x30);
    static_assert(sizeof(DVDFileInfo) == 0x3C);
    static_assert(sizeof(DVDDirectory) == 0xC);
    static_assert(sizeof(DVDDirectoryEntry) == 0xC);

    extern "C"
    {
        // __DVDFSInit
        // DVDConvertPathToEntrynum
        bool DVDOpen(const char *fileName, DVDFileInfo *fileInfo);
        bool DVDClose(DVDFileInfo *fileInfo);
        // entryToPath
        // DVDGetCurrentDir
        int32_t DVDReadPrio(DVDFileInfo *fileInfo, void *buffer, int32_t length, int32_t offset, int32_t priority);
        // cbForReadSync

        inline int32_t DVDRead(DVDFileInfo *fileInfo, void *buffer, int32_t length, int32_t offset)
        {
            return DVDReadPrio(fileInfo, buffer, length, offset, 2);
        }
    }
} // namespace gc::dvd
