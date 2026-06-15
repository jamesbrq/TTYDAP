#pragma once

#include "gc/types.h"
#include "ttyd/dispdrv.h"
#include <cstdint>
#include <cstddef>

namespace mod::ghosts
{

    constexpr int kMaxPeers = 16;

    constexpr uint32_t kMagic = 0x47484F53;
    constexpr uint32_t kVersion = 0;

    constexpr uint8_t kGameRoleNone = 0;
    constexpr uint8_t kGameRoleHider = 1;
    constexpr uint8_t kGameRoleSeeker = 2;

    constexpr uint32_t kFlags2EffectsMask = 0x10000000;
    constexpr uint32_t kFlags2RearMask = 0x80000000;

    constexpr int kSfxEventsPerSlot = 4;

    constexpr uint8_t kSfxFlag3D = 0x01;

    constexpr int kActiveLoopsPerPeer = 6;

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

        int8_t spinDirHintY;
        int8_t spinDirHintX;
        int8_t spinDirHintZ;

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
        uint8_t activeLoopCount; // v26: count of valid entries in activeLoops below
        uint8_t gameRole;        // v27: 0/1/2 = none/hider/seeker (offset 0xB6)
        uint8_t colorIndex;      // emblem index 0..3 (none/L/W/WL) at 0xB7, selects body AGB triplet
        SfxEvent sfxEvents[kSfxEventsPerSlot];

        uint16_t activeLoops[kActiveLoopsPerPeer];

    } __attribute__((__packed__));

    static_assert(sizeof(SfxEvent) == 4, "SfxEvent must be 4 bytes");
    static_assert(sizeof(PeerSlot) == 212,
                  "PeerSlot must be exactly 212 bytes (v26 added activeLoops; v27 reuses pad as gameRole, size unchanged)");
    static_assert(offsetof(PeerSlot, gameRole) == 0xB6, "v27 gameRole must sit at PeerSlot+0xB6");
    static_assert(offsetof(PeerSlot, colorIndex) == 0xB7, "colorIndex must sit at PeerSlot+0xB7 (reused sfx pad)");

    struct SharedBlock
    {
        uint32_t magic;
        uint32_t version;
        uint32_t reserved0;
        uint32_t reserved1;
        PeerSlot peers[kMaxPeers];
    } __attribute__((__packed__));

    static_assert(sizeof(SharedBlock) == 16 + kMaxPeers * sizeof(PeerSlot), "SharedBlock layout drifted");

    constexpr int kSelfPaperAgbLen = 32;
    constexpr int kHitPoseNameLen = 16;

    constexpr uint8_t kHitKindNone = 0;
    constexpr uint8_t kHitKindHammer = 1;

    constexpr const char *kDefaultHitPoseName = "M_N_7";
    // Widened in P2.1 from 1.0/15.0 so HnS seekers can land hammer
    // hits without near-pixel-perfect alignment.
    constexpr float kDefaultHitReachScale = 1.25f;
    constexpr float kDefaultHitPeerWidth = 30.0f;
    // Vertical tolerance for the cylinder check — hits are rejected
    // when |peer.y - me.y| exceeds this. Keeps stacked-platform
    // sanity (peer two floors below shouldn't get hit).
    constexpr float kDefaultHitVerticalTolerance = 80.0f;

    constexpr uint8_t kTeamNone = 0;
    constexpr uint8_t kTeamRed = 1;
    constexpr uint8_t kTeamBlue = 2;
    constexpr uint8_t kTeamGreen = 3;
    constexpr uint8_t kTeamYellow = 4;

    constexpr int kSfxRingCapacity = 32;

    constexpr uint32_t kLobbyHudMagic = 0x4C4F4259;
    constexpr uint8_t kLobbyHudVersion = 1;
    constexpr int kLobbyMaxMembers = 32;
    constexpr int kLobbyMemberSize = 24;
    constexpr int kLobbyHeaderSize = 32;
    constexpr int kLobbyMembersOffset = kLobbyHeaderSize;
    constexpr int kLobbyTextOffset = kLobbyMembersOffset + kLobbyMaxMembers * kLobbyMemberSize;
    constexpr int kLobbyTextLen = 192;
    constexpr int kLobbyHudSize = 1024;

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
    static_assert(sizeof(LobbyHudHeader) == kLobbyHeaderSize, "LobbyHudHeader size mismatch");

    struct LobbyHudMember
    {
        uint8_t slot;
        uint8_t role;
        uint8_t alive;
        uint8_t pad;
        char name[16];
        uint32_t reserved;
    };
    static_assert(sizeof(LobbyHudMember) == kLobbyMemberSize, "LobbyHudMember size mismatch");
#pragma pack(pop)

    constexpr int kDefaultMaxRenderedPeers = 12;

    struct GhostState
    {
        SharedBlock peerBlock;

        // Hit/team/grace scratch. These were previously at
        // 0x80003B60-0x80003B8E.
        uint32_t pendingHit;               // Python -> mod, kind in byte 0
        char hitPoseName[kHitPoseNameLen]; // live-tunable, default "M_N_7"
        float hitReachScale;               // live-tunable, default 1.0
        float hitPeerWidth;                // live-tunable, default 15.0
        uint32_t outboundHit;              // mod -> Python, kind+peer index
        uint8_t hitGrace;                  // mod -> Python, 1 = in iframes
        uint8_t selfTeamId;                // Python -> mod, 0..4
        uint8_t selfFriendlyFire;          // Python -> mod, 0/1
        uint8_t pad_team;

        // Tunable cap on number of rendered peers.
        uint32_t maxRenderedPeers;

        char selfPaperAgbName[kSelfPaperAgbLen];

        uint8_t sfxRingHead;
        uint8_t sfxRingTail;
        uint8_t sfxRingSeq;
        uint8_t pad_sfx;
        SfxEvent sfxRingEvents[kSfxRingCapacity];

        uint8_t lobbyHudBlock[kLobbyHudSize];

        uint8_t selfActiveLoopCount;
        uint8_t pad_loops[3];
        uint16_t selfActiveLoops[kActiveLoopsPerPeer];

        uint8_t selfGameRole;
        uint8_t pad_role[3];

        uint8_t selfFrozen;
        uint8_t pendingTeleportSeq;
        uint8_t pad_v29[2];
        char pendingTeleportMap[16];
        char pendingTeleportBero[16];

        // Debug "play this SFX" command bus. Python writes debugSfxId
        // and bumps debugSfxSeq; mod's UpdateAll edge-detects on the
        // sequence counter and calls psndSFXOn(id) (or psndSFXOn_3D
        // at the local Mario position if debugSfxFlags bit 0 is set).
        // Driven by /hns play_sfx <id> [3d] for ad-hoc SFX-ID probing.
        uint32_t debugSfxId;
        uint8_t debugSfxSeq;
        uint8_t debugSfxFlags; // bit 0 = use 3D variant
        uint8_t pad_debug_sfx[2];
    };

    // Offsets after peerBlock are anchored to sizeof(SharedBlock) so they
    // auto-track any change to kMaxPeers (peerBlock is the first member and
    // the only size-variable one). The deltas below are fixed by the
    // post-peerBlock layout. Python's GS_OFF_* derive the same way.
    static_assert(offsetof(GhostState, peerBlock) == 0, "peerBlock must start at offset 0");
    static_assert(offsetof(GhostState, pendingHit) == sizeof(SharedBlock) + 0, "pendingHit offset drift");
    static_assert(offsetof(GhostState, hitPoseName) == sizeof(SharedBlock) + 4, "hitPoseName offset drift");
    static_assert(offsetof(GhostState, hitReachScale) == sizeof(SharedBlock) + 20, "hitReachScale offset drift");
    static_assert(offsetof(GhostState, hitPeerWidth) == sizeof(SharedBlock) + 24, "hitPeerWidth offset drift");
    static_assert(offsetof(GhostState, outboundHit) == sizeof(SharedBlock) + 28, "outboundHit offset drift");
    static_assert(offsetof(GhostState, hitGrace) == sizeof(SharedBlock) + 32, "hitGrace offset drift");
    static_assert(offsetof(GhostState, selfTeamId) == sizeof(SharedBlock) + 33, "selfTeamId offset drift");
    static_assert(offsetof(GhostState, selfFriendlyFire) == sizeof(SharedBlock) + 34, "selfFriendlyFire offset drift");
    static_assert(offsetof(GhostState, maxRenderedPeers) == sizeof(SharedBlock) + 36, "maxRenderedPeers offset drift");
    static_assert(offsetof(GhostState, selfPaperAgbName) == sizeof(SharedBlock) + 40, "selfPaperAgbName offset drift");
    static_assert(offsetof(GhostState, sfxRingHead) == sizeof(SharedBlock) + 72, "sfxRingHead offset drift");
    static_assert(offsetof(GhostState, sfxRingEvents) == sizeof(SharedBlock) + 76, "sfxRingEvents offset drift");
    static_assert(offsetof(GhostState, lobbyHudBlock) == sizeof(SharedBlock) + 204, "lobbyHudBlock offset drift");
    static_assert(offsetof(GhostState, selfActiveLoopCount) == sizeof(SharedBlock) + 1228, "selfActiveLoopCount offset drift");
    static_assert(offsetof(GhostState, selfActiveLoops) == sizeof(SharedBlock) + 1232, "selfActiveLoops offset drift");
    static_assert(offsetof(GhostState, selfGameRole) == sizeof(SharedBlock) + 1244, "selfGameRole offset drift");
    static_assert(offsetof(GhostState, selfFrozen) == sizeof(SharedBlock) + 1248, "selfFrozen offset drift");
    static_assert(offsetof(GhostState, pendingTeleportSeq) == sizeof(SharedBlock) + 1249, "pendingTeleportSeq offset drift");
    static_assert(offsetof(GhostState, pendingTeleportMap) == sizeof(SharedBlock) + 1252, "pendingTeleportMap offset drift");
    static_assert(offsetof(GhostState, pendingTeleportBero) == sizeof(SharedBlock) + 1268, "pendingTeleportBero offset drift");
    static_assert(offsetof(GhostState, debugSfxId) == sizeof(SharedBlock) + 1284, "debugSfxId offset drift");
    static_assert(offsetof(GhostState, debugSfxSeq) == sizeof(SharedBlock) + 1288, "debugSfxSeq offset drift");
    static_assert(offsetof(GhostState, debugSfxFlags) == sizeof(SharedBlock) + 1289, "debugSfxFlags offset drift");
    static_assert(sizeof(GhostState) == sizeof(SharedBlock) + 1292, "GhostState total size drift - check Python GS_TOTAL_SIZE");

    // Global pointer to the heap-allocated GhostState. Set by Init();
    // null before that. All accessors below dereference through this.
    extern GhostState *g_ghostState;

    inline SharedBlock *GetBlock()
    {
        return &g_ghostState->peerBlock;
    }

    inline char *GetSelfPaperAgbName()
    {
        return g_ghostState->selfPaperAgbName;
    }

    inline volatile uint32_t *GetPendingHitPtr()
    {
        return reinterpret_cast<volatile uint32_t *>(&g_ghostState->pendingHit);
    }

    inline volatile float *GetHitReachScalePtr()
    {
        return reinterpret_cast<volatile float *>(&g_ghostState->hitReachScale);
    }

    inline volatile float *GetHitPeerWidthPtr()
    {
        return reinterpret_cast<volatile float *>(&g_ghostState->hitPeerWidth);
    }

    inline volatile uint32_t *GetOutboundHitPtr()
    {
        return reinterpret_cast<volatile uint32_t *>(&g_ghostState->outboundHit);
    }

    inline uint32_t PackOutboundHit(uint8_t kind, uint8_t peerIndex)
    {
        return (static_cast<uint32_t>(kind) << 24) | (static_cast<uint32_t>(peerIndex) << 16);
    }

    inline volatile uint8_t *GetHitGracePtr()
    {
        return &g_ghostState->hitGrace;
    }

    inline volatile uint8_t *GetSelfTeamIdPtr()
    {
        return &g_ghostState->selfTeamId;
    }
    inline volatile uint8_t *GetSelfFriendlyFirePtr()
    {
        return &g_ghostState->selfFriendlyFire;
    }

    inline volatile uint32_t *GetMaxRenderedPeersPtr()
    {
        return reinterpret_cast<volatile uint32_t *>(&g_ghostState->maxRenderedPeers);
    }

    inline volatile uint8_t *GetSfxRingHeadPtr()
    {
        return &g_ghostState->sfxRingHead;
    }
    inline volatile uint8_t *GetSfxRingTailPtr()
    {
        return &g_ghostState->sfxRingTail;
    }
    inline volatile uint8_t *GetSfxRingSeqPtr()
    {
        return &g_ghostState->sfxRingSeq;
    }
    inline volatile SfxEvent *GetSfxRingEvents()
    {
        return reinterpret_cast<volatile SfxEvent *>(g_ghostState->sfxRingEvents);
    }

    inline const LobbyHudHeader *GetLobbyHudHeader()
    {
        return reinterpret_cast<const LobbyHudHeader *>(g_ghostState->lobbyHudBlock);
    }
    inline const LobbyHudMember *GetLobbyHudMembers()
    {
        return reinterpret_cast<const LobbyHudMember *>(g_ghostState->lobbyHudBlock + kLobbyMembersOffset);
    }
    inline const char *GetLobbyHudText()
    {
        return reinterpret_cast<const char *>(g_ghostState->lobbyHudBlock + kLobbyTextOffset);
    }

    inline volatile uint8_t *GetSelfActiveLoopCountPtr()
    {
        return &g_ghostState->selfActiveLoopCount;
    }
    inline volatile uint16_t *GetSelfActiveLoopsPtr()
    {
        return reinterpret_cast<volatile uint16_t *>(g_ghostState->selfActiveLoops);
    }

    void Init();
    void Shutdown();
    void UpdateAll();
    void DrawAll(ttyd::dispdrv::CameraId cam, void *user);
    void DrawNameTagsAll(ttyd::dispdrv::CameraId cam, void *user);
    void DrawLobbyHud(ttyd::dispdrv::CameraId cam, void *user);

    void OnLocalSfxFired(int sfxId, bool is3D, int channel);
    void OnLocalSfxStopped(int channel);

    void installSfxHooks();

    // Hook + trampoline for animPoseAutoRelease (installed in init.rel). Keeps
    // ghost pose bookkeeping in sync when the engine bulk-frees pose group 2
    // on map/area transitions.
    void animPoseAutoReleaseHook(int32_t group);
    extern void (*g_animPoseAutoRelease_trampoline)(int32_t group);
} // namespace mod::ghosts