#include "GhostPeers.h"

#include "OWR.h"
#include "StateManager.h"
#include "gc/mtx.h"
#include "ttyd/animdrv.h"
#include "ttyd/dispdrv.h"
#include "ttyd/fontmgr.h"
#include "ttyd/mario.h"
#include "ttyd/mario_motion.h"
#include "ttyd/mariost.h"
#include "ttyd/memory.h"
#include "ttyd/npcdrv.h"
#include "ttyd/pmario_sound.h"
#include "ttyd/seqdrv.h"
#include "ttyd/string.h"
#include "visibility.h"

#include <cstring>

using namespace ttyd::pmario_sound;

namespace mod::ghosts
{

    GhostState *g_ghostState = nullptr;

    extern "C" void *camGetPtr(int cameraId);

    extern "C" float reviseAngle(float deg);

    extern "C" int psndSFXOff(int channel);

    namespace
    {

        constexpr const char *kAgbForward = "a_mario";
        constexpr const char *kAgbRear = "a_mario_r";
        constexpr const char *kAgbEffects = "e_mario";
        constexpr int32_t kPoseGroup = 2;

        constexpr float kGhostScale = 2.0f;

        constexpr float kNameTagScreenScaleX = 280.0f;
        constexpr float kNameTagScreenScaleY = 240.0f;
        constexpr float kNameTagWorldYOffset = 50.0f;
        constexpr float kNameTagFontScale = 0.5f;

        struct GhostSlot
        {
            bool forwardAllocated;
            bool rearAllocated;
            bool effectsAllocated;
            int32_t forwardPoseId;
            int32_t rearPoseId;
            int32_t effectsPoseId;

            int8_t activePose;

            char lastAnimForward[16];
            char lastAnimRear[16];
            char lastAnimEffects[16];

            char lastPaperAnim[16];

            int32_t paperPoseId;
            char lastPaperAgb[32];

            float renderX;
            float renderY;
            float renderZ;
            float renderRotY;

            float renderRotX;
            float renderRotZ;
            float renderPivotX;
            float renderPivotY;
            float renderPivotZ;

            float lastSeenRotY;
            float lastSeenRotX;
            float lastSeenRotZ;
            float velRotY;
            float velRotX;
            float velRotZ;
            bool spinTrackingInitY;
            bool spinTrackingInitX;
            bool spinTrackingInitZ;

            bool renderInitialized;

            int hitFramesRemaining;

            uint8_t lastConsumedSfxSeq;
            bool sfxSeqInitialized;

            struct ActiveLoop
            {
                uint16_t sfxId;
                int channel;
                char animNameAtStart[16];
                bool inUse;

                char animCandidate[16];
                uint8_t stableFrames;
                bool watching;
            };
            static constexpr int kActiveLoopsPerSlot = 8;
            ActiveLoop activeLoops[kActiveLoopsPerSlot];

            static constexpr int kBlockedSfxPerSlot = 8;
            uint16_t blockedSfx[kBlockedSfxPerSlot];
        };

        constexpr float kLerpAlpha = 0.30f;

        constexpr uint8_t kAnimStabilityFrames = 10;

        GhostSlot g_slots[kMaxPeers];
        bool g_initialized = false;

        constexpr int kHitGraceFrames = 90;
        constexpr int kHitLockDurationFrames = 30;        // ~0.5s at 60Hz
        constexpr int kHitQueueTimeoutFrames = 60 * 5;    // 5s before giving up
        int g_hitGraceRemaining = 0;
        int g_hitLockRemaining = 0;
        int g_hitQueuedTimeout = 0;
        bool g_hitQueued = false;
        bool g_hitLockApplied = false;  // tracks whether we hold a
                                         // marioKeyOff() contribution
                                         // we still need to release.

        bool g_inReceiverReplay = false;

        const SharedBlock *GetValidBlock()
        {
            const SharedBlock *block = GetBlock();
            if (block->magic != kMagic)
                return nullptr;
            if (block->version != kVersion)
                return nullptr;
            return block;
        }

        bool PeerOnLocalMap(const PeerSlot &peer)
        {
            const auto *gw = _globalWorkPtr;
            if (gw == nullptr)
                return false;
            return std::strncmp(peer.mapName, gw->currentMapName, sizeof(peer.mapName)) == 0;
        }

        constexpr uint16_t kSfxWhitelist[] = {
            // -- Voice grunts (Mario "ha!", "yahoo!", etc.) --
            //    mot_jump.s lines 416-434 (jump-launch voice variants)
            0x09D,
            0x09E,
            0x09F,
            0x0A0,
            0x0A1,

            // -- Plane/boat ambient cloth/wing flap --
            //    mot_plane.s + mot_ship.s
            0x0AF,

            // -- Body landing thud (mario_motion.s line 363) --
            0x0B9,

            // -- Damage voice (mot_damage.s) --
            0x0BA, // "ow!" damage grunt
            0x0CB, // damage variant 2 (KO/heavy)


            0x140,
            0x141,
            0x142,
            0x143,
            0x144,
            0x145,
            0x146,
            0x147,
            0x148,
            0x149,
            0x14A,
            0x14B,

            // -- Jump-launch cloth/whoosh (mot_jump.s line 437) --
            0x14D,

            // -- Universal terrain footstep (mario_motion.s 1591-1620) --
            //    Used outside walk motion (e.g. landing recovery).
            0x14F,
            0x150,
            0x151,
            0x152,
            0x153,
            0x154,

            0x159,

            0x15E, // hammer impact, power tier 1 (one-shot)
            0x15F, // hammer impact, power tier 2 (one-shot)
            0x160, // hammer impact, power tier 3 (one-shot)
            0x163, // hammer spin variant from mot_hammer2 (one-shot)

            // -- Misc Mario actions (legacy, unverified) --
            0x16A,

            0x173,

            // -- Slide-under entry/exit (mot_slit.s) --
            0x177,
            0x178,

            0x179,
            0x17A,
            0x17B,

            0x17D,
            0x180,

            // -- Pipe-grab / jabara swing (mot_jabara.s) --
            //    Spin SFX plus held-shimmy variants
            0x182,
            0x183,
            0x184,
            0x186,
            0x187,
            0x188,
            0x189,
            0x18A,
            0x18B,

            // -- Boat motion (mot_ship.s) --
            0x18D,
            0x18F,
            0x190,
            0x192,

            // -- Damage extra sounds (mot_damage.s) --
            0x194,
            0x195,

            0x197,
        };
        constexpr int kSfxWhitelistLen = sizeof(kSfxWhitelist) / sizeof(kSfxWhitelist[0]);

        bool SfxIsAllowed(int sfxId)
        {
            if (sfxId < 0 || sfxId > 0xFFFF)
                return false;
            const uint16_t id = static_cast<uint16_t>(sfxId);
            for (int i = 0; i < kSfxWhitelistLen; ++i)
            {
                if (kSfxWhitelist[i] == id)
                    return true;
            }
            return false;
        }


        // Find an entry by sfxId. Returns nullptr if not present.
        GhostSlot::ActiveLoop *FindActiveLoop(GhostSlot &slot, uint16_t sfxId)
        {
            for (auto &e : slot.activeLoops)
            {
                if (e.inUse && e.sfxId == sfxId)
                    return &e;
            }
            return nullptr;
        }

        // Find a free slot in the activeLoops table for a new entry.
        // Returns nullptr if all slots are occupied.
        GhostSlot::ActiveLoop *FindFreeActiveLoop(GhostSlot &slot)
        {
            for (auto &e : slot.activeLoops)
            {
                if (!e.inUse)
                    return &e;
            }
            return nullptr;
        }

        void ClearActiveLoop(GhostSlot::ActiveLoop &entry)
        {
            // Channel 0 IS a valid index; only -1 means "no channel."
            if (entry.inUse && entry.channel != -1)
            {
                psndSFXOff(entry.channel);
            }
            entry.inUse = false;
            entry.sfxId = 0;
            entry.channel = 0;
            entry.animNameAtStart[0] = '\0';
            entry.animCandidate[0] = '\0';
            entry.stableFrames = 0;
            entry.watching = false;
        }

        // Stop ALL active loops for a slot. Used on slot release and
        // whenever we want a clean slate (map change, peer disconnect).
        void StopAllActiveLoops(GhostSlot &slot)
        {
            g_inReceiverReplay = true;
            for (auto &e : slot.activeLoops)
            {
                ClearActiveLoop(e);
            }
            g_inReceiverReplay = false;
        }

        // Compatibility wrapper used by ReleaseSlot.
        void StopPeerLoop(GhostSlot &slot)
        {
            StopAllActiveLoops(slot);
        }

        bool IsBlockedSfx(const GhostSlot &slot, uint16_t sfxId);
        bool IsAnimBoundLoop(uint16_t sfxId);

        void SyncActiveLoopsFromState(const PeerSlot &peer, GhostSlot &slot)
        {
            const int published = peer.activeLoopCount > kActiveLoopsPerPeer ? kActiveLoopsPerPeer : peer.activeLoopCount;

            g_inReceiverReplay = true;
            for (auto &e : slot.activeLoops)
            {
                if (!e.inUse)
                    continue;
                bool stillPublished = false;
                for (int i = 0; i < published; ++i)
                {
                    if (peer.activeLoops[i] == e.sfxId)
                    {
                        stillPublished = true;
                        break;
                    }
                }
                if (!stillPublished)
                {
                    ClearActiveLoop(e);
                }
            }
            g_inReceiverReplay = false;

            // Pass 2: start any published loop we aren't tracking.
            // Off-map peers don't get audible loops (no spatial pos).
            if (!PeerOnLocalMap(peer))
                return;

            for (int i = 0; i < published; ++i)
            {
                const uint16_t sfxId = peer.activeLoops[i];
                if (sfxId == 0)
                    continue;
                if (FindActiveLoop(slot, sfxId) != nullptr)
                    continue; // already tracked
                if (IsBlockedSfx(slot, sfxId))
                    continue;

                g_inReceiverReplay = true;
                const int channel = psndSFXOn_3D(sfxId, &peer.position);
                g_inReceiverReplay = false;

                if (channel == -1)
                {
                    continue;
                }

                GhostSlot::ActiveLoop *e = FindFreeActiveLoop(slot);
                if (e == nullptr)
                {
                    // Table full - stop the call to avoid orphan.
                    g_inReceiverReplay = true;
                    psndSFXOff(channel);
                    g_inReceiverReplay = false;
                    continue;
                }
                e->sfxId = sfxId;
                e->channel = channel;
                e->inUse = true;
                std::memcpy(e->animCandidate, peer.animName, sizeof(peer.animName));
                e->animCandidate[sizeof(e->animCandidate) - 1] = '\0';
                e->stableFrames = 1;
                e->watching = false;
                e->animNameAtStart[0] = '\0';
            }
        }

        constexpr uint16_t kAnimBoundLoopSfx[] = {
            0x17B, // mot_roll - sub-phase loop
            0x18F, // mot_ship - phase loop
            0x190, // mot_ship - phase loop
            0x192, // mot_ship - phase loop
        };
        constexpr int kAnimBoundLoopSfxLen = sizeof(kAnimBoundLoopSfx) / sizeof(kAnimBoundLoopSfx[0]);

        bool IsAnimBoundLoop(uint16_t sfxId)
        {
            for (int i = 0; i < kAnimBoundLoopSfxLen; ++i)
            {
                if (kAnimBoundLoopSfx[i] == sfxId)
                    return true;
            }
            return false;
        }

        bool IsBlockedSfx(const GhostSlot &slot, uint16_t sfxId)
        {
            for (uint16_t b : slot.blockedSfx)
            {
                if (b == sfxId)
                    return true;
            }
            return false;
        }

        void AddBlockedSfx(GhostSlot &slot, uint16_t sfxId)
        {
            // Skip if already in the list.
            for (uint16_t b : slot.blockedSfx)
            {
                if (b == sfxId)
                    return;
            }
            // Find a free slot (sfxId == 0).
            for (auto &b : slot.blockedSfx)
            {
                if (b == 0)
                {
                    b = sfxId;
                    return;
                }
            }
            // List full. Highly unlikely - we have 8 slots and only 4
            // anim-bound IDs - but if it happens, drop quietly.
        }

        void RemoveBlockedSfx(GhostSlot &slot, uint16_t sfxId)
        {
            for (auto &b : slot.blockedSfx)
            {
                if (b == sfxId)
                {
                    b = 0;
                    return;
                }
            }
        }

        void RunAnimBoundJanitor(const PeerSlot &peer, GhostSlot &slot)
        {
            const int published = peer.activeLoopCount > kActiveLoopsPerPeer ? kActiveLoopsPerPeer : peer.activeLoopCount;

            // Pass 1: advance latch and/or check drift.
            g_inReceiverReplay = true;
            for (auto &e : slot.activeLoops)
            {
                if (!e.inUse)
                    continue;
                if (!IsAnimBoundLoop(e.sfxId))
                    continue;

                if (!e.watching)
                {
                    if (std::memcmp(e.animCandidate, peer.animName, sizeof(peer.animName)) == 0)
                    {
                        if (e.stableFrames < 0xFF)
                            ++e.stableFrames;
                        if (e.stableFrames >= kAnimStabilityFrames)
                        {
                            // Promote to anchor and start watching.
                            std::memcpy(e.animNameAtStart, e.animCandidate, sizeof(e.animCandidate));
                            e.animNameAtStart[sizeof(e.animNameAtStart) - 1] = '\0';
                            e.watching = true;
                        }
                    }
                    else
                    {
                        // Anim changed during latch period. Reset.
                        std::memcpy(e.animCandidate, peer.animName, sizeof(peer.animName));
                        e.animCandidate[sizeof(e.animCandidate) - 1] = '\0';
                        e.stableFrames = 1;
                    }
                    continue; // not yet armed - no drift check this frame
                }

                // Watching phase: standard drift check.
                if (std::memcmp(e.animNameAtStart, peer.animName, sizeof(peer.animName)) == 0)
                    continue; // anim still matches - leave alone

                const uint16_t stoppedId = e.sfxId;
                ClearActiveLoop(e);

                bool inStateSync = false;
                for (int i = 0; i < published; ++i)
                {
                    if (peer.activeLoops[i] == stoppedId)
                    {
                        inStateSync = true;
                        break;
                    }
                }
                if (inStateSync)
                    AddBlockedSfx(slot, stoppedId);
            }
            g_inReceiverReplay = false;

            // Pass 2: prune blocklist for sfxIds the source has dropped.
            for (auto &b : slot.blockedSfx)
            {
                if (b == 0)
                    continue;
                bool stillPublished = false;
                for (int i = 0; i < published; ++i)
                {
                    if (peer.activeLoops[i] == b)
                    {
                        stillPublished = true;
                        break;
                    }
                }
                if (!stillPublished)
                    b = 0;
            }
        }

        int CurrentMaxRenderedPeers()
        {
            uint32_t v = *GetMaxRenderedPeersPtr();
            if (v > static_cast<uint32_t>(kMaxPeers))
                v = static_cast<uint32_t>(kMaxPeers);
            return static_cast<int>(v);
        }

        bool SlotMayAllocate(const GhostSlot &slot)
        {
            const ptrdiff_t idx = &slot - &g_slots[0];
            return idx >= 0 && idx < CurrentMaxRenderedPeers();
        }

        void EnsurePosesAllocated(GhostSlot &slot)
        {
            if (!SlotMayAllocate(slot))
                return;

            const bool isSlot0 = (&slot == &g_slots[0]);

            if (!slot.forwardAllocated)
            {
                int32_t id = ttyd::animdrv::animPoseEntry(kAgbForward, kPoseGroup);
                if (id >= 0)
                {
                    slot.forwardPoseId = id;
                    slot.forwardAllocated = true;
                    slot.lastAnimForward[0] = '\0';
                    if (isSlot0)
                        ttyd::animdrv::animPoseSetAnim(id, "M_S_1", 1);
                }
            }
            if (!slot.rearAllocated)
            {
                int32_t id = ttyd::animdrv::animPoseEntry(kAgbRear, kPoseGroup);
                if (id >= 0)
                {
                    slot.rearPoseId = id;
                    slot.rearAllocated = true;
                    slot.lastAnimRear[0] = '\0';
                    if (isSlot0)
                        ttyd::animdrv::animPoseSetAnim(id, "M_S_1R", 1);
                }
            }
            if (!slot.effectsAllocated)
            {
                int32_t id = ttyd::animdrv::animPoseEntry(kAgbEffects, kPoseGroup);
                if (id >= 0)
                {
                    slot.effectsPoseId = id;
                    slot.effectsAllocated = true;
                    slot.lastAnimEffects[0] = '\0';
                }
            }
        }

        void ReleaseSlot(GhostSlot &slot)
        {
            if (slot.forwardAllocated)
            {
                ttyd::animdrv::animPoseRelease(slot.forwardPoseId);
                slot.forwardAllocated = false;
                slot.forwardPoseId = -1;
                slot.lastAnimForward[0] = '\0';
            }
            if (slot.rearAllocated)
            {
                ttyd::animdrv::animPoseRelease(slot.rearPoseId);
                slot.rearAllocated = false;
                slot.rearPoseId = -1;
                slot.lastAnimRear[0] = '\0';
            }
            if (slot.effectsAllocated)
            {
                ttyd::animdrv::animPoseRelease(slot.effectsPoseId);
                slot.effectsAllocated = false;
                slot.effectsPoseId = -1;
                slot.lastAnimEffects[0] = '\0';
            }
            slot.activePose = 0;
            slot.lastPaperAnim[0] = '\0';

            if (slot.paperPoseId >= 0)
            {
                ttyd::animdrv::animPaperPoseRelease(slot.paperPoseId);
                slot.paperPoseId = -1;
            }
            slot.lastPaperAgb[0] = '\0';

            slot.renderInitialized = false;

            slot.hitFramesRemaining = 0;

            slot.sfxSeqInitialized = false;
            slot.lastConsumedSfxSeq = 0;

            StopPeerLoop(slot);
            for (auto &b : slot.blockedSfx)
            {
                b = 0;
            }
        }

        int8_t PickPoseIndex(uint32_t flags2)
        {
            if (flags2 & kFlags2EffectsMask)
                return 2;
            if (flags2 & kFlags2RearMask)
                return 1;
            return 0;
        }

        float Lerp(float current, float target, float alpha)
        {
            return current + (target - current) * alpha;
        }

        float LerpAngleDeg(float current, float target, float alpha)
        {
            float diff = target - current;
            while (diff > 180.0f) diff -= 360.0f;
            while (diff < -180.0f) diff += 360.0f;
            return current + diff * alpha;
        }

        constexpr float kFastSpinThresholdDegPerPublish = 90.0f;

        constexpr float kVelocityFilterAlpha = 0.4f;

        float SpinAwareLerpAngle(float current,
                                 float target,
                                 float alpha,
                                 int8_t peerHint,
                                 float &lastSeen,
                                 float &smoothedVel,
                                 bool &initialized)
        {
            if (target != lastSeen)
            {
                if (!initialized)
                {
                    smoothedVel = 0.0f;
                    initialized = true;
                }
                else
                {
                    float pubDelta = target - lastSeen;
                    while (pubDelta > 180.0f) pubDelta -= 360.0f;
                    while (pubDelta < -180.0f) pubDelta += 360.0f;
                    smoothedVel = smoothedVel * (1.0f - kVelocityFilterAlpha) + pubDelta * kVelocityFilterAlpha;
                }
                lastSeen = target;
            }

            float diff = target - current;
            while (diff > 180.0f) diff -= 360.0f;
            while (diff < -180.0f) diff += 360.0f;

            // Direction override priority: peer hint > velocity fallback.
            int directionSign = 0;
            if (peerHint > 0)
                directionSign = 1;
            else if (peerHint < 0)
                directionSign = -1;
            else if (smoothedVel > kFastSpinThresholdDegPerPublish)
                directionSign = 1;
            else if (smoothedVel < -kFastSpinThresholdDegPerPublish)
                directionSign = -1;

            if (directionSign > 0 && diff < 0.0f)
                diff += 360.0f;
            else if (directionSign < 0 && diff > 0.0f)
                diff -= 360.0f;

            return current + diff * alpha;
        }

        void ApplyPeerToSlot(const PeerSlot &peer, GhostSlot &slot)
        {
            EnsurePosesAllocated(slot);

            const int8_t poseIdx = PickPoseIndex(peer.flags2);

            int32_t poseId = -1;
            char *cache = nullptr;
            bool canDrive = false;

            switch (poseIdx)
            {
                case 0:
                    if (slot.forwardAllocated)
                    {
                        poseId = slot.forwardPoseId;
                        cache = slot.lastAnimForward;
                        canDrive = true;
                        slot.activePose = 0;
                    }
                    break;
                case 1:
                    if (slot.rearAllocated)
                    {
                        poseId = slot.rearPoseId;
                        cache = slot.lastAnimRear;
                        canDrive = true;
                        slot.activePose = 1;
                    }
                    break;
                case 2:
                    if (slot.effectsAllocated)
                    {
                        poseId = slot.effectsPoseId;
                        cache = slot.lastAnimEffects;
                        canDrive = true;
                        slot.activePose = 2;
                    }
                    break;
            }

            if (!canDrive)
                return;

            gc::color4 tint = {peer.r, peer.g, peer.b, 255};
            ttyd::animdrv::animPoseSetMaterialEvtColor(poseId, &tint);

            if (std::memcmp(cache, peer.animName, sizeof(peer.animName)) != 0)
            {
                ttyd::animdrv::animPoseSetAnim(poseId, peer.animName, 1);
                std::memcpy(cache, peer.animName, sizeof(peer.animName));
            }


            if (slot.forwardAllocated)
            {
                const bool peerInPaper = peer.paperAgbName[0] != '\0' && peer.paperAnimName[0] != '\0';
                const bool slotInPaper = slot.paperPoseId >= 0;

                const bool agbChanged = std::memcmp(slot.lastPaperAgb, peer.paperAgbName, sizeof(peer.paperAgbName)) != 0;
                const bool animChanged = std::memcmp(slot.lastPaperAnim, peer.paperAnimName, sizeof(peer.paperAnimName)) != 0;

                if (slotInPaper && !peerInPaper)
                {
                    ttyd::animdrv::animPoseSetPaperAnimGroup(slot.forwardPoseId, nullptr, 0);
                    ttyd::animdrv::animPaperPoseRelease(slot.paperPoseId);
                    slot.paperPoseId = -1;
                    slot.lastPaperAgb[0] = '\0';
                    slot.lastPaperAnim[0] = '\0';
                }
                else if (peerInPaper && (!slotInPaper || agbChanged))
                {
                    if (slotInPaper)
                    {
                        ttyd::animdrv::animPoseSetPaperAnimGroup(slot.forwardPoseId, nullptr, 0);
                        ttyd::animdrv::animPaperPoseRelease(slot.paperPoseId);
                        slot.paperPoseId = -1;
                    }

                    int bUnkOn = 1;
                    static const uintptr_t kGauntlet[] = {
                        0x802C1BA0,
                        0x802C1BA8,
                        0x802C1BB0,
                        0x802C1BB8,
                        0x802C1BC4,
                        0x802C1BD0,
                    };
                    for (uintptr_t s : kGauntlet)
                    {
                        const char *cmp = reinterpret_cast<const char *>(s);

                        const char *a = peer.paperAgbName;
                        const char *b = cmp;
                        while (*a && (*a == *b))
                        {
                            ++a;
                            ++b;
                        }
                        if (*a == *b)
                        {
                            bUnkOn = 0;
                            break;
                        }
                    }
                    slot.paperPoseId = ttyd::animdrv::animPaperPoseEntry(peer.paperAgbName, 2);

                    if (slot.paperPoseId >= 0)
                    {
                        ttyd::animdrv::animPoseSetPaperAnimGroup(slot.forwardPoseId, peer.paperAgbName, bUnkOn);
                        ttyd::animdrv::animPoseSetPaperAnim(slot.forwardPoseId, peer.paperAnimName);
                        std::memcpy(slot.lastPaperAgb, peer.paperAgbName, sizeof(peer.paperAgbName));
                        std::memcpy(slot.lastPaperAnim, peer.paperAnimName, sizeof(peer.paperAnimName));
                    }
                }
                else if (peerInPaper && slotInPaper && animChanged)
                {
                    ttyd::animdrv::animPoseSetPaperAnim(slot.forwardPoseId, peer.paperAnimName);
                    std::memcpy(slot.lastPaperAnim, peer.paperAnimName, sizeof(peer.paperAnimName));
                }
            }

            if (peer.sfxCount > 0 && PeerOnLocalMap(peer))
            {
                const int n = peer.sfxCount > kSfxEventsPerSlot ? kSfxEventsPerSlot : peer.sfxCount;

                if (!slot.sfxSeqInitialized)
                {
                    uint8_t maxSeq = 0;
                    for (int i = 0; i < n; ++i)
                    {
                        const uint8_t s = peer.sfxEvents[i].seq;
                        const uint8_t diff = static_cast<uint8_t>(s - maxSeq);
                        if (diff != 0 && diff < 128)
                            maxSeq = s;
                    }
                    slot.lastConsumedSfxSeq = maxSeq;
                    slot.sfxSeqInitialized = true;
                }
                else
                {
                    uint8_t newest = slot.lastConsumedSfxSeq;
                    g_inReceiverReplay = true;
                    for (int i = 0; i < n; ++i)
                    {
                        const SfxEvent &ev = peer.sfxEvents[i];
                        const uint8_t diff = static_cast<uint8_t>(ev.seq - slot.lastConsumedSfxSeq);
                        if (diff == 0 || diff >= 128)
                            continue;

                        bool isLoopState = false;
                        const int published =
                            peer.activeLoopCount > kActiveLoopsPerPeer ? kActiveLoopsPerPeer : peer.activeLoopCount;
                        for (int j = 0; j < published; ++j)
                        {
                            if (peer.activeLoops[j] == ev.sfxId)
                            {
                                isLoopState = true;
                                break;
                            }
                        }
                        if (!isLoopState && FindActiveLoop(slot, ev.sfxId) != nullptr)
                        {
                            isLoopState = true;
                        }
                        if (!isLoopState)
                        {
                            if (IsAnimBoundLoop(ev.sfxId))
                            {
                                if (IsBlockedSfx(slot, ev.sfxId))
                                {
                                    // Skip - janitor previously stopped.
                                }
                                else
                                {
                                    const int channel = psndSFXOn_3D(ev.sfxId, &peer.position);
                                    if (channel != -1)
                                    {
                                        GhostSlot::ActiveLoop *e = FindFreeActiveLoop(slot);
                                        if (e != nullptr)
                                        {
                                            e->sfxId = ev.sfxId;
                                            e->channel = channel;
                                            e->inUse = true;
                                            std::memcpy(e->animCandidate, peer.animName, sizeof(peer.animName));
                                            e->animCandidate[sizeof(e->animCandidate) - 1] = '\0';
                                            e->stableFrames = 1;
                                            e->watching = false;
                                            e->animNameAtStart[0] = '\0';
                                        }
                                        else
                                        {
                                            // Table full - stop to avoid orphan.
                                            psndSFXOff(channel);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                psndSFXOn_3D(ev.sfxId, &peer.position);
                            }
                        }

                        const uint8_t newDiff = static_cast<uint8_t>(ev.seq - newest);
                        if (newDiff != 0 && newDiff < 128)
                            newest = ev.seq;
                    }
                    g_inReceiverReplay = false;
                    slot.lastConsumedSfxSeq = newest;
                }
            }

            SyncActiveLoopsFromState(peer, slot);

            RunAnimBoundJanitor(peer, slot);
        }

        bool g_hammerSwingFired = false;

        int CheckPeerHammerHits(const SharedBlock *block)
        {
            using ttyd::mario_motion::MarioMotion;

            ttyd::mario::Player *me = ttyd::mario::marioGetPtr();
            if (me == nullptr)
                return -1;

            if (g_ghostState != nullptr && g_ghostState->selfGameRole == kGameRoleHider)
            {
                g_hammerSwingFired = true;
                return -1;
            }

            const uint8_t *mpBytes = reinterpret_cast<const uint8_t *>(me);
            const uint16_t curMotRaw = *reinterpret_cast<const uint16_t *>(mpBytes + 0x2E);
            const auto curMot = static_cast<MarioMotion>(curMotRaw);

            if (curMot != MarioMotion::kHammer)
            {
                g_hammerSwingFired = false;
                return -1;
            }

            const int32_t swingFrame = *reinterpret_cast<const int32_t *>(mpBytes + 0x48);

            if (swingFrame != 10)
                return -1;

            if (g_hammerSwingFired)
                return -1;

            const float reachBase = *reinterpret_cast<const float *>(mpBytes + 0x1B8);
            float reachScale = *GetHitReachScalePtr();
            if (!(reachScale > 0.0f))
                reachScale = kDefaultHitReachScale;
            const float reach = reachScale * reachBase;

            void *cam = camGetPtr(4);
            if (cam == nullptr)
                return -1;
            const uint8_t *camBytes = reinterpret_cast<const uint8_t *>(cam);

            const float playerYaw = *reinterpret_cast<const float *>(mpBytes + 0x1AC);
            const float cameraYaw = *reinterpret_cast<const float *>(camBytes + 0x114);
            const float yaw = reviseAngle(playerYaw - 90.0f + cameraYaw);

            const uint8_t selfTeamId = *GetSelfTeamIdPtr();
            const bool friendlyFire = (*GetSelfFriendlyFirePtr() != 0);

            for (int i = 0; i < kMaxPeers; ++i)
            {
                const PeerSlot &peer = block->peers[i];
                if (!peer.active)
                    continue;
                if (!PeerOnLocalMap(peer))
                    continue;
                if (peer.hammerable != 0)
                    continue;

                if (peer.teamId != kTeamNone && peer.teamId == selfTeamId && !friendlyFire)
                    continue;

                NpcEntry fakeNpc {};
                fakeNpc.position = peer.position;

                float peerWidth = *GetHitPeerWidthPtr();
                if (!(peerWidth > 0.0f))
                    peerWidth = kDefaultHitPeerWidth;
                fakeNpc.width = peerWidth;
                fakeNpc.height = 60.0f;

                float distance = 0.0f;
                if (_npcHitCheckHammerAllMotion(&fakeNpc, &distance, reach, yaw) != nullptr)
                {
                    g_hammerSwingFired = true;
                    return i;
                }
            }
            return -1;
        }
    } // namespace

    KEEP_FUNC void Init()
    {
        if (g_initialized)
            return;

        if (g_ghostState == nullptr)
        {
            void *raw = ttyd::memory::__memAlloc(ttyd::memory::HeapType::HEAP_DEFAULT, sizeof(GhostState));
            g_ghostState = reinterpret_cast<GhostState *>(raw);
        }
        if (g_ghostState == nullptr)
        {
            // Allocation failed - cannot proceed. Leaving g_initialized
            // false ensures UpdateAll/DrawAll bail at their first check.
            return;
        }

        std::memset(g_ghostState, 0, sizeof(GhostState));

        g_ghostState->peerBlock.magic = kMagic;
        g_ghostState->peerBlock.version = kVersion;

        // Hit-system tunables (live-tunable from Python via memory
        // edits). These match the old Init() defaults.
        g_ghostState->hitReachScale = kDefaultHitReachScale;
        g_ghostState->hitPeerWidth = kDefaultHitPeerWidth;
        std::memset(g_ghostState->hitPoseName, 0, sizeof(g_ghostState->hitPoseName));
        std::strncpy(g_ghostState->hitPoseName, kDefaultHitPoseName, sizeof(g_ghostState->hitPoseName) - 1);

        // Renderer cap, default 12. Python can edit this byte to
        // change the cap at runtime.
        g_ghostState->maxRenderedPeers = static_cast<uint32_t>(kDefaultMaxRenderedPeers);

        // Self team defaults (no team, no friendly fire). All other
        // hit/SFX-ring/lobby fields stay zero from the memset above.
        g_ghostState->selfTeamId = kTeamNone;
        g_ghostState->selfFriendlyFire = 0;

        if (mod::owr::gState != nullptr && mod::owr::gState->apSettings != nullptr)
        {
            mod::owr::gState->apSettings->ghostStatePtr = g_ghostState;
        }

        // GhostSlot bookkeeping (mod-internal, not in GhostState).
        for (auto &s : g_slots)
        {
            s.forwardAllocated = false;
            s.rearAllocated = false;
            s.effectsAllocated = false;
            s.forwardPoseId = -1;
            s.rearPoseId = -1;
            s.effectsPoseId = -1;
            s.activePose = 0;
            s.lastAnimForward[0] = '\0';
            s.lastAnimRear[0] = '\0';
            s.lastAnimEffects[0] = '\0';
            s.lastPaperAnim[0] = '\0';
            s.paperPoseId = -1;
            s.lastPaperAgb[0] = '\0';

            s.renderX = 0.0f;
            s.renderY = 0.0f;
            s.renderZ = 0.0f;
            s.renderRotY = 0.0f;
            s.renderRotX = 0.0f;
            s.renderRotZ = 0.0f;
            s.renderPivotX = 0.0f;
            s.renderPivotY = 0.0f;
            s.renderPivotZ = 0.0f;
            s.lastSeenRotY = 0.0f;
            s.lastSeenRotX = 0.0f;
            s.lastSeenRotZ = 0.0f;
            s.velRotY = 0.0f;
            s.velRotX = 0.0f;
            s.velRotZ = 0.0f;
            s.spinTrackingInitY = false;
            s.spinTrackingInitX = false;
            s.spinTrackingInitZ = false;
            s.renderInitialized = false;
            s.hitFramesRemaining = 0;

            s.lastConsumedSfxSeq = 0;
            s.sfxSeqInitialized = false;
            for (auto &e : s.activeLoops)
            {
                e.inUse = false;
                e.sfxId = 0;
                e.channel = 0;
                e.animNameAtStart[0] = '\0';
                e.animCandidate[0] = '\0';
                e.stableFrames = 0;
                e.watching = false;
            }
            for (auto &b : s.blockedSfx)
            {
                b = 0;
            }
        }

        g_initialized = true;
    }

    KEEP_FUNC void Shutdown()
    {
        for (auto &s : g_slots) ReleaseSlot(s);
        g_initialized = false;
    }

    namespace
    {
        constexpr int kLocalChannelMapSize = 64;
        struct LocalChannelEntry
        {
            int channel;
            uint16_t sfxId;
            bool inUse;
        };
        LocalChannelEntry g_localChannelMap[kLocalChannelMapSize] = {};

        void RecordLocalChannel(int channel, uint16_t sfxId)
        {
            if (channel == -1)
                return;

            // Update existing entry first (channel reuse).
            for (auto &e : g_localChannelMap)
            {
                if (e.inUse && e.channel == channel)
                {
                    e.sfxId = sfxId;
                    return;
                }
            }
            // Else find free slot.
            for (auto &e : g_localChannelMap)
            {
                if (!e.inUse)
                {
                    e.channel = channel;
                    e.sfxId = sfxId;
                    e.inUse = true;
                    return;
                }
            }
        }

        uint16_t RemoveLocalChannel(int channel)
        {
            if (channel == -1)
                return 0;
            for (auto &e : g_localChannelMap)
            {
                if (e.inUse && e.channel == channel)
                {
                    uint16_t sfxId = e.sfxId;
                    e.inUse = false;
                    e.channel = 0;
                    e.sfxId = 0;
                    return sfxId;
                }
            }
            return 0;
        }

        int SampleActiveLoops(uint16_t *out, int maxOut)
        {
            int count = 0;
            for (auto &e : g_localChannelMap)
            {
                if (count >= maxOut)
                    break;
                if (e.inUse && e.sfxId != 0)
                {
                    out[count++] = e.sfxId;
                }
            }
            return count;
        }

        bool PushSfxRingEvent(uint16_t sfxId, uint8_t flags)
        {
            volatile uint8_t *headPtr = GetSfxRingHeadPtr();
            volatile uint8_t *tailPtr = GetSfxRingTailPtr();
            volatile uint8_t *seqPtr = GetSfxRingSeqPtr();
            volatile SfxEvent *ring = GetSfxRingEvents();

            const uint8_t head = *headPtr;
            const uint8_t tail = *tailPtr;
            const uint8_t nextHead = static_cast<uint8_t>((head + 1) % kSfxRingCapacity);
            if (nextHead == tail)
                return false;

            const uint8_t newSeq = static_cast<uint8_t>(*seqPtr + 1);
            *seqPtr = newSeq;
            ring[head].sfxId = sfxId;
            ring[head].seq = newSeq;
            ring[head].flags = flags;
            *headPtr = nextHead;
            return true;
        }
    } // namespace

    KEEP_FUNC void UpdateAll()
    {
        if (!g_initialized)
            return;

        const SharedBlock *block = GetValidBlock();
        if (block == nullptr)
        {
            for (auto &s : g_slots) ReleaseSlot(s);
            return;
        }

        {
            constexpr size_t kMapNameLen = 16;
            static char s_lastMapName[kMapNameLen] = {0};
            const auto *gw = _globalWorkPtr;
            if (gw != nullptr)
            {
                const char *currentMap = gw->currentMapName;
                if (std::memcmp(s_lastMapName, currentMap, kMapNameLen) != 0)
                {
                    for (auto &s : g_slots) ReleaseSlot(s);
                    std::memcpy(s_lastMapName, currentMap, kMapNameLen);
                }
            }
        }

        {
            static bool s_ourLockApplied = false;
            const bool wantFrozen = (g_ghostState->selfFrozen != 0);
            const bool inputFree  = (ttyd::mario::marioChkKey() != 0);

            if (wantFrozen)
            {
                if (inputFree)
                {
                    ttyd::mario::marioKeyOff();
                    s_ourLockApplied = true;
                }
            }
            else if (s_ourLockApplied)
            {
                if (!inputFree)
                {
                    ttyd::mario::marioKeyOn();
                }
                s_ourLockApplied = false;
            }
        }

        {
            static uint8_t s_lastTeleportSeq = 0;
            const uint8_t curSeq = g_ghostState->pendingTeleportSeq;
            const bool seqChanged = (curSeq != s_lastTeleportSeq);
            const bool mapPresent = (g_ghostState->pendingTeleportMap[0] != '\0');
            if (seqChanged && mapPresent)
            {
                const char *bero = (g_ghostState->pendingTeleportBero[0] != '\0')
                                       ? g_ghostState->pendingTeleportBero
                                       : nullptr;
                s_lastTeleportSeq = curSeq;
                ttyd::seqdrv::seqSetSeq(
                    ttyd::seqdrv::SeqIndex::kMapChange,
                    g_ghostState->pendingTeleportMap,
                    bero);
            }
        }

        for (int i = 0; i < kMaxPeers; ++i)
        {
            const PeerSlot &peer = block->peers[i];
            GhostSlot &slot = g_slots[i];

            if (!peer.active)
            {
                ReleaseSlot(slot);
                continue;
            }

            ApplyPeerToSlot(peer, slot);


            // (removed) hitFramesRemaining decrement. Field no longer
            // gates rendering — see the prediction-removal note above.

            if (!slot.renderInitialized)
            {
                slot.renderX = peer.position.x;
                slot.renderY = peer.position.y;
                slot.renderZ = peer.position.z;
                slot.renderRotY = peer.rotationY;
                slot.renderRotX = peer.rotationX;
                slot.renderRotZ = peer.rotationZ;
                slot.renderPivotX = peer.rotPivotX;
                slot.renderPivotY = peer.rotPivotY;
                slot.renderPivotZ = peer.rotPivotZ;

                slot.lastSeenRotY = peer.rotationY;
                slot.lastSeenRotX = peer.rotationX;
                slot.lastSeenRotZ = peer.rotationZ;
                slot.velRotY = 0.0f;
                slot.velRotX = 0.0f;
                slot.velRotZ = 0.0f;
                slot.spinTrackingInitY = false;
                slot.spinTrackingInitX = false;
                slot.spinTrackingInitZ = false;

                slot.renderInitialized = true;
            }
            else
            {
                slot.renderX = Lerp(slot.renderX, peer.position.x, kLerpAlpha);
                slot.renderY = Lerp(slot.renderY, peer.position.y, kLerpAlpha);
                slot.renderZ = Lerp(slot.renderZ, peer.position.z, kLerpAlpha);

                slot.renderRotY = SpinAwareLerpAngle(slot.renderRotY,
                                                     peer.rotationY,
                                                     kLerpAlpha,
                                                     peer.spinDirHintY,
                                                     slot.lastSeenRotY,
                                                     slot.velRotY,
                                                     slot.spinTrackingInitY);
                slot.renderRotX = SpinAwareLerpAngle(slot.renderRotX,
                                                     peer.rotationX,
                                                     kLerpAlpha,
                                                     peer.spinDirHintX,
                                                     slot.lastSeenRotX,
                                                     slot.velRotX,
                                                     slot.spinTrackingInitX);
                slot.renderRotZ = SpinAwareLerpAngle(slot.renderRotZ,
                                                     peer.rotationZ,
                                                     kLerpAlpha,
                                                     peer.spinDirHintZ,
                                                     slot.lastSeenRotZ,
                                                     slot.velRotZ,
                                                     slot.spinTrackingInitZ);

                slot.renderPivotX = Lerp(slot.renderPivotX, peer.rotPivotX, kLerpAlpha);
                slot.renderPivotY = Lerp(slot.renderPivotY, peer.rotPivotY, kLerpAlpha);
                slot.renderPivotZ = Lerp(slot.renderPivotZ, peer.rotPivotZ, kLerpAlpha);
            }

            if (PeerOnLocalMap(peer))
            {
                int32_t activePoseId = -1;
                if (slot.activePose == 2 && slot.effectsAllocated)
                {
                    ttyd::animdrv::animPoseMain(slot.effectsPoseId);
                    activePoseId = slot.effectsPoseId;
                }
                else if (slot.activePose == 1 && slot.rearAllocated)
                {
                    ttyd::animdrv::animPoseMain(slot.rearPoseId);
                    activePoseId = slot.rearPoseId;
                }
                else if (slot.forwardAllocated)
                {
                    ttyd::animdrv::animPoseMain(slot.forwardPoseId);
                    activePoseId = slot.forwardPoseId;
                }

                if (activePoseId >= 0 && peer.paperLocalTime != -1.0f)
                {
                    ttyd::animdrv::animPoseSetLocalTime(activePoseId, peer.paperLocalTime);
                }
            }
        }

        ttyd::mario::Player *mp = ttyd::mario::marioGetPtr();
        if (mp != nullptr)
        {
            const uint8_t *mpBytes = reinterpret_cast<const uint8_t *>(mp);
            const int32_t marioFwd = *reinterpret_cast<const int32_t *>(mpBytes + 0x22C);

            const uint32_t marioMatFlag = ttyd::animdrv::animPoseGetMaterialFlag(marioFwd);
            const uint32_t marioLitFlag = ttyd::animdrv::animPoseGetMaterialLightFlag(marioFwd);

            for (auto &slot : g_slots)
            {
                if (slot.forwardAllocated)
                {
                    ttyd::animdrv::animPoseSetMaterialFlagOn(slot.forwardPoseId, marioMatFlag);
                    ttyd::animdrv::animPoseSetMaterialFlagOff(slot.forwardPoseId, ~marioMatFlag);
                    ttyd::animdrv::animPoseSetMaterialLightFlagOn(slot.forwardPoseId, marioLitFlag);
                    ttyd::animdrv::animPoseSetMaterialLightFlagOff(slot.forwardPoseId, ~marioLitFlag);
                }
                if (slot.rearAllocated)
                {
                    ttyd::animdrv::animPoseSetMaterialFlagOn(slot.rearPoseId, marioMatFlag);
                    ttyd::animdrv::animPoseSetMaterialFlagOff(slot.rearPoseId, ~marioMatFlag);
                    ttyd::animdrv::animPoseSetMaterialLightFlagOn(slot.rearPoseId, marioLitFlag);
                    ttyd::animdrv::animPoseSetMaterialLightFlagOff(slot.rearPoseId, ~marioLitFlag);
                }
                if (slot.effectsAllocated)
                {
                    ttyd::animdrv::animPoseSetMaterialFlagOn(slot.effectsPoseId, marioMatFlag);
                    ttyd::animdrv::animPoseSetMaterialFlagOff(slot.effectsPoseId, ~marioMatFlag);
                    ttyd::animdrv::animPoseSetMaterialLightFlagOn(slot.effectsPoseId, marioLitFlag);
                    ttyd::animdrv::animPoseSetMaterialLightFlagOff(slot.effectsPoseId, ~marioLitFlag);
                }
            }

            char *selfPaperAgb = GetSelfPaperAgbName();

            for (int i = 0; i < kSelfPaperAgbLen; ++i) selfPaperAgb[i] = '\0';

            const int32_t marioPaperPoseId = *reinterpret_cast<const int32_t *>(mpBytes + 0x240);
            if (marioPaperPoseId >= 0)
            {
                const uint16_t motionId = *reinterpret_cast<const uint16_t *>(mpBytes + 0x2E);

                const char *shortName = nullptr;
                switch (motionId)
                {
                    case 0x13:

                        shortName = reinterpret_cast<const char *>(0x802C3F70);
                        break;
                    case 0x14:

                        shortName = reinterpret_cast<const char *>(0x802C4008);
                        break;
                    case 0x15:

                        shortName = reinterpret_cast<const char *>(0x802C433C);
                        break;
                    case 0x16:

                        shortName = reinterpret_cast<const char *>(0x802C4248);
                        break;
                    case 0x18:
                    {
                        const int8_t color = ttyd::mario::marioGetColor();
                        const uint32_t *arr = reinterpret_cast<const uint32_t *>(0x80416228);
                        shortName = reinterpret_cast<const char *>(arr[color & 3]);
                        break;
                    }
                    case 0x19:
                    {
                        const int8_t color = ttyd::mario::marioGetColor();
                        const uint32_t *arr = reinterpret_cast<const uint32_t *>(0x80416280);
                        shortName = reinterpret_cast<const char *>(arr[color & 3]);
                        break;
                    }
                    case 0x1D:
                    {
                        const int8_t dokanType = *reinterpret_cast<const int8_t *>(mpBytes + 0x3E);
                        const bool isVertical = (dokanType == 1) || (dokanType == 3);
                        shortName = reinterpret_cast<const char *>(isVertical ? 0x802F45FC : 0x802F4610);
                        break;
                    }
                    default:

                        break;
                }

                if (shortName != nullptr)
                {
                    for (int i = 0; i < kSelfPaperAgbLen - 1; ++i)
                    {
                        const char c = shortName[i];
                        selfPaperAgb[i] = c;
                        if (c == '\0')
                            break;
                    }
                }
            }

            {
                volatile uint16_t *out = GetSelfActiveLoopsPtr();
                volatile uint8_t *outCount = GetSelfActiveLoopCountPtr();
                uint16_t buf[kActiveLoopsPerPeer] = {};
                const int n = SampleActiveLoops(buf, kActiveLoopsPerPeer);
                for (int i = 0; i < kActiveLoopsPerPeer; ++i)
                {
                    out[i] = buf[i];
                }
                *outCount = static_cast<uint8_t>(n);
            }
        }

        {
            volatile uint32_t *pending = GetPendingHitPtr();
            const uint32_t raw = *pending;
            if (raw != 0)
            {
                const uint8_t kind = static_cast<uint8_t>(raw >> 24);
                if (kind == kHitKindHammer && g_hitGraceRemaining == 0 && !g_hitQueued)
                {
                    g_hitQueued = true;
                    g_hitQueuedTimeout = kHitQueueTimeoutFrames;
                    g_hitGraceRemaining = kHitGraceFrames;
                }
                *pending = 0;
            }
        }

        if (g_hitQueued)
        {
            if (g_hitQueuedTimeout > 0)
                --g_hitQueuedTimeout;

            const bool ready = (ttyd::mario::marioChkKey() != 0);
            if (g_hitQueuedTimeout == 0)
            {
                // Timed out without ever becoming ready. Drop the hit.
                g_hitQueued = false;
            }
            else if (ready && !g_hitLockApplied)
            {
                ttyd::mario::Player *me = ttyd::mario::marioGetPtr();
                if (me != nullptr && g_ghostState != nullptr)
                {
                    me->animName = g_ghostState->hitPoseName;

                    me->flags2 |= 0x1000u;

                    ttyd::pmario_sound::psndSFXOn(0x0BA);

                    ttyd::mario::marioKeyOff();
                    g_hitLockApplied = true;
                    g_hitLockRemaining = kHitLockDurationFrames;
                }
                g_hitQueued = false;
            }
        }

        if (g_hitLockRemaining > 0)
        {
            --g_hitLockRemaining;
            if (g_hitLockRemaining == 0)
            {
                ttyd::mario::Player *me = ttyd::mario::marioGetPtr();
                if (me != nullptr)
                {
                    me->flags2 &= ~0x1000u;
                }
                if (g_hitLockApplied)
                {
                    if (ttyd::mario::marioChkKey() == 0)
                    {
                        ttyd::mario::marioKeyOn();
                    }
                    g_hitLockApplied = false;
                }
            }
        }

        if (g_hitGraceRemaining > 0)
            --g_hitGraceRemaining;
        *GetHitGracePtr() = (g_hitGraceRemaining > 0) ? 1 : 0;

        {
            const int hitSlot = CheckPeerHammerHits(block);
            if (hitSlot >= 0)
            {
                volatile uint32_t *outbound = GetOutboundHitPtr();
                if (*outbound == 0)
                {
                    *outbound = PackOutboundHit(kHitKindHammer, static_cast<uint8_t>(hitSlot));
                }
            }
        }

        {
            uint32_t activePeers = 0;
            uint32_t posesAlloc = 0;
            for (int i = 0; i < kMaxPeers; ++i)
            {
                if (block->peers[i].active)
                    ++activePeers;
                if (g_slots[i].forwardAllocated)
                    ++posesAlloc;
                if (g_slots[i].rearAllocated)
                    ++posesAlloc;
                if (g_slots[i].effectsAllocated)
                    ++posesAlloc;
                if (g_slots[i].paperPoseId >= 0)
                    ++posesAlloc;
            }
        }
    }

    KEEP_FUNC void DrawAll(ttyd::dispdrv::CameraId, void *)
    {
        if (!g_initialized)
            return;

        const SharedBlock *block = GetValidBlock();
        if (block == nullptr)
            return;

        for (int i = 0; i < kMaxPeers; ++i)
        {
            const PeerSlot &peer = block->peers[i];
            const GhostSlot &slot = g_slots[i];

            if (!peer.active)
                continue;
            if (!PeerOnLocalMap(peer))
                continue;

            int32_t poseId;
            if (slot.activePose == 2)
            {
                if (!slot.effectsAllocated)
                    continue;
                poseId = slot.effectsPoseId;
            }
            else if (slot.activePose == 1)
            {
                if (!slot.rearAllocated)
                    continue;
                poseId = slot.rearPoseId;
            }
            else
            {
                if (!slot.forwardAllocated)
                    continue;
                poseId = slot.forwardPoseId;
            }

            constexpr float kDeg2Rad = 0.01745329252f;

            gc::mat3x4 matA;
            gc::mat3x4 matStep;

            float sx = peer.scaleX, sy = peer.scaleY, sz = peer.scaleZ;
            if (sx == 0.0f && sy == 0.0f && sz == 0.0f)
            {
                sx = sy = sz = 1.0f;
            }

            float fixupX = 1.0f, fixupY = 1.0f, fixupZ = 1.0f;
            switch (peer.motionId)
            {
                case 0x16:
                    fixupX = 0.75f;
                    break;
                default:
                    break;
            }

            gc::mtx::PSMTXScale(&matA, sx * kGhostScale * fixupX, sy * kGhostScale * fixupY, sz * kGhostScale * fixupZ);

            if (!(peer.flags2 & 0x8) && peer.motionId == 0x14)
            {
                float pitchAng = slot.renderRotX;
                while (pitchAng < 0.0f) pitchAng += 360.0f;
                while (pitchAng >= 360.0f) pitchAng -= 360.0f;
                if (pitchAng >= 90.0f && pitchAng <= 270.0f)
                {
                    gc::mtx::PSMTXScale(&matStep, 1.0f, 1.0f, -1.0f);
                    gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
                }
            }

            if (!(peer.flags2 & 0x8))
            {
                float ang = slot.renderRotY;

                while (ang < 0.0f) ang += 360.0f;
                while (ang >= 360.0f) ang -= 360.0f;
                if (ang > 90.0f && ang <= 270.0f)
                {
                    gc::mtx::PSMTXScale(&matStep, 1.0f, 1.0f, -1.0f);
                    gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
                }
            }

            const bool pivotActive = slot.renderPivotX != 0.0f || slot.renderPivotY != 0.0f || slot.renderPivotZ != 0.0f;
            if (pivotActive)
            {
                gc::mtx::PSMTXTrans(reinterpret_cast<gc::mtx34 *>(&matStep),
                                    -slot.renderPivotX,
                                    -slot.renderPivotY,
                                    -slot.renderPivotZ);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            if (slot.renderRotZ != 0.0f)
            {
                gc::mtx::PSMTXRotRad(&matStep, 0x7A, slot.renderRotZ * kDeg2Rad);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            if (slot.renderRotX != 0.0f)
            {
                gc::mtx::PSMTXRotRad(&matStep, 0x78, slot.renderRotX * kDeg2Rad);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            float yawDeg = slot.renderRotY - peer.cameraAngle;
            while (yawDeg < 0.0f) yawDeg += 360.0f;
            while (yawDeg >= 360.0f) yawDeg -= 360.0f;
            gc::mtx::PSMTXRotRad(&matStep, 0x79, yawDeg * kDeg2Rad);
            gc::mtx::PSMTXConcat(&matStep, &matA, &matA);

            if (pivotActive)
            {
                gc::mtx::PSMTXTrans(reinterpret_cast<gc::mtx34 *>(&matStep),
                                    slot.renderPivotX,
                                    slot.renderPivotY,
                                    slot.renderPivotZ);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            if (peer.stretchY != 1.0f && peer.stretchY != 0.0f)
            {
                gc::mtx::PSMTXScale(&matStep, 1.0f, peer.stretchY, 1.0f);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            gc::mtx::PSMTXTrans(reinterpret_cast<gc::mtx34 *>(&matStep), slot.renderX, slot.renderY, slot.renderZ);
            gc::mtx::PSMTXConcat(&matStep, &matA, &matA);

            ttyd::animdrv::animPoseDrawMtx(poseId, &matA, 2, 0.0f, 1.0f);
            ttyd::animdrv::animPoseDrawMtx(poseId, &matA, 1, 0.0f, 1.0f);
        }
    }

    KEEP_FUNC void DrawNameTagsAll(ttyd::dispdrv::CameraId, void *)
    {
        if (!g_initialized)
            return;

        const SharedBlock *block = GetValidBlock();
        if (block == nullptr)
            return;

        void *camPtr = camGetPtr(4);
        if (camPtr == nullptr)
            return;

        gc::mat4x4 *projMtx = reinterpret_cast<gc::mat4x4 *>(reinterpret_cast<char *>(camPtr) + 0x15C);

        gc::mat3x4 *viewMtx = reinterpret_cast<gc::mat3x4 *>(reinterpret_cast<char *>(camPtr) + 0x11C);

        ttyd::fontmgr::FontDrawStart();
        ttyd::fontmgr::FontDrawEdge();

        ttyd::fontmgr::FontDrawScale(kNameTagFontScale);

        const uint8_t selfRole = (g_ghostState != nullptr)
                                     ? g_ghostState->selfGameRole
                                     : kGameRoleNone;

        for (int i = 0; i < kMaxPeers; ++i)
        {
            const PeerSlot &peer = block->peers[i];
            const GhostSlot &slot = g_slots[i];

            if (!peer.active)
                continue;
            if (!PeerOnLocalMap(peer))
                continue;
            if (peer.slotName[0] == '\0')
                continue;

            if (peer.showName != 0)
                continue;

            if (selfRole == kGameRoleHider)
                continue;
            if (selfRole == kGameRoleSeeker && peer.gameRole != kGameRoleSeeker)
                continue;

            gc::vec3 worldPos = {slot.renderX, slot.renderY + kNameTagWorldYOffset, slot.renderZ};
            gc::vec3 camPos = {0.0f, 0.0f, 0.0f};
            gc::mtx::PSMTXMultVec(viewMtx, &worldPos, &camPos);

            gc::vec3 ndcPos = {0.0f, 0.0f, 0.0f};
            gc::mtx::PSMTX44MultVec(projMtx, &camPos, &ndcPos);

            const float ndcX = ndcPos.x;
            const float ndcY = ndcPos.y;
            const float ndcZ = ndcPos.z;

            if (ndcZ < -1.5f || ndcZ > 1.5f)
            {
                continue;
            }

            if (ndcX < -1.5f || ndcX > 1.5f || ndcY < -1.5f || ndcY > 1.5f)
            {
                continue;
            }

            float screenX = ndcX * kNameTagScreenScaleX;
            float screenY = ndcY * kNameTagScreenScaleY;

            const uint16_t textWidth = ttyd::fontmgr::FontGetMessageWidth(peer.slotName);
            screenX -= (static_cast<float>(textWidth) * kNameTagFontScale) * 0.5f;

            uint32_t packed;
            if (peer.gameRole == kGam