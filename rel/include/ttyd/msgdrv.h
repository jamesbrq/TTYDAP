#pragma once

#include <cstdint>

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

    struct NumSelectWindow
    {
        uint16_t state;                // 0x00 - Window state
        uint16_t flags;                // 0x02 - Window flags
        int16_t depth_offset;          // 0x04 - Depth for layering
        int16_t alpha;                 // 0x06 - Alpha for fade effects (0-255)
        MessageData *parent_msg_data;  // 0x08 - Parent message that triggered this window
        float pos_x;                   // 0x0C - X position
        float pos_y;                   // 0x10 - Y position
        float width;                   // 0x14 - Window width
        float height;                  // 0x18 - Window height
        int current_value;             // 0x1C - Current selected number
        int min_value;                 // 0x20 - Minimum allowed value
        int max_value;                 // 0x24 - Maximum allowed value
        int initial_value;             // 0x28 - Starting value
        MessageData *display_msg_data; // 0x2C - MessageData for rendering the number
        // Total size: 0x30 (48 bytes)
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
        // msgWindow_Entry
        // msgIconStr2ID
        // msgGetCommand
        // _ismbblead
        const char *msgSearch(const char *msgKey);
        // msg_compare
        // ?msgGetWork
        void msgAnalize(MessageData *msg_data, const char *text);
        // msgDispKeyWait_render
        // msgDisp
        void msgMain(MessageData *msg_data);
        // msgLoad
        // msgInit
    }
} // namespace ttyd::msgdrv
