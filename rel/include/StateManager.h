#pragma once

#include <cstdint>

namespace mod::owr
{
    struct APSettings
    {
        uint8_t requiredChapterClears;
        uint8_t startingPartner;
        uint8_t yoshiColor;
        uint8_t apEnabled;
        char *yoshiName;
        uint8_t inGame;
        uint8_t palaceSkip;
    };

    static_assert(sizeof(APSettings) == 0xC);

    class StateManager
    {
       public:
        void Init();
        StateManager();
        APSettings *apSettings;
    };
} // namespace mod::owr
