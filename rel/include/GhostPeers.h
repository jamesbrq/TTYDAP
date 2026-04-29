#pragma once

#include "gc/types.h"
#include "ttyd/dispdrv.h"
#include <cstdint>
#include <cstddef>

namespace mod::ghosts
{

    constexpr int kMaxPeers = 16;

    constexpr uint32_t kMagic = 0x47484F53;
    constexpr uint32_t kVersion = 26;

    constexpr uint32_t kFlags2EffectsMask = 0x10000000;
    constexpr uint32_t kFlags2RearMask = 0x80000000;

    constexpr int kSfxEventsPerSlot = 4;

    constexpr uint8_t kSfxFlag3D = 0x01;

    // v26: state-sync for loops. Each peer publishes the set of
    // currently-playing loop sfxIds (max kActiveLoopsPerPeer per
    // publish). Receivers diff against their tracked set: anything
    // newly-appeared starts; anything no-longer-published stops. This
    // replaces the v25 stop-event mechanism (kSfxFlagStop), which was
    // unreliable under per-publish event budget pressure (4 events/
    // publish). State sync is bandwidth-bounded but reliable: even
    // if a publish is dropped entirely, the next one self-heals.
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

        // Per-axis spin direction hint. Source-side observation of the
        // most recent unwrapped delta sign for each rotation axis:
        //   +1 = recent rotation was in positive direction
        //   -1 = recent rotation was in negative direction
        //    0 = no fast rotation observed (or freshly-spawned peer)
        // Receiver uses these to disambiguate fast spins (>180 deg per
        // publish) where shortest-path lerp would pick the wrong way.
        // Source decides "fast" by tracking unwrapped angle and
        // setting the hint only when angular speed exceeds a threshold.
        // Replaces what was pad_v20[3] in v23.
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
        uint8_t _pad_sfx[2];
        SfxEvent sfxEvents[kSfxEventsPerSlot];

        // v26 state-sync: currently-playing loop sfxIds for this peer.
        // Receivers diff against their tracked set. Any entry == 0 is
        // an empty slot (sfxId 0 is not a real engine SFX). The set is
        // unordered; receivers don't depend on positional consistency
        // between publishes.
        uint16_t activeLoops[kActiveLoopsPerPeer];

    } __attribute__((__packed__));

    static_assert(sizeof(SfxEvent) == 4, "SfxEvent must be 4 bytes");
    static_assert(sizeof(PeerSlot) == 212, "PeerSlot must be exactly 212 bytes (v26: +12 for activeLoops)");

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
    constexpr float kDefaultHitReachScale = 1.0f;
    constexpr float kDefaultHitPeerWidth = 15.0f;

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

    // GhostState - a single heap-allocated container holding ALL ghost
    // peer scratch state. Replaces the previous arrangement where data
    // was scattered across hardcoded low-RAM addresses (0x80001800,
    // 0x80003B20-0x80003BE4, 0x80003D00). Those low-RAM addresses
    // overlapped game/OS regions on some users' Dolphin sessions and
    // caused deterministic crashes immediately on AP connect.
    //
    // This struct is allocated once at Init() time via `new`. The
    // pointer is published into APSettings::ghostStatePtr so Python
    // can find it. Layout is fixed and matches Python's mirror in
    // Ghosts.py - any layout change must bump kVersion in BOTH files
    // simultaneously.
    //
    // The packed PeerSlot/SharedBlock are kept packed for wire-format
    // consistency; the surrounding container is NOT packed so the
    // compiler can pad as needed for natural alignment.
    struct GhostState
    {
        // Wire format: peer block. SharedBlock starts with a 16-byte
        // header (magic, version, two reserved words) followed by
        // 16 packed PeerSlot entries. This is the "hot" data Python
        // writes 60Hz.
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

        // Self paper-mode AGB name. Mod writes (decoded from local
        // Mario each frame), Python reads it back to republish via
        // the peer block's paperAgbName field.
        char selfPaperAgbName[kSelfPaperAgbLen];

        // SFX ring. SPSC: mod producer (psndSFXOn/3D hooks), Python
        // consumer (drains and republishes via peer block sfx_events).
        // Was 4-byte header at 0x80003BA0 + events at 0x80003BA4.
        uint8_t sfxRingHead;
        uint8_t sfxRingTail;
        uint8_t sfxRingSeq;
        uint8_t pad_sfx;
        SfxEvent sfxRingEvents[kSfxRingCapacity];

        // Lobby HUD. 1024 bytes of structured + free-form text data.
        // Python writes 20Hz, mod reads each draw frame. Was at
        // 0x80003D00. Stored as raw bytes here since it has its own
        // sub-layout (LobbyHudHeader + members[] + text region).
        uint8_t lobbyHudBlock[kLobbyHudSize];

        // v26 state-sync: mod -> Python scratch holding the LOCAL
        // player's currently-active loop sfxIds. Sampled from
        // g_localChannelMap each frame so the latest value is always
        // available when Python's 20Hz publish tick runs. Python reads
        // these and embeds in its own peer slot's activeLoops field.
        // selfActiveLoopCount is the number of valid entries (0..N).
        uint8_t selfActiveLoopCount;
        uint8_t pad_loops[3];
        uint16_t selfActiveLoops[kActiveLoopsPerPeer];
    };

    // Sanity checks on field offsets. Python's mirror layout depends
    // on these. If anything moves, bump kVersion AND update Ghosts.py
    // GS_OFF_* constants. The compiler will catch drift here at build
    // time so layouts can't silently desync.
    static_assert(offsetof(GhostState, peerBlock) == 0, "peerBlock must start at offset 0");
    static_assert(offsetof(GhostState, pendingHit) == 3408, "pendingHit offset drift");
    static_assert(offsetof(GhostState, hitPoseName) == 3412, "hitPoseName offset drift");
    static_assert(offsetof(GhostState, hitReachScale) == 3428, "hitReachScale offset drift");
    static_assert(offsetof(GhostState, hitPeerWidth) == 3432, "hitPeerWidth offset drift");
    static_assert(offsetof(GhostState, outboundHit) == 3436, "outboundHit offset drift");
    static_assert(offsetof(GhostState, hitGrace) == 3440, "hitGrace offset drift");
    static_assert(offsetof(GhostState, selfTeamId) == 3441, "selfTeamId offset drift");
    static_assert(offsetof(GhostState, selfFriendlyFire) == 3442, "selfFriendlyFire offset drift");
    static_assert(offsetof(GhostState, maxRenderedPeers) == 3444, "maxRenderedPeers offset drift");
    static_assert(offsetof(GhostState, selfPaperAgbName) == 3448, "selfPaperAgbName offset drift");
    static_assert(offsetof(GhostState, sfxRingHead) == 3480, "sfxRingHead offset drift");
    static_assert(offsetof(GhostState, sfxRingEvents) == 3484, "sfxRingEvents offset drift");
    static_assert(offsetof(GhostState, lobbyHudBlock) == 3612, "lobbyHudBlock offset drift");
    static_assert(offsetof(GhostState, selfActiveLoopCount) == 4636, "selfActiveLoopCount offset drift");
    static_assert(offsetof(GhostState, selfActiveLoops) == 4640, "selfActiveLoops offset drift");
    static_assert(sizeof(GhostState) == 4652, "GhostState total size drift - check Python GS_TOTAL_SIZE");

    // Global pointer to the heap-allocated GhostState. Set by Init();
    // null before that. All accessors below dereference through this.
    extern GhostState *g_ghostState;

    // ===== Accessors =====
    // These replace the old hardcoded-address inline accessors. They
    // assume Init() has run and g_ghostState is non-null. This is the
    // normal case for any code reachable from the game's update/draw
    // callbacks (which are only registered after Init()).

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

    // v26: scratch holding LOCAL player's currently-active loops, for
    // Python's publish to read. Mod writes once per frame from
    // SampleActiveLoops.
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
} // namespace mod::ghosts