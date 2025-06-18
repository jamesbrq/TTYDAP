#pragma once
#include <cstdint>

namespace ttyd::msgdrv
{
    // Forward declaration of structures
    struct MsgData;
    struct SmartAlloc;
    struct Window;

    // MsgData structure (partial definition with layout section)
    struct MsgData
    {
        void *msgBuffer;        // 0x00 - Message buffer pointer
        uint32_t flags;         // 0x04 - Message flags
        uint8_t pad_08[0x10];   // 0x08
        float pos1[2];          // 0x18 - Position data
        uint8_t pad_20[0x10];   // 0x20
        float pos2[2];          // 0x30 - More position data
        uint8_t pad_38[0xF1C4]; // 0x38 - Large padding to layout section

        // Layout fields at high offsets (0xF1FC+)
        union
        {
            uint8_t rawLayout[0xE04];
            struct
            {
                uint8_t pad_00[0x44];     // 0x00 (0xF1FC)
                uint8_t numOptions;       // 0x44 (0xF240)
                uint8_t selectedOption;   // 0x45 (0xF241)
                uint8_t cancelOption;     // 0x46 (0xF242)
                uint8_t currentOption;    // 0x47 (0xF243)
                uint16_t width;           // 0x48 (0xF244)
                uint16_t height;          // 0x4A (0xF246)
                float bottomY;            // 0x4C (0xF248)
                Window *windowPtr;        // 0x50 (0xF24C)
                uint8_t pad_54[0x04];     // 0x54 (0xF250)
                uint32_t minValue;        // 0x58 (0xF254) - For numeric windows
                uint32_t maxValue;        // 0x5C (0xF258) - For numeric windows
                uint32_t currentValue;    // 0x60 (0xF25C) - For numeric windows
                uint8_t remaining[0xDA0]; // Rest of layout data
            } fields;
        } layout;
    };

    struct SmartAlloc
    {
        MsgData *ptr;
    };

    // Window structure (partial definition - used by msgdrv)
    struct Window
    {
        uint16_t windowType;        // 0x00 - Current state of window
        uint16_t flags;             // 0x02 - Window flags
        uint16_t subType;           // 0x04 - Alpha value during fade transitions
        uint16_t padding_06;        // 0x06
        uint32_t param;             // 0x08 - Window parameter
        float x;                    // 0x0C - X position
        float y;                    // 0x10 - Y position
        float width;                // 0x14 - Window width
        float height;               // 0x18 - Window height
        uint8_t pad_1C[0x0C];       // 0x1C
        SmartAlloc *msgData;        // 0x28 - Pointer to message data
        uint8_t pad_2C[0x10];       // 0x2C
        void (*mainFunc)(Window *); // 0x3C - Main function pointer
        void (*deleteFunc)(void);   // 0x40 - Delete function pointer
        uint8_t pad_44[0x100];      // 0x44+ (rest of structure)
    };

// Helper macros for accessing high-offset fields in MsgData
#define MSG_NUM_OPTIONS(msgData) (*(uint8_t *)((uint8_t *)(msgData) + 0xF240))
#define MSG_CURRENT_OPT(msgData) (*(uint8_t *)((uint8_t *)(msgData) + 0xF243))
#define MSG_SELECTED_OPT(msgData) (*(uint8_t *)((uint8_t *)(msgData) + 0xF241))
#define MSG_CANCEL_OPT(msgData) (*(uint8_t *)((uint8_t *)(msgData) + 0xF242))
#define MSG_WIDTH(msgData) (*(uint16_t *)((uint8_t *)(msgData) + 0xF244))
#define MSG_HEIGHT(msgData) (*(uint16_t *)((uint8_t *)(msgData) + 0xF246))
#define MSG_BOTTOM_Y(msgData) (*(float *)((uint8_t *)(msgData) + 0xF248))
#define MSG_WINDOW_PTR(msgData) (*(Window **)((uint8_t *)(msgData) + 0xF24C))
#define MSG_MIN_VALUE(msgData) (*(uint32_t *)((uint8_t *)(msgData) + 0xF254))
#define MSG_MAX_VALUE(msgData) (*(uint32_t *)((uint8_t *)(msgData) + 0xF258))
#define MSG_CURRENT_VALUE(msgData) (*(uint32_t *)((uint8_t *)(msgData) + 0xF25C))

    // Window type constants
    enum WindowType
    {
        WINDOW_TYPE_INACTIVE = 0,
        WINDOW_TYPE_ACTIVE = 1,
        WINDOW_TYPE_PAUSED = 2,
        WINDOW_TYPE_CLOSING = 3,
        WINDOW_TYPE_COMPLETE = 4,
        WINDOW_TYPE_FADE_IN = 5,
        WINDOW_TYPE_NUMERIC = 6, // Custom numeric input type
        WINDOW_TYPE_FADE_OUT = 7
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
        int32_t msgWindow_Entry(const char *msgText, int32_t unused, int32_t windowType);
        // msgIconStr2ID
        // msgGetCommand
        // _ismbblead
        const char *msgSearch(const char *msgKey);
        // msg_compare
        // ?msgGetWork
        // msgAnalize
        // msgDispKeyWait_render
        void msgDisp(SmartAlloc *msgDataAlloc, float x, float y, int32_t alpha);
        void msgMain(SmartAlloc *msgDataAlloc);
        // msgLoad
        // msgInit

        extern uint32_t unk_80429584;
        extern uint32_t dat_804205fc;
    }
} // namespace ttyd::msgdrv