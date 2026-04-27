#pragma once

// =============================================================================
// GhostPeers shared memory contract
// =============================================================================
//
// This header is the SINGLE SOURCE OF TRUTH for the layout of the ghost peer
// shared memory block. Include it in BOTH:
//   * The TTYD mod (custom.rel) - reads the block each frame
//   * The Archipelago client / external tool - writes the block at runtime
//
// If the AP client is in Python, mirror this struct using `struct` module
// format codes - layout is documented inline below.
//
// Memory layout
// -------------
// The block lives at a fixed address in the GameCube's main RAM, in the
// low scratch region just above the existing AP item queue at
// 0x80000FFC / 0x80001000.
//
// If you ever need to change the address, change kBlockAddress in ONE place
// here and rebuild both sides.
//
// Freshness model
// ---------------
// The AP client overwrites the entire block continuously (every poll). The
// mod reads whatever is currently there each frame. There is no heartbeat
// or tick counter - this matches the existing item-queue convention.
//
// Consequence: if the AP client crashes mid-session, ghosts freeze in
// place at their last known positions. They do not auto-disappear. To make
// a peer disappear, the client writes active=0 in that slot.
//
// To distinguish "AP client connected" from "garbage memory at boot", the
// header has:
//   * magic - must equal kMagic, otherwise the mod treats the entire block
//     as absent and renders nothing. Cheap insurance against rendering
//     ghosts from uninitialized RAM on the first frame after boot.
//   * version - bump if the layout changes incompatibly. Mod refuses to
//     read if version != kVersion.

#include "gc/types.h"
#include "ttyd/dispdrv.h"
#include <cstdint>

namespace mod::ghosts
{
    // -------------------------------------------------------------------------
    // Tunables
    // -------------------------------------------------------------------------
    constexpr int kMaxPeers = 32;

    // The fixed RAM address the AP client writes to. Must match on both sides.
    // Lives in low main-RAM scratch, just above the existing AP item queue
    // at 0x80000FFC / 0x80001000.
    // v17: moved from 0x80002000 to 0x80001800 to free up the 0x80002000-
    //      0x80002800 region for other purposes. Block now occupies
    //      0x80001800-0x80002E10 (32 peers).
    constexpr uintptr_t kBlockAddress = 0x80001800;

    // Magic + version. Bump kVersion if the on-wire layout below changes.
    // The magic byte distinguishes "AP client has written" from "uninitialized
    // garbage RAM at boot" - without it the mod would try to render ghosts
    // from random memory contents on the first frame.
    constexpr uint32_t kMagic = 0x47484F53; // 'GHOS'
    constexpr uint32_t kVersion = 17;       // v17: block address moved 0x80002000 -> 0x80001800. PeerSlot layout unchanged.

    // -------------------------------------------------------------------------
    // On-the-wire types. PACKED. Both sides must agree byte-for-byte.
    // -------------------------------------------------------------------------

    // Bit masks for the flags2 field. Mirrors the game's marioPreDisp logic
    // exactly: certain bits of Player.flags2 (offset 0x4) decide which of
    // Mario's three poses to render. We mirror it 1:1 so the ghost picks
    // pose the same way Mario himself does.
    //
    //   bit 0x10000000 set -> effects pose (e_mario - paper, special states)
    //   bit 0x80000000 set -> rear-facing pose (a_mario_r)
    //   else                 -> forward pose (a_mario)
    //
    // Tested against marioPreDisp decomp - this is exact game logic.
    constexpr uint32_t kFlags2EffectsMask = 0x10000000;
    constexpr uint32_t kFlags2RearMask = 0x80000000;

    // Bit masks for the flags3 field. These are SET BY marioMakeDispDir
    // based on which side Mario is rotating toward. The renderer (matrix
    // construction inside marioDisp) consumes these bits to apply a 180-deg
    // Y-axis rotation when 'right' is set, so the same forward sprite shows
    // mirrored. Without applying this rotation, the ghost shows
    // unmirrored sprites - looks correct facing left, broken facing right.
    //
    //   bit 0x200 set -> facing LEFT  (no rotation; sprite as-is)
    //   bit 0x400 set -> facing RIGHT (apply 180-deg Y rotation to flip)
    //   neither set    -> directly facing front or back (no rotation)
    constexpr uint32_t kFlags3LeftMask = 0x200;
    constexpr uint32_t kFlags3RightMask = 0x400;

    // One peer's state. 176 bytes packed (was 172 in v14).
    //
    // Python equivalent (for reference, '>' = big-endian / network):
    //   struct.pack(">B 15s 16s fff f BBBB I I H 2x f 16s 32s 16s ff fff fff f H 2x f",
    //       active, mapName, animName,
    //       x, y, z, rotY,
    //       r, g, b, a,
    //       flags2, flags3, motionTimer, cameraAngle,
    //       slotName, paperAgbName, paperAnimName,
    //       rotX, rotZ,
    //       rotPivotX, rotPivotY, rotPivotZ,
    //       scaleX, scaleY, scaleZ,
    //       stretchY,
    //       motionId,
    //       paperLocalTime)
    struct PeerSlot
    {
        uint8_t active;         // 0 = empty slot (mod ignores), 1 = render
        char mapName[15];       // current map of this peer; mod gates on this
        char animName[16];      // animation name e.g. "M_W_1", "M_R_1", "M_S_1"
        float x, y, z;          // world position (already-summed render pos)
        float rotationY;        // facing yaw, degrees (wPlayerDirCur)
        uint8_t r, g, b, a;     // tint; alpha = ghost translucency
        uint32_t flags2;        // mirror of Player.flags2 - drives pose pick
        uint32_t flags3;        // mirror of Player.flags3 - drives L/R yaw
        uint16_t motionTimer;   // mirror of Player.unk_28 - drives anim playhead
        uint8_t _pad[2];        // pad for float alignment
        float cameraAngle;      // mirror of Player.unk_19c - camera-yaw offset
        char slotName[16];      // player's display name (NUL-padded). Rendered
                                // as a name tag above the ghost. Empty string
                                // means "no name tag".
        char paperAgbName[32];  // paper-mode AGB name. Widened from 16 to 32
                                // in v9 because TTYD's actual AGB resource
                                // names exceed 16 chars - e.g. paper-plane's
                                // AGB is "slit_mdel3_pPlane" (17+ chars).
                                // Read from Mario's forward pose group
                                // (mod-side helper writes to 0x80003B20).
                                // Empty string = no paper mode active.
        char paperAnimName[16]; // paper-mode anim name (mp+0x1C dereferenced).
                                // Empty = no paper anim. With non-empty
                                // paperAgbName, mod allocates paper pose
                                // and applies anim.
        float rotationX;        // pitch, degrees (mp+0xBC). Plane mode uses
                                // this for nose-up/down while gliding;
                                // tube/roll mode uses it for spinning.
                                // marioDisp at 0x80056DE4 reads it via
                                // PSMTXRotRad with axis 'x' (0x78).
        float rotationZ;        // roll, degrees (mp+0xC4). Plane mode uses
                                // this for banking when turning; tube/roll
                                // uses it for axis-rotation.
                                // marioDisp at 0x80056E0C reads it via
                                // PSMTXRotRad with axis 'z' (0x7A).
        float rotPivotX;        // rotation pivot X (mp+0xB0). marioDisp at
        float rotPivotY;        // 0x80056DA0 translates by -(B0,B4,B8)
        float rotPivotZ;        // BEFORE the X/Y/Z rotations, then
                                // translates by +(B0,B4,B8) AFTER, so
                                // rotations happen around this pivot
                                // instead of the model origin. This is
                                // needed for paper modes (plane wing-tip,
                                // tube center) - without it rotations spin
                                // the model around its origin and produce
                                // visibly wrong results.
                                // Idle: typically (0,0,0).
        float scaleX;           // per-axis scale (mp+0xC8). marioDisp at
        float scaleY;           // 0x80056C0C reads (mp+0xC8, mp+0xCC,
        float scaleZ;           // mp+0xD0) and multiplies each by 2.0
                                // (or 1.2 for mini-Mario mode flag) for
                                // the base scale. Paper modes set these
                                // to non-(1,1,1) to size the paper meshes
                                // correctly - without mirroring, the
                                // ghost's tube/plane is squished/stretched.
                                // Idle: usually (1,1,1).
        float stretchY;         // additional Y-axis scale, gated source-
                                // side. marioDisp at 0x80056F8C tests
                                // flags1 bit 0x01000000 - if set, applies
                                // PSMTXScale(1, mp+0x130, 1) AFTER all
                                // rotations and pivot translates. If clear,
                                // this scale step is skipped entirely.
                                // We pre-resolve: source publishes either
                                // mp+0x130 (when flag set) or 1.0 (when
                                // clear) so the mod just unconditionally
                                // applies it without a branch.
                                // Without this, paper modes that compress
                                // Y (like tube) leave the ghost taller/
                                // oblong vs the local Mario.
        uint16_t motionId;      // mp+0x2E - currentMotionId. From
                                // marioMotTbl: 0x15=slit, 0x16=roll/tube,
                                // 0x18=plane, 0x19=ship, 0x1C=vivian,
                                // 0x1D=dokan, etc. The mod uses this to
                                // apply per-motion fixups that aren't
                                // captured by the generic scale/rotation
                                // fields - e.g. tube needs an extra X
                                // scale of 0.75 that we couldn't trace
                                // back to a specific player-struct field.
        uint8_t _pad2[2];       // pad to 4-byte align next field
        float paperLocalTime;   // animPoseSetLocalTime override.
                                //
                                // Background: some "held" anims don't
                                // progress visually unless someone
                                // manually advances their playhead. Two
                                // known cases:
                                //
                                // 1. mot_hammer2 (motion 0x13) +
                                //    paper anim "P_H_1A" (held hammer
                                //    spin charge): line 80097B94 does
                                //      animPoseSetLocalTime(activePoseId,
                                //                          mp+0x2C8 / 6.0)
                                //    where mp+0x2C8 is the accumulated
                                //    spin charge.
                                //
                                // 2. mot_jabara (motion 0x14) + regular
                                //    anim "M_W_6" (held-shimmy on pipe
                                //    after the swing): line 80097738 does
                                //      animPoseSetLocalTime(activePoseId,
                                //                          (float)mp+0x2D3)
                                //    where mp+0x2D3 is a byte that
                                //    counts up 0..8 then clamps. Without
                                //    the override, the ghost keeps
                                //    cycling M_W_6 instead of holding.
                                //
                                // Sentinel: -1.0 means "no override".
                                // We use -1.0 (not 0.0) because 0.0 is
                                // a legitimate override value (e.g.
                                // mp+0x2D3 starts at 0 on first frame
                                // of jabara held-rest).
    } __attribute__((__packed__));

    static_assert(sizeof(PeerSlot) ==
                      1 + 15 + 16 + 4 * 4 + 4 + 4 + 4 + 2 + 2 + 4 + 16 + 32 + 16 + 4 + 4 + 4 * 3 + 4 * 3 + 4 + 2 + 2 + 4,
                  "PeerSlot layout drifted");
    static_assert(sizeof(PeerSlot) == 176, "PeerSlot must be exactly 176 bytes");

    // The whole block. Header + array of slots.
    // Header is 8 bytes (magic + version) padded to 16 for alignment.
    struct SharedBlock
    {
        uint32_t magic;     // == kMagic when client is connected
        uint32_t version;   // == kVersion
        uint32_t reserved0; // pad; future use (heartbeat, flags, etc.)
        uint32_t reserved1; // pad
        PeerSlot peers[kMaxPeers];
    } __attribute__((__packed__));

    static_assert(sizeof(SharedBlock) == 16 + kMaxPeers * sizeof(PeerSlot), "SharedBlock layout drifted");

    // Convenience accessor used inside the mod. Don't call from external code.
    inline SharedBlock *GetBlock()
    {
        return reinterpret_cast<SharedBlock *>(kBlockAddress);
    }

    // -------------------------------------------------------------------------
    // Self-paper-state struct (v8)
    // -------------------------------------------------------------------------
    //
    // Paper mode requires knowing the AGB name (the "type" of paper transform:
    // kuru, plane, etc) to allocate a paper pose on the receiving side. The
    // game stores the paper pose handle at mp+0x240 but NOT the AGB name -
    // that has to be fetched via animPoseGetGroupName(poseId, groupId).
    //
    // The Python client can't call into the GameCube's animdrv functions, so
    // we add a small mod-side helper that writes the current paper-AGB name
    // (or empty if not in paper mode) to a fixed memory address each frame.
    // The client reads this address and includes it in its outbound publish.
    //
    // Address chosen well past peer block + diagnostics:
    //   peer block:       0x80001800-0x80002E10 (32 peers * 176 + 16 hdr)
    //   diagnostics:      0x80003700-0x800038XX
    //   self-paper-state: 0x80003B20 (32 bytes - just the AGB name)
    // History:
    //   v16: relocated from 0x80002820 to 0x80003B20 (peer block grew
    //        from 1424 bytes / 8 peers to 5648 bytes / 32 peers).
    //   v17: peer block moved from 0x80002000 to 0x80001800. Diagnostics
    //        and self-paper kept at 0x80003700+ / 0x80003B20 (still
    //        well past new block end at 0x80002E10).
    constexpr uintptr_t kSelfPaperAddress = 0x80003B20;
    constexpr int kSelfPaperAgbLen = 32; // v9: widened from 16

    inline char *GetSelfPaperAgbName()
    {
        return reinterpret_cast<char *>(kSelfPaperAddress);
    }

    // -------------------------------------------------------------------------
    // Mod-side public API. Called by mod.cpp / init.rel.
    // -------------------------------------------------------------------------
    void Init();                                                   // once at startup
    void Shutdown();                                               // mod teardown
    void UpdateAll();                                              // every frame
    void DrawAll(ttyd::dispdrv::CameraId cam, void *user);         // dispdrv callback (k3d)
    void DrawNameTagsAll(ttyd::dispdrv::CameraId cam, void *user); // dispdrv callback (k2d)
} // namespace mod::ghosts