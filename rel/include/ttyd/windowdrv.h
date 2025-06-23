#pragma once

#include <cstdint>
#include <ttyd/memory.h>

namespace ttyd::windowdrv
{
    struct Window
    {
        uint16_t windowState;
        uint16_t flags;
        int16_t alpha;
        uint16_t padding_06;
        uint32_t param;
        float x, y, width, height;
        uint8_t pad_1C[0x0C];
        ttyd::memory::SmartAllocationData *msgData;
        uint8_t pad_2C[0x10];
        void *mainFunc;
        void *deleteFunc;
        uint8_t pad_44[0x100];
    };

    extern "C"
    {
        void windowDispGX_Waku_col(int32_t cameraId, uint32_t *color, float x, float y, float width, float height, float depth);
        Window *windowGetPointer(int window_id);
        void windowDeleteID(int window_id);
        int windowCheckID(int window_id);
    }
}