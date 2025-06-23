#pragma once

#include <cstdint>
#include <ttyd/memory.h>

namespace ttyd::msgdrv
{
    struct TextCommand
    {
        uint32_t flags;      // 0x00 - Main data/color value
        uint16_t type;    // 0x04 - Command type
        int16_t char_or_param1; // 0x06 - First parameter (often Y coordinate)
        int16_t param2;     // 0x08 - Second parameter (often X coordinate)
        int16_t param3;     // 0x0A - Third parameter (width/additional data)
        uint32_t reserved1; // 0x0C - Reserved/padding
        float scale;        // 0x10 - Scale factor
        float rotation;     // 0x14 - Rotation angle
    };

    // Message display context structure
    struct MessageData
    {
        uint32_t field_00;
        uint32_t flags; // 0x04
        uint32_t field_08;
        uint32_t field_0C;
        uint32_t command_count; // 0x10 - Number of commands
        uint32_t field_14;
        uint64_t timestamp; // 0x18 - Used for timing effects
        uint32_t field_20;
        uint32_t field_24;
        uint32_t currentLine; // 0x28 - Current line being processed
        uint32_t field_2C;
        uint32_t field_30;
        uint32_t field_34;
        uint32_t field_38;
        TextCommand commands[]; // 0x3C - Array of text commands
    };

    extern "C"
    {
        // selectWindow_Disp
        // selectWindow_Main
        // msgWindow_Disp
        // msgWindow_Clear_Main
        // msgWindow_Main
        // msgWindow_ForceClose
        // msgWindow_Repeat
        // msgWindow_Continue
        // msgWindow_Add
        // msgWindow_Delete
        // msgIconStr2ID
        // msgGetCommand
        // _ismbblead
        int msgWindow_Entry(const char *message, int unk1, int windowType);
        const char *msgSearch(const char *msgKey);
        // msg_compare
        // ?msgGetWork
        void msgAnalize(ttyd::memory::SmartAllocationData *msg_data, const char *text);
        // msgDispKeyWait_render
        void msgDisp(ttyd::memory::SmartAllocationData *msg_data, float x, float y, uint8_t alpha);
        void msgMain(ttyd::memory::SmartAllocationData *msg_data);
        // msgLoad
        // msgInit
    }
} // namespace ttyd::msgdrv
