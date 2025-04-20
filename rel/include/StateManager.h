#pragma once

#include <cstdint>
#include <ttyd/party.h>

namespace mod::owr
{
    struct APSettings
    {
        uint8_t requiredChapterClears;
        ttyd::party::PartyMembers startingPartner;
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
