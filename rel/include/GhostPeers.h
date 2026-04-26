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
    constexpr int kMaxPeers = 8;

    // The fixed RAM address the AP client writes to. Must match on both sides.
    // Lives in low main-RAM scratch, just above the existing AP item queue
    // at 0x80000FFC / 0x80001000.
    constexpr uintptr_t kBlockAddress = 0x80002000;

    // Magic + version. Bump kVersion if the on-wire layout below changes.
    // The magic byte distinguishes "AP client has written" from "uninitialized
    // garbage RAM at boot" - without it the mod would try to render ghosts
    // from random memory contents on the first frame.
    constexpr uint32_t kMagic = 0x47484F53; // 'GHOS'
    constexpr uint32_t kVersion = 6;        // v6: added slotName to PeerSlot

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

    // One peer's state. 84 bytes packed (was 68 in v5).
    //
    // Python equivalent (for reference, '>' = big-endian / network):
    //   struct.pack(">B 15s 16s fff f BBBB I I H 2x f 16s",
    //       active, mapName, animName,
    //       x, y, z, rotY,
    //       r, g, b, a,
    //       flags2, flags3, motionTimer, cameraAngle,
    //       slotName)
    struct PeerSlot
    {
        uint8_t active;       // 0 = empty slot (mod ignores), 1 = render
        char mapName[15];     // current map of this peer; mod gates on this
        char animName[16];    // animation name e.g. "M_W_1", "M_R_1", "M_S_1"
        float x, y, z;        // world position
        float rotationY;      // facing yaw, degrees (wPlayerDirCur)
        uint8_t r, g, b, a;   // tint; alpha = ghost translucency
        uint32_t flags2;      // mirror of Player.flags2 - drives pose pick
        uint32_t flags3;      // mirror of Player.flags3 - drives L/R yaw
        uint16_t motionTimer; // mirror of Player.unk_28 - drives anim playhead
        uint8_t _pad[2];      // pad for float alignment
        float cameraAngle;    // mirror of Player.unk_19c - camera-yaw offset
        char slotName[16];    // player's display name (NUL-padded). Rendered
                              // as a name tag above the ghost. Empty string
                              // means "no name tag".
    } __attribute__((__packed__));

    static_assert(sizeof(PeerSlot) == 1 + 15 + 16 + 4 * 4 + 4 + 4 + 4 + 2 + 2 + 4 + 16, "PeerSlot layout drifted");
    static_assert(sizeof(PeerSlot) == 84, "PeerSlot must be exactly 84 bytes");

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
    // Mod-side public API. Called by mod.cpp / init.rel.
    // -------------------------------------------------------------------------
    void Init();                                                   // once at startup
    void Shutdown();                                               // mod teardown
    void UpdateAll();                                              // every frame
    void DrawAll(ttyd::dispdrv::CameraId cam, void *user);         // dispdrv callback (k3d)
    void DrawNameTagsAll(ttyd::dispdrv::CameraId cam, void *user); // dispdrv callback (k2d)
} // namespace mod::ghosts