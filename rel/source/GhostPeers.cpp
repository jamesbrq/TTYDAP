#include "GhostPeers.h"

#include "ttyd/animdrv.h"
#include "ttyd/dispdrv.h"
#include "ttyd/fontmgr.h"
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

        // ===== TEMPORARY DIAGNOSTICS =====
        // These let the AP client (or Dolphin's memory editor) read back what
        // the mod is doing without needing OSReport / log infrastructure. Each
        // gets bumped at a specific point in the lifecycle. Read them via
        // memory editor at the addresses below.
        //
        // The block sits at 0x80002400, past our 432-byte peer block which
        // ends at 0x800021B0, so we don't collide with the data path.
        //
        //   0x80002400  init_count        bumped once when Init() runs
        //   0x80002404  update_count      bumped each frame UpdateAll runs
        //                                 with a valid block (magic OK)
        //   0x80002408  draw_count        bumped each frame DrawAll runs
        //                                 with a valid block (magic OK)
        //   0x8000240C  pose_alloc_ok     bumped each successful animPoseEntry
        //   0x80002410  pose_alloc_fail   bumped each failed animPoseEntry
        //   0x80002414  draw_calls        bumped once per peer per frame
        //                                  (Note: each bump produces 2 stages
        //                                  of animPoseDrawMtx, not 1 simple)
        //   0x80002418  (was scale_tunable - now unused, scale is hardcoded)
        //   0x8000241C  setanim_calls     bumped each animPoseSetAnim call
        //   0x80002420  last_rot_y        float; last rotationY received
        //   0x80002424  posemain_calls    bumped each animPoseMain call
        //   0x80002428  fwd_poseId_slot0  forward pose ID allocated for slot 0
        //   0x8000242C  rear_poseId_slot0 rear pose ID allocated for slot 0
        //   0x80002430  lastAnimFwd_slot0 16 bytes; current cached anim
        //                                 in forward branch for slot 0
        //   0x80002440  nametag_entered   bumped each frame DrawNameTagsAll
        //                                 is invoked at all (gates on g_init
        //                                 and block validity passing)
        //   0x80002444  nametag_drawcalls bumped per peer that actually calls
        //                                 FontDrawStringVecPitch (passed all
        //                                 gates: active, on-map, non-empty name)
        //
        // Remove this block entirely once the test passes.
        // =================================
        volatile uint32_t *kDiagInit = reinterpret_cast<volatile uint32_t *>(0x80002400);
        volatile uint32_t *kDiagUpdate = reinterpret_cast<volatile uint32_t *>(0x80002404);
        volatile uint32_t *kDiagDraw = reinterpret_cast<volatile uint32_t *>(0x80002408);
        volatile uint32_t *kDiagPoseOk = reinterpret_cast<volatile uint32_t *>(0x8000240C);
        volatile uint32_t *kDiagPoseFail = reinterpret_cast<volatile uint32_t *>(0x80002410);
        volatile uint32_t *kDiagDrawCall = reinterpret_cast<volatile uint32_t *>(0x80002414);
        volatile uint32_t *kDiagSetAnim = reinterpret_cast<volatile uint32_t *>(0x8000241C);
        volatile float *kDiagLastRotY = reinterpret_cast<volatile float *>(0x80002420);
        volatile uint32_t *kDiagPoseMain = reinterpret_cast<volatile uint32_t *>(0x80002424);
        volatile int32_t *kDiagFwdPoseId = reinterpret_cast<volatile int32_t *>(0x80002428);
        volatile int32_t *kDiagRearPoseId = reinterpret_cast<volatile int32_t *>(0x8000242C);
        volatile char *kDiagLastAnimFwd = reinterpret_cast<volatile char *>(0x80002430);
        volatile uint32_t *kDiagNameEntered = reinterpret_cast<volatile uint32_t *>(0x80002440);
        volatile uint32_t *kDiagNameDrawCall = reinterpret_cast<volatile uint32_t *>(0x80002444);
        // Projection diagnostics (slot 0 only, last drawn peer):
        //   0x80002448  screenX (float) - final 2D X coord passed to FontDrawString
        //   0x8000244C  screenY (float) - final 2D Y coord passed to FontDrawString
        //   0x80002450  ndcZ    (float) - NDC Z component after PSMTX44MultVec
        //                                 (was clipW; PSMTX44MultVec already does
        //                                 perspective divide so we get NDC, not
        //                                 clip space).
        //   0x80002454  cull_code (u32) - 0=drew, 1=z out of range, 2=xy out of range
        volatile float *kDiagScreenX = reinterpret_cast<volatile float *>(0x80002448);
        volatile float *kDiagScreenY = reinterpret_cast<volatile float *>(0x8000244C);
        volatile float *kDiagClipW = reinterpret_cast<volatile float *>(0x80002450);
        volatile uint32_t *kDiagCullCode = reinterpret_cast<volatile uint32_t *>(0x80002454);

        // Live-tunable name-tag positioning constants.
        // Edit these at runtime with Dolphin's memory editor (write a float
        // at the address shown). Changes take effect immediately, no rebuild.
        //
        //   0x800024D0  scaleX  (float) - multiplier for NDC->screen X.
        //                                  Smaller = text closer to center.
        //                                  Larger  = text further from center.
        //   0x800024D4  scaleY  (float) - multiplier for NDC->screen Y.
        //   0x800024D8  worldYOffset (float) - world units above ghost's
        //                                       feet to anchor text. Default 50.
        //                                       Larger = text higher above head.
        //   0x800024DC  fontScale (float) - text size. Default 0.5.
        //                                    Smaller = smaller text.
        //
        // Defaults are written on first frame if the address still reads
        // as zero (assumes a fresh boot zeroed the memory). To force a
        // reset to defaults, write 0.0 to the slot you want defaulted.
        volatile float *kTuneScaleX = reinterpret_cast<volatile float *>(0x800024D0);
        volatile float *kTuneScaleY = reinterpret_cast<volatile float *>(0x800024D4);
        volatile float *kTuneWorldYOffset = reinterpret_cast<volatile float *>(0x800024D8);
        volatile float *kTuneFontScale = reinterpret_cast<volatile float *>(0x800024DC);

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

        // Allocate forward, rear, and effects poses for this slot if not
        // already. Each pose is from a different AGB. Group 2 matches what
        // marioPoseInit uses.
        void EnsurePosesAllocated(GhostSlot &slot)
        {
            const bool isSlot0 = (&slot == &g_slots[0]);

            if (!slot.forwardAllocated)
            {
                int32_t id = ttyd::animdrv::animPoseEntry(kAgbForward, kPoseGroup);
                if (id < 0)
                {
                    *kDiagPoseFail = *kDiagPoseFail + 1;
                }
                else
                {
                    *kDiagPoseOk = *kDiagPoseOk + 1;
                    slot.forwardPoseId = id;
                    slot.forwardAllocated = true;
                    slot.lastAnimForward[0] = '\0';
                    if (isSlot0)
                        *kDiagFwdPoseId = id;
                    ttyd::animdrv::animPoseSetAnim(id, "M_S_1", /*forceReset=*/1);
                }
            }
            if (!slot.rearAllocated)
            {
                int32_t id = ttyd::animdrv::animPoseEntry(kAgbRear, kPoseGroup);
                if (id < 0)
                {
                    *kDiagPoseFail = *kDiagPoseFail + 1;
                }
                else
                {
                    *kDiagPoseOk = *kDiagPoseOk + 1;
                    slot.rearPoseId = id;
                    slot.rearAllocated = true;
                    slot.lastAnimRear[0] = '\0';
                    if (isSlot0)
                        *kDiagRearPoseId = id;
                    ttyd::animdrv::animPoseSetAnim(id, "M_S_1R", /*forceReset=*/1);
                }
            }
            if (!slot.effectsAllocated)
            {
                int32_t id = ttyd::animdrv::animPoseEntry(kAgbEffects, kPoseGroup);
                if (id < 0)
                {
                    *kDiagPoseFail = *kDiagPoseFail + 1;
                }
                else
                {
                    *kDiagPoseOk = *kDiagPoseOk + 1;
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
                *kDiagSetAnim = *kDiagSetAnim + 1;
            }

            // Diagnostic: copy slot 0's active cache to memory.
            if (&slot == &g_slots[0])
            {
                for (size_t i = 0; i < 16; ++i)
                {
                    kDiagLastAnimFwd[i] = cache[i];
                }
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
            // Lerp fields - explicit init.
            s.renderX = 0.0f;
            s.renderY = 0.0f;
            s.renderZ = 0.0f;
            s.renderRotY = 0.0f;
            s.renderInitialized = false;
        }
        g_initialized = true;
        *kDiagInit = *kDiagInit + 1;
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

        *kDiagUpdate = *kDiagUpdate + 1;

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

            // Smoothing: lerp render position toward the wire-format peer
            // position each frame. Snap on first frame after activation
            // so we don't visibly slide across the map from a stale state.
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
                slot.renderRotY = LerpAngleDeg(slot.renderRotY, peer.rotationY, kLerpAlpha);
            }

            // Tick the animation forward on the active pose only - off-map
            // ghosts skip this since they aren't being drawn.
            if (PeerOnLocalMap(peer))
            {
                if (slot.activePose == 2 && slot.effectsAllocated)
                {
                    ttyd::animdrv::animPoseMain(slot.effectsPoseId);
                    *kDiagPoseMain = *kDiagPoseMain + 1;
                }
                else if (slot.activePose == 1 && slot.rearAllocated)
                {
                    ttyd::animdrv::animPoseMain(slot.rearPoseId);
                    *kDiagPoseMain = *kDiagPoseMain + 1;
                }
                else if (slot.forwardAllocated)
                {
                    ttyd::animdrv::animPoseMain(slot.forwardPoseId);
                    *kDiagPoseMain = *kDiagPoseMain + 1;
                }
            }
        }
    }

    void DrawAll(ttyd::dispdrv::CameraId /*cam*/, void * /*user*/)
    {
        if (!g_initialized)
            return;

        const SharedBlock *block = GetValidBlock();
        if (block == nullptr)
            return;

        *kDiagDraw = *kDiagDraw + 1;

        for (int i = 0; i < kMaxPeers; ++i)
        {
            const PeerSlot &peer = block->peers[i];
            const GhostSlot &slot = g_slots[i];

            if (!peer.active)
                continue;
            if (!PeerOnLocalMap(peer))
                continue;

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

            *kDiagDrawCall = *kDiagDrawCall + 1;
            *kDiagLastRotY = slot.renderRotY;

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

            // Step 1: base scale
            gc::mtx::PSMTXScale(&matA, kGhostScale, kGhostScale, kGhostScale);

            // Step 2: conditional Z-flip for L/R mirror
            // marioDisp does: revise360(wPlayerDirCur), flip if (90, 270]
            // This uses the RAW (lerped) rotationY (no camera adjustment).
            float ang = slot.renderRotY;
            // revise360 in-line: wrap to [0, 360)
            while (ang < 0.0f) ang += 360.0f;
            while (ang >= 360.0f) ang -= 360.0f;
            if (ang > 90.0f && ang <= 270.0f)
            {
                gc::mtx::PSMTXScale(&matStep, 1.0f, 1.0f, -1.0f);
                gc::mtx::PSMTXConcat(&matStep, &matA, &matA);
            }

            // Step 3: Y-axis rotation by (rotationY - cameraAngle).
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

            // Step 4: translate to world position (lerped). PSMTXTrans is
            // the one function in the API that takes 'mtx34*' (raw
            // float[3][4]) instead of 'mat3x4*' (struct wrapper). They're
            // layout-identical, so reinterpret_cast is safe.
            gc::mtx::PSMTXTrans(reinterpret_cast<gc::mtx34 *>(&matStep), slot.renderX, slot.renderY, slot.renderZ);
            gc::mtx::PSMTXConcat(&matStep, &matA, &matA);

            // Draw with two stages, like marioDisp does. Drawing only one
            // stage submits an incomplete model; the GPU shows nothing or
            // partial content. animPoseDrawMtx's last two args (rotY/scale)
            // are zero/one because the matrix already encodes everything.
            ttyd::animdrv::animPoseDrawMtx(poseId,
                                           &matA,
                                           /*stage=*/2,
                                           /*rotY=*/0.0f,
                                           /*scale=*/1.0f);
            ttyd::animdrv::animPoseDrawMtx(poseId,
                                           &matA,
                                           /*stage=*/1,
                                           /*rotY=*/0.0f,
                                           /*scale=*/1.0f);
        }
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

        const SharedBlock *block = GetValidBlock();
        if (block == nullptr)
            return;

        // Diagnostic: function entered, gates passed.
        *kDiagNameEntered = *kDiagNameEntered + 1;

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

        // DEBUG: dump both matrices.
        //   0x80002460-0x8000249F (64 bytes): mat4x4 projection at cam+0x15C
        //   0x800024A0-0x800024CF (48 bytes): mat3x4 view at cam+0x11C
        {
            volatile float *projDump = reinterpret_cast<volatile float *>(0x80002460);
            const float *projSrc = reinterpret_cast<const float *>(projMtx);
            for (int j = 0; j < 16; ++j) projDump[j] = projSrc[j];

            volatile float *viewDump = reinterpret_cast<volatile float *>(0x800024A0);
            const float *viewSrc = reinterpret_cast<const float *>(viewMtx);
            for (int j = 0; j < 12; ++j) viewDump[j] = viewSrc[j];
        }

        // Initialize font state ONCE per frame (not per peer). This matches
        // OWR::DrawString's pattern - state is global and persistent so we
        // don't need to re-init per draw call. We use FontDrawStart (no
        // _alpha) because that's what the proven OWR/error-message path
        // uses.
        ttyd::fontmgr::FontDrawStart();
        ttyd::fontmgr::FontDrawEdge(); // black outline for readability

        // Read live-tunable values. If zero (fresh memory or user reset),
        // fall back to defaults. Use locals so the read is consistent
        // across the whole frame even if user pokes mid-frame.
        float kNameTagYOffset = *kTuneWorldYOffset;
        float kNameTagScale = *kTuneFontScale;
        if (kNameTagYOffset == 0.0f)
            kNameTagYOffset = 50.0f;
        if (kNameTagScale == 0.0f)
            kNameTagScale = 0.5f;

        ttyd::fontmgr::FontDrawScale(kNameTagScale);

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
            gc::vec3 worldPos = {slot.renderX, slot.renderY + kNameTagYOffset, slot.renderZ};
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

            // Diagnostics: capture slot 0 every frame so we can read back
            // what the projection actually produced.
            if (i == 0)
            {
                *kDiagClipW = ndcZ; // repurposed - now stores NDC Z
            }

            // Step 2: cull behind-camera peers via NDC Z.
            // In TTYD's projection, points in front of the near plane have
            // negative NDC Z (typical OpenGL-style: far is +1, near is -1,
            // behind-camera goes outside [-1, +1] usually toward +inf or
            // becomes invalid). Conservative cull: skip if NDC Z is
            // out of plausible visible range.
            if (ndcZ < -1.5f || ndcZ > 1.5f)
            {
                if (i == 0)
                    *kDiagCullCode = 1;
                continue;
            }

            // Step 3: cull off-screen on X/Y (with margin for partial
            // visibility at edges).
            if (ndcX < -1.5f || ndcX > 1.5f || ndcY < -1.5f || ndcY > 1.5f)
            {
                if (i == 0)
                    *kDiagCullCode = 2;
                continue;
            }

            // Step 4: map NDC to TTYD's centered screen coordinate system.
            // Multipliers are live-tunable - read fresh per peer so changes
            // take effect immediately. Defaults applied if memory is zero.
            float scaleX = *kTuneScaleX;
            float scaleY = *kTuneScaleY;
            if (scaleX == 0.0f)
                scaleX = 280.0f; // empirically tuned
            if (scaleY == 0.0f)
                scaleY = 240.0f;

            float screenX = ndcX * scaleX;
            float screenY = ndcY * scaleY;

            // FontDrawString anchors text at its top-left, so the visual
            // text would appear shifted right of where we want. Center
            // it horizontally by subtracting half the message width
            // (scaled). FontGetMessageWidth returns width in pixels at
            // scale 1.0, so we multiply by our scale.
            const uint16_t textWidth = ttyd::fontmgr::FontGetMessageWidth(peer.slotName);
            screenX -= (static_cast<float>(textWidth) * kNameTagScale) * 0.5f;

            if (i == 0)
            {
                *kDiagScreenX = screenX;
                *kDiagScreenY = screenY;
                *kDiagCullCode = 0; // drew this frame
            }

            // Step 5: set per-peer color (text is per-ghost tinted) then
            // draw. FontDrawColor takes a uint8_t[4] but reads as a packed
            // uint32. Pack RGBA in big-endian byte order so the bytes map
            // R,G,B,A from msb to lsb.
            const uint32_t packed = (static_cast<uint32_t>(peer.r) << 24) | (static_cast<uint32_t>(peer.g) << 16) |
                                    (static_cast<uint32_t>(peer.b) << 8) | 0xFFu;
            ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(const_cast<uint32_t *>(&packed)));

            ttyd::fontmgr::FontDrawString(screenX, screenY, peer.slotName);

            // Diagnostic: this peer's text draw was issued.
            *kDiagNameDrawCall = *kDiagNameDrawCall + 1;
        }
    }
} // namespace mod::ghosts