#pragma once
#include <cstdint>

namespace ttyd::windowdrv
{
    extern "C"
    {
        void windowDispGX_Waku_col(int32_t param, float x, float y, float width, float height, float f5, void *color);
        Window *windowGetPointer(int32_t windowId);
        int windowCheckID(int32_t windowId);
        void windowDeleteID(int32_t windowId);
    }
} // namespace ttyd::windowdrv