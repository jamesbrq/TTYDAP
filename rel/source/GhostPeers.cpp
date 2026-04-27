#include "GhostPeers.h"

#include "ttyd/animdrv.h"
#include "ttyd/dispdrv.h"
#include "ttyd/fontmgr.h"
#include "ttyd/mario.h"
#include "ttyd/mariost.h"
#include "gc/mtx.h"
#include "visibility.h"

#include <cstring>

namespace mod::ghosts
{
    // External camera lookup function. Not in any header - declared here so
    // we can grab the k3d camera's projection matrix for our 3D->2D projection.
    // marioPreDisp uses this with ID 4 (k3d) to compute Mario's screen-space
    // depth for shadow scaling.
    extern "C" void *camGetPtr(int cameraId);

    // -------------------------------------------------------------------------
    // Internal state - lives entirely inside the mod (custom.rel).
    // The shared block at kBlockAddress is read-only from our perspective; we
    // use these slots to track game-side allocations (animPose IDs).
    // -------------------------------------------------------------------------
    namespace
    {
        // The game's marioPoseInit calls animPoseEntry three times per
        // colorId, with three different agb names - all in group 2:
        //   marioAnimeGroupData[colorId].agbName_a  (forward body, "a_mario")
        //   marioAnimeGroupData[colorId].agbName_b  (rear body, "a_mario_r")
        //   marioAnimeGroupData[colorId].agbName_c  (effects, "e_mario")
        //
        // We allocate all three. Routing happens via flags2 in marioPreDisp:
        //   bit 0x10000000 -> effects pose (yawn, sleep, paper-mode special)
        //   bit 0x80000000 -> rear pose (rear-facing variants like M_S_1R)
        //   else            -> forward pose (normal locomotion + idle)
        // Picking the wrong pose makes animPoseSetAnim silently no-op
        // because the named anim doesn't exist in that pose's AGB.
        constexpr const char *kAgbForward = "a_mario";
        constexpr const char *kAgbRear = "a_mario_r";
        constexpr const char *kAgbEffects = "e_mario";
        constexpr int32_t kPoseGroup = 2;

        // 2.0 is what visually matches Mario's apparent size. Determined
        // empirically via the live-tunable scale we used during testing -
        // animPoseDraw's scale parameter scales relative to the model's
        // intrinsic size, and Mario's model has roughly a 0.5x base built in.
        constexpr float kGhostScale = 2.0f;

        // Name-tag positioning constants. Tuned empirically against the
        // local game's font/projection. NDC->screen scale matches TTYD's
        // 2D drawing system; world-Y offset places the tag above Mario's
        // head; font scale matches the chat-bubble text in vanilla.
        constexpr float kNameTagScreenScaleX = 280.0f;
        constexpr float kNameTagScreenScaleY = 240.0f;
        constexpr float kNameTagWorldYOffset = 50.0f;
        constexpr float kNameTagFontScale = 0.5f;

        // ===== DIAGNOSTICS: 32-peer crash investigation =====
        // Lives at 0x80003C00, past the 32-peer block end (0x80002E10) and
        // well separate from the self-paper-AGB scratch (0x80003B20).
        //
        // Layout:
        //   0x80003C00  u32  update_heartbeat       ++each frame UpdateAll completes
        //   0x80003C04  u32  draw_heartbeat         ++each frame DrawAll completes
        //   0x80003C08  u32  active_peers_seen      slots with peer.active last UpdateAll
        //   0x80003C0C  u32  fwd_alloc_attempts     animPoseEntry calls (forward)
        //   0x80003C10  u32  fwd_alloc_failures     return < 0
        //   0x80003C14  u32  rear_alloc_attempts
        //   0x80003C18  u32  rear_alloc_failures
        //   0x80003C1C  u32  effects_alloc_attempts
        //   0x80003C20  u32  effects_alloc_failures
        //   0x80003C24  u32  paper_alloc_attempts
        //   0x80003C28  u32  paper_alloc_failures
        //   0x80003C2C  u32  draw_calls_total       ghosts drawn last frame
        //   0x80003C30  u32  poses_allocated_total  sum of *_allocated across our slots
        //   0x80003C34  u32  last_failed_slot       slot index of most recent failure (0xFF=none)
        //   0x80003C38  u32  last_failure_phase     1=fwd, 2=rear, 3=effects, 4=paper
        //   0x80003C3C  u32  agb_defns_in_use       wp[0x00] array (cap 64). animPoseEntry
        //                                           crashes (null write) when this fills up.
        //   0x80003C40  u32  TUNABLE max_rendered   LIVE TUNABLE: poke values via Dolphin
        //                                           memory editor to probe pool capacity.
        //                                           Seeded with 8 at Init. Clamped [0, 32].
        //   0x80003C44  u32  global_pool_in_use     wp[0x10] pose array (cap 256). Includes
        //                                           Mario, party, NPCs + ours.
        //   0x80003C48  u32  draw_current_slot      slot index entering the per-slot work.
        //                                           Reset to 0xFFFFFFFF at start of DrawAll
        //                                           so stale value isn't misread as "slot 0".
        //   0x80003C4C  u32  draw_current_phase     what we're doing for that slot:
        //                                             0 = idle (between slots)
        //                                             1 = picking pose / matrix building
        //                                             2 = inside animPoseDrawMtx stage 2 call
        //                                             3 = inside animPoseDrawMtx stage 1 call
        //                                             4 = finished, about to ++drawn
        //   0x80003C50  u32  agb_instances_in_use   wp[0x08] instance array (cap 64).
        //                                           animPoseEntry ALSO crashes (null write)
        //                                           when this fills up. Tracks per-pose
        //                                           instance allocations, not refcounted.
        //
        // Read with Dolphin's memory editor. If the game crashes:
        //   - draw_current_slot tells you the slot it died ON
        //   - draw_current_phase tells you WHERE in the per-slot work
        //   - global_pool_in_use approaches 256 -> truly pool-exhausted
        // Remove this block once the 32-peer crash is fixed.
        volatile uint32_t *kDiagUpdateHB = reinterpret_cast<volatile uint32_t *>(0x80003C00);
        volatile uint32_t *kDiagDrawHB = reinterpret_cast<volatile uint32_t *>(0x80003C04);
        volatile uint32_t *kDiagActivePeers = reinterpret_cast<volatile uint32_t *>(0x80003C08);
        volatile uint32_t *kDiagFwdAttempt = reinterpret_cast<volatile uint32_t *>(0x80003C0C);
        volatile uint32_t *kDiagFwdFail = reinterpret_cast<volatile uint32_t *>(0x80003C10);
        volatile uint32_t *kDiagRearAttempt = reinterpret_cast<volatile uint32_t *>(0x80003C14);
        volatile uint32_t *kDiagRearFail = reinterpret_cast<volatile uint32_t *>(0x80003C18);
        volatile uint32_t *kDiagEffAttempt = reinterpret_cast<volatile uint32_t *>(0x80003C1C);
        volatile uint32_t *kDiagEffFail = reinterpret_cast<volatile uint32_t *>(0x80003C20);
        volatile uint32_t *kDiagPaperAttempt = reinterpret_cast<volatile uint32_t *>(0x80003C24);
        volatile uint32_t *kDiagPaperFail = reinterpret_cast<volatile uint32_t *>(0x80003C28);
        volatile uint32_t *kDiagDrawCalls = reinterpret_cast<volatile uint32_t *>(0x80003C2C);
        volatile uint32_t *kDiagPosesAlloc = reinterpret_cast<volatile uint32_t *>(0x80003C30);
        volatile uint32_t *kDiagLastFailSlot = reinterpret_cast<volatile uint32_t *>(0x80003C34);
        volatile uint32_t *kDiagLastFailPhase = reinterpret_cast<volatile uint32_t *>(0x80003C38);
        volatile uint32_t *kDiagAgbDefnsInUse = reinterpret_cast<volatile uint32_t *>(0x80003C3C);
        volatile uint32_t *kDiagPoolInUse = reinterpret_cast<volatile uint32_t *>(0x80003C44);
        volatile uint32_t *kDiagDrawSlot = reinterpret_cast<volatile uint32_t *>(0x80003C48);
        volatile uint32_t *kDiagDrawPhase = reinterpret_cast<volatile uint32_t *>(0x80003C4C);
        volatile uint32_t *kDiagAgbInstInUse = reinterpret_cast<volatile uint32_t *>(0x80003C50);
        // 0x80003C58: increments each time we detect a map change in
        // UpdateAll and force-release all ghost slots. Lets us verify the
        // transition-detection logic is actually firing during room
        // transitions. Should bump exactly once per transition.
        volatile uint32_t *kDiagMapChanges = reinterpret_cast<volatile uint32_t *>(0x80003C58);
        // Per-slot phase tracking for UpdateAll. Incremented at named
        // checkpoints so a crash freezes both fields where it died.
        //
        //   0x80003C5C  update_current_slot   slot index entering UpdateAll iter
        //   0x80003C60  update_current_phase  what we're doing for that slot:
        //                                       1 = top of loop / read peer
        //                                       2 = ReleaseSlot (peer not active)
        //                                       3 = entering ApplyPeerToSlot
        //                                       4 = entering EnsurePosesAllocated
        //                                       5 = pre-fwd animPoseEntry
        //                                       6 = post-fwd animPoseEntry
        //                                       7 = pre-rear animPoseEntry
        //                                       8 = post-rear animPoseEntry
        //                                       9 = pre-effects animPoseEntry
        //                                      10 = post-effects animPoseEntry
        //                                      11 = pre-paper animPaperPoseEntry
        //                                      12 = post-paper animPaperPoseEntry
        //                                      13 = post-ApplyPeerToSlot, lerp
        //                                      14 = post-lerp, animPoseMain
        //                                      15 = end of iter
        //                                       0 = between iters
        volatile uint32_t *kDiagUpdateSlot = reinterpret_cast<volatile uint32_t *>(0x80003C5C);
        volatile uint32_t *kDiagUpdatePhase = reinterpret_cast<volatile uint32_t *>(0x80003C60);

        // ===== FEATURE MASK (bisection probe) =====
        //
        // Live-tunable u32 at 0x80003C54. Each bit gates one piece of mod
        // behavior. To bisect the 16-ghost crash, write values to this
        // address with Dolphin's memory editor and observe which bit's
        // absence makes 16 ghosts work.
        //
        //   bit 0  (0x01)  call animPoseMain per slot (per-frame anim tick)
        //   bit 1  (0x02)  issue the SECOND animPoseDrawMtx stage call
        //                  (halves per-ghost GPU command volume if cleared)
        //   bit 2  (0x04)  mirror Mario's material flags onto each ghost pose
        //   bit 3  (0x08)  render name-tags for ghosts
        //   bit 4  (0x10)  allocate effects pose (a 3rd pose per ghost)
        //   bit 5  (0x20)  allocate rear pose (a 2nd pose per ghost)
        //   bit 6  (0x40)  allow paper-pose allocation / transitions
        //   bit 7  (0x80)  (reserved - currently a no-op; was dispEntry but
        //                  we don't actually call dispEntry per ghost)
        //
        // Default: 0xFF (all features enabled). Drop to 0x00 for "minimum
        // viable" rendering. Bisection technique: try 0xFD (disable stage 2),
        // then 0xFB (mat-flag mirror), then 0xF7, etc. The bit whose absence
        // lets 16 ghosts work tells us the culprit subsystem.
        //
        // NOTE: Bits 4 and 5 only affect FUTURE allocations - already-allocated
        // poses won't be released. Reset by toggling stress test off/on, or
        // restart Dolphin.
        constexpr uint32_t kFeatAnimMain = 0x01;
        constexpr uint32_t kFeatDrawStage2 = 0x02;
        constexpr uint32_t kFeatMatFlagMirror = 0x04;
        constexpr uint32_t kFeatNameTags = 0x08;
        constexpr uint32_t kFeatEffectsPose = 0x10;
        constexpr uint32_t kFeatRearPose = 0x20;
        constexpr uint32_t kFeatPaperMode = 0x40;
        constexpr uint32_t kFeatReserved = 0x80;
        constexpr uint32_t kFeatAllOn = 0xFF;
        volatile uint32_t *kFeatureMask = reinterpret_cast<volatile uint32_t *>(0x80003C54);

        // Test if a feature is currently enabled. Read fresh per call so
        // toggling the mask in Dolphin takes effect immediately.
        bool FeatOn(uint32_t bit)
        {
            return (*kFeatureMask & bit) != 0u;
        }

        // Game-wide pose pool. wp = 0x803D9470, found via breakpoint at the
        // entry of animPoseEntry (instruction "lwz r5, wp@sda21(r0)" loaded
        // it). Layout from animdrv.s + animInit:
        //   wp[0x00] = ptr to AGB-definition array  (64 entries x 0x10 bytes)
        //   wp[0x04] = max count (= 64)             cap for arrays at wp[0x00] AND wp[0x08]
        //   wp[0x08] = ptr to AGB-instance array    (64 entries x 0x0C bytes)
        //   wp[0x0C] = max count (= 64)             ...same cap re-used
        //   wp[0x10] = ptr to pose array            (256 entries x 0x170 bytes)
        //   wp[0x14] = max count (= 256)
        //   pose[0x00] bit 0 set -> entry is "alive"
        //   defn[0x00] non-zero  -> AGB-defn slot is in use
        //   inst[0x00] non-zero  -> AGB-instance slot is in use
        //
        // CRITICAL: animPoseEntry has THREE arrays it can overflow:
        //   1. wp[0x10] pose array (256) - usually OK, plenty of headroom
        //   2. wp[0x00] AGB defn array (64) - can fill if many distinct AGB
        //      names live concurrently
        //   3. wp[0x08] instance array (64) - can fill if too many parallel
        //      pose-instances exist (allocated per-poseEntry, not refcounted
        //      against existing entries the same way)
        // ANY of these overflowing causes a null-pointer write inside
        // animPoseEntry (r26=0 or r21=0 falls through to stw 0(reg)).
        constexpr uintptr_t kAnimWpAddress = 0x803D9470;
        constexpr ptrdiff_t kPoseArrayPtrOffset = 0x10;
        constexpr ptrdiff_t kPoseArrayCountOffset = 0x14;
        constexpr size_t kPoseEntrySize = 0x170;
        constexpr ptrdiff_t kAgbDefnArrayPtrOffset = 0x00;
        constexpr ptrdiff_t kAgbDefnCountOffset = 0x04;
        constexpr size_t kAgbDefnEntrySize = 0x10;
        constexpr ptrdiff_t kAgbInstArrayPtrOffset = 0x08;
        constexpr ptrdiff_t kAgbInstCountOffset = 0x0C;
        constexpr size_t kAgbInstEntrySize = 0x0C;

        // Walk a uint32-keyed array, count entries with non-zero "alive" word.
        // The first u32 of each entry indicates "in use" for all 3 arrays
        // (pose: bit 0 ; defn: non-zero ; instance: non-zero).
        uint32_t CountArrayInUse(uintptr_t arrayBase, uint32_t maxCount, size_t entrySize, uint32_t cap, bool bitMaskOnly)
        {
            if (arrayBase == 0 || maxCount == 0 || maxCount > cap)
                return 0;
            uint32_t count = 0;
            for (uint32_t i = 0; i < maxCount; ++i)
            {
                const uint32_t *entry = reinterpret_cast<const uint32_t *>(arrayBase + i * entrySize);
                const uint32_t v = entry[0];
                if (bitMaskOnly ? (v & 1u) != 0u : v != 0u)
                    ++count;
            }
            return count;
        }

        uint32_t CountGlobalPosesInUse()
        {
            const auto *wp = reinterpret_cast<const uint32_t *>(kAnimWpAddress);
            return CountArrayInUse(wp[kPoseArrayPtrOffset / 4],
                                   wp[kPoseArrayCountOffset / 4],
                                   kPoseEntrySize,
                                   /*cap=*/256,
                                   /*bitMaskOnly=*/true);
        }

        uint32_t CountGlobalAgbDefnsInUse()
        {
            const auto *wp = reinterpret_cast<const uint32_t *>(kAnimWpAddress);
            return CountArrayInUse(wp[kAgbDefnArrayPtrOffset / 4],
                                   wp[kAgbDefnCountOffset / 4],
                                   kAgbDefnEntrySize,
                                   /*cap=*/64,
                                   /*bitMaskOnly=*/false);
        }

        uint32_t CountGlobalAgbInstancesInUse()
        {
            const auto *wp = reinterpret_cast<const uint32_t *>(kAnimWpAddress);
            return CountArrayInUse(wp[kAgbInstArrayPtrOffset / 4],
                                   wp[kAgbInstCountOffset / 4],
                                   kAgbInstEntrySize,
                                   /*cap=*/64,
                                   /*bitMaskOnly=*/false);
        }

        struct GhostSlot
        {
            // Three poses per ghost - matches what the game's marioPoseInit
            // allocates for Mario himself. Each pose binds a different AGB
            // and animPoseSetAnim only resolves names within its own AGB.
            //   forward (a_mario)   - normal locomotion, idle
            //   rear    (a_mario_r) - rear-facing variants
            //   effects (e_mario)   - yawn, sleep, paper-mode special states
            bool forwardAllocated;
            bool rearAllocated;
            bool effectsAllocated;
            int32_t forwardPoseId;
            int32_t rearPoseId;
            int32_t effectsPoseId;

            // Which pose was most recently driven by an anim - this is the
            // one DrawAll renders. Tracks the source player's facing.
            //   0 = forward, 1 = rear, 2 = effects
            int8_t activePose;

            // Last anim name set on each pose, for change detection.
            char lastAnimForward[16];
            char lastAnimRear[16];
            char lastAnimEffects[16];

            // Last paper-anim name applied to forward pose, for change
            // detection. Empty string ("") means no paper anim is active
            // on the slot's forward pose.
            //
            // Paper anim is a separate state from regular anim - it's a
            // second animation layered on the forward pose, used for
            // paper-mode states (curl, plane, etc). The game tracks it
            // via mp+0x1C (a string pointer); we mirror the dereferenced
            // string into peer.paperAnimName.
            char lastPaperAnim[16];

            // Paper-pose lifecycle (v8). Paper mode is a SEPARATE pose
            // allocation from the regular forward/rear/effects poses.
            // marioPaperOn calls animPaperPoseEntry(agbName, 2) which
            // returns a poseId; this is stored at mp+0x240. The pose ID
            // is then used by the rendering pipeline as a "paper layer"
            // on top of the forward pose. marioPaperOff releases it.
            //
            // We mirror this:
            //   peer.paperAgbName empty   -> no paper pose; paperPoseId = -1
            //   peer.paperAgbName changed -> release old (if any), allocate new
            //   peer.paperAgbName same    -> nothing to do here, but paperAnim
            //                                may still need updating
            //
            // After allocation, the paper pose is linked to the forward
            // pose via animPoseSetPaperAnimGroup, and the actual anim is
            // applied via animPoseSetPaperAnim.
            int32_t paperPoseId;   // -1 = none allocated
            char lastPaperAgb[32]; // v9: widened to 32 (TTYD AGB names exceed 16)

            // Smoothed (interpolated) position and rotation. The wire-format
            // peer.x/y/z and peer.rotationY can wobble between frames due to:
            //   - asyncio sleep imprecision (publish timing wobble)
            //   - Dolphin IPC latency variance
            //   - non-atomic reads on the source side
            //   - AP network jitter (relevant in real multi-client play)
            // Lerping each frame on the render side hides all of these and
            // produces visually smooth ghost motion regardless of source
            // jitter. We render from these fields, NOT directly from peer.*.
            float renderX;
            float renderY;
            float renderZ;
            float renderRotY;
            // First-frame snap flag: when a peer becomes active we want
            // render* to jump to peer.* immediately rather than lerp from
            // wherever the last (now-stale) values pointed. After the first
            // frame this stays false until the peer goes inactive again.
            bool renderInitialized;
        };

        // Per-frame fraction of the gap between renderX/Y/Z and peer.x/y/z
        // that gets closed. Higher = snappier (less smoothing, more accurate
        // tracking, more visible jitter). Lower = smoother (more apparent
        // lag, less jitter). Tune empirically.
        constexpr float kLerpAlpha = 0.30f;

        GhostSlot g_slots[kMaxPeers];
        bool g_initialized = false;

        // Read the shared block IF it's valid - magic/version match. Returns
        // null if the AP client hasn't connected yet, the block is corrupted,
        // or the version is incompatible.
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

        // Maximum number of ghosts we'll attempt to allocate poses for
        // simultaneously. Set BELOW the wire-format kMaxPeers because the
        // game's animPoseEntry pool has limited capacity and it CORRUPTS
        // MEMORY on overflow (see disasm at 0x8004181C: when no empty
        // slot is found, r31 stays uninitialized and gets dereferenced).
        //
        // **The 12 default is empirical, not theoretical.** Stress testing
        // showed:
        //   - The "safe" ceiling varies by map. gor_01 (relatively empty)
        //     held 22 ghosts stably for 7000+ frames. Other maps with more
        //     NPCs crashed at 16.
        //   - Crashes are deterministic for a given map+ghost-count combo
        //     but manifest in unrelated subsystems (GXGetGPStatus, DSP)
        //     which suggests memory corruption far from the actual cause.
        //   - Diagnostics ruled out the obvious capacity caps: the engine's
        //     pose pool (256), AGB-defn array (64), and AGB-instance array
        //     (64) all had ample headroom at every observed crash. Adding
        //     hundreds of KB to the smart heap didn't help either.
        //   - Room transitions are independently fragile: the engine appears
        //     to call animPoseAutoRelease on our pose group during transit,
        //     freeing buffers we still reference. Even with map-change
        //     detection in UpdateAll, transitions at high ghost counts
        //     remain crash-prone.
        //
        // 12 leaves a comfortable margin below the lowest observed safe
        // ceiling (16 on busy maps) so most multiworld sessions should be
        // crash-free. Power users can probe their specific map by writing
        // a higher value at 0x80003C40 with Dolphin's memory editor; the
        // tunable is read fresh per allocation attempt.
        //
        // Slots beyond the cap stay un-rendered: the wire format still
        // carries all 32 peers' data but EnsurePosesAllocated returns early
        // and DrawAll skips them via the `*Allocated` guards. The data is
        // still on the wire so future raises of the cap "just work".
        constexpr int kDefaultMaxRenderedPeers = 12;
        volatile uint32_t *kTuneMaxRenderedPeers = reinterpret_cast<volatile uint32_t *>(0x80003C40);

        int CurrentMaxRenderedPeers()
        {
            // Read tunable; clamp to [0, kMaxPeers] so a rogue write can't
            // make us either skip everyone (negative -- impossible since
            // unsigned) or overflow the slot array.
            uint32_t v = *kTuneMaxRenderedPeers;
            if (v > static_cast<uint32_t>(kMaxPeers))
                v = static_cast<uint32_t>(kMaxPeers);
            return static_cast<int>(v);
        }

        // Helper: determine if this slot is allowed to allocate poses.
        // We allow the first N slot indices in order, where N is the
        // current live-tunable max (read fresh per call).
        bool SlotMayAllocate(const GhostSlot &slot)
        {
            const ptrdiff_t idx = &slot - &g_slots[0];
            return idx >= 0 && idx < CurrentMaxRenderedPeers();
        }

        void EnsurePosesAllocated(GhostSlot &slot)
        {
            // Cap allocations to avoid the game's pose pool overflow crash.
            // Slots past kMaxRenderedPeers won't render but won't crash either.
            if (!SlotMayAllocate(slot))
                return;

            const bool isSlot0 = (&slot == &g_slots[0]);
            const uint32_t slotIdx = static_cast<uint32_t>(&slot - &g_slots[0]);

            *kDiagUpdatePhase = 4; // entering EnsurePosesAllocated body

            if (!slot.forwardAllocated)
            {
                *kDiagUpdatePhase = 5; // pre-fwd animPoseEntry
                *kDiagFwdAttempt = *kDiagFwdAttempt + 1;
                int32_t id = ttyd::animdrv::animPoseEntry(kAgbForward, kPoseGroup);
                *kDiagUpdatePhase = 6; // post-fwd animPoseEntry
                if (id < 0)
                {
                    *kDiagFwdFail = *kDiagFwdFail + 1;
                    *kDiagLastFailSlot = slotIdx;
                    *kDiagLastFailPhase = 1;
                }
                else
                {
                    slot.forwardPoseId = id;
                    slot.forwardAllocated = true;
                    slot.lastAnimForward[0] = '\0';
                    if (isSlot0)
                        ttyd::animdrv::animPoseSetAnim(id, "M_S_1", /*forceReset=*/1);
                }
            }
            if (FeatOn(kFeatRearPose) && !slot.rearAllocated)
            {
                *kDiagUpdatePhase = 7; // pre-rear animPoseEntry
                *kDiagRearAttempt = *kDiagRearAttempt + 1;
                int32_t id = ttyd::animdrv::animPoseEntry(kAgbRear, kPoseGroup);
                *kDiagUpdatePhase = 8; // post-rear animPoseEntry
                if (id < 0)
                {
                    *kDiagRearFail = *kDiagRearFail + 1;
                    *kDiagLastFailSlot = slotIdx;
                    *kDiagLastFailPhase = 2;
                }
                else
                {
                    slot.rearPoseId = id;
                    slot.rearAllocated = true;
                    slot.lastAnimRear[0] = '\0';
                    if (isSlot0)
                        ttyd::animdrv::animPoseSetAnim(id, "M_S_1R", /*forceReset=*/1);
                }
            }
            if (FeatOn(kFeatEffectsPose) && !slot.effectsAllocated)
            {
                *kDiagUpdatePhase = 9; // pre-effects animPoseEntry
                *kDiagEffAttempt = *kDiagEffAttempt + 1;
                int32_t id = ttyd::animdrv::animPoseEntry(kAgbEffects, kPoseGroup);
                *kDiagUpdatePhase = 10; // post-effects animPoseEntry
                if (id < 0)
                {
                    *kDiagEffFail = *kDiagEffFail + 1;
                    *kDiagLastFailSlot = slotIdx;
                    *kDiagLastFailPhase = 3;
                }
                else
                {
                    slot.effectsPoseId = id;
                    slot.effectsAllocated = true;
                    slot.lastAnimEffects[0] = '\0';
                    // Don't pre-seed an anim - effects pose only plays when
                    // the source player has flags2 effects bit set.
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
            // Release paper pose if allocated. animPaperPoseRelease is the
            // counterpart to animPaperPoseEntry (used by marioPaperOff at
            // 80058A70). Without this, every peer disconnect/reconnect
            // would leak a paper pose handle.
            if (slot.paperPoseId >= 0)
            {
                ttyd::animdrv::animPaperPoseRelease(slot.paperPoseId);
                slot.paperPoseId = -1;
            }
            slot.lastPaperAgb[0] = '\0';
            // Mark render position as needing a snap on next activation.
            // Without this, when a peer reactivates we'd lerp from wherever
            // their old (stale) render position was to their new one,
            // causing a visible "swoop" across the map.
            slot.renderInitialized = false;
        }

        // Route pose by flags2 - matches marioPreDisp's own logic exactly.
        // The R suffix on anim names is COINCIDENTAL; we don't look at the
        // name, only at flags2.
        //   flags2 & 0x10000000 -> effects pose (e_mario)   - return 2
        //   flags2 & 0x80000000 -> rear pose    (a_mario_r) - return 1
        //   else                -> forward pose (a_mario)   - return 0
        int8_t PickPoseIndex(uint32_t flags2)
        {
            if (flags2 & kFlags2EffectsMask)
                return 2;
            if (flags2 & kFlags2RearMask)
                return 1;
            return 0;
        }

        // Linear interp between current and target position. Standard lerp:
        // result = current + (target - current) * alpha. With alpha = 0.30
        // at 60Hz, ~98% of the gap closes in 10 frames (~167ms) - smooth
        // but still responsive.
        float Lerp(float current, float target, float alpha)
        {
            return current + (target - current) * alpha;
        }

        // Angle lerp (degrees) that takes the SHORT way around the circle.
        // Naive lerp from 350 to 10 would sweep through 180 instead of just
        // crossing 360->0. Wrap the delta into [-180, +180] before lerping.
        float LerpAngleDeg(float current, float target, float alpha)
        {
            float diff = target - current;
            while (diff > 180.0f) diff -= 360.0f;
            while (diff < -180.0f) diff += 360.0f;
            return current + diff * alpha;
        }

        // Apply one peer's data to its game-side animPoses. Picks pose by
        // flags2 (matching marioPreDisp). The anim name is set on whichever
        // pose was selected; we gate on memcmp because calling setAnim every
        // frame with forceReset=1 freezes the model on frame 0 (the system
        // doesn't behave the way the game's marioPreDisp implies it should
        // when called from outside the normal Mario render path).
        //
        // Side effect: one-shot anims like yawn/sleep don't replay properly
        // because they finish their cycle and hold their last frame. This
        // is a known limitation we'll address separately.
        void ApplyPeerToSlot(const PeerSlot &peer, GhostSlot &slot)
        {
            EnsurePosesAllocated(slot);

            const int8_t poseIdx = PickPoseIndex(peer.flags2);

            int32_t poseId = -1;
            char *cache = nullptr;
            bool canDrive = false;

            switch (poseIdx)
            {
                case 0: // forward
                    if (slot.forwardAllocated)
                    {
                        poseId = slot.forwardPoseId;
                        cache = slot.lastAnimForward;
                        canDrive = true;
                        slot.activePose = 0;
                    }
                    break;
                case 1: // rear
                    if (slot.rearAllocated)
                    {
                        poseId = slot.rearPoseId;
                        cache = slot.lastAnimRear;
                        canDrive = true;
                        slot.activePose = 1;
                    }
                    break;
                case 2: // effects (yawn, sleep, paper-mode)
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
                ttyd::animdrv::animPoseSetAnim(poseId, peer.animName, /*forceReset=*/1);
                std::memcpy(cache, peer.animName, sizeof(peer.animName));
            }

            // -----------------------------------------------------------------
            // Paper-mode lifecycle. Paper mode is a separate pose allocation
            // layered on top of the forward pose. Three things happen on
            // setup (matches marioPaperOn at 80058A90 + marioChgPaper at
            // 80058C68 + marioPreDisp at 80057948):
            //
            //   1. animPaperPoseEntry(agbName, 2) -> paperPoseId
            //   2. animPoseSetPaperAnimGroup(forwardPose, agbName, bUnkOn)
            //   3. animPoseSetPaperAnim(forwardPose, animName)
            //
            // Wire format:
            //   peer.paperAgbName  - AGB name (e.g. "a_kuru"). Empty = OFF.
            //   peer.paperAnimName - anim name within that AGB. Empty = OFF.
            //
            // Either field empty -> paper mode is OFF for this peer.
            //
            // We track paperPoseId / lastPaperAgb / lastPaperAnim per slot
            // so we only do allocations on transitions, not every frame.
            //
            // Skip if forward pose isn't allocated yet (lazy alloc retry).
            // Feature bit 6 lets us bisect the paper-mode subsystem.
            // -----------------------------------------------------------------
            if (slot.forwardAllocated && FeatOn(kFeatPaperMode))
            {
                const bool peerInPaper = peer.paperAgbName[0] != '\0' && peer.paperAnimName[0] != '\0';
                const bool slotInPaper = slot.paperPoseId >= 0;

                const bool agbChanged = std::memcmp(slot.lastPaperAgb, peer.paperAgbName, sizeof(peer.paperAgbName)) != 0;
                const bool animChanged = std::memcmp(slot.lastPaperAnim, peer.paperAnimName, sizeof(peer.paperAnimName)) != 0;

                if (slotInPaper && !peerInPaper)
                {
                    // ---- Transition OFF paper mode ----
                    // Mirrors marioPaperOff at 80058A30. Clear the link from
                    // forward pose to paper-anim group, then release the
                    // paper pose itself.
                    ttyd::animdrv::animPoseSetPaperAnimGroup(slot.forwardPoseId, /*agbName=*/nullptr, /*bUnkOn=*/0);
                    ttyd::animdrv::animPaperPoseRelease(slot.paperPoseId);
                    slot.paperPoseId = -1;
                    slot.lastPaperAgb[0] = '\0';
                    slot.lastPaperAnim[0] = '\0';
                }
                else if (peerInPaper && (!slotInPaper || agbChanged))
                {
                    // ---- Transition ON paper mode (or AGB type changed) ----
                    // If we already had a paper pose for a DIFFERENT AGB,
                    // release it first to avoid leaking handles when a peer
                    // morphs between paper types.
                    if (slotInPaper)
                    {
                        ttyd::animdrv::animPoseSetPaperAnimGroup(slot.forwardPoseId, /*agbName=*/nullptr, /*bUnkOn=*/0);
                        ttyd::animdrv::animPaperPoseRelease(slot.paperPoseId);
                        slot.paperPoseId = -1;
                    }

                    // Allocate fresh paper pose. The "2" matches the group
                    // arg marioPaperOn passes (line 80058ADC: li r4, 0x2).
                    // Compute bUnkOn the same way marioPaperOn does
                    // (lines 80058AF4-80058B70 in mario.s). marioPaperOn
                    // strcmp's the AGB name against six "short name"
                    // strings; if ANY match, bUnkOn = 0, otherwise = 1.
                    //
                    // The strings live in rodata (we hardcode the addresses
                    // since we can't access the local sda21 base directly):
                    //   0x802C1BA0  "p_roll"
                    //   0x802C1BA8  "p_plane"
                    //   0x802C1BB0  "p_hune"
                    //   0x802C1BB8  "p_jyabara"
                    //   0x802C1BC4  "p_dokan_x"
                    //   0x802C1BD0  "p_dokan_y"
                    //
                    // bUnkOn changes which flag bits get set on the paper
                    // pose (animPoseSetPaperAnimGroup at 80041020-80041058):
                    // bUnkOn=1 sets pose+0x0 bits 0x2 and 0x4; bUnkOn=0
                    // clears bits 1, 2, and 5. These pose flags affect the
                    // render path - getting the wrong one causes paper
                    // modes to display incorrectly (e.g. dokan going-down
                    // visually appearing as un-transformed Mario).
                    int bUnkOn = 1;
                    static const uintptr_t kGauntlet[] = {
                        0x802C1BA0, // p_roll
                        0x802C1BA8, // p_plane
                        0x802C1BB0, // p_hune
                        0x802C1BB8, // p_jyabara
                        0x802C1BC4, // p_dokan_x
                        0x802C1BD0, // p_dokan_y
                    };
                    for (uintptr_t s : kGauntlet)
                    {
                        const char *cmp = reinterpret_cast<const char *>(s);
                        // Inline strcmp - the local strcmp impl is in libc;
                        // we don't pull in libc just for this.
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
                    *kDiagUpdatePhase = 11; // pre-paper animPaperPoseEntry
                    *kDiagPaperAttempt = *kDiagPaperAttempt + 1;
                    slot.paperPoseId = ttyd::animdrv::animPaperPoseEntry(peer.paperAgbName, /*group=*/2);
                    *kDiagUpdatePhase = 12; // post-paper animPaperPoseEntry

                    if (slot.paperPoseId >= 0)
                    {
                        ttyd::animdrv::animPoseSetPaperAnimGroup(slot.forwardPoseId, peer.paperAgbName, bUnkOn);
                        ttyd::animdrv::animPoseSetPaperAnim(slot.forwardPoseId, peer.paperAnimName);
                        std::memcpy(slot.lastPaperAgb, peer.paperAgbName, sizeof(peer.paperAgbName));
                        std::memcpy(slot.lastPaperAnim, peer.paperAnimName, sizeof(peer.paperAnimName));
                    }
                    else
                    {
                        *kDiagPaperFail = *kDiagPaperFail + 1;
                        *kDiagLastFailSlot = static_cast<uint32_t>(&slot - &g_slots[0]);
                        *kDiagLastFailPhase = 4;
                    }
                    // If allocation fails (paper-pose pool full?), we just
                    // don't set up paper mode for this slot. Ghost stays
                    // as regular Mario - graceful degradation.
                }
                else if (peerInPaper && slotInPaper && animChanged)
                {
                    // ---- Same paper type, different anim ----
                    // e.g. plane curling in vs flying. Just update the anim.
                    ttyd::animdrv::animPoseSetPaperAnim(slot.forwardPoseId, peer.paperAnimName);
                    std::memcpy(slot.lastPaperAnim, peer.paperAnimName, sizeof(peer.paperAnimName));
                }
                // else: paper state unchanged or both empty - nothing to do
            }
        }
    } // anonymous namespace

    // -------------------------------------------------------------------------
    // Public API
    // -------------------------------------------------------------------------
    KEEP_FUNC void Init()
    {
        if (g_initialized)
            return;
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
            // Lerp fields - explicit init.
            s.renderX = 0.0f;
            s.renderY = 0.0f;
            s.renderZ = 0.0f;
            s.renderRotY = 0.0f;
            s.renderInitialized = false;
        }

        // Initialize diagnostic block. last_failed_slot starts as 0xFFFFFFFF
        // so that "slot 0 failure" doesn't get confused with "no failure yet".
        *kDiagUpdateHB = 0;
        *kDiagDrawHB = 0;
        *kDiagActivePeers = 0;
        *kDiagFwdAttempt = 0;
        *kDiagFwdFail = 0;
        *kDiagRearAttempt = 0;
        *kDiagRearFail = 0;
        *kDiagEffAttempt = 0;
        *kDiagEffFail = 0;
        *kDiagPaperAttempt = 0;
        *kDiagPaperFail = 0;
        *kDiagDrawCalls = 0;
        *kDiagPosesAlloc = 0;
        *kDiagLastFailSlot = 0xFFFFFFFFu;
        *kDiagLastFailPhase = 0;
        *kDiagAgbDefnsInUse = 0;
        *kDiagPoolInUse = 0;
        *kDiagAgbInstInUse = 0;
        *kDiagMapChanges = 0;
        *kDiagDrawSlot = 0xFFFFFFFFu;
        *kDiagDrawPhase = 0;

        // Seed the live-tunable max-rendered-peers cap with our default.
        // Write a different value to 0x80003C40 with Dolphin's memory
        // editor at any time to probe pool capacity.
        *kTuneMaxRenderedPeers = static_cast<uint32_t>(kDefaultMaxRenderedPeers);

        // Seed feature mask = all features ON. Toggle bits at 0x80003C54
        // to bisect the 16-ghost crash without rebuilding.
        *kFeatureMask = kFeatAllOn;

        g_initialized = true;
    }

    KEEP_FUNC void Shutdown()
    {
        for (auto &s : g_slots) ReleaseSlot(s);
        g_initialized = false;
    }

    void UpdateAll()
    {
        if (!g_initialized)
            return;

        const SharedBlock *block = GetValidBlock();
        if (block == nullptr)
        {
            // No connected AP client (magic mismatch). Tear down any
            // allocated poses so we don't keep stale ghosts visible if
            // the client crashes/exits before the magic gets cleared.
            for (auto &s : g_slots) ReleaseSlot(s);
            return;
        }

        // ---------------------------------------------------------------
        // Map-change detection.
        //
        // When TTYD transitions between maps, it appears to call
        // animPoseAutoRelease(group=2) on all entities in our pose group.
        // animPoseAutoRelease (animdrv.s line 2009) walks the pose array
        // and for poses where refcount==0 (which is OUR initial state),
        // takes the .L_8003B110 branch that smartFree's the joint matrix
        // buffer at pose+0x15C. After this, our `slot.forwardPoseId` etc
        // point at a pose whose joint buffer is freed and possibly
        // reassigned to a new map's NPC. Drawing that pose leads to
        // memory corruption / crashes downstream (manifesting in
        // GXGetGPStatus, __DSPHandler, etc).
        //
        // Fix: snapshot _globalWorkPtr->currentMapName each frame. If it
        // changes, force-release all our slots locally (we don't know if
        // the engine already released the underlying poses, but
        // ReleaseSlot calls animPoseRelease which is safe to call on a
        // potentially-already-freed pose - it just clears bits and
        // decrements the refcount). Then natural lazy re-allocation in
        // ApplyPeerToSlot kicks in next frame.
        //
        // We only compare the first 16 bytes (matches PeerSlot's mapName
        // length) which is enough to disambiguate any TTYD map name.
        {
            constexpr size_t kMapNameLen = 16;
            static char s_lastMapName[kMapNameLen] = {0};
            const auto *gw = _globalWorkPtr;
            if (gw != nullptr)
            {
                const char *currentMap = gw->currentMapName;
                if (std::memcmp(s_lastMapName, currentMap, kMapNameLen) != 0)
                {
                    // Map changed (or just-booted from zeros). Release all
                    // ghost slots so lazy alloc rebuilds them with fresh
                    // pose entries.
                    for (auto &s : g_slots) ReleaseSlot(s);
                    std::memcpy(s_lastMapName, currentMap, kMapNameLen);
                    *kDiagMapChanges = *kDiagMapChanges + 1;
                }
            }
        }

        for (int i = 0; i < kMaxPeers; ++i)
        {
            *kDiagUpdateSlot = static_cast<uint32_t>(i);
            *kDiagUpdatePhase = 1; // top of iter / read peer
            const PeerSlot &peer = block->peers[i];
            GhostSlot &slot = g_slots[i];

            if (!peer.active)
            {
                *kDiagUpdatePhase = 2; // ReleaseSlot path
                ReleaseSlot(slot);
                continue;
            }

            *kDiagUpdatePhase = 3; // entering ApplyPeerToSlot
            ApplyPeerToSlot(peer, slot);
            *kDiagUpdatePhase = 13; // post-ApplyPeerToSlot

            // Smoothing: lerp render position toward the wire-format peer
            // position each frame. Snap on first frame after activation
            // so we don't visibly slide across the map from a stale state.
            //
            // Some motions intentionally have rapid per-frame rotation
            // changes that the lerp would visibly smooth out. The
            // hammer-spin attack (motionId 0x13) twists Mario's body via
            // mp+0x1AC each frame as part of the visual effect. Lerping
            // at 30% kills the twitch entirely. For these motions we
            // snap rotY to the peer value instead.
            //
            // We still lerp position for these - the body twist is
            // rotational, not positional, and Mario's actual XYZ
            // position is stable during the spin.
            const bool snapRotY = (peer.motionId == 0x13); // hammer spin

            if (!slot.renderInitialized)
            {
                slot.renderX = peer.x;
                slot.renderY = peer.y;
                slot.renderZ = peer.z;
                slot.renderRotY = peer.rotationY;
                slot.renderInitialized = true;
            }
            else
            {
                slot.renderX = Lerp(slot.renderX, peer.x, kLerpAlpha);
                slot.renderY = Lerp(slot.renderY, peer.y, kLerpAlpha);
                slot.renderZ = Lerp(slot.renderZ, peer.z, kLerpAlpha);
                slot.renderRotY = snapRotY ? peer.rotationY : LerpAngleDeg(slot.renderRotY, peer.rotationY, kLerpAlpha);
            }

            // Tick the animation forward on the active pose only - off-map
            // ghosts skip this since they aren't being drawn.
            //
            // After the tick, optionally apply paperLocalTime override.
            // Some held anims don't progress under animPoseMain alone; the
            // local game manually drives their playhead each frame from
            // some state field. We replicate this by overriding the time
            // immediately after animPoseMain.
            //
            // Cases (driven from source side, see TTYDClient.py):
            //   - Hammer spin held P_H_1A: time = mp+0x2C8 / 6.0
            //   - Jabara M_S_4 held-rest:  time = (float)(int8_t)mp+0x2D3
            //
            // Sentinel: peer.paperLocalTime == -1.0 means "no override".
            // We use -1.0 (not 0.0) because 0.0 is a legitimate override
            // value (e.g. when the source's playhead byte is 0 on the
            // first frame of the held state).
            if (PeerOnLocalMap(peer))
            {
                int32_t activePoseId = -1;
                const bool runAnimMain = FeatOn(kFeatAnimMain);
                if (slot.activePose == 2 && slot.effectsAllocated)
                {
                    if (runAnimMain)
                        ttyd::animdrv::animPoseMain(slot.effectsPoseId);
                    activePoseId = slot.effectsPoseId;
                }
                else if (slot.activePose == 1 && slot.rearAllocated)
                {
                    if (runAnimMain)
                        ttyd::animdrv::animPoseMain(slot.rearPoseId);
                    activePoseId = slot.rearPoseId;
                }
                else if (slot.forwardAllocated)
                {
                    if (runAnimMain)
                        ttyd::animdrv::animPoseMain(slot.forwardPoseId);
                    activePoseId = slot.forwardPoseId;
                }

                if (activePoseId >= 0 && peer.paperLocalTime != -1.0f)
                {
                    ttyd::animdrv::animPoseSetLocalTime(activePoseId, peer.paperLocalTime);
                }
            }
        }

        // -----------------------------------------------------------------
        // Mirror Mario's pose material flags onto every active ghost pose.
        //
        // Why this matters: the door event scripts call npc_dark when you
        // walk into a building, which calls animPoseSetMaterialFlagOn(0x1800)
        // on Mario's three poseIds (mp+0x22C/0x230/0x234) and on every
        // party member, NPC, mapobj, and item in the scene. The 0x1800
        // material flag is what tells the renderer "this object belongs to
        // the indoor render path" - without it, the object is invisible
        // inside buildings.
        //
        // Our ghost poses are independent allocations, so npc_dark never
        // touches them. They render against the OUTDOOR path, which isn't
        // active indoors -> ghost invisible.
        //
        // The cleanest fix: each frame, just COPY Mario's current material
        // flag state onto our poses. We don't need to know what 0x1800 means
        // or when it's set/cleared - we just track Mario. When he gets dark,
        // we get dark. When he doesn't, we don't.
        //
        // We also copy LightFlag in case there's a light-flag mirror of the
        // same mechanism (animPoseSetMaterialLightFlag* exists in symbols).
        // -----------------------------------------------------------------
        ttyd::mario::Player *mp = ttyd::mario::marioGetPtr();
        if (mp != nullptr)
        {
            // Mario's three pose IDs are at fixed offsets in the Player
            // struct (verified from npc_dark disasm):
            //   mp+0x22C = forward pose (a_mario)
            //   mp+0x230 = rear pose    (a_mario_r)
            //   mp+0x234 = effects pose (e_mario)
            const uint8_t *mpBytes = reinterpret_cast<const uint8_t *>(mp);
            const int32_t marioFwd = *reinterpret_cast<const int32_t *>(mpBytes + 0x22C);
            const int32_t marioRear = *reinterpret_cast<const int32_t *>(mpBytes + 0x230);
            const int32_t marioFx = *reinterpret_cast<const int32_t *>(mpBytes + 0x234);

            // Read Mario's current flags. animPoseGetMaterialFlag and
            // animPoseGetMaterialLightFlag both take a pose ID; we don't
            // know what flags are currently set (they vary by indoor/
            // outdoor/cutscene/etc) - we just copy them as-is.
            const uint32_t marioMatFlag = ttyd::animdrv::animPoseGetMaterialFlag(marioFwd);
            const uint32_t marioLitFlag = ttyd::animdrv::animPoseGetMaterialLightFlag(marioFwd);
            // (Rear and effects might be in different states but typically
            //  npc_dark sets them all to the same value, so just trust forward.)

            // Apply to every active ghost slot's three poses.
            // Gated by feature bit 2 - drop it during bisection to skip
            // all per-pose flag writes.
            if (FeatOn(kFeatMatFlagMirror))
                for (auto &slot : g_slots)
                {
                    if (slot.forwardAllocated)
                    {
                        // The ON/OFF API is "set these bits in the mask".
                        // To make our flags match Mario's exactly, we need to
                        // ON the bits Mario has and OFF the bits he doesn't.
                        // Easier: use the Set api directly. But TTYD only
                        // exposes On/Off, not Set. So:
                        //   ON  = marioMatFlag (turns on the bits Mario has)
                        //   OFF = ~marioMatFlag (turns off all OTHER bits)
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

            // -----------------------------------------------------------------
            // Publish local Mario's paper-pose SHORT name for the AP client.
            //
            // Why "short name": animPaperPoseEntry's internal lookup compares
            // against the file's primary label - the SHORT name passed to
            // marioPaperOn (e.g. "p_hikohki" for plane, "p_slit" for slit).
            // The LONG name returned by animPoseGetGroupName (e.g.
            // "slit_mdel3_pPlane1") lives in a different memory region and
            // isn't what the lookup checks.
            //
            // Strategy: the short name is determined by which motion is
            // active. mp+0x2E (currentMotionId) tells us. The motion IDs
            // and their AGB lookup arrays come straight from marioMotTbl
            // (the motion dispatch table) and the per-motion code:
            //
            //   0x15 (slit):  hardcoded "p_slit" at 0x802C433C
            //   0x16 (roll):  hardcoded "p_roll" at 0x802C4248
            //   0x18 (plane): paper_plane[color] array at 0x80416228
            //   0x19 (ship):  paper_ship[color] array at 0x80416280
            //
            // Skipping less-common transformations (jabara, dokan, etc) -
            // they use similar patterns but are rare in normal play; can
            // add when needed.
            //
            // The paper pose ID lives at mp+0x240. If it's < 0, no paper
            // mode is active - we write an empty string.
            // -----------------------------------------------------------------
            char *selfPaperAgb = GetSelfPaperAgbName();
            // Always start by clearing - writes a zero byte at minimum.
            for (int i = 0; i < kSelfPaperAgbLen; ++i) selfPaperAgb[i] = '\0';

            const int32_t marioPaperPoseId = *reinterpret_cast<const int32_t *>(mpBytes + 0x240);
            if (marioPaperPoseId >= 0)
            {
                // Read currentMotionId from mp+0x2E (u16). This identifies
                // which mot_* function is currently driving Mario's state.
                const uint16_t motionId = *reinterpret_cast<const uint16_t *>(mpBytes + 0x2E);

                const char *shortName = nullptr;
                switch (motionId)
                {
                    case 0x13: // mot_hammer2 - hammer spin attack
                        // mot_hammer2 passes str_p_kaiten_h_802c3f70 to
                        // marioPaperOn at line 800976BC. "kaiten" =
                        // Japanese for spin/rotation, "_h" = hammer. This
                        // is the Super/Ultra Hammer multi-bonk spin.
                        // No color variants.
                        shortName = reinterpret_cast<const char *>(0x802C3F70);
                        break;
                    case 0x14: // mot_jabara - paper accordion fold
                        // No color variants. String at 0x802C4008.
                        shortName = reinterpret_cast<const char *>(0x802C4008);
                        break;
                    case 0x15: // mot_slit
                        // mot_slit always passes str_p_slit_802c433c to
                        // marioPaperOn (no color variants, line 800A43D8).
                        shortName = reinterpret_cast<const char *>(0x802C433C);
                        break;
                    case 0x16: // mot_roll
                        // mot_roll passes str_p_roll_802c4248 (line 8009EB44).
                        shortName = reinterpret_cast<const char *>(0x802C4248);
                        break;
                    case 0x18: // mot_plane
                    {
                        // mot_plane indexes into paper_plane[] at
                        // 0x80416228 by marioGetColor() (line 8009D930).
                        const int8_t color = ttyd::mario::marioGetColor();
                        const uint32_t *arr = reinterpret_cast<const uint32_t *>(0x80416228);
                        shortName = reinterpret_cast<const char *>(arr[color & 3]);
                        break;
                    }
                    case 0x19: // mot_ship
                    {
                        // mot_ship indexes into paper_ship[] at 0x80416280
                        // (line 800A2650).
                        const int8_t color = ttyd::mario::marioGetColor();
                        const uint32_t *arr = reinterpret_cast<const uint32_t *>(0x80416280);
                        shortName = reinterpret_cast<const char *>(arr[color & 3]);
                        break;
                    }
                    case 0x1D: // mot_dokan - pipe entry/exit
                    {
                        // mot_dokan dispatches based on mp+0x3E ONLY on the
                        // first frame (gated by mp+0xC bit 0), then mp+0x3E
                        // gets modified by other code (event scripts, anim
                        // events). So we can't use the dispatch values from
                        // the disasm directly - we need empirically observed
                        // values during the held state.
                        //
                        // Observed values during sustained dokan animations:
                        //   mp+0x3E == 1 -> coming UP a vertical pipe   -> p_dokan_y
                        //   mp+0x3E == 3 -> going DOWN a vertical pipe  -> p_dokan_y
                        //   other        -> horizontal pipe (any dir)   -> p_dokan_x
                        //
                        // Strings at 0x802F4610 (x) and 0x802F45FC (y).
                        const int8_t dokanType = *reinterpret_cast<const int8_t *>(mpBytes + 0x3E);
                        const bool isVertical = (dokanType == 1) || (dokanType == 3);
                        shortName = reinterpret_cast<const char *>(isVertical ? 0x802F45FC   // p_dokan_y
                                                                              : 0x802F4610); // p_dokan_x
                        break;
                    }
                    default:
                        // Not a paper-mode we currently handle. Leave
                        // selfPaperAgb empty so the peer doesn't try to
                        // allocate.
                        // Other known paper modes we haven't mapped yet:
                        //   0x1C mot_vivian (Vivian shadow merge)
                        //   ?    mot_bibi   - "p_bibi"
                        //   ?    mot_b_st   - "p_b_st" (boat stop?)
                        break;
                }

                if (shortName != nullptr)
                {
                    // Copy up to kSelfPaperAgbLen-1 chars (leave room for NUL).
                    for (int i = 0; i < kSelfPaperAgbLen - 1; ++i)
                    {
                        const char c = shortName[i];
                        selfPaperAgb[i] = c;
                        if (c == '\0')
                            break;
                    }
                    // Last byte stays NUL (initialized above).
                }
            }
        }

        // Diagnostic: count active peers + poses allocated, then tick heartbeat.
        // If the game crashes during UpdateAll, the heartbeat will lag the
        // draw counter, which tells us where the death happened.
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
            *kDiagActivePeers = activePeers;
            *kDiagPosesAlloc = posesAlloc;
            *kDiagPoolInUse = CountGlobalPosesInUse();
            *kDiagAgbDefnsInUse = CountGlobalAgbDefnsInUse();
            *kDiagAgbInstInUse = CountGlobalAgbInstancesInUse();
            *kDiagUpdateHB = *kDiagUpdateHB + 1;
        }
    }

    void DrawAll(ttyd::dispdrv::CameraId /*cam*/, void * /*user*/)
    {
        if (!g_initialized)
            return;

        const SharedBlock *block = GetValidBlock();
        if (block == nullptr)
            return;

        // Reset per-slot draw tracking so a stale value from a prior frame
        // can't masquerade as "we crashed on slot 0" when actually we just
        // never reached the draw loop body this frame.
        *kDiagDrawSlot = 0xFFFFFFFFu;
        *kDiagDrawPhase = 0;

        uint32_t drawn = 0;
        for (int i = 0; i < kMaxPeers; ++i)
        {
            const PeerSlot &peer = block->peers[i];
            const GhostSlot &slot = g_slots[i];

            if (!peer.active)
                continue;
            if (!PeerOnLocalMap(peer))
                continue;

            // Mark this slot as the one we're now working on, so a crash
            // freezes both fields and we know which slot died.
            *kDiagDrawSlot = static_cast<uint32_t>(i);
            *kDiagDrawPhase = 1; // setup / pose pick / matrix build

            // Pick the active pose; bail if it isn't allocated yet (we're
            // still in the lazy-alloc retry phase).
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

            // -----------------------------------------------------------------
            // Build the model matrix the same way marioDisp does. The key
            // step is the conditional Z-axis scale flip when wPlayerDirCur
            // is in the rear hemisphere - this is what gives Mario the
            // L/R mirroring. animPoseDraw (the simple variant) cannot do
            // this; we have to use animPoseDrawMtx with a built matrix.
            //
            // Sequence (mirrors marioDisp's standard branch):
            //   matA = scale by kGhostScale on all axes
            //   if (90 < dirCur <= 270): matA = matA * scale(1, 1, -1)
            //   matA = matA * rotateY((dirCur - cameraAngle) * deg2rad)
            //   matA = matA * translate(slot.renderX/Y/Z)
            //
            // We use slot.render* (lerped each frame in UpdateAll) instead
            // of peer.* (raw wire-format values). The lerping smooths
            // out network/IPC jitter so the ghost moves visually smooth
            // even when wire-format updates come in chunks.
            //
            // Note the Z-flip uses the RAW dirCur, while the Y rotation
            // uses dirCur MINUS the camera angle (unk_19c). This matches
            // marioDisp exactly: the flip is a world-space directional
            // decision, but the rotation is camera-frame-relative so the
            // sprite stays oriented correctly when the camera spins.
            // -----------------------------------------------------------------
            constexpr float kDeg2Rad = 0.01745329252f; // pi/180

            gc::mat3x4 matA;
            gc::mat3x4 matStep;

            // Step 1: base scale - per-axis from peer's scaleVec.
            // marioDisp at 0x80056C0C does:
            //   PSMTXScale(mp+0xC8 * 2.0, mp+0xCC * 2.0, mp+0xD0 * 2.0)
            // (or *1.2 in mini-Mario mode - flags1 bit 6).
            //
            // Idle: scaleVec is (1,1,1) so this is (2,2,2) - same as the
            // old uniform kGhostScale=2.0 path. Paper modes: scaleVec is
            // non-uniform - tube/plane meshes have aspect ratios that need
            // axis-specific scaling. Without per-axis scale, the ghost's
            // tube was squished/stretched.
            //
            // Defensive default: if scaleVec is all zeros (a freshly-zeroed
            // slot or pre-v12 peer), treat as (1,1,1) so the ghost doesn't
            // collapse to a point.
            float sx = peer.scaleX, sy = peer.scaleY, sz = peer.scaleZ;
            if (sx == 0.0f && sy == 0.0f && sz == 0.0f)
            {
                sx = sy = sz = 1.0f;
            }

            // -----------------------------------------------------------------
            // Per-motion scale fixups.
            //
            // Some paper modes need scale adjustments that we couldn't trace
            // back to a specific player-struct field (we replicated
            // marioDisp's known scale steps - base scaleVec, conditional
            // Z-flip, stretchY - but the visible result still didn't match
            // Mario in some modes). These are empirically-determined
            // corrections, applied based on motionId.
            //
            // Currently:
            //   0x16 (tube/roll): X scale * 0.75
            //
            // If a future mode shows similar issues, add it here. The
            // tunable block at 0x80002600..F (used during initial dev) is
            // gone; if you need to retune, rebuild with the empirical
            // tunables temporarily restored.
            //
            // Why hardcoded vs published: the value is a constant per mode,
            // shipping it on every frame would waste wire bandwidth, and
            // it's not derived from a player-struct field that could vary
            // per-frame.
            // -----------------------------------------------------------------
            float fixupX = 1.0f, fixupY = 1.0f, fixupZ = 1.0f;
            switch (peer.motionId)
            {
                case 0x16: // mot_roll - tube/paper-roll
                    fixupX = 0.75f;
                    break;
                default:
                    break;
            }

            gc::mtx::PSMTXScale(&matA, sx * kGhostScale * fixupX, sy * kGhostScale * fixupY, sz * kGhostScale * fixupZ);

            // Step 1.5: jabara-specific rotX-based Z-flip.
            //
            // marioDisp (line 0x80056C2C-0x80056CA8) has a SPECIAL pre-flip
            // that runs ONLY for motionId 0x14 (mot_jabara - hanging on a
            // pipe). The pipe-hang anim spins Mario around the X axis as he
            // goes 360 around the pipe; without this flip, his sprite
            // would show its back during the upside-down half. The flip
            // gating:
            //   - flags2 & 0x8 must be CLEAR (same as yaw-flip gate below)
            //   - motionId must equal 0x14
            //   - rotX (mp+0xBC, after revise360 wrap) must be in [90, 270]
            // When all three hold, scale Z by -1.
            //
            // This runs BEFORE the yaw-based flip below, so both can
            // potentially apply (cancelling each other out if both fire).
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

            // Step 2: conditional Z-flip for L/R mirror.
            //
            // marioDisp's flip (line 80056CAC-80056D58) is gated by a chain
            // of flags2 bits, not just by yaw hemisphere. The relevant gate
            // for our path is:
            //   flags2 & 0x8   (bit 28, "skip yaw-based flip")
            // When SET, marioDisp jumps straight past the flip block to the
            // pivot translate. Paper modes (tube, plane, etc) typically set
            // this bit because their meshes are built without the L/R
            // mirroring assumption that base Mario relies on.
            //
            // Without this gate, our ghost flips when local Mario doesn't,
            // producing visible model distortion / "snapping" as the ghost
            // crosses the 90/270 hemisphere boundary.
            //
            // Note: there's a second flip path in marioDisp (.L_80056D5C,
            // gated by flags2 bits 20-21) that we don't replicate. Those
            // bits typically aren't set in paper modes and idle Mario uses
            // the bit-28 gate, so we get the right behavior for everyday
            // motions. If we ever see flips happen wrong, that's the
            // next thing to investigate.
            if (!(peer.flags2 & 0x8))
            {
                float ang = slot.renderRotY;
                // revise360 in-line: wrap to [0, 360)
                while (ang < 0.0f) ang += 360.0f;
                while (ang >= 360.0f) ang -= 360.0f;
                if (ang > 90.0f && ang <= 270.0f)
                {
                    gc::mtx::PSMTXScale(&matStep, 1.0f, 1.0f, -1.0f);
                    gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
                }
            }

            // Step 3-pre: translate by -(rotPivot). marioDisp at 0x80056DA0
            // applies this BEFORE any of the X/Z/Y rotations so that the
            // rotations happen around the pivot instead of the model origin.
            // Idle: pivot is (0,0,0) - this is identity. Paper modes set
            // a real pivot (wing-tip for plane, axis for tube). The
            // matching +pivot translate after Y rotation reverses the
            // pre-translate.
            //
            // We test against !=0 not for performance (the matrix is
            // identity anyway when pivot is zero) but to skip the
            // PSMTXTrans/PSMTXConcat call entirely for typical play.
            const bool pivotActive = peer.rotPivotX != 0.0f || peer.rotPivotY != 0.0f || peer.rotPivotZ != 0.0f;
            if (pivotActive)
            {
                gc::mtx::PSMTXTrans(reinterpret_cast<gc::mtx34 *>(&matStep), -peer.rotPivotX, -peer.rotPivotY, -peer.rotPivotZ);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            // Step 3a: Z-axis rotation (roll, mp+0xC4 on local Mario).
            // Plane mode uses this to bank when turning; tube/roll mode uses
            // it for axis-rotation. marioDisp's non-plane branch applies Z
            // before X (line 80056E44). Idle motion has rotZ = 0 so this is
            // identity in normal play - it only matters in paper modes.
            //
            // We don't have motionId on the peer side to pick the
            // plane-specific X-then-Z order, but for the small angles
            // typical in plane gliding the visual difference is minor.
            if (peer.rotationZ != 0.0f)
            {
                gc::mtx::PSMTXRotRad(&matStep, /*axis=*/0x7A, peer.rotationZ * kDeg2Rad);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            // Step 3b: X-axis rotation (pitch, mp+0xBC on local Mario).
            // Plane uses this for nose up/down while gliding, tube for
            // spinning forward.
            if (peer.rotationX != 0.0f)
            {
                gc::mtx::PSMTXRotRad(&matStep, /*axis=*/0x78, peer.rotationX * kDeg2Rad);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            // Step 4: Y-axis rotation by (rotationY - cameraAngle).
            // marioDisp does revise360(wPlayerDirCur - unk_19c) and rotates
            // by that. The subtraction makes the model rotate in the
            // camera frame instead of world-absolute, so it stays oriented
            // correctly when the camera spins (rotating-world maps).
            //
            // cameraAngle is NOT lerped (it's read fresh from the local
            // game's camera state each frame anyway, no jitter source).
            float yawDeg = slot.renderRotY - peer.cameraAngle;
            while (yawDeg < 0.0f) yawDeg += 360.0f;
            while (yawDeg >= 360.0f) yawDeg -= 360.0f;
            gc::mtx::PSMTXRotRad(&matStep, /*axis=*/0x79, yawDeg * kDeg2Rad);
            gc::mtx::PSMTXConcat(&matStep, &matA, &matA);

            // Step 4-post: translate by +(rotPivot) to undo the pre-rotation
            // pivot translate. Pair with the -pivot at 3-pre. Idle: identity.
            if (pivotActive)
            {
                gc::mtx::PSMTXTrans(reinterpret_cast<gc::mtx34 *>(&matStep), peer.rotPivotX, peer.rotPivotY, peer.rotPivotZ);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            // Step 4b: stretchY - additional Y-axis scale from mp+0x130
            // (gated source-side by flags1 bit 0x01000000). marioDisp at
            // 0x80056F94-FA8 applies PSMTXScale(1, mp+0x130, 1) right
            // here (after pivot restore, before world translate). Source
            // pre-resolves the gate: 1.0 when flag is clear (no-op),
            // mp+0x130 when set. We always apply unconditionally - the
            // 1.0 case is functionally equivalent to skipping but keeps
            // the matrix-build code branch-free.
            if (peer.stretchY != 1.0f && peer.stretchY != 0.0f)
            {
                gc::mtx::PSMTXScale(&matStep, 1.0f, peer.stretchY, 1.0f);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            // Step 5: translate to world position (lerped). PSMTXTrans is
            // the one function in the API that takes 'mtx34*' (raw
            // float[3][4]) instead of 'mat3x4*' (struct wrapper). They're
            // layout-identical, so reinterpret_cast is safe.
            gc::mtx::PSMTXTrans(reinterpret_cast<gc::mtx34 *>(&matStep), slot.renderX, slot.renderY, slot.renderZ);
            gc::mtx::PSMTXConcat(&matStep, &matA, &matA);

            // Draw with two stages, like marioDisp does. Drawing only one
            // stage submits an incomplete model; the GPU shows nothing or
            // partial content. animPoseDrawMtx's last two args (rotY/scale)
            // are zero/one because the matrix already encodes everything.
            // Stage 2 gated by feature bit 1 - drop it to halve per-ghost
            // GPU command volume during bisection.
            if (FeatOn(kFeatDrawStage2))
            {
                *kDiagDrawPhase = 2;
                ttyd::animdrv::animPoseDrawMtx(poseId,
                                               &matA,
                                               /*stage=*/2,
                                               /*rotY=*/0.0f,
                                               /*scale=*/1.0f);
            }
            *kDiagDrawPhase = 3; // entering animPoseDrawMtx stage 1
            ttyd::animdrv::animPoseDrawMtx(poseId,
                                           &matA,
                                           /*stage=*/1,
                                           /*rotY=*/0.0f,
                                           /*scale=*/1.0f);
            *kDiagDrawPhase = 4; // both stages complete
            ++drawn;
        }

        // Reset phase to idle so the lingering value reflects "outside loop"
        // rather than the last slot's phase=4.
        *kDiagDrawPhase = 0;

        // Diagnostic: how many ghosts got drawn this frame, and tick heartbeat.
        *kDiagDrawCalls = drawn;
        *kDiagDrawHB = *kDiagDrawHB + 1;
    }

    // -------------------------------------------------------------------------
    // Name-tag rendering.
    //
    // Approach: project the ghost's world position to 2D screen coordinates,
    // then render with the 2D FontDrawString. This is the same path the mod's
    // existing error-text overlay uses (errorHandling.cpp -> OWR::DrawString
    // -> FontDrawString) and the only proven-to-work path for arbitrary text
    // in this codebase.
    //
    // Why not 3D-matrix-driven font rendering: FontDrawStringMtx and friends
    // technically take a world matrix and concat with the camera view, but in
    // practice they only render visibly when called from a context where GX
    // state has been set up by the matching engine subsystem (winMgr for the
    // shop, the dialogue box engine for messages, etc). Calling them from a
    // raw dispdrv callback in k3d/k2d either silently no-ops or produces
    // text at the wrong screen position. The 2D path is rock solid.
    //
    // Recommended registration: kDebug3d, same camera as mod's draw() callback
    // in mod.cpp, with whatever order. Example:
    //   ttyd::dispdrv::dispEntry(CameraId::kDebug3d, 1, /*order=*/100.0f,
    //                            DrawNameTagsAll, nullptr);
    // -------------------------------------------------------------------------
    void DrawNameTagsAll(ttyd::dispdrv::CameraId /*cam*/, void * /*user*/)
    {
        if (!g_initialized)
            return;

        // Feature bit 3 - bypass entire name-tag pass for bisection.
        if (!FeatOn(kFeatNameTags))
            return;

        const SharedBlock *block = GetValidBlock();
        if (block == nullptr)
            return;

        // Get the 3D world camera (k3d = id 4 in the dispdrv enum).
        // The camera's projection matrix lives at offset 0x15C (mat4x4).
        // marioPreDisp uses this exact pattern at 0x80057A20.
        // If the camera lookup fails (shouldn't, but defensive), skip frame.
        void *camPtr = camGetPtr(/*k3d=*/4);
        if (camPtr == nullptr)
            return;

        // The projection matrix is at camPtr + 0x15C - it's a mat4x4 (16 floats)
        // that transforms CAMERA-space (x,y,z,1) to clip-space (x,y,z,w).
        // CRITICAL: this is camera-relative, not world-relative. We need
        // view * projection * world, not just projection * world.
        gc::mat4x4 *projMtx = reinterpret_cast<gc::mat4x4 *>(reinterpret_cast<char *>(camPtr) + 0x15C);

        // The view matrix is at camPtr + 0x11C - it's a mat3x4 (12 floats),
        // verified from JUTFont_DrawStart which loads it as the GPU pos
        // matrix and from FontDrawStringMtx which concats user model with it.
        gc::mat3x4 *viewMtx = reinterpret_cast<gc::mat3x4 *>(reinterpret_cast<char *>(camPtr) + 0x11C);

        // Initialize font state ONCE per frame (not per peer). This matches
        // OWR::DrawString's pattern - state is global and persistent so we
        // don't need to re-init per draw call. We use FontDrawStart (no
        // _alpha) because that's what the proven OWR/error-message path
        // uses.
        ttyd::fontmgr::FontDrawStart();
        ttyd::fontmgr::FontDrawEdge(); // black outline for readability

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

            // Step 1: world -> camera space (apply view matrix).
            // PSMTXMultVec does mat3x4 * vec3 -> vec3 (treats input as
            // homogeneous with implicit W=1, no perspective divide since
            // there's no W in the output for a 3x4 matrix).
            //
            // We use slot.render* (lerped) instead of peer.* (raw) so the
            // text follows the same smoothed position as the model. If we
            // used the raw peer position here, the text would jitter even
            // though the model is smooth - they'd visibly desync.
            //
            // Y offset puts the text above the ghost's head rather than
            // at its feet. Mario is roughly 30 world units tall.
            gc::vec3 worldPos = {slot.renderX, slot.renderY + kNameTagWorldYOffset, slot.renderZ};
            gc::vec3 camPos = {0.0f, 0.0f, 0.0f};
            gc::mtx::PSMTXMultVec(viewMtx, &worldPos, &camPos);

            // Step 2: camera space -> NDC (apply projection matrix).
            // PSMTX44MultVec does the perspective divide internally
            // (verified from disasm at 0x8029EB08), so output is already NDC.
            gc::vec3 ndcPos = {0.0f, 0.0f, 0.0f};
            gc::mtx::PSMTX44MultVec(projMtx, &camPos, &ndcPos);

            const float ndcX = ndcPos.x;
            const float ndcY = ndcPos.y;
            const float ndcZ = ndcPos.z; // depth in NDC, [-1, +1] when on-screen

            // Step 2: cull behind-camera peers via NDC Z.
            // In TTYD's projection, points in front of the near plane have
            // negative NDC Z (typical OpenGL-style: far is +1, near is -1,
            // behind-camera goes outside [-1, +1] usually toward +inf or
            // becomes invalid). Conservative cull: skip if NDC Z is
            // out of plausible visible range.
            if (ndcZ < -1.5f || ndcZ > 1.5f)
            {
                continue;
            }

            // Step 3: cull off-screen on X/Y (with margin for partial
            // visibility at edges).
            if (ndcX < -1.5f || ndcX > 1.5f || ndcY < -1.5f || ndcY > 1.5f)
            {
                continue;
            }

            // Step 4: map NDC to TTYD's centered screen coordinate system.
            float screenX = ndcX * kNameTagScreenScaleX;
            float screenY = ndcY * kNameTagScreenScaleY;

            // FontDrawString anchors text at its top-left, so the visual
            // text would appear shifted right of where we want. Center
            // it horizontally by subtracting half the message width
            // (scaled). FontGetMessageWidth returns width in pixels at
            // scale 1.0, so we multiply by our scale.
            const uint16_t textWidth = ttyd::fontmgr::FontGetMessageWidth(peer.slotName);
            screenX -= (static_cast<float>(textWidth) * kNameTagFontScale) * 0.5f;

            // Step 5: set per-peer color (text is per-ghost tinted) then
            // draw. FontDrawColor takes a uint8_t[4] but reads as a packed
            // uint32. Pack RGBA in big-endian byte order so the bytes map
            // R,G,B,A from msb to lsb.
            const uint32_t packed = (static_cast<uint32_t>(peer.r) << 24) | (static_cast<uint32_t>(peer.g) << 16) |
                                    (static_cast<uint32_t>(peer.b) << 8) | 0xFFu;
            ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(const_cast<uint32_t *>(&packed)));

            ttyd::fontmgr::FontDrawString(screenX, screenY, peer.slotName);
        }
    }
} // namespace mod::ghosts