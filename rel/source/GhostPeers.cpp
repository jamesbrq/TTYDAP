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

            // Per-frame lerp targets for the rotation/pivot fields the
            // source publishes at 20Hz. Without smoothing, these
            // teleport between publishes and produce visible chop on
            // motions that rotate rapidly (kHammer2 spin attack,
            // kRoll tube roll). LerpAngleDeg handles 360-degree wrap
            // for the rotations; pivot is plain Lerp.
            float renderRotX;
            float renderRotZ;
            float renderPivotX;
            float renderPivotY;
            float renderPivotZ;

            // Spin-direction tracking for the rotation fields. Each
            // pair stores (last seen published angle, smoothed angular
            // velocity in degrees per publish interval). When abs(vel)
            // exceeds kFastSpinThresholdDegPerPublish, the lerp uses
            // the velocity sign to disambiguate the direction across
            // the 180-degree wrap; without this, fast spins (~5
            // rev/sec) appear to reverse direction whenever a publish
            // happens to land in the >180-degree crossing region.
            // Updated only on publish arrival (peer angle change).
            // Each axis has its own initialized flag because they may
            // not all see their first publish change in the same frame.
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

            // Active looping SFX state, state-sync driven (v26).
            //
            // Each entry tracks one loop currently playing for this
            // peer's slot. Populated when peer.activeLoops contains an
            // sfxId we aren't tracking (state-sync diff start). Cleared
            // when an entry is no longer in peer.activeLoops (state-
            // sync diff stop), when the animName-change janitor
            // force-stops it, or on ReleaseSlot.
            //
            // animNameAtStart records peer.animName at the moment we
            // started the loop. Each frame, if peer.animName has
            // drifted away, the janitor force-stops. This is a
            // belt-and-suspenders backup: state-sync should be enough
            // for clean cases, but for engine-managed loops where
            // psndSFXOff isn't observable (or is observable but our
            // chain breaks), the animation-driven hard stop catches
            // them when the source moves on to a different animation.
            //
            // Capacity 8 covers up to 8 simultaneous loops per peer;
            // wire format publishes max kActiveLoopsPerPeer (currently
            // 6) so 8 has cushion. Linear search is cheap.
            struct ActiveLoop
            {
                uint16_t sfxId;
                int channel;
                char animNameAtStart[16];
                bool inUse;

                // Anim-stability latch (v26 backup mechanism). When
                // an anim-bound loop is started, the peer's animName
                // at that instant is often a transient transition
                // animation, not the steady-state anim that should
                // anchor the loop's lifetime. To avoid killing the
                // loop the moment the transition completes, we delay
                // janitor activation until peer.animName has been
                // stable for kAnimStabilityFrames consecutive frames.
                //
                // Per-frame in the janitor:
                //  - if !watching: compare peer.animName to
                //    animCandidate. If equal, ++stableFrames. If
                //    stableFrames hits the threshold, snapshot
                //    animCandidate into animNameAtStart and set
                //    watching=true. If different, reset stableFrames=1
                //    and overwrite animCandidate.
                //  - if watching: standard drift check against
                //    animNameAtStart.
                char animCandidate[16];
                uint8_t stableFrames;
                bool watching;
            };
            static constexpr int kActiveLoopsPerSlot = 8;
            ActiveLoop activeLoops[kActiveLoopsPerSlot];

            // Anim-bound loop blocklist (v26 backup): sfxIds in this
            // list are skipped by state-sync's start path until the
            // source drops them from peer.activeLoops. Populated by
            // RunAnimBoundJanitor when it force-stops a loop due to
            // animName drift; cleared automatically once the source
            // catches up. 8 entries is generous - max 4 anim-bound
            // IDs total in kAnimBoundLoopSfx, plus headroom.
            static constexpr int kBlockedSfxPerSlot = 8;
            uint16_t blockedSfx[kBlockedSfxPerSlot];
        };

        constexpr float kLerpAlpha = 0.30f;

        // Anim-bound loop janitor: peer.animName must be stable for
        // this many consecutive frames before we snapshot it as the
        // anchor and start watching for drift. Protects against
        // mid-transition snapshots (e.g. boat entry anim flipping
        // through a brief transition before settling on the steady-
        // state sail anim). 10 frames @ 60Hz = ~167ms - long enough
        // to outlast typical transitions, short enough not to leave
        // the loop unprotected for noticeable time.
        constexpr uint8_t kAnimStabilityFrames = 10;

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
        //
        // v26: state-sync now handles LOOP termination via the
        // peer.activeLoops field (diff-based). The SFX ring carries
        // ONE-SHOTS only on receivers - any sfxId that's currently
        // in peer.activeLoops gets filtered out of ring replay
        // (it'll be started by the state-sync diff if not already).
        // This means it's safe to include loop sfxIds in the whitelist:
        // their start event arrives via the ring (records source-side
        // intent) but their playback comes from state-sync.
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

        // ====================================================================
        // activeLoops table helpers (v26 state-sync driven loop tracking)
        // ====================================================================
        //
        // Per-slot table tracking which loops are currently playing for
        // a peer. Populated/cleared by SyncActiveLoopsFromState (the
        // diff against peer.activeLoops). No motion-id heuristics:
        // the source's published activeLoops is the ground truth.

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

        // Stop the channel associated with one entry and clear it.
        // Caller is responsible for any g_inReceiverReplay guarding -
        // typically wrapped around batches of stops.
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

        // Forward decls for use in SyncActiveLoopsFromState's start
        // path and the SFX ring replay. Definitions live alongside
        // the rest of the anim-bound janitor machinery further down.
        bool IsBlockedSfx(const GhostSlot &slot, uint16_t sfxId);
        bool IsAnimBoundLoop(uint16_t sfxId);

        // ====================================================================
        // State-sync: reconcile slot.activeLoops with peer.activeLoops
        // ====================================================================
        //
        // The source publishes its currently-active loop sfxIds each
        // tick. We diff:
        //  - sfxIds in peer.activeLoops not in slot.activeLoops -> start.
        //  - sfxIds in slot.activeLoops not in peer.activeLoops -> stop.
        //  - already in both -> no-op.
        //
        // Robust to dropped publishes: even if a publish is entirely
        // lost, the next one re-converges. No event/sequence tracking,
        // no risk of a stuck loop from a dropped stop event.
        void SyncActiveLoopsFromState(const PeerSlot &peer, GhostSlot &slot)
        {
            // The peer.activeLoops slot may contain trailing zeros after
            // peer.activeLoopCount valid entries. Treat zeros as empty
            // regardless of count to be defensive.
            const int published = peer.activeLoopCount > kActiveLoopsPerPeer ? kActiveLoopsPerPeer : peer.activeLoopCount;

            // Pass 1: stop any tracked loop that's not in published.
            // Wrap in receiver-replay guard since psndSFXOff goes
            // through our hook.
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
                // v26 backup: anim-bound loop blocklist. The janitor
                // adds an sfxId here when it force-stops on animName
                // drift; we skip the state-sync start to avoid the
                // re-start cycle. The block clears automatically once
                // the source stops publishing this sfxId in
                // peer.activeLoops (handled by RunAnimBoundJanitor's
                // pass 2).
                if (IsBlockedSfx(slot, sfxId))
                    continue;

                g_inReceiverReplay = true;
                const int channel = psndSFXOn_3D(sfxId, &peer.position);
                g_inReceiverReplay = false;

                if (channel == -1)
                {
                    // Engine returned no channel (allocation failure).
                    // Don't track; the source's next publish either
                    // drops it (one-shot finished) or it'll show up
                    // again and we'll retry. Channel 0 IS valid.
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
                // Seed the anim-stability latch. Don't snapshot
                // animNameAtStart yet - the current peer.animName
                // could be a transient transition. The janitor will
                // promote animCandidate to animNameAtStart once it's
                // been stable for kAnimStabilityFrames.
                std::memcpy(e->animCandidate, peer.animName, sizeof(peer.animName));
                e->animCandidate[sizeof(e->animCandidate) - 1] = '\0';
                e->stableFrames = 1;
                e->watching = false;
                e->animNameAtStart[0] = '\0';
            }
        }

        // ====================================================================
        // Animation-bound loop janitor (targeted hard-stop for known cases)
        // ====================================================================
        //
        // Some engine-managed loops have proven unreliable to terminate
        // via the normal state-sync path - we observe them get stuck
        // playing indefinitely on receivers despite source-side
        // psndSFXOff calls. The state-sync chain is supposed to handle
        // these (psndSFXOff hook -> RemoveLocalChannel -> drop from
        // selfActiveLoops -> diff stop on receiver) but in practice
        // these specific IDs slip through.
        //
        // Workaround: tag entries for these specific sfxIds with the
        // peer's animName at start time, and force-stop locally if
        // peer.animName drifts away. Once stopped, blocklist the sfxId
        // for this slot until the source drops it from peer.activeLoops
        // - otherwise state-sync would just re-start it next frame.
        //
        // Other loops continue to use pure state-sync without
        // animation tagging or blocklisting; they're not subject to
        // this re-start cycle because state-sync handles them cleanly.
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

        // Run the animation-change janitor: for each tracked entry
        // whose sfxId is anim-bound, if peer.animName has drifted from
        // animNameAtStart, force-stop. If the sfxId is currently in
        // peer.activeLoops (i.e., state-sync would re-start it next
        // frame), also blocklist to prevent the re-start cycle. If
        // the sfxId is NOT in peer.activeLoops (it came from the SFX
        // ring as a loop-pretending-to-be-one-shot), no blocklist
        // needed - sequence numbers prevent the same event from
        // re-firing, so a future legitimate start would arrive as a
        // NEW event and play normally.
        //
        // Pass 2 prunes the blocklist of any sfxId no longer in
        // peer.activeLoops so future legitimate plays go through.
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
                    // Latch phase: wait for peer.animName to be stable
                    // for kAnimStabilityFrames consecutive frames
                    // before committing to an anchor.
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

                // Blocklist only if state-sync would re-add it next
                // frame. SFX-ring-only entries can be stopped cleanly
                // without blocklist.
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

            // Stop any active loops (state-sync diff) and reset
            // tracking. Called whenever the slot transitions to
            // inactive (peer.active=0) or during map-change cleanup.
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

        // Velocity threshold above which we trust the smoothed velocity
        // sign over the shortest-path delta. 90 deg/publish at 20Hz =
        // 1800 deg/sec = 5 revolutions per second, which is roughly
        // where shortest-path-sign starts being unreliable. Below this,
        // a noisy publish (e.g. small back-and-forth motion) shouldn't
        // be treated as a sustained spin.
        constexpr float kFastSpinThresholdDegPerPublish = 90.0f;

        // Angular IIR for the velocity estimate. New samples weighted
        // 0.4, history 0.6 - tuned so two consecutive same-direction
        // samples cross the fast-spin threshold (avoids latching on a
        // single noisy delta) but not so heavy that the estimate lags
        // a real spin's first half-revolution.
        constexpr float kVelocityFilterAlpha = 0.4f;

        // SpinAwareLerpAngle: like LerpAngleDeg, but disambiguates
        // direction during sustained fast rotation. Two signals:
        //
        //   1. peerHint (-1, 0, +1): source-side hint published when
        //      observed angular speed exceeds the hint threshold. The
        //      authoritative signal: source tracked unwrapped angle
        //      at 60Hz so it can't be fooled by aliasing.
        //   2. smoothedVel: receiver-side IIR-filtered publish-to-publish
        //      delta. Catches sustained rotation even if the source
        //      didn't set a hint (e.g. peer running an older protocol
        //      version that always sends 0).
        //
        // Hint takes priority. Without a hint, falls back to velocity.
        // Without either, plain shortest-path (LerpAngleDeg-equivalent).
        //
        // current        : slot.render* (interpolated angle, wraps freely)
        // target         : peer.rotation* (raw published angle, [-180,180])
        // peerHint       : peer.spinDirHint* from the wire format
        // lastSeen       : peer.rotation* from the previous frame (per-slot)
        // smoothedVel    : exponentially-smoothed publish-to-publish delta
        // initialized    : false until first publish change is observed
        //
        // Returns the new lerped angle. Updates lastSeen, smoothedVel,
        // initialized in place.
        float SpinAwareLerpAngle(float current,
                                 float target,
                                 float alpha,
                                 int8_t peerHint,
                                 float &lastSeen,
                                 float &smoothedVel,
                                 bool &initialized)
        {
            // Velocity bookkeeping (unchanged from previous version).
            // We still maintain a smoothedVel even when peerHint is
            // present, because the hint can clear back to 0 mid-spin
            // if the source's instantaneous accumulator dipped under
            // threshold for one publish - smoothedVel provides a
            // graceful tail.
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

                        // v26: SFX ring is one-shots only. Loops are
                        // handled by SyncActiveLoopsFromState. Skip any
                        // event whose sfxId is:
                        //  (a) currently in peer.activeLoops (state-sync
                        //      start this frame or already in flight), or
                        //  (b) already tracked in slot.activeLoops (we
                        //      previously started it via state-sync and
                        //      this ring event is the redundant start
                        //      from the source's psndSFXOn hook).
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
                            // Anim-bound loops sometimes arrive ONLY via
                            // the SFX ring (the source-side state-sync
                            // chain misses them - we suspect channel
                            // tracking glitches for these specific IDs).
                            // For those, capture the channel and tag
                            // with animName so the janitor can hard-stop
                            // on animName drift. Blocklist also applies
                            // here - if the janitor previously stopped
                            // this sfxId, refuse to re-start until the
                            // source drops it.
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
                                            // Seed the anim-stability
                                            // latch. The current
                                            // peer.animName is often
                                            // a transient transition
                                            // for SFX-ring-only loops;
                                            // wait for stability before
                                            // committing to an anchor.
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

            // v26 state-sync: reconcile tracked loops with peer's
            // published active set. Independent of the SFX ring -
            // works even when the ring is empty.
            SyncActiveLoopsFromState(peer, slot);

            // Targeted backup: for known anim-bound loops (mot_roll
            // 0x17B and mot_ship 0x18F/0x190/0x192) we observed
            // state-sync alone fails to terminate them in some cases.
            // The janitor force-stops these specific IDs when the
            // peer's animName drifts from the one at start, and
            // blocklists them until the source drops them from
            // peer.activeLoops. Other loops continue with pure
            // state-sync (no animation tagging or blocklisting -
            // those would create re-start cycles).
            RunAnimBoundJanitor(peer, slot);
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

        // Insert/refresh the mapping channel -> sfxId. If channel is
        // already present, update it (engine reused the slot). Otherwise
        // find a free slot. With a 64-entry map and TTYD's <32 active
        // channels, the table never fills.
        //
        // The "no channel allocated" sentinel returned by the engine is
        // -1 (0xFFFFFFFF as unsigned), confirmed by reading the engine's
        // own pre-checks (e.g. mot_ship.s line 628-630: addis r0, r3,
        // 0x1; cmplwi r0, 0xFFFF tests whether channel == -1). Channel
        // 0 IS a valid channel index in this engine - the previous
        // version of this filter incorrectly rejected it, which caused
        // the boat and roll loops (which sometimes alloc channel 0)
        // to never appear in our state-sync sample, so receivers
        // never knew to stop them.
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
            // No free slot. Map is sized generously so this should
            // never trigger; if it does, log via a counter? For now
            // silently drop the new entry; the sound still plays.
        }

        // Remove the mapping for channel. Returns the prior sfxId or 0
        // if not found. v26: callers only use the side effect (free
        // the slot); the returned sfxId is no longer used to emit a
        // stop event since state-sync handles that.
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

        // Sample the channel map into a fixed-size out array. Returns
        // the count written (<= maxOut). Used at publish time to build
        // the activeLoops field of the local peer's wire format.
        //
        // No filtering happens here - we publish whatever the engine
        // currently has alive. One-shots that haven't been stopped yet
        // (because the engine just hasn't gotten around to it) appear
        // here too. That's fine: receivers re-trigger them via the
        // diff, which sounds identical to the SFX ring path. The only
        // odd case is a one-shot that's still in the map after its
        // sound naturally finished - the receiver "starts" it but
        // since it's a one-shot it just plays through quickly and
        // appears as a stutter on the next publish (when the entry
        // ages out by being overwritten via channel reuse). In practice
        // the 50ms publish window is short enough that this case is
        // imperceptible; real loops dominate the table.
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

        // Push a single SFX event onto the ring. Used by OnLocalSfxFired
        // for start events (one-shots and loops both flow through the
        // ring; receivers filter loops out of the SFX-ring replay path
        // since state-sync handles them). Returns true on success.
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

            // v26: Loop sync runs INSIDE ApplyPeerToSlot via
            // SyncActiveLoopsFromState. The motion-id-driven backup
            // mechanism (LoopSfxForMotion / UpdatePeerLoop /
            // RunMotionChangeJanitor) was removed - state-sync is
            // self-healing across dropped publishes and doesn't need
            // motion-id heuristics.

            if (slot.hitFramesRemaining > 0)
                --slot.hitFramesRemaining;

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

                // Seed spin tracking: lastSeen matches the published
                // angle so the first delta computed on the next change
                // is meaningful (not relative to zero). Per-axis init
                // flags stay false until the first publish change is
                // observed for THAT axis - one full publish interval
                // is needed before we have a velocity sample, and
                // different axes may receive their first change on
                // different frames.
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

                // Spin-aware angle lerp. Tracks publish-to-publish
                // angular velocity per axis; when smoothed velocity
                // exceeds kFastSpinThresholdDegPerPublish (~5 rev/sec),
                // forces lerp direction to follow velocity rather than
                // shortest-path. Without this, the fastest hammer spin
                // appeared to reverse direction whenever the publish
                // landed in the >180-degree-from-receiver crossing
                // region. Each axis gets its own velocity tracker;
                // they're independent because spin attack rotates yaw
                // while plane mode rotates pitch+roll, etc.
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

                // Pivot moves between (0,0,0) idle and motion-specific
                // offsets when the source enters/exits paper modes.
                // Linear lerp is fine here since pivot values are
                // straight world offsets, not angles.
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

            // v26: sample currently-active loop sfxIds from the
            // channel map and write to selfActiveLoops scratch.
            // Python's 20Hz publisher reads this each tick and embeds
            // it in our peer slot's activeLoops field. Receivers diff
            // and start/stop accordingly. Updated every frame so the
            // Python tick always sees fresh data.
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

            // Pitch-flip for kJabara: when source pitches into the
            // back-half of a circle, mirror Z so the body silhouette
            // stays right-side-up. Use lerped pitch so the flip
            // toggles cleanly on a smooth angle rather than chasing
            // 20Hz publish snaps.
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

            // Pivot/rotation values are taken from slot.render* rather
            // than peer.* so they smooth across 20Hz publish boundaries.
            // Without lerping, kHammer2 (spin attack) and kRoll (tube
            // roll) showed visible chop because their pitch/roll/yaw
            // changed faster than the publish rate.
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