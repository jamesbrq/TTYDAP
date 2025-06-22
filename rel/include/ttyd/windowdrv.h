#pragma once

#include <cstdint>
#include <ttyd/memory.h>

namespace ttyd::windowdrv
{
    // Window structure (partial definition - used by msgdrv)
    struct Window
    {
        uint16_t windowState;       // 0x00 - Current state of window
        uint16_t flags;             // 0x02 - Window flags
        int16_t alpha;             // 0x04 - Alpha value during fade transitions
        uint16_t padding_06;        // 0x06
        uint32_t param;             // 0x08 - Window parameter
        float x;                    // 0x0C - X position
        float y;                    // 0x10 - Y position
        float width;                // 0x14 - Window width
        float height;               // 0x18 - Window height
        uint8_t pad_1C[0x0C];       // 0x1C
        ttyd::memory::SmartAllocationData *msgData; // 0x28 - Pointer to message data
        uint8_t pad_2C[0x10];       // 0x2C
        void *mainFunc; // 0x3C - Main function pointer
        void *deleteFunc;   // 0x40 - Delete function pointer
        uint8_t pad_44[0x100];      // 0x44+ (rest of structure)
    };

    extern "C"
    {
        void windowDispGX_Waku_col(int32_t cameraId, uint32_t *color, float x, float y, float width, float height, float depth);
        Window *windowGetPointer(int window_id);
        void windowDeleteID(int window_id);
        int windowCheckID(int window_id);
    }
}