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
    };

    static_assert(sizeof(APSettings) == 0x8);

    class StateManager
    {
       public:
        void Init();
        StateManager();
        APSettings *apSettings;
    };
} // namespace mod::owr
