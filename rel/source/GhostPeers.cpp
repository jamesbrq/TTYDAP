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

            // Last-seen peer.mapName, used to detect cross-map
            // transitions so we can drop the SFX queue once on
            // arrival — otherwise pre-arrival landing/footstep SFX
            // sitting in the ring replay as a "room entered" chorus.
            char lastMapName[16];

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
            // Mario-firing SFX from pssfxlist (overworld + voice).
            // Battle-only Mario SFX (BTL_MARIO_*, MARIO_BATTLE_*,
            // MARIO_DAMAGE_*, MARIO_GUARD/CHARGE/APPEAL, etc., plus
            // VOICE_MARIO_BATTLE_ACTION/ACROBAT/JUMP) are excluded —
            // battle is single-player, no peer can be in our battle,
            // and broadcasting them produces a 'crowd murmur' chatter
            // from peers in their own battles. See SFX_TABLE.csv +
            // WHITELIST_NAMES.txt for the index -> name mapping.
            0x064, 0x085, 0x087, 0x088, 0x089, 0x08A, 0x08B, 0x08C,
            0x08D, 0x08F, 0x091, 0x09D, 0x09E, 0x09F, 0x0A0, 0x0A1,
            0x0A5, 0x0A6, 0x0A7, 0x0A8, 0x0A9, 0x0AB, 0x0AC, 0x0AD,
            0x0AE, 0x0AF, 0x0B1, 0x0B2, 0x0B3, 0x0B4, 0x0B5, 0x0B7,
            0x0B9, 0x0BA, 0x0BC, 0x0BE, 0x0BF, 0x0C0, 0x0C2, 0x0C3,
            0x0C4, 0x0C5, 0x0C7, 0x0C8, 0x0CA, 0x0CB, 0x0CD, 0x0CE,
            0x0D0, 0x0D1, 0x0D2, 0x0D3, 0x0D4, 0x0D5, 0x0D7, 0x0D8,
            0x0D9, 0x0DB, 0x0DC, 0x0DE, 0x0DF, 0x0E1, 0x0E2, 0x0E4,
            0x0E6, 0x0E7, 0x0E8, 0x0E9, 0x0EA, 0x0EC, 0x0ED, 0x0EF,
            0x140, 0x141, 0x142, 0x143, 0x144, 0x145, 0x146, 0x147,
            0x148, 0x149, 0x14A, 0x14B, 0x14D, 0x14F, 0x150, 0x151,
            0x152, 0x153, 0x154, 0x156, 0x157, 0x158, 0x159, 0x15B,
            0x15C, 0x15D, 0x15E, 0x15F, 0x160, 0x161, 0x162, 0x163,
            0x165, 0x166, 0x167, 0x168, 0x169, 0x16B, 0x16D, 0x16F,
            0x170, 0x171, 0x173, 0x174, 0x175, 0x177, 0x178, 0x179,
            0x17A, 0x17B, 0x17C, 0x17D, 0x17E, 0x17F, 0x180, 0x181,
            0x182, 0x183, 0x184, 0x185, 0x186, 0x187, 0x188, 0x189,
            0x18A, 0x18B, 0x18C, 0x18D, 0x18E, 0x18F, 0x190, 0x191,
            0x192, 0x194, 0x195, 0x196, 0x197, 0x199, 0x19B, 0x19C,
            0x19E, 0x19F, 0x1A1, 0x1A2, 0x1A4, 0x1A5, 0x1A6, 0x1A8,
            0x1A9, 0x1AA, 0x1AB, 0x200, 0x3BA, 0x3BB, 0x3BC, 0x3ED,
            0x3EF, 0x42A, 0x42B,
            0x42E,                                  // SFX_STG4_WAVE1
            0x5D9, 0x5DA, 0x5F7, 0x5F9, 0x5FA,
            0x686,                                  // SFX_STG5_WAVE1 (Keelhaul Key)
            0x6EF, 0x702, 0x703, 0x704, 0x705, 0x7B5,
            // Water / sea / ship ambient — re-added after the
            // "Mario-only substring" pass dropped them. Routes
            // through psndSFXOn so the existing hook captures them;
            // mot_ship.s itself does not call psndENV*. Restores
            // boat-ride water ambient for peers on the Rogueport ↔
            // Keelhaul Key boat sequence.
            0x8D9, 0x8DA, 0x8DB, 0x8DC,             // SFX_EVT_GAME_BOAT_*
            0x1013, 0x1014,                          // SFX_AMB_SEA1/2
            0x1015, 0x1016,                          // SFX_ENV_SHIP1/2
            0x1017,                                  // SFX_ENV_SEA_GULL1
            0x1018,                                  // SFX_AMB_SHIP_CREAK1
            0x1027, 0x1028,                          // SFX_ENV_WATER1/2
            0x1040,                                  // SFX_AMB_WATER3
            0x1048, 0x1049,                          // SFX_AMB_WATER_WOOD1/2
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
                // Defense in depth against peers running an older
                // mod that didn't filter selfActiveLoops on its
                // source side: refuse to start any sfxId that isn't
                // in our local replay whitelist. Stops UI/menu
                // sounds (e.g. SFX_PRESS_START1 = 8) leaking in as
                // arrival jingles.
                if (!SfxIsAllowed(sfxId))
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
            slot.lastMapName[0] = '\0';

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

            // Detect peer-side map changes. When a peer transitions
            // to a new map (their own teleport, or first-time becoming
            // visible to us on a map they were already on), drop the
            // SFX sequence cursor so the existing skip-on-first-sample
            // path silently consumes any pre-arrival landing/footstep
            // events that are still in the ring. Without this, every
            // peer's room-entry SFX replays as a chorus of arrival
            // sounds whenever they cross a map boundary while their
            // slot stays active.
            if (std::memcmp(slot.lastMapName, peer.mapName, sizeof(peer.mapName)) != 0)
            {
                slot.sfxSeqInitialized = false;
                StopAllActiveLoops(slot);
                std::memcpy(slot.lastMapName, peer.mapName, sizeof(peer.mapName));
            }

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

            const float meY = me->playerPosition.y;
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

                // Vertical tolerance: reject hits when the peer is more
                // than kDefaultHitVerticalTolerance units above or below
                // us. Stops "stacked-platform" false positives where the
                // horizontal cylinder check would otherwise land a hit
                // through a floor/ceiling.
                const float dy = peer.position.y - meY;
                const float absDy = dy < 0.0f ? -dy : dy;
                if (absDy > kDefaultHitVerticalTolerance)
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
            s.lastMapName[0] = '\0';
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
                // RecordLocalChannel intentionally tracks EVERY channel
                // the engine allocates (so RemoveLocalChannel can clean
                // up on stop). But state-sync should only republish
                // SFX that are actually intended for peer mirroring —
                // i.e., the same whitelist the SFX ring uses. Without
                // this filter, UI/menu sounds like SFX_PRESS_START1
                // (index 8) leak into selfActiveLoops, get published,
                // and play on every peer's screen as a "ghost arrived"
                // jingle the moment they first see us.
                if (e.inUse && e.sfxId != 0 && SfxIsAllowed(e.sfxId))
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
                ttyd::mario_motion::marioChgMot(ttyd::mario_motion::MarioMotion::kStay);
                ttyd::seqdrv::seqSetSeq(
                    ttyd::seqdrv::SeqIndex::kMapChange,
                    g_ghostState->pendingTeleportMap,
                    bero);
            }
        }

        // Debug /hns play_sfx command bus. Edge-detect on debugSfxSeq
        // and fire psndSFXOn for the requested id. The OWR.cpp hook
        // captures the call into OnLocalSfxFired (where SfxIsAllowed
        // gates whether it propagates to peers), so this can also be
        // used to verify whether a candidate id is whitelisted.
        {
            static uint8_t s_lastDebugSfxSeq = 0;
            const uint8_t curSeq = g_ghostState->debugSfxSeq;
            if (curSeq != s_lastDebugSfxSeq)
            {
                s_lastDebugSfxSeq = curSeq;
                const int sfxId = static_cast<int>(g_ghostState->debugSfxId);
                if (sfxId > 0)
                {
                    if (g_ghostState->debugSfxFlags & 0x01)
                    {
                        ttyd::mario::Player *me = ttyd::mario::marioGetPtr();
                        if (me != nullptr)
                        {
                            ttyd::pmario_sound::psndSFXOn_3D(sfxId, &me->playerPosition);
                        }
                        else
                        {
                            ttyd::pmario_sound::psndSFXOn(sfxId);
                        }
                    }
                    else
                    {
                        ttyd::pmario_sound::psndSFXOn(sfxId);
                    }
                }
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

                if (peer.paperLocalTime != -1.0f)
                {
                    // The engine's animPoseSetLocalTime call site
                    // depends on which pose the motion targets:
                    //   - mot_jabara (M_W_6, motion_id 0x14) and
                    //     mot_hammer2 (P_H_1A, motion_id 0x13) pin
                    //     the body pose (forward/rear/effects).
                    //   - N_marioForceVivianAnime in party_vivian.s
                    //     pins the *paper* pose (mp+0x240) — the
                    //     animId index returned by marioAnimeId
                    //     resolves to mp[0x240] for Vivian.
                    // Pin both so any motion that uses the
                    // paperLocalTime field works without the publisher
                    // having to tell us which pose to target. The
                    // body-pose pin is a near-free no-op when the
                    // active body pose is static (M_S_1 during the
                    // Vivian rise sub-phase).
                    if (activePoseId >= 0)
                    {
                        ttyd::animdrv::animPoseSetLocalTime(activePoseId, peer.paperLocalTime);
                    }
                    if (slot.paperPoseId >= 0)
                    {
                        ttyd::animdrv::animPoseSetLocalTime(slot.paperPoseId, peer.paperLocalTime);
                    }
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
                    case 0x1C:
                        // kVivian (Veil — hide in shadow). Paper rig
                        // is "p_bibi" (str_p_bibi_802f898c in
                        // party_vivian.s). Without this case the
                        // publisher wrote an empty paper_agb during
                        // Veil, so receivers never loaded p_bibi and
                        // the ghost rendered the M_B_3 body pose
                        // without the shadow paper effect.
                        shortName = reinterpret_cast<const char *>(0x802F898C);
                        break;
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
                    // Take Mario out of paper / tube mode before we
                    // force-apply the stagger pose. Mirrors the start
                    // of N_marioForceVivianAnime in party_vivian.s
                    // (marioPaperOff + marioChgPaper(0)). Without
                    // this, a hit while in tube mode (motion_id 0x14
                    // / kJabara) visually triggers M_N_7 but leaves
                    // Mario engine-wise still papered up, and the
                    // engine re-applies the tube paper anim next
                    // frame, snapping back into the rolled state.
                    // Idempotent if Mario isn't already papered.
                    ttyd::mario::marioPaperOff();
                    ttyd::mario::marioChgPaper(nullptr);

                    // Mirrors the evt_mario_set_pose "name not in
                    // a_mario_group" path (evt_mario.s 4686-4693).
                    // M_N_7 lives in e_mario (effects pose), not
                    // a_mario, so the engine's pose pipeline only
                    // picks it up if we set the effects-route bit:
                    //
                    //   Player.0x18 = "M_N_7"        - anim pointer
                    //   Player.0x0C |= 0x1000        - flags3 pose-pending
                    //   Player.0x04 |= 0x10000000    - flags2 effects route
                    //
                    // Without flags2 |= 0x10000000, marioPreDisp
                    // tries to apply M_N_7 to a_mario and silently
                    // no-ops — that was the prior bug that left the
                    // victim's own renderer with no stagger while
                    // peers still saw it via the published anim.
                    uint8_t *mpRw = reinterpret_cast<uint8_t *>(me);
                    *reinterpret_cast<const char **>(mpRw + 0x18) =
                        g_ghostState->hitPoseName;
                    *reinterpret_cast<uint32_t *>(mpRw + 0x0C) |= 0x1000u;
                    *reinterpret_cast<uint32_t *>(mpRw + 0x04) |= 0x10000000u;

                    // Damage grunt — same SFX mot_damage.s fires
                    // (line 122). The hook captures it into the
                    // SFX ring so every other peer hears it on us.
                    ttyd::pmario_sound::psndSFXOn(0x0BA);

                    // Input lock for the stagger duration. Released
                    // by the marioChkKey-verified path below. We
                    // skip the cinematic-letterbox suppression that
                    // the original evt_mario_set_pose did — leaving
                    // it on broke shadow rendering elsewhere.
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
            if (g_hitLockRemaining == 0 && g_hitLockApplied)
            {
                if (ttyd::mario::marioChkKey() == 0)
                {
                    ttyd::mario::marioKeyOn();
                }
                g_hitLockApplied = false;
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
                    fixupX = 1.0f;
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

        const uint8_t selfRole = (g_ghostState != nullptr) ? g_ghostState->selfGameRole : kGameRoleNone;

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

            const uint32_t packed = (static_cast<uint32_t>(peer.r) << 24) | (static_cast<uint32_t>(peer.g) << 16) |
                                    (static_cast<uint32_t>(peer.b) << 8) | 0xFFu;
            ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(const_cast<uint32_t *>(&packed)));

            ttyd::fontmgr::FontDrawString(screenX, screenY, peer.slotName);
        }

        // Self-label: render "Seeker" above the local Mario when our
        // role is seeker. Cosmetic confirmation of role; mirrors the
        // red color we use for seeker peers.
        //
        // Position: use the three-vector sum (playerPosition +
        // wModelPosition + wAnimPosition), matching what Python's
        // _read_self_state publishes at offsets 0x8C/0x98/0xA4 so
        // peers see us at the same world location. wAnimPosition
        // alone lagged for a frame or two after seqSetSeq teleports,
        // which left the label anchored at the post-spawn point
        // while Mario walked away from it.
        if (selfRole == kGameRoleSeeker)
        {
            ttyd::mario::Player *me = ttyd::mario::marioGetPtr();
            if (me != nullptr)
            {
                gc::vec3 worldPos = {
                    me->playerPosition.x + me->wModelPosition.x + me->wAnimPosition.x,
                    me->playerPosition.y + me->wModelPosition.y + me->wAnimPosition.y + kNameTagWorldYOffset,
                    me->playerPosition.z + me->wModelPosition.z + me->wAnimPosition.z,
                };
                gc::vec3 camPos = {0.0f, 0.0f, 0.0f};
                gc::mtx::PSMTXMultVec(viewMtx, &worldPos, &camPos);

                gc::vec3 ndcPos = {0.0f, 0.0f, 0.0f};
                gc::mtx::PSMTX44MultVec(projMtx, &camPos, &ndcPos);

                if (ndcPos.z >= -1.5f && ndcPos.z <= 1.5f && ndcPos.x >= -1.5f && ndcPos.x <= 1.5f && ndcPos.y >= -1.5f &&
                    ndcPos.y <= 1.5f)
                {
                    const char *label = "Seeker";
                    float screenX = ndcPos.x * kNameTagScreenScaleX;
                    const float screenY = ndcPos.y * kNameTagScreenScaleY;
                    const uint16_t textWidth = ttyd::fontmgr::FontGetMessageWidth(label);
                    screenX -= (static_cast<float>(textWidth) * kNameTagFontScale) * 0.5f;
                    const uint32_t packedSelf = 0xFF4040FFu;
                    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(const_cast<uint32_t *>(&packedSelf)));
                    ttyd::fontmgr::FontDrawString(screenX, screenY, label);
                }
            }
        }
    }

    namespace
    {

        constexpr float kLobbyHudAnchorX = 270.0f;
        constexpr float kLobbyHudAnchorY = 220.0f;
        constexpr float kLobbyHudFontScale = 0.5f;
        constexpr float kLobbyHudLineHeight = 22.0f;

        const char *LobbyStatusLabel(uint8_t status)
        {
            switch (status)
            {
                case kLobbyStatusIdle:
                    return "Idle";
                case kLobbyStatusWaiting:
                    return "Hide";
                case kLobbyStatusCountdown:
                    return "Seek";
                case kLobbyStatusPlaying:
                    return "Round Over";
                case kLobbyStatusFinished:
                    return "Match End";
                default:
                    return "?";
            }
        }

        const char *LobbyGameTypeLabel(uint8_t gameType)
        {
            switch (gameType)
            {
                case kGameTypeHideAndSeek:
                    return "Hide and Seek";
                default:
                    return "";
            }
        }

        float RightAlignX(const char *str, float screenX, float fontScale)
        {
            const uint16_t textWidth = ttyd::fontmgr::FontGetMessageWidth(str);
            return screenX - static_cast<float>(textWidth) * fontScale;
        }
    } // namespace

    KEEP_FUNC void DrawLobbyHud(ttyd::dispdrv::CameraId, void *)
    {
        if (!g_initialized)
            return;

        const LobbyHudHeader *header = GetLobbyHudHeader();

        if (header->magic != kLobbyHudMagic)
            return;
        if (header->version != kLobbyHudVersion)
            return;

        if (header->active == 0)
            return;

        ttyd::fontmgr::FontDrawStart();
        ttyd::fontmgr::FontDrawEdge();
        ttyd::fontmgr::FontDrawScale(kLobbyHudFontScale);

        const uint32_t packedWhite = 0xFFFFFFFFu;
        ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(const_cast<uint32_t *>(&packedWhite)));

        float y = kLobbyHudAnchorY;

        char buf[64];
        char nameBuf[17];
        std::memcpy(nameBuf, header->name, 16);
        nameBuf[16] = '\0';

        ttyd::string::strcpy(buf, "Lobby: ");
        ttyd::string::strcat(buf, nameBuf);

        ttyd::fontmgr::FontDrawString(RightAlignX(buf, kLobbyHudAnchorX, kLobbyHudFontScale), y, buf);
        y -= kLobbyHudLineHeight;

        const char *gameLabel = LobbyGameTypeLabel(header->gameType);
        if (gameLabel[0] != '\0')
        {
            ttyd::string::strcpy(buf, "Game: ");
            ttyd::string::strcat(buf, gameLabel);
            ttyd::fontmgr::FontDrawString(RightAlignX(buf, kLobbyHudAnchorX, kLobbyHudFontScale), y, buf);
            y -= kLobbyHudLineHeight;
        }

        ttyd::string::strcpy(buf, "Status: ");
        ttyd::string::strcat(buf, LobbyStatusLabel(header->status));
        ttyd::fontmgr::FontDrawString(RightAlignX(buf, kLobbyHudAnchorX, kLobbyHudFontScale), y, buf);
        y -= kLobbyHudLineHeight;

        if (header->timerSeconds > 0)
        {
            char numBuf[8] = {0};
            uint16_t t = header->timerSeconds;
            int idx = 0;
            char rev[8];
            int rlen = 0;
            if (t == 0)
            {
                rev[rlen++] = '0';
            }
            else
            {
                while (t > 0 && rlen < 6)
                {
                    rev[rlen++] = static_cast<char>('0' + (t % 10));
                    t /= 10;
                }
            }

            for (int i = rlen - 1; i >= 0; --i) numBuf[idx++] = rev[i];
            numBuf[idx++] = 's';
            numBuf[idx] = '\0';

            ttyd::string::strcpy(buf, "Time: ");
            ttyd::string::strcat(buf, numBuf);
            ttyd::fontmgr::FontDrawString(RightAlignX(buf, kLobbyHudAnchorX, kLobbyHudFontScale), y, buf);
            y -= kLobbyHudLineHeight;
        }

        const char *text = GetLobbyHudText();
        const char *end = text + kLobbyTextLen;
        const char *cur = text;

        char lineBuf[80];

        while (cur < end && *cur != '\0')
        {
            const char *lineStart = cur;
            while (cur < end && *cur != '\0' && *cur != '\n') ++cur;

            const int lineLen = static_cast<int>(cur - lineStart);

            // Per-line color markers written by Python's
            // format_match_text. \x01 = red (seeker), \x02 = green
            // (hider). Strip the marker before rendering.
            const char *renderStart = lineStart;
            int renderLen = lineLen;
            uint32_t lineColor = packedWhite;
            if (renderLen > 0)
            {
                if (*renderStart == '\x01')
                {
                    lineColor = 0xFF4040FFu;
                    ++renderStart;
                    --renderLen;
                }
                else if (*renderStart == '\x02')
                {
                    lineColor = 0x40FF40FFu;
                    ++renderStart;
                    --renderLen;
                }
            }

            const int copyLen =
                (renderLen < static_cast<int>(sizeof(lineBuf)) - 1) ? renderLen : static_cast<int>(sizeof(lineBuf)) - 1;
            std::memcpy(lineBuf, renderStart, copyLen);
            lineBuf[copyLen] = '\0';

            if (copyLen == 0)
            {
                y -= kLobbyHudLineHeight;
            }
            else
            {
                ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&lineColor));
                ttyd::fontmgr::FontDrawString(RightAlignX(lineBuf, kLobbyHudAnchorX, kLobbyHudFontScale), y, lineBuf);
                y -= kLobbyHudLineHeight;
            }

            if (cur < end && *cur == '\n')
                ++cur;
        }
    }

    // ====================================================================
    // SFX hook entry points (called from OWR.cpp psndSFX*Hook)
    // ====================================================================
    //
    // OnLocalSfxFired runs on every psndSFXOn[/3D] call. It records the
    // (channel, sfxId) mapping for state-sync sampling and pushes a
    // start event onto the SFX ring (which receivers consult for one-
    // shot replay). OnLocalSfxStopped runs on every psndSFXOff and
    // just frees the channel map entry; loop termination is handled
    // by state-sync diff on the receiver side.

    KEEP_FUNC void OnLocalSfxFired(int sfxId, bool is3D, int channel)
    {
        if (!g_initialized)
            return;
        if (g_inReceiverReplay)
            return;

        // Record the channel mapping so:
        //  (a) the publish-time SampleActiveLoops sees this sfxId
        //      until the engine stops it;
        //  (b) when the engine eventually calls psndSFXOff on this
        //      channel, OnLocalSfxStopped can free the entry so the
        //      next publish drops it from activeLoops.
        // For one-shots that didn't allocate (channel == -1), this
        // is a no-op (RecordLocalChannel filters them out). Channel 0
        // is a real channel index, NOT a sentinel.
        RecordLocalChannel(channel, static_cast<uint16_t>(sfxId & 0xFFFF));

        // Push a start event regardless. Receivers filter loops out of
        // SFX-ring replay (they handle them via state-sync diff), but
        // one-shots flow through normally. The ring-side filter on
        // receivers depends on knowing if the sfxId is in
        // peer.activeLoops, which they have at receive time.
        if (!SfxIsAllowed(sfxId))
            return;
        PushSfxRingEvent(static_cast<uint16_t>(sfxId & 0xFFFF), is3D ? kSfxFlag3D : 0);
    }

    // v26: stop hook just frees the channel map entry. The next publish
    // will omit that sfxId from activeLoops, and receivers will diff
    // and stop their tracked loop. No event ring traffic for stops.
    KEEP_FUNC void OnLocalSfxStopped(int channel)
    {
        if (!g_initialized)
            return;
        if (g_inReceiverReplay)
            return;

        RemoveLocalChannel(channel);
    }
} // namespace mod::ghosts
