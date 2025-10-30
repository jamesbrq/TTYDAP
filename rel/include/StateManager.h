#pragma once

#include <cstdint>
#include <ttyd/party.h>
#include <ttyd/msgdrv.h>

namespace mod::owr
{
    struct APSettings
    {
        uint8_t palaceStars;                       // 0x0
        ttyd::party::PartyMembers startingPartner; //0x1
        uint8_t yoshiColor;         // 0x2
        uint8_t apEnabled;          // 0x3
        char *yoshiName;            // 0x4
        uint8_t inGame;             // 0x8
        uint8_t palaceSkip;         // 0x9
        uint8_t openWestside;       // 0xA
        uint8_t peekaboo;           // 0xB
        uint8_t intermissions;      // 0xC
        uint8_t startingHP;         // 0xD
        uint8_t startingFP;         // 0xE
        uint8_t startingBP;         // 0xF
        uint8_t runFill;            // 0x10
        uint8_t requiredStars[7];   // 0x11
        uint8_t tattlesanity;       // 0x18
        uint8_t fastTravel;         // 0x19
        uint8_t touConditions;      // 0x1A
        uint8_t collectedStars;     // 0x1B
        uint8_t cutsceneSkip;       // 0x1C
        uint8_t expMultiplier;      // 0x1D
        uint8_t startingLevel;      // 0x1E
        uint8_t deathLinkTriggered; // 0x1F
        uint8_t deathLinkSent;      // 0x20
        uint8_t music;              // 0x21
        uint8_t blockVisibility;    // 0x22
        uint8_t firstAttack;        // 0x23
        uint32_t musicSeed;         // 0x24
        uint8_t goalStars;          // 0x28
        uint8_t goal;               // 0x29
        uint8_t starShuffle;        // 0x2A
        uint8_t dazzle;             // 0x2B
    };

    static_assert(sizeof(APSettings) == 0x2C);

    class StateManager
    {
       public:
        void Init();
        StateManager();
        APSettings *apSettings;
        uint16_t *tattleItems;
        uint16_t *maniacItems;
        uint32_t state_msgWork[17];
        bool newTattle = false;
        bool firstDeath = false;
        void *starItemPtr = nullptr;
        const char *starstoneName = nullptr;
        int32_t *starstoneFunctionPtr = nullptr;
    };
} // namespace mod::owr
