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
#include "ttyd/string.h"
#include "visibility.h"

#include <cstring>

using namespace ttyd::pmario_sound;

namespace mod::ghosts
{

    // Heap-allocated container for ALL ghost-peer scratch state. Set
    // once in Init() and never re-assigned. Accessor inlines in
    // GhostPeers.h dereference through this pointer. Null-checked at
    // entry points (UpdateAll, DrawAll, etc.) via the existing
    // g_initialized flag, which is only set after this is non-null.
    GhostState *g_ghostState = nullptr;

    extern "C" void *camGetPtr(int cameraId);

    extern "C" float reviseAngle(float deg);

    // psndSFXOff stops a previously-started SFX channel. Takes the
    // channel handle returned by psndSFXOn[/3D]. Used by the
    // motion-driven loop control to stop ghost-side loops when the
    // peer's motionId transitions out of a looping state. Not declared
    // in pmario_sound.h yet; declared here for local use.
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

            bool renderInitialized;

            int hitFramesRemaining;

            uint8_t lastConsumedSfxSeq;
            bool sfxSeqInitialized;

            // Active looping SFX state. The mod auto-starts looping
            // sounds when peer.motionId implies one (e.g. plane mode,
            // hammer windup) and stops them on motion change. This is
            // independent of the SFX ring, which only carries one-shots.
            // activeLoopSfxId == 0 means no loop active for this slot.
            // activeLoopChannel is the psndSFXOn_3D return value used
            // to stop the loop later via psndSFXOff.
            uint16_t activeLoopSfxId;
            int activeLoopChannel;
        };

        constexpr float kLerpAlpha = 0.30f;

        GhostSlot g_slots[kMaxPeers];
        bool g_initialized = false;

        constexpr int kHitLockDurationFrames = 60;
        int g_hitLockRemaining = 0;

        constexpr int kHitGraceFrames = 90;
        int g_hitGraceRemaining = 0;

        constexpr int kHitQueueTimeoutFrames = 60 * 5;
        bool g_hitQueued = false;
        int g_hitQueuedTimeout = 0;

        // Reentrancy guard: when the receiver replay fires psndSFXOn[/3D]
        // to play a peer's SFX, that call goes through our hook again.
        // Without this guard, the hook would re-capture the replay into
        // the ring, Python would drain it, the ghost would replay the
        // replay, ad infinitum (1s delay echoes). When this is true,
        // OnLocalSfxFired returns early and skips ring capture - but
        // the trampoline (in OWR.cpp) still runs, so the sound plays.
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

        // Whitelist of SFX IDs that should be mirrored to peers. Every
        // ID here was confirmed by reading the engine assembly that
        // calls psndSFXOn[/3D] - see mario_motion.s, mot_*.s under
        // /mnt/user-data/uploads. Adding random IDs is risky: many SFX
        // have stateful side-effects (env reverb, looping channels)
        // that aren't safe to fire on a remote.
        //
        // Architecture note: this whitelist is purely a CAPTURE filter.
        // The receiver replays whatever IDs arrive on the wire without
        // checking the peer's animation - SFX and animations are
        // independent. So an "incorrect" ID in this list at most
        // causes a phantom sound on peer screens; it doesn't desync
        // anything.
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

            // -- Hip-drop (mot_hip.s) --
            //    NOTE: 0xDB is LOOPING (hip-drop spin/charge sustained).
            //    Excluded until loop-mirror subsystem is added.

            // -- Idle/stand voice (mot_stay.s) --
            //    NOTE: All five variants 0xDC/0xE1/0xE2/0xE4/0xEF are
            //    LOOPING (snore / sleep-talking, channels saved at
            //    mp offsets and stopped via psndSFXOff). Excluded here
            //    until loop-mirror subsystem is added. They'd otherwise
            //    snore forever on peer ghosts after a brief idle.

            // -- Walk/run footsteps, terrain variants (mot_walk.s) --
            //    The 0x140-0x14B block covers regular ground, water,
            //    sand, snow, special, etc. Per-frame footstep dispatcher.
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

            // -- Hip-drop (mot_hip.s) --
            // NOTE: 0xDB and 0x158 are LOOPING (saved channels at
            // mp offsets, stopped via psndSFXOff). 0x159 is one-shot.
            0x159,

            // -- Hammer (mot_hammer.s) --
            // NOTE: 0x15B/0x15C/0x15D are LOOPING windup sounds tied
            // to mot_hammer2 (kHammer2 = 0x13). 0x162 is also looping
            // (saved at mp+0x2D0, stopped explicitly). They need
            // explicit psndSFXOff to stop. Replaying them as one-shots
            // would make them play forever on peer ghosts. Excluded
            // here; mirror via dedicated loop-tracking subsystem when
            // added.
            0x15E, // hammer impact, power tier 1 (one-shot)
            0x15F, // hammer impact, power tier 2 (one-shot)
            0x160, // hammer impact, power tier 3 (one-shot)
            0x163, // hammer spin variant from mot_hammer2 (one-shot)

            // -- Misc Mario actions (legacy, unverified) --
            0x16A,

            // -- Idle voice (mot_stay.s) --
            // NOTE: 0xDC, 0xE1, 0xE2, 0xE4, 0xEF are LOOPING (sleeping
            // snore loops, etc.). 0x173 is one-shot.
            0x173,

            // -- Slide-under entry/exit (mot_slit.s) --
            0x177,
            0x178,

            // -- Roll / tube (mot_roll.s) --
            // NOTE: 0x17C is LOOPING (tube spin loop). Others are
            // one-shot transitions.
            0x179,
            0x17A,
            0x17B,

            // -- Plane transitions (mot_plane.s) --
            // NOTE: 0x17F is the LOOPING wing-flap sound. Excluded
            // here for the same reason as hammer windup loops above.
            // 0x17D and 0x180 are one-shot transitions (entry/exit).
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

            // -- Ground impact + screen shake combo --
            //    Paired with 0x0B9 from mario_motion.s for landings.
            //    Also fired from mot_damage.s on heavy hits.
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

        // Map peer.motionId to the looping SFX that should be playing
        // for this peer right now. Returns 0 if no loop applies.
        //
        // Motion IDs from ttyd::mario_motion::MarioMotion:
        //   0x10 kHip / 0x11 kHip2 -> 0x158 (hip-drop spin loop)
        //   0x13 kHammer2          -> 0x15B (hammer windup base; the
        //                            local game varies tier 1/2/3 by
        //                            pouchGetHammerLv but we don't
        //                            transmit hammer level)
        //   0x16 kRoll             -> 0x17C (tube roll loop)
        //   0x18 kPlane            -> 0x17F (plane wing flap loop)
        //
        // Motions without loops return 0; the receiver stops any
        // active loop on transition to a no-loop motion.
        //
        // NOT covered (yet):
        //   - mot_stay sleep snore loops (need sub-motion state, not
        //     just motionId - they only fire after Mario falls asleep
        //     during prolonged kStay)
        //   - mot_ship boat motion loop (motion 0x19; the loop SFX
        //     wasn't conclusively identified by the heuristic)
        //   - hammer windup tiers 2/3 (would need wire format change
        //     to transmit hammer level)
        uint16_t LoopSfxForMotion(uint16_t motionId)
        {
            switch (motionId)
            {
                case 0x10: // kHip
                case 0x11: // kHip2
                    return 0x158;
                case 0x13: // kHammer2 (spin attack)
                    return 0x15B;
                case 0x16: // kRoll (tube)
                    return 0x17C;
                case 0x18: // kPlane
                    return 0x17F;
                default:
                    return 0;
            }
        }

        // Apply a loop-state transition for one peer slot. Compares
        // the desired loop (from motionId) against what's currently
        // active; stops/starts as needed. Skips entirely if peer is
        // off-map (no audible position).
        void UpdatePeerLoop(const PeerSlot &peer, GhostSlot &slot)
        {
            const uint16_t desired = PeerOnLocalMap(peer) ? LoopSfxForMotion(peer.motionId) : 0;
            const uint16_t current = slot.activeLoopSfxId;

            if (desired == current)
                return; // no change needed

            // Stop the current loop if any
            if (current != 0)
            {
                g_inReceiverReplay = true;
                psndSFXOff(slot.activeLoopChannel);
                g_inReceiverReplay = false;
                slot.activeLoopSfxId = 0;
                slot.activeLoopChannel = 0;
            }

            // Start the new loop if any
            if (desired != 0)
            {
                g_inReceiverReplay = true;
                const int channel = psndSFXOn_3D(desired, &peer.position);
                g_inReceiverReplay = false;
                slot.activeLoopSfxId = desired;
                slot.activeLoopChannel = channel;
            }
        }

        // Force-stop any active loop for a slot. Called when the slot
        // transitions to inactive (active=0), or when the peer
        // disappears via map change.
        void StopPeerLoop(GhostSlot &slot)
        {
            if (slot.activeLoopSfxId != 0)
            {
                g_inReceiverReplay = true;
                psndSFXOff(slot.activeLoopChannel);
                g_inReceiverReplay = false;
                slot.activeLoopSfxId = 0;
                slot.activeLoopChannel = 0;
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

            // Stop any motion-driven loop that was playing for this
            // slot. Called whenever the slot transitions to inactive
            // (peer.active=0) or during map-change cleanup.
            StopPeerLoop(slot);
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

            if (slot.hitFramesRemaining > 0 && slot.effectsAllocated)
            {
                const int forceReset = (slot.hitFramesRemaining == kHitLockDurationFrames) ? 1 : 0;
                ttyd::animdrv::animPoseSetAnim(slot.effectsPoseId, kDefaultHitPoseName, forceReset);

                slot.activePose = 2;

                slot.lastAnimEffects[0] = '\0';
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

                        psndSFXOn_3D(ev.sfxId, &peer.position);

                        const uint8_t newDiff = static_cast<uint8_t>(ev.seq - newest);
                        if (newDiff != 0 && newDiff < 128)
                            newest = ev.seq;
                    }
                    g_inReceiverReplay = false;
                    slot.lastConsumedSfxSeq = newest;
                }
            }
        }

        bool g_hammerSwingFired = false;

        int CheckPeerHammerHits(const SharedBlock *block)
        {
            using ttyd::mario_motion::MarioMotion;

            ttyd::mario::Player *me = ttyd::mario::marioGetPtr();
            if (me == nullptr)
                return -1;

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

        // Allocate the heap-resident GhostState. This replaces the
        // previous arrangement where scratch lived at fixed low-RAM
        // addresses (0x80001800, 0x80003B20-0x80003BE4, 0x80003D00).
        // Those addresses overlapped game/OS regions on some users'
        // Dolphin sessions and caused deterministic crashes on AP
        // connect. By allocating here instead, the OS gives us a
        // region that is guaranteed not to alias anything else, and
        // we publish the pointer to Python via APSettings.
        //
        // Use __memAlloc directly rather than `new GhostState()` to
        // avoid pulling in operator new (_Znwj). The cxx.h inline
        // exists but the elf2rel linker reports it as missing in
        // custom.rel builds. GhostState is trivially constructible
        // (POD members + arrays + packed wire structs), so a raw
        // alloc + memset is equivalent to value-initialization here.
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

        // Zero the entire allocation. __memAlloc returns uninitialized
        // memory; memsetting here puts every field in a known state
        // (numbers/pointers as 0, char arrays as empty strings, etc.)
        // before we overwrite specific fields below.
        std::memset(g_ghostState, 0, sizeof(GhostState));

        // Initialize peer block header. SharedBlock has its own magic
        // and version that Python validates - these must match the
        // Python-side constants exactly.
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

        // Publish the pointer into APSettings. Python reads this on
        // startup to discover where to write/read all subsequent
        // ghost-peer data. The APSettings struct lives at a stable
        // address (0x80003220) that Python already knows about, and
        // we just added a `ghostStatePtr` field at the end of it.
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
            s.renderInitialized = false;
            s.hitFramesRemaining = 0;

            s.lastConsumedSfxSeq = 0;
            s.sfxSeqInitialized = false;
            s.activeLoopSfxId = 0;
            s.activeLoopChannel = 0;
        }

        g_initialized = true;
    }

    KEEP_FUNC void Shutdown()
    {
        for (auto &s : g_slots) ReleaseSlot(s);
        g_initialized = false;
    }

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

            // Motion-driven loop control: examine peer.motionId, and
            // start/stop looping SFX accordingly. Independent of the
            // SFX ring (one-shots) - loops are inferred from motion
            // state because the wire format only carries event
            // start-points, not stops.
            UpdatePeerLoop(peer, slot);

            if (slot.hitFramesRemaining > 0)
                --slot.hitFramesRemaining;

            const bool snapRotY = (peer.motionId == 0x13);

            if (!slot.renderInitialized)
            {
                slot.renderX = peer.position.x;
                slot.renderY = peer.position.y;
                slot.renderZ = peer.position.z;
                slot.renderRotY = peer.rotationY;
                slot.renderInitialized = true;
            }
            else
            {
                slot.renderX = Lerp(slot.renderX, peer.position.x, kLerpAlpha);
                slot.renderY = Lerp(slot.renderY, peer.position.y, kLerpAlpha);
                slot.renderZ = Lerp(slot.renderZ, peer.position.z, kLerpAlpha);
                slot.renderRotY = snapRotY ? peer.rotationY : LerpAngleDeg(slot.renderRotY, peer.rotationY, kLerpAlpha);
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
        }

        {
            volatile uint32_t *pending = GetPendingHitPtr();
            const uint32_t raw = *pending;
            if (raw != 0)
            {
                const uint8_t kind = static_cast<uint8_t>(raw >> 24);
                if (kind == kHitKindHammer)
                {
                    if (g_hitGraceRemaining == 0)
                    {
                        g_hitQueued = true;
                        g_hitQueuedTimeout = kHitQueueTimeoutFrames;
                    }
                }

                *pending = 0;
            }
        }

        if (g_hitQueued)
        {
            ttyd::mario::Player *mp = ttyd::mario::marioGetPtr();
            bool fired = false;
            if (mp != nullptr)
            {
                using ttyd::mario_motion::MarioMotion;
                const uint8_t *mpBytes = reinterpret_cast<const uint8_t *>(mp);
                const uint16_t curMotRaw = *reinterpret_cast<const uint16_t *>(mpBytes + 0x2E);
                const uint32_t flags1 = *reinterpret_cast<const uint32_t *>(mpBytes + 0x0);
                const auto curMot = static_cast<MarioMotion>(curMotRaw);

                constexpr uint32_t kCtrlLockedMask = 0x10000000;
                const bool ctrlLocked = (flags1 & kCtrlLockedMask) != 0;
                const bool ready = (curMot == MarioMotion::kStay) && !ctrlLocked;

                if (ready)
                {
                    uint8_t *mpRw = reinterpret_cast<uint8_t *>(mp);
                    *reinterpret_cast<const char **>(mpRw + 0x18) = kDefaultHitPoseName;
                    *reinterpret_cast<uint32_t *>(mpRw + 0x0C) |= 0x1000;
                    *reinterpret_cast<uint32_t *>(mpRw + 0x04) |= 0x10000000;

                    uint8_t *counter = mpRw + 0x39;
                    *counter = static_cast<uint8_t>(*counter + 1);

                    void *letterboxCam = camGetPtr(8);
                    if (letterboxCam != nullptr)
                    {
                        uint16_t *camFlags = reinterpret_cast<uint16_t *>(letterboxCam);
                        *camFlags = static_cast<uint16_t>(*camFlags | 0x0200);
                    }

                    g_hitLockRemaining = kHitLockDurationFrames;

                    psndSFXOn(0xBA);

                    g_hitGraceRemaining = kHitGraceFrames;

                    fired = true;
                }
            }

            if (fired)
            {
                g_hitQueued = false;
                g_hitQueuedTimeout = 0;
            }
            else
            {
                --g_hitQueuedTimeout;
                if (g_hitQueuedTimeout <= 0)
                {
                    g_hitQueued = false;
                    g_hitQueuedTimeout = 0;
                }
            }
        }

        if (g_hitLockRemaining > 0)
        {
            --g_hitLockRemaining;
            if (g_hitLockRemaining == 0)
            {
                ttyd::mario::Player *mp = ttyd::mario::marioGetPtr();
                if (mp != nullptr)
                {
                    uint8_t *counter = reinterpret_cast<uint8_t *>(mp) + 0x39;
                    if (*counter > 0)
                        *counter = static_cast<uint8_t>(*counter - 1);
                }

                void *letterboxCam = camGetPtr(8);
                if (letterboxCam != nullptr)
                {
                    uint16_t *camFlags = reinterpret_cast<uint16_t *>(letterboxCam);
                    *camFlags = static_cast<uint16_t>(*camFlags & ~0x0200);
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
                g_slots[hitSlot].hitFramesRemaining = kHitLockDurationFrames;

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
                float pitchAng = peer.rotationX;
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

            const bool pivotActive = peer.rotPivotX != 0.0f || peer.rotPivotY != 0.0f || peer.rotPivotZ != 0.0f;
            if (pivotActive)
            {
                gc::mtx::PSMTXTrans(reinterpret_cast<gc::mtx34 *>(&matStep), -peer.rotPivotX, -peer.rotPivotY, -peer.rotPivotZ);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            if (peer.rotationZ != 0.0f)
            {
                gc::mtx::PSMTXRotRad(&matStep, 0x7A, peer.rotationZ * kDeg2Rad);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            if (peer.rotationX != 0.0f)
            {
                gc::mtx::PSMTXRotRad(&matStep, 0x78, peer.rotationX * kDeg2Rad);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            float yawDeg = slot.renderRotY - peer.cameraAngle;
            while (yawDeg < 0.0f) yawDeg += 360.0f;
            while (yawDeg >= 360.0f) yawDeg -= 360.0f;
            gc::mtx::PSMTXRotRad(&matStep, 0x79, yawDeg * kDeg2Rad);
            gc::mtx::PSMTXConcat(&matStep, &matA, &matA);

            if (pivotActive)
            {
                gc::mtx::PSMTXTrans(reinterpret_cast<gc::mtx34 *>(&matStep), peer.rotPivotX, peer.rotPivotY, peer.rotPivotZ);
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
                    return "Waiting";
                case kLobbyStatusCountdown:
                    return "Starting";
                case kLobbyStatusPlaying:
                    return "Playing";
                case kLobbyStatusFinished:
                    return "Finished";
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
            const int copyLen =
                (lineLen < static_cast<int>(sizeof(lineBuf)) - 1) ? lineLen : static_cast<int>(sizeof(lineBuf)) - 1;
            std::memcpy(lineBuf, lineStart, copyLen);
            lineBuf[copyLen] = '\0';

            if (copyLen == 0)
            {
                y -= kLobbyHudLineHeight;
            }
            else
            {
                ttyd::fontmgr::FontDrawString(RightAlignX(lineBuf, kLobbyHudAnchorX, kLobbyHudFontScale), y, lineBuf);
                y -= kLobbyHudLineHeight;
            }

            if (cur < end && *cur == '\n')
                ++cur;
        }
    }

    KEEP_FUNC void OnLocalSfxFired(int sfxId, bool is3D)
    {
        if (!g_initialized)
            return;
        // Reentrancy guard - see g_inReceiverReplay declaration. The
        // receiver replay fires psndSFXOn[/3D] to play peer SFX; that
        // re-enters our hook. Skip capture so we don't loop.
        if (g_inReceiverReplay)
            return;
        if (!SfxIsAllowed(sfxId))
            return;

        volatile uint8_t *headPtr = GetSfxRingHeadPtr();
        volatile uint8_t *tailPtr = GetSfxRingTailPtr();
        volatile uint8_t *seqPtr = GetSfxRingSeqPtr();
        volatile SfxEvent *ring = GetSfxRingEvents();

        const uint8_t head = *headPtr;
        const uint8_t tail = *tailPtr;
        const uint8_t nextHead = static_cast<uint8_t>((head + 1) % kSfxRingCapacity);

        if (nextHead == tail)
            return;

        const uint8_t newSeq = static_cast<uint8_t>(*seqPtr + 1);
        *seqPtr = newSeq;

        ring[head].sfxId = static_cast<uint16_t>(sfxId & 0xFFFF);
        ring[head].seq = newSeq;
        ring[head].flags = is3D ? kSfxFlag3D : 0;
        *headPtr = nextHead;
    }
} // namespace mod::ghosts