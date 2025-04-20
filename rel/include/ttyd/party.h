#pragma once

#include <cstdint>
#include "gc/types.h"
#include "mario.h"


namespace ttyd::party
{
    enum class PartySlotId : int32_t
    {
        kNone = -1,
        kParty,
        kFollower,
    };

    enum class PartyMembers : uint8_t
    {
        kNone = 0,
        kGoombella,
        kKoops,
        kBobbery,
        kYoshi,
        kFlurrie,
        kVivian,
        kMsMowz,
        kEgg,
        kFlavio,
        kPunio,
        kFrankly,
        kCraw,
        kGoombellaFollower,
        kKoopsFollower,
        kBobberyFollower,
        kYoshiFollower,
        kFlurrieFollower,
        kVivianFollower,
        kMsMowzFollower,
    };

    struct PartyEntry
    {
        uint32_t flags;
        uint32_t flags2;
        uint32_t field_0x8;
        int32_t field_0xC;
        int32_t field_0x10;
        int32_t field_0x14;
        uint8_t field_0x18[0x2F - 0x18];
        int8_t currentSlotId;
        uint8_t field_0x30;
        PartyMembers currentMemberId;
        uint8_t field_0x32[0x39 - 0x32];
        uint8_t wActionState;
        uint8_t field_0x3A[0x58 - 0x3A];
        gc::vec3 partnerPosition;
        uint8_t field_0x64[0xFC - 0x64];
        float wPartnerAngleCurrent;
        float wPartnerAngle;
        uint8_t field_0x104[0x10C - 0x104];
        float wPartnerDirectionCurrent;
        float wPartnerDirection;
        uint8_t field_0x114[0x160 - 0x114];
        ttyd::mario::Player *playerPtr;
        int32_t camId;
        uint8_t field_0x168[0x188 - 0x168];
    } __attribute__((__packed__));

    static_assert(sizeof(PartyEntry) == 0x188);

    extern "C"
    {
        // partySetFamicomMode
        // partyShadowOff
        // partyUpdateKeyData
        // partyClearCont
        // partyKeyOff
        // partyKeyOn
        // partyDisp
        // partyPreDisp
        // partyMakeDispDir
        // partySetCamId
        // partyInitCamId
        // party_damage_return
        // partyRideYoshiMain
        // party_ride_yoshi_force_move
        // party_force_ride_yoshi
        // partyRideMain
        // partyRideChk
        // allPartyRideOff2
        // allPartyRideOff
        // allPartyRideOn
        // allPartyForceRideOn
        // getRidePos
        // allPartyRideShip
        // allPartyRidePlane
        // partyDokanEnd
        // partyDokanMain
        // partyDokanInit
        // partyGoodbyeMain
        // partyGoodbyeInit
        // partyHelloMain
        // partyHello
        // partyDoWork
        // driveParty
        // partyMain
        // _partyForceChgRunMode
        // _partyChgRunMode
        // partyChgRunMode
        // partyUsePost
        // partyReInit
        // partyInit
        int32_t partyEntry2Pos(PartyMembers id, float x, float y, float z);
        PartySlotId partyEntry2Hello(PartyMembers id);
        // partyEntry2
        // partyEntryMain
        // partyGetHeight
        uint32_t yoshiSetColor();
        // partyKill2
        // partyKill
        // partyGoodbye
        // partyEntryPos
        // partyEntry
        // partyEntryHello
        void partySetForceMove(PartyEntry *ptr, float x, float z, float speed);
        // partyStop
        void partyRun(PartyEntry *partyPtr);
        // partyCtrlOn
        // partyCtrlOff
        // partyChgPaper
        // partyChgPose
        // partyChgPoseId
        // partyPaperLightOff
        // partyPaperOff
        // partyPaperOn
        // anotherPartyGetPtr
        PartyEntry *partyGetPtr(PartySlotId partyId);
    }
} // namespace ttyd::party
