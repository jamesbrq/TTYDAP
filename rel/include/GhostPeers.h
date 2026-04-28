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
    constexpr uint32_t kVersion = 20; // v20: added `teamId` byte (predefined teams: 0=none, 1=red, 2=blue, 3=green, 4=yellow).
                                      // Slot grows by 4 bytes (one byte teamId + 3 pad before float-aligned cameraAngle).

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
    //   struct.pack(">B 15s 16s fff f BBBB I I H B B B 3x f 16s 32s 16s ff fff fff f H 2x f",
    //       active, mapName, animName,
    //       x, y, z, rotY,
    //       r, g, b, a,
    //       flags2, flags3, motionTimer, showName, hammerable, teamId, cameraAngle,
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
        uint8_t showName;       // v18: name-tag visibility for this peer.
                                // 0 = show (default; back-compat with v17
                                //     zero pad - any v17 publisher is
                                //     interpreted as "show").
                                // 1 = hide name tag for this peer.
                                // Consumed by DrawNameTagsAll. The receiver
                                // also has a local feature_mask bit 3 that
                                // disables ALL name tags regardless of this
                                // field; the two layers compose.
        uint8_t hammerable;     // v19: peer-vs-peer hammer opt-out.
                                // 0 = can be hammered (default; back-compat
                                //     with v18 zero pad).
                                // 1 = opted out; attackers should skip
                                //     emitting hit events targeting this
                                //     peer.
                                // Enforced client-side: the attacker reads
                                // this field of the candidate victim and
                                // declines to fire the Bounce. Trivially
                                // bypassable - threat model is "obnoxious
                                // peer accidentally griefs me," not
                                // malicious actors. Same threat model as
                                // showName.
        uint8_t teamId;         // v20: predefined team membership.
                                //   0 = no team (default)
                                //   1 = red, 2 = blue, 3 = green, 4 = yellow
                                //   5..255 = reserved
                                // Used by attackers' CheckPeerHammerHits
                                // alongside the local self-team-id and
                                // friendly-fire flag: if peer is on the
                                // same non-zero team as the local player
                                // AND friendly fire is OFF, skip the hit.
                                // Friendly fire is per-attacker (the local
                                // FF flag is what matters for that
                                // player's swings); each peer's own FF
                                // setting governs only their own attacks.
        uint8_t pad_v20[3];     // align cameraAngle (f32) on 4-byte
                                // boundary. Reserved for future use.
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

    static_assert(sizeof(PeerSlot) == 180, "PeerSlot must be exactly 180 bytes");

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
    // Pending-hit flag (v19)
    // -------------------------------------------------------------------------
    //
    // Single u32 written by the AP client when an inbound hit event arrives
    // (a peer hammered us, delivered via the AP `Bounce` packet). The mod
    // consumes it in UpdateAll: if non-zero, attempts to enter mot_damage
    // (motion id 0x1F) via marioChgMot, then clears the flag.
    //
    // Layout:
    //   byte 0: kind code (1 = hammer; reserved 0 = no event, others = future)
    //   bytes 1..3: reserved (currently 0; future use for source slot id, etc.)
    //
    // Address: 0x80003B60. Sits 0x40 bytes past the self-paper-AGB scratch
    // (which ends at 0x80003B40), with breathing room. Well clear of the
    // diagnostics block at 0x80003C00.
    //
    // Single-byte writes from the client side suffice; we read u32 because
    // PowerPC reads are naturally aligned to 4 bytes anyway and the upper
    // bytes are reserved.
    constexpr uintptr_t kPendingHitAddress = 0x80003B60;

    // Hit-kind codes for byte 0 of the pending-hit flag.
    constexpr uint8_t kHitKindNone = 0;   // no event pending
    constexpr uint8_t kHitKindHammer = 1; // peer hammered us
    // (2..255 reserved for future attack types)

    inline volatile uint32_t *GetPendingHitPtr()
    {
        return reinterpret_cast<volatile uint32_t *>(kPendingHitAddress);
    }

    // -------------------------------------------------------------------------
    // Hit-reaction pose name (v19, live-tunable)
    // -------------------------------------------------------------------------
    //
    // The mod's pending-hit consumer calls marioChgEvtPose(<name>) when a
    // hit event arrives. The pose name is read live from this buffer so we
    // can iterate on which animation feels right (M_D_2 vs M_D_1 vs M_U_3
    // vs anything else in a_mario's anim table) without rebuilding.
    //
    // Layout: 16 bytes, NUL-terminated ASCII. Edit live with Dolphin's
    // memory editor:
    //   1. Find address 0x80003B70 in the memory viewer
    //   2. Type new pose name (e.g. "M_D_1\0" or "M_U_2\0")
    //   3. Trigger a hit (write 0x01 to 0x80003B60)
    //
    // Init() seeds the buffer with the default ("M_D_2"). If at consume-
    // time the buffer is empty (first byte == NUL) we fall back to the
    // compiled-in default rather than calling marioChgEvtPose with "".
    //
    // Address: 0x80003B70 - 16 bytes after the pending-hit flag.
    constexpr uintptr_t kHitPoseNameAddress = 0x80003B70;
    constexpr int kHitPoseNameLen = 16;

    // Compiled-in default. Picked from e_mario's anim table:
    //   M_N_7 - 18 frames, looping. The "winded/wincing" expression
    //           animation. Plays on the effects pose (Player.0x234) so
    //           Mario's body keeps doing whatever it was doing while
    //           his face reacts to the hit. Identified through testing
    //           against the e_mario AGB.
    //
    // Other interesting candidates to try by writing into the buffer:
    //   M_F_1  - 16 frames, LOOPING. The actual mot_damage entry face.
    //   M_D_7  - 8 frames, LOOPING. mot_damage's stunned face.
    //   M_D_8  - 16 frames, non-loop. Damage variant.
    //   M_D_2  - 8 frames, non-loop (a_mario body, not e_mario).
    //   M_I_Y  - 8 frames, non-loop. Generic blink/look (a_mario).
    constexpr const char *kDefaultHitPoseName = "M_N_7";

    inline char *GetHitPoseNamePtr()
    {
        return reinterpret_cast<char *>(kHitPoseNameAddress);
    }

    // -------------------------------------------------------------------------
    // Hit-detection reach scale (v19, live-tunable)
    // -------------------------------------------------------------------------
    //
    // Multiplier on the engine's per-level hammer reach (Player.0x1B8).
    // The full formula is:
    //
    //   reach = kHitReachScale * Player.0x1B8
    //
    // Then the engine's hit-check internally adds (NpcEntry.width * 0.5)
    // to the reach when computing the actual hit boundary.
    //
    // Engine default: 1.5 (this is what fbatHitCheck uses for NPC hits).
    // We default lower because peer-vs-peer hits otherwise feel too generous.
    //
    // Edit live at 0x80003B80 with Dolphin's memory editor (4 bytes,
    // big-endian IEEE 754). Init() seeds with kDefaultHitReachScale on
    // boot. If the read value is <= 0 (not yet initialized, or someone
    // wrote zero) we fall back to the default.
    constexpr uintptr_t kHitReachScaleAddress = 0x80003B80;
    constexpr float kDefaultHitReachScale = 1.0f;

    inline volatile float *GetHitReachScalePtr()
    {
        return reinterpret_cast<volatile float *>(kHitReachScaleAddress);
    }

    // Same idea for the per-peer "NPC width" used by the engine's geometry
    // test. The engine reads NpcEntry.width (offset 0x14C) and uses it as
    // a capsule radius around the target's position. Smaller width = the
    // engine treats the peer as a smaller target = Mario needs to be
    // closer to register a hit.
    //
    // The original engine value for our fake-NPC was 30. We seed lower
    // for the same "peers feel too generous" reason.
    //
    // Edit live at 0x80003B84 with Dolphin's memory editor (4 bytes,
    // big-endian float). If the read value is <= 0 we fall back to the
    // default.
    constexpr uintptr_t kHitPeerWidthAddress = 0x80003B84;
    constexpr float kDefaultHitPeerWidth = 15.0f;

    inline volatile float *GetHitPeerWidthPtr()
    {
        return reinterpret_cast<volatile float *>(kHitPeerWidthAddress);
    }

    // -------------------------------------------------------------------------
    // Outbound hit event (v19, mod -> client)
    // -------------------------------------------------------------------------
    //
    // Single u32 written by the mod when our local hammer swing connects
    // with a peer. The AP client polls this on its ghost-sync tick; when
    // non-zero it emits a `Bounce` packet to the targeted peer's AP slot
    // and clears the field.
    //
    // Layout:
    //   byte 0: kind code (1 = hammer; same codes as kPendingHitAddress)
    //   byte 1: peer block index (0..31) - which slot in the 32-slot
    //           peer block we hit. The client looks this up in its
    //           sorted peer-key list (matching pack_peer_block's order)
    //           to find the AP slot ID.
    //   byte 2-3: reserved
    //
    // Address: 0x80003B88. Sits right after the reach/peer-width
    // tunables (which end at 0x80003B88).
    constexpr uintptr_t kOutboundHitAddress = 0x80003B88;

    inline volatile uint32_t *GetOutboundHitPtr()
    {
        return reinterpret_cast<volatile uint32_t *>(kOutboundHitAddress);
    }

    // Pack a hit event into the u32 word the mod writes / client reads.
    inline uint32_t PackOutboundHit(uint8_t kind, uint8_t peerIndex)
    {
        return (static_cast<uint32_t>(kind) << 24) | (static_cast<uint32_t>(peerIndex) << 16);
    }

    // -------------------------------------------------------------------------
    // Hit grace period (iframes) flag (v19)
    // -------------------------------------------------------------------------
    //
    // Single u8 written by the mod when the local player has been hit
    // and is within their post-hit invulnerability window. The Python AP
    // client reads this byte during its self-state poll and ORs it into
    // the published `hammerable` field, so other peers see us as
    // un-hammerable for the duration of our grace period.
    //
    //   0 = hittable (default)
    //   1 = in grace period - other peers' attackers should skip us
    //
    // The mod also self-checks this byte in its hit-queue drain so a
    // race-condition hit arriving DURING the grace period gets dropped.
    //
    // Address: 0x80003B8C. Sits right after the outbound-hit u32
    // (which ends at 0x80003B8B). Single byte; reads as u8.
    constexpr uintptr_t kHitGraceAddress = 0x80003B8C;

    inline volatile uint8_t *GetHitGracePtr()
    {
        return reinterpret_cast<volatile uint8_t *>(kHitGraceAddress);
    }

    // -------------------------------------------------------------------------
    // Team IDs (v20)
    // -------------------------------------------------------------------------
    //
    // Predefined team identifiers. Stored on the wire in PeerSlot.teamId
    // (u8) and in the self-team scratch byte. Numeric values are stable
    // (don't reorder) since both Python and C++ encode them by value.
    constexpr uint8_t kTeamNone = 0; // no team / default
    constexpr uint8_t kTeamRed = 1;
    constexpr uint8_t kTeamBlue = 2;
    constexpr uint8_t kTeamGreen = 3;
    constexpr uint8_t kTeamYellow = 4;
    // 5..255 reserved.

    // -------------------------------------------------------------------------
    // Self-team scratch (v20)
    // -------------------------------------------------------------------------
    //
    // Two single-byte fields the AP client writes alongside the peer
    // block. The mod reads them in CheckPeerHammerHits to decide whether
    // a candidate victim should be skipped on friendly-fire grounds:
    //
    //   if (peer.teamId != kTeamNone &&
    //       peer.teamId == self_team_id &&
    //       !friendly_fire)
    //       skip;
    //
    //   0x80003B8D  u8 self_team_id    Local player's team. Mirrors
    //                                  ctx._ghost_team_id from the
    //                                  Python client. Same kTeam* codes
    //                                  as the wire-format teamId.
    //   0x80003B8E  u8 friendly_fire   Local FF flag. 0 = off (default,
    //                                  same-team hits skipped),
    //                                  1 = on (same-team hits land
    //                                  normally).
    //
    // Both bytes are written by Python on every publish tick; the mod
    // simply reads the latest snapshot each frame.
    constexpr uintptr_t kSelfTeamIdAddress = 0x80003B8D;
    constexpr uintptr_t kSelfFriendlyFireAddress = 0x80003B8E;

    inline volatile uint8_t *GetSelfTeamIdPtr()
    {
        return reinterpret_cast<volatile uint8_t *>(kSelfTeamIdAddress);
    }
    inline volatile uint8_t *GetSelfFriendlyFirePtr()
    {
        return reinterpret_cast<volatile uint8_t *>(kSelfFriendlyFireAddress);
    }

    // -------------------------------------------------------------------------
    // Minigame lobby HUD (v20)
    // -------------------------------------------------------------------------
    //
    // Single 1KB block at 0x80003D00. Python writes; mod reads in
    // DrawLobbyHud and renders an in-game overlay top-right of screen.
    //
    // Layout (mirrors Ghosts.py):
    //
    //   offset  size  field
    //   ------  ----  -----------------------------------------------------
    //   0x000   4     magic (u32, 'LOBY' = 0x4C4F4259)
    //   0x004   1     version (u8, must equal kLobbyHudVersion)
    //   0x005   1     active (0 = no lobby; 1 = render HUD)
    //   0x006   1     status (kLobbyStatus*)
    //   0x007   1     game_type (kGameType*)
    //   0x008   1     member_count (0..32)
    //   0x009   1     self_role (kLobbyRole*)
    //   0x00A   2     timer_seconds (u16, big-endian; 0 = no timer)
    //   0x00C   4     reserved
    //   0x010   16    lobby_name (char[16], NUL-padded)
    //   0x020   768   members[32] - each 24 bytes
    //   0x320   192   free-form HUD text (NUL-terminated, multi-line via \n)
    //   0x3E0   ...   reserved tail
    //   0x400         end (1024 bytes)
    //
    // Per-member (24 bytes):
    //   0x00  1   slot
    //   0x01  1   role (kLobbyRole*)
    //   0x02  1   alive (1 = active, 0 = out / spectating)
    //   0x03  1   pad
    //   0x04  16  name (char[16], NUL-padded)
    //   0x14  4   reserved
    //
    // Magic byte distinguishes "Python wrote here" from "uninitialized
    // RAM at boot" - mod skips drawing if it doesn't match. To clear
    // the HUD immediately on disconnect, write 4 zero bytes to the
    // magic field.
    constexpr uintptr_t kLobbyHudAddress = 0x80003D00;
    constexpr uint32_t kLobbyHudMagic = 0x4C4F4259; // 'LOBY'
    constexpr uint8_t kLobbyHudVersion = 1;
    constexpr int kLobbyHudSize = 1024;
    constexpr int kLobbyMaxMembers = 32;
    constexpr int kLobbyMemberSize = 24;
    constexpr int kLobbyHeaderSize = 32;
    constexpr int kLobbyMembersOffset = kLobbyHeaderSize;
    constexpr int kLobbyTextOffset = kLobbyMembersOffset + kLobbyMaxMembers * kLobbyMemberSize;
    constexpr int kLobbyTextLen = 192;

    // Status enum values (must match Ghosts.py).
    constexpr uint8_t kLobbyStatusIdle = 0;
    constexpr uint8_t kLobbyStatusWaiting = 1;
    constexpr uint8_t kLobbyStatusCountdown = 2;
    constexpr uint8_t kLobbyStatusPlaying = 3;
    constexpr uint8_t kLobbyStatusFinished = 4;

    // Game type enum (must match Ghosts.py).
    constexpr uint8_t kGameTypeNone = 0;
    constexpr uint8_t kGameTypeHideAndSeek = 1;

    // Role enum (must match Ghosts.py).
    constexpr uint8_t kLobbyRoleNone = 0;
    constexpr uint8_t kLobbyRoleHost = 1;
    constexpr uint8_t kLobbyRoleParticipant = 2;
    constexpr uint8_t kLobbyRoleHider = 3;
    constexpr uint8_t kLobbyRoleSeeker = 4;
    constexpr uint8_t kLobbyRoleSpectator = 5;

// Header view. Cast the block pointer to this for convenient field
// access. Padding/alignment matches Ghosts.py's struct.pack(">IBBBBBBHI16s").
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
    static_assert(sizeof(LobbyHudHeader) == kLobbyHeaderSize, "LobbyHudHeader size mismatch with kLobbyHeaderSize");

    struct LobbyHudMember
    {
        uint8_t slot;
        uint8_t role;
        uint8_t alive;
        uint8_t pad;
        char name[16];
        uint32_t reserved;
    };
    static_assert(sizeof(LobbyHudMember) == kLobbyMemberSize, "LobbyHudMember size mismatch with kLobbyMemberSize");
#pragma pack(pop)

    inline const LobbyHudHeader *GetLobbyHudHeader()
    {
        return reinterpret_cast<const LobbyHudHeader *>(kLobbyHudAddress);
    }
    inline const LobbyHudMember *GetLobbyHudMembers()
    {
        return reinterpret_cast<const LobbyHudMember *>(kLobbyHudAddress + kLobbyMembersOffset);
    }
    inline const char *GetLobbyHudText()
    {
        return reinterpret_cast<const char *>(kLobbyHudAddress + kLobbyTextOffset);
    }

    // -------------------------------------------------------------------------
    // Mod-side public API. Called by mod.cpp / init.rel.
    // -------------------------------------------------------------------------
    void Init();                                                   // once at startup
    void Shutdown();                                               // mod teardown
    void UpdateAll();                                              // every frame
    void DrawAll(ttyd::dispdrv::CameraId cam, void *user);         // dispdrv callback (k3d)
    void DrawNameTagsAll(ttyd::dispdrv::CameraId cam, void *user); // dispdrv callback (k2d)
    void DrawLobbyHud(ttyd::dispdrv::CameraId cam, void *user);    // dispdrv callback (k2d)
} // namespace mod::ghosts