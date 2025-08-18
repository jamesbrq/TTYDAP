#include "util.h"
#include "ttyd/party.h"
#include "ttyd/mario.h"
#include "ttyd/mario_party.h"
#include "gc/pad.h"
#include "ttyd/system.h"
#include "ttyd/fontmgr.h"

#include <cstdint>
#include <cstring>

using namespace ttyd::party;
using namespace ttyd::mario_party;

namespace mod::util
{
    bool checkButtonCombo(uint32_t combo)
    {
        if ((ttyd::system::keyGetButton(gc::pad::PadId::CONTROLLER_ONE) & combo) != combo)
        {
            return false;
        }

        return ttyd::system::keyGetButtonTrg(gc::pad::PadId::CONTROLLER_ONE) & combo;
    }

    bool checkButtonsEveryFrame(uint32_t buttons)
    {
        return ttyd::system::keyGetButton(gc::pad::PadId::CONTROLLER_ONE) & buttons;
    }

    bool checkButtonComboEveryFrame(uint32_t combo)
    {
        return (ttyd::system::keyGetButton(gc::pad::PadId::CONTROLLER_ONE) & combo) == combo;
    }

    PartySlotId spawnFailsafePartnerOrFollower(bool shouldSpawnPartner)
    {
        const ttyd::mario::Player *playerPtr = ttyd::mario::marioGetPtr();
        PartyMembers previousOut;

        if (shouldSpawnPartner)
        {
            // Get the previous partner out
            previousOut = playerPtr->prevPartnerId;
        }
        else
        {
            // Get the previous follower out
            previousOut = playerPtr->prevFollowerId;
        }

        // Check if a partner/follower was previously out
        if (previousOut != PartyMembers::kNone)
        {
            // A partner/follower was previously out, so bring them back out
            return spawnPartnerOrFollower(previousOut);
        }
        else if (shouldSpawnPartner)
        {
            // No partner was previously out, so bring out a partner that the player has obtained
            return spawnPartnerOrFollower(getFirstPartner());
        }
        else
        {
            // No follower was previously out, so bring out a random one
            constexpr uint32_t range =
                static_cast<uint32_t>(PartyMembers::kMsMowzFollower) - static_cast<uint32_t>(PartyMembers::kEgg) + 1;

            const uint32_t follower = ttyd::system::irand(range) + static_cast<uint32_t>(PartyMembers::kEgg);
            return spawnPartnerOrFollower(static_cast<PartyMembers>(follower));
        }
    }

    PartyMembers getFirstPartner()
    {
        for (int i = static_cast<int>(PartyMembers::kGoombella); i <= static_cast<int>(PartyMembers::kMsMowz); i++)
        {
            if (partyChkJoin(static_cast<PartyMembers>(i)))
                return static_cast<PartyMembers>(i);
        }
        return PartyMembers::kGoombella;
    }

    PartySlotId spawnPartnerOrFollower(PartyMembers id)
    {
        // Make sure the partner/follower is valid
        if ((id <= PartyMembers::kNone) || (id > PartyMembers::kMsMowzFollower))
        {
            return PartySlotId::kNone;
        }

        PartySlotId slotId;

        // If spawning a partner, make sure they are enabled when marioPartyEntry is called
        if ((id >= PartyMembers::kGoombella) && (id <= PartyMembers::kMsMowz))
        {
            // Check if the selected partner is in the partner menu
            const bool partnerInPauseMenu = partyChkJoin(id);

            // Make sure the selected partner is in the partner menu, as otherwise marioPartyEntry will not spawn them
            partyJoin(id);

            // Spawn the partner
            slotId = marioPartyEntry(id);

            // If the selected partner was not in the partner menu, then remove them
            if (!partnerInPauseMenu)
            {
                partyLeft(id);
            }
        }
        else
        {
            // Spawn the follower
            if (id == PartyMembers::kFlurrieFollower)
                id = PartyMembers::kFlavio; // Failsafe for Flurrie due to ch.6 softlock
            slotId = marioPartyEntry(id);
        }

        // Make sure the partner/follower spawned properly
        if (slotId <= PartySlotId::kNone)
        {
            return slotId;
        }

        // Make sure the partner/follower is moving
        partyRun(partyGetPtr(slotId));

        return slotId;
    }

    uint32_t ptrIsValid(void *ptr)
    {
        const uint32_t ptrRaw = reinterpret_cast<uint32_t>(ptr);

        // Cached memory
        if ((ptrRaw >= 0x80000000) && (ptrRaw < 0x81800000))
        {
            return mod::util::PointerVerificationType::PTR_CACHED;
        }

        // Uncached memory
        if ((ptrRaw >= 0xC0000000) && (ptrRaw < 0xC1800000))
        {
            return mod::util::PointerVerificationType::PTR_UNCACHED;
        }

        return mod::util::PointerVerificationType::PTR_INVALID;
    }

    __attribute__((noinline)) float intToFloat(int32_t value)
    {
        return static_cast<float>(value);
    }

    float getTextMultilineIncrement(const char *text, float scale, uint32_t additionalLines)
    {
        uint16_t numLines;
        ttyd::fontmgr::FontGetMessageWidthLine(text, &numLines);

        const float lineCount = intToFloat(numLines + additionalLines);
        return lineCount * (32.f * scale);
    }
} // namespace mod::util
