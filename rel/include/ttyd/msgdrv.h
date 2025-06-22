#pragma once

#include <cstdint>
#include <ttyd/memory.h>

namespace ttyd::msgdrv
{
    struct TextCommand
    {
        uint32_t flags;          // 0x00 - Command flags
        uint16_t type;           // 0x04 - Command type (character code or special command)
        uint16_t char_or_param1; // 0x06 - Character code or first parameter
        int16_t param2;          // 0x08 - Second parameter
        int16_t param3;          // 0x0A - Third parameter
        uint32_t timing;         // 0x0C - Timing information
        float scale;             // 0x10 - Text scale
        float shadow_offset;     // 0x14 - Shadow offset
    };

    struct MessageData
    {
        uint32_t flags;                 // 0x00 - Message flags (including pause flag)
        uint32_t padding1[14];          // 0x04-0x38 - Various message system data
        uint32_t command_count;         // 0x3C - Number of text commands
        uint32_t current_command_index; // 0x40 - Current processing position
        uint32_t padding2[6];           // 0x44-0x58 - More message data
        TextCommand commands[];         // 0x5C+ - Array of text commands
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
        void msgAnalize(MessageData *msg_data, const char *text);
        // msgDispKeyWait_render
        void msgDisp(ttyd::memory::SmartAllocationData *msg_data, float x, float y, uint8_t alpha);
        void msgMain(ttyd::memory::SmartAllocationData *msg_data);
        // msgLoad
        // msgInit
    }
} // namespace ttyd::msgdrv
