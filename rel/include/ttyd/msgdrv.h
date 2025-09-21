#pragma once

#include <cstdint>
#include <ttyd/memory.h>

namespace ttyd::msgdrv
{
    struct TextCommand
    {
        uint32_t flags;
        uint16_t type;
        int16_t char_or_param1;
        int16_t param2;
        int16_t param3;
        uint32_t reserved1;
        float scale;
        float rotation;
    };

    struct MessageData
    {
        uint32_t field_00;
        uint32_t flags;
        uint32_t field_08;
        uint32_t field_0C;
        uint32_t command_count;
        uint32_t field_14;
        uint64_t timestamp;
        uint32_t field_20;
        uint32_t field_24;
        uint32_t currentLine;
        uint32_t field_2C;
        uint32_t field_30;
        uint32_t field_34;
        uint32_t field_38;
        TextCommand commands[];
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
        void msgLoad(const char *filename, int tableIndex);
        void msgInit();
        extern uint32_t *msgw;
        extern uint32_t msgWork[9];
    }
} // namespace ttyd::msgdrv
