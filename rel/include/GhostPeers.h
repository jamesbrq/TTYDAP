#pragma once

#include "gc/types.h"
#include "ttyd/dispdrv.h"
#include <cstdint>

namespace mod::ghosts
{

    constexpr int kMaxPeers = 32;

    constexpr uintptr_t kBlockAddress = 0x80001800;

    constexpr uint32_t kMagic = 0x47484F53;
    constexpr uint32_t kVersion = 21;

    constexpr uint32_t kFlags2EffectsMask = 0x10000000;
    constexpr uint32_t kFlags2RearMask = 0x80000000;

    constexpr int kSfxEventsPerSlot = 4;

    constexpr uint8_t kSfxFlag3D = 0x01;

    struct SfxEvent
    {
        uint16_t sfxId;
        uint8_t seq;
        uint8_t flags;
    } __attribute__((__packed__));

    struct PeerSlot
    {
        uint8_t active;
        char mapName[15];
        char animName[16];
        gc::vec3 position;
        float rotationY;
        uint8_t r, g, b, a;
        uint32_t flags2;
        uint32_t flags3;
        uint16_t motionTimer;
        uint8_t showName;

        uint8_t hammerable;

        uint8_t teamId;

        uint8_t pad_v20[3];

        float cameraAngle;
        char slotName[16];

        char paperAgbName[32];

        char paperAnimName[16];

        float rotationX;

        float rotationZ;

        float rotPivotX;
        float rotPivotY;
        float rotPivotZ;

        float scaleX;
        float scaleY;
        float scaleZ;

        float stretchY;

        uint16_t motionId;

        uint8_t _pad2[2];
        float paperLocalTime;

        uint8_t sfxCount;
        uint8_t _pad_sfx[3];
        SfxEvent sfxEvents[kSfxEventsPerSlot];

    } __attribute__((__packed__));

    static_assert(sizeof(SfxEvent) == 4, "SfxEvent must be 4 bytes");
    static_assert(sizeof(PeerSlot) == 200, "PeerSlot must be exactly 200 bytes");

    struct SharedBlock
    {
        uint32_t magic;
        uint32_t version;
        uint32_t reserved0;
        uint32_t reserved1;
        PeerSlot peers[kMaxPeers];
    } __attribute__((__packed__));

    static_assert(sizeof(SharedBlock) == 16 + kMaxPeers * sizeof(PeerSlot), "SharedBlock layout drifted");

    inline SharedBlock *GetBlock()
    {
        return reinterpret_cast<SharedBlock *>(kBlockAddress);
    }

    constexpr uintptr_t kSelfPaperAddress = 0x80003B20;
    constexpr int kSelfPaperAgbLen = 32;

    inline char *GetSelfPaperAgbName()
    {
        return reinterpret_cast<char *>(kSelfPaperAddress);
    }

    constexpr uintptr_t kPendingHitAddress = 0x80003B60;

    constexpr uint8_t kHitKindNone = 0;
    constexpr uint8_t kHitKindHammer = 1;

    inline volatile uint32_t *GetPendingHitPtr()
    {
        return reinterpret_cast<volatile uint32_t *>(kPendingHitAddress);
    }

    constexpr const char *kDefaultHitPoseName = "M_N_7";

    constexpr uintptr_t kHitReachScaleAddress = 0x80003B80;
    constexpr float kDefaultHitReachScale = 1.0f;

    inline volatile float *GetHitReachScalePtr()
    {
        return reinterpret_cast<volatile float *>(kHitReachScaleAddress);
    }

    constexpr uintptr_t kHitPeerWidthAddress = 0x80003B84;
    constexpr float kDefaultHitPeerWidth = 15.0f;

    inline volatile float *GetHitPeerWidthPtr()
    {
        return reinterpret_cast<volatile float *>(kHitPeerWidthAddress);
    }

    constexpr uintptr_t kOutboundHitAddress = 0x80003B88;

    inline volatile uint32_t *GetOutboundHitPtr()
    {
        return reinterpret_cast<volatile uint32_t *>(kOutboundHitAddress);
    }

    inline uint32_t PackOutboundHit(uint8_t kind, uint8_t peerIndex)
    {
        return (static_cast<uint32_t>(kind) << 24) | (static_cast<uint32_t>(peerIndex) << 16);
    }

    constexpr uintptr_t kHitGraceAddress = 0x80003B8C;

    inline volatile uint8_t *GetHitGracePtr()
    {
        return reinterpret_cast<volatile uint8_t *>(kHitGraceAddress);
    }

    constexpr uint8_t kTeamNone = 0;
    constexpr uint8_t kTeamRed = 1;
    constexpr uint8_t kTeamBlue = 2;
    constexpr uint8_t kTeamGreen = 3;
    constexpr uint8_t kTeamYellow = 4;

    constexpr uintptr_t kSelfTeamIdAddress = 0x80003B8D;
    constexpr uintptr_t kSelfFriendlyFireAddress = 0x80003B8E;

    inline volatile uint8_t *GetSelfTeamIdPtr()
    {
        return reinterpret_cast<volatile uint8_t *>(kSelfTeamIdAddress);
    }
    inline volatile uint8_t *GetSelfFriendlyFirePtr()
    {
        return reinterpret_cast<volatile uint8_t *>(kSelfFriendlyFireAddress);
    }

    constexpr int kSfxRingCapacity = 16;

    constexpr uintptr_t kSfxRingAddress = 0x80003BA0;
    constexpr uintptr_t kSfxRingHeadAddress = kSfxRingAddress + 0;
    constexpr uintptr_t kSfxRingTailAddress = kSfxRingAddress + 1;
    constexpr uintptr_t kSfxRingSeqAddress = kSfxRingAddress + 2;
    constexpr uintptr_t kSfxRingEventsAddress = kSfxRingAddress + 4;

    inline volatile uint8_t *GetSfxRingHeadPtr()
    {
        return reinterpret_cast<volatile uint8_t *>(kSfxRingHeadAddress);
    }
    inline volatile uint8_t *GetSfxRingTailPtr()
    {
        return reinterpret_cast<volatile uint8_t *>(kSfxRingTailAddress);
    }
    inline volatile uint8_t *GetSfxRingSeqPtr()
    {
        return reinterpret_cast<volatile uint8_t *>(kSfxRingSeqAddress);
    }
    inline volatile SfxEvent *GetSfxRingEvents()
    {
        return reinterpret_cast<volatile SfxEvent *>(kSfxRingEventsAddress);
    }

    constexpr uintptr_t kLobbyHudAddress = 0x80003D00;
    constexpr uint32_t kLobbyHudMagic = 0x4C4F4259;
    constexpr uint8_t kLobbyHudVersion = 1;
    constexpr int kLobbyMaxMembers = 32;
    constexpr int kLobbyMemberSize = 24;
    constexpr int kLobbyHeaderSize = 32;
    constexpr int kLobbyMembersOffset = kLobbyHeaderSize;
    constexpr int kLobbyTextOffset = kLobbyMembersOffset + kLobbyMaxMembers * kLobbyMemberSize;
    constexpr int kLobbyTextLen = 192;

    constexpr uint8_t kLobbyStatusIdle = 0;
    constexpr uint8_t kLobbyStatusWaiting = 1;
    constexpr uint8_t kLobbyStatusCountdown = 2;
    constexpr uint8_t kLobbyStatusPlaying = 3;
    constexpr uint8_t kLobbyStatusFinished = 4;

    constexpr uint8_t kGameTypeNone = 0;
    constexpr uint8_t kGameTypeHideAndSeek = 1;

    constexpr uint8_t kLobbyRoleNone = 0;
    constexpr uint8_t kLobbyRoleHost = 1;
    constexpr uint8_t kLobbyRoleParticipant = 2;
    constexpr uint8_t kLobbyRoleHider = 3;
    constexpr uint8_t kLobbyRoleSeeker = 4;
    constexpr uint8_t kLobbyRoleSpectator = 5;

#pragma pack(push, 1)
    struct LobbyHudHeader
    {
        uint32_t magic;
        uint8_t version;
        uint8_t active;
        uint8_t status;
        uint8_t gameType;
        uint8_t memberCount;
        uint8_t selfRole;
        uint16_t timerSeconds;
        uint32_t reserved;
        char name[16];
    };
    static_assert(sizeof(LobbyHudHeader) == kLobbyHeaderSize, "LobbyHudHeader size mismatch with kLobbyHeaderSize");

    struct LobbyHudMember
    {
        uint8_t slot;
        uint8_t role;
        uint8_t alive;
        uint8_t pad;
        char name[16];
        uint32_t reserved;
    };
    static_assert(sizeof(LobbyHudMember) == kLobbyMemberSize, "LobbyHudMember size mismatch with kLobbyMemberSize");
#pragma pack(pop)

    inline const LobbyHudHeader *GetLobbyHudHeader()
    {
        return reinterpret_cast<const LobbyHudHeader *>(kLobbyHudAddress);
    }
    inline const LobbyHudMember *GetLobbyHudMembers()
    {
        return reinterpret_cast<const LobbyHudMember *>(kLobbyHudAddress + kLobbyMembersOffset);
    }
    inline const char *GetLobbyHudText()
    {
        return reinterpret_cast<const char *>(kLobbyHudAddress + kLobbyTextOffset);
    }

    void Init();
    void Shutdown();
    void UpdateAll();
    void DrawAll(ttyd::dispdrv::CameraId cam, void *user);
    void DrawNameTagsAll(ttyd::dispdrv::CameraId cam, void *user);
    void DrawLobbyHud(ttyd::dispdrv::CameraId cam, void *user);

    void OnLocalSfxFired(int sfxId, bool is3D);

    void installSfxHooks();
} // namespace mod::ghosts