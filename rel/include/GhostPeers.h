#pragma once

#include "gc/types.h"
#include "ttyd/dispdrv.h"
#include <cstdint>
#include <cstddef>

namespace mod::ghosts
{

    constexpr int kMaxPeers = 16;

    constexpr uint32_t kMagic = 0x47484F53;
    constexpr uint32_t kVersion = 29;

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
        uint8_t _pad_b6;         // reserved (offset 0xB6)
        uint8_t _pad_sfx;        // align sfxEvents[] (offset 0xB7)
        SfxEvent sfxEvents[kSfxEventsPerSlot];

        uint16_t activeLoops[kActiveLoopsPerPeer];

    } __attribute__((__packed__));

    static_assert(sizeof(SfxEvent) == 4, "SfxEvent must be 4 bytes");
    static_assert(sizeof(PeerSlot) == 212, "PeerSlot must be exactly 212 bytes");

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
    // Widened from 1.0/15.0 so hammer hits land without
    // near-pixel-perfect alignment.
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

    // Reserved block size — kept so the GhostState layout is
    // unchanged from v29.
    constexpr int kReservedBlockSize = 1024;

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

        uint8_t reservedBlock[kReservedBlockSize];

        uint8_t selfActiveLoopCount;
        uint8_t pad_loops[3];
        uint16_t selfActiveLoops[kActiveLoopsPerPeer];

        // Reserved tail (0x122C..0x125B): bytes kept in place so the
        // layout matches v29 byte-for-byte. Nothing reads or writes them.
        uint8_t reservedTail[0x30];
    };

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
    static_assert(offsetof(GhostState, reservedBlock) == 3612, "reservedBlock offset drift");
    static_assert(offsetof(GhostState, selfActiveLoopCount) == 4636, "selfActiveLoopCount offset drift");
    static_assert(offsetof(GhostState, selfActiveLoops) == 4640, "selfActiveLoops offset drift");
    static_assert(offsetof(GhostState, reservedTail) == 4652, "reservedTail offset drift");
    static_assert(sizeof(GhostState) == 4700, "GhostState total size drift - check Python GS_TOTAL_SIZE");

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

    void OnLocalSfxFired(int sfxId, bool is3D, int channel);
    void OnLocalSfxStopped(int channel);

    void installSfxHooks();
} // namespace mod::ghosts