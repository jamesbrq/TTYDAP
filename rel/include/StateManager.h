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
        uint8_t openWestside;
        uint8_t peekaboo;
        uint8_t intermissions;
        uint8_t startingHP;
        uint8_t startingFP;
        uint8_t startingBP;
    };

    static_assert(sizeof(APSettings) == 0x10);

    class StateManager
    {
       public:
        void Init();
        StateManager();
        APSettings *apSettings;
    };
} // namespace mod::owr
