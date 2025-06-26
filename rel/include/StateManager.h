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
        uint8_t runFill;
        uint8_t requiredStars[7];
        uint8_t tattlesanity;
        uint8_t fastTravel;
        uint8_t touConditions;
    };

    static_assert(sizeof(APSettings) == 0x1C);

    class StateManager
    {
       public:
        void Init();
        StateManager();
        APSettings *apSettings;
        uint16_t *tattleItems;
        bool newTattle = false;
    };
} // namespace mod::owr
