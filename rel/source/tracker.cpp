#include "tracker.h"
#include "OWR.h"

#include <AP/rel_patch_definitions.h>
#include "gc/dvd.h"
#include "ttyd/animdrv.h"
#include "ttyd/dispdrv.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/memory.h"
#include "ttyd/seqdrv.h"
#include "ttyd/swdrv.h"
#include "ttyd/win_log.h"

#include <cstdint>
#include <cstring>

namespace mod::tracker
{
    using namespace gc::dvd;
    using namespace ttyd::mario_pouch;
    using namespace ttyd::memory;

    // Bytecode ops and file layout are produced by worlds/ttyd/Tracker.py.
    enum RuleOp : uint8_t
    {
        OP_TRUE = 0x00,
        OP_FALSE = 0x01,
        OP_AND = 0x02,
        OP_OR = 0x03,
        OP_HAS = 0x04,
        OP_REACH_LOC = 0x05,
        OP_REACH_REGION = 0x06,
        OP_COUNT_LOCS = 0x07,
        OP_COUNT_ITEMS = 0x08,
    };

    static constexpr uint16_t kStarsPseudoItem = 0x7F00;
    static constexpr uint16_t kNoRule = 0xFFFF;
    static constexpr uint32_t kLocEntrySize = 12;
    static constexpr uint32_t kNodeEntrySize = 16;
    static constexpr uint32_t kTattleLocationCount = 124;
    static constexpr uint32_t kTattleGswfBase = 0x117A;
    static constexpr int32_t kSolveIntervalFrames = 30;
    static constexpr uint32_t kMaxRegions = 64;
    static constexpr uint32_t kItemCacheSize = 512;

    struct EverCounter
    {
        uint16_t rom;
        uint16_t gsw;
    };
    static constexpr EverCounter kEverCounters[] = {
        {125, kEverGswStarPiece}, // Star Piece
        {38, 1802},               // Palace Key (Tower)
        {12, 1803},               // Castle Key
        {46, 1804},               // Palace Key
        {73, 1805},               // Wedding Ring
    };

    static uint8_t *sBin = nullptr;
    static bool sLoadTried = false;

    static uint16_t sNumRegions = 0;
    static uint16_t sNumLocs = 0;
    static uint16_t sNumConns = 0;
    static uint16_t sNumNodes = 0;
    static const uint8_t *sLocs = nullptr;
    static const uint8_t *sConns = nullptr;
    static const uint8_t *sNodes = nullptr;
    static const uint8_t *sRules = nullptr;
    static const uint8_t *sStrings = nullptr;
    static const uint8_t *sTattle = nullptr;

    static uint8_t sRegionOk[kMaxRegions];
    static uint8_t *sLocOk = nullptr;      // location is in logic
    static uint8_t *sLocChecked = nullptr; // location already collected
    static int16_t sItemCache[kItemCacheSize];
    static int32_t sSolveCooldown = 0;

    static inline uint16_t rd16(const uint8_t *p)
    {
        return static_cast<uint16_t>((p[0] << 8) | p[1]);
    }

    static inline uint32_t rd32(const uint8_t *p)
    {
        return (static_cast<uint32_t>(p[0]) << 24) | (static_cast<uint32_t>(p[1]) << 16) |
               (static_cast<uint32_t>(p[2]) << 8) | p[3];
    }

    static bool ensureLoaded()
    {
        if (sBin)
            return true;
        if (sLoadTried)
            return false;
        sLoadTried = true;

        DVDFileInfo fileInfo;
        if (!DVDOpen("/mod/tracker.bin", &fileInfo))
            return false;

        const uint32_t size = fileInfo.length;
        const uint32_t alignedSize = (size + DVD_READ_SIZE - 1) & ~(DVD_READ_SIZE - 1);

        uint8_t *buffer = reinterpret_cast<uint8_t *>(__memAlloc(HeapType::HEAP_DEFAULT, alignedSize));
        if (!buffer)
        {
            DVDClose(&fileInfo);
            return false;
        }

        const int32_t result = DVDReadPrio(&fileInfo, buffer, alignedSize, 0, 0);
        DVDClose(&fileInfo);

        if (result <= 0 || size < 36 || memcmp(buffer, "TRK2", 4) != 0)
        {
            __memFree(HeapType::HEAP_DEFAULT, buffer);
            return false;
        }

        sNumRegions = rd16(buffer + 4);
        sNumLocs = rd16(buffer + 6);
        sNumConns = rd16(buffer + 8);
        sNumNodes = rd16(buffer + 10);
        sLocs = buffer + rd32(buffer + 12);
        sConns = buffer + rd32(buffer + 16);
        sNodes = buffer + rd32(buffer + 20);
        sRules = buffer + rd32(buffer + 24);
        sStrings = buffer + rd32(buffer + 28); // location-name pool
        sTattle = buffer + rd32(buffer + 32);

        if (sNumRegions > kMaxRegions)
        {
            __memFree(HeapType::HEAP_DEFAULT, buffer);
            return false;
        }

        sLocOk = reinterpret_cast<uint8_t *>(__memAlloc(HeapType::HEAP_DEFAULT, sNumLocs * 2));
        if (!sLocOk)
        {
            __memFree(HeapType::HEAP_DEFAULT, buffer);
            return false;
        }
        sLocChecked = sLocOk + sNumLocs;

        sBin = buffer;
        return true;
    }

    static int32_t itemCount(uint16_t rom)
    {
        PouchData *pouch = pouchGetPtr();
        switch (rom)
        {
            case kStarsPseudoItem: // crystal stars owned
            {
                int32_t count = 0;
                for (int32_t i = 114; i <= 120; i++)
                {
                    if (pouchCheckItem(i) > 0)
                        count++;
                }
                return count;
            }
            case 6: // Progressive Boots
            {
                const int32_t level = static_cast<int32_t>(pouchGetJumpLv());
                return level > 0 ? level - 1 : 0;
            }
            case 9: // Progressive Hammer
            {
                // Same as the boots: N progressives owned = pouch level N+1
                const int32_t level = static_cast<int32_t>(pouchGetHammerLv());
                return level > 0 ? level - 1 : 0;
            }
            case 55: // Goombella
                return (pouch->party_data[1].flags & 1) ? 1 : 0;
            case 56: // Koops
                return (pouch->party_data[2].flags & 1) ? 1 : 0;
            case 57: // Flurrie
                return (pouch->party_data[5].flags & 1) ? 1 : 0;
            case 58: // Yoshi
                return (pouch->party_data[4].flags & 1) ? 1 : 0;
            case 59: // Vivian
                return (pouch->party_data[6].flags & 1) ? 1 : 0;
            case 111: // Bobbery
                return (pouch->party_data[3].flags & 1) ? 1 : 0;
            case 112: // Ms. Mowz
                return (pouch->party_data[7].flags & 1) ? 1 : 0;
            case 87: // Shine Sprite
                return pouch->shine_sprites;
            default:
            {
                // Star Piece (125) lives in a pouch field, not the item arrays
                int32_t count = rom == 125 ? pouch->star_pieces : pouchCheckItem(rom);
                for (const EverCounter &ec : kEverCounters)
                {
                    if (ec.rom == rom)
                    {
                        const int32_t ever = ttyd::swdrv::swByteGet(ec.gsw);
                        return ever > count ? ever : count;
                    }
                }
                if (count == 0 && rom >= 1 && rom < 256 && ttyd::swdrv::swGet(kEverFlagBase + rom))
                    count = 1;
                return count;
            }
        }
    }

    void recordItemObtained(int32_t item)
    {
        if (item < 1 || item > 255)
            return;
        ttyd::swdrv::swSet(kEverFlagBase + item);
        for (const EverCounter &ec : kEverCounters)
        {
            if (ec.rom == item)
            {
                const int32_t v = ttyd::swdrv::swByteGet(ec.gsw);
                if (v < 255)
                    ttyd::swdrv::swByteSet(ec.gsw, v + 1);
            }
        }
    }

    static int32_t cachedItemCount(uint16_t rom)
    {
        if (rom >= kItemCacheSize)
            return itemCount(rom);
        if (sItemCache[rom] < 0)
        {
            int32_t count = itemCount(rom);
            if (count > 0x7FFF)
                count = 0x7FFF;
            sItemCache[rom] = static_cast<int16_t>(count);
        }
        return sItemCache[rom];
    }

    static bool evalAt(const uint8_t *&pc)
    {
        const uint8_t op = *pc++;
        switch (op)
        {
            case OP_TRUE:
                return true;
            case OP_FALSE:
                return false;
            case OP_AND:
            case OP_OR:
            {
                const uint8_t n = *pc++;
                bool result = (op == OP_AND);
                for (uint8_t i = 0; i < n; i++)
                {
                    const bool sub = evalAt(pc);
                    result = (op == OP_AND) ? (result && sub) : (result || sub);
                }
                return result;
            }
            case OP_HAS:
            {
                const uint16_t rom = rd16(pc);
                const uint16_t count = rd16(pc + 2);
                pc += 4;
                return cachedItemCount(rom) >= static_cast<int32_t>(count);
            }
            case OP_REACH_LOC:
            {
                const uint16_t idx = rd16(pc);
                pc += 2;
                return idx < sNumLocs && sLocOk[idx] != 0;
            }
            case OP_REACH_REGION:
            {
                const uint8_t idx = *pc++;
                return idx < sNumRegions && sRegionOk[idx] != 0;
            }
            case OP_COUNT_LOCS:
            {
                const uint8_t n = pc[0];
                const uint8_t k = pc[1];
                pc += 2;
                uint8_t count = 0;
                for (uint8_t i = 0; i < n; i++)
                {
                    const uint16_t idx = rd16(pc);
                    pc += 2;
                    if (idx < sNumLocs && sLocOk[idx])
                        count++;
                }
                return count >= k;
            }
            case OP_COUNT_ITEMS:
            {
                const uint8_t n = pc[0];
                const uint8_t k = pc[1];
                pc += 2;
                uint8_t count = 0;
                for (uint8_t i = 0; i < n; i++)
                {
                    const uint16_t rom = rd16(pc);
                    pc += 2;
                    if (cachedItemCount(rom) >= 1)
                        count++;
                }
                return count >= k;
            }
            default:
                // Unknown op: fail closed and stop walking this rule
                return false;
        }
    }

    static bool evalRule(uint16_t ruleOff)
    {
        if (ruleOff == kNoRule)
            return true;
        const uint8_t *pc = sRules + ruleOff;
        return evalAt(pc);
    }

    static bool locationChecked(const uint8_t *entry)
    {
        const uint8_t gswType = entry[5];
        const uint16_t gswId = rd16(entry + 6);
        const uint8_t gswValue = entry[8];

        switch (gswType)
        {
            case 0: // GSW byte
                return ttyd::swdrv::swByteGet(gswId) >= gswValue;
            case 1: // GSWF
                return ttyd::swdrv::swGet(gswId);
            case 2: // tattle: any unit's monosiri flag
            {
                const uint16_t recOff = rd16(sTattle + gswId * 2);
                const uint8_t *rec = sTattle + kTattleLocationCount * 2 + recOff;
                const uint8_t numUnits = rec[0];
                for (uint8_t i = 0; i < numUnits; i++)
                {
                    if (ttyd::swdrv::swGet(kTattleGswfBase + rec[1 + i]))
                        return true;
                }
                return false;
            }
            default: // undetectable
                return false;
        }
    }

    static void solve()
    {
        memset(sItemCache, 0xFF, sizeof(sItemCache)); // -1 = not cached
        memset(sRegionOk, 0, sizeof(sRegionOk));
        memset(sLocOk, 0, sNumLocs);
        sRegionOk[0] = 1; // Menu

        for (uint16_t i = 0; i < sNumLocs; i++)
            sLocChecked[i] = locationChecked(sLocs + i * kLocEntrySize) ? 1 : 0;

        bool changed = true;
        while (changed)
        {
            changed = false;

            for (uint16_t i = 0; i < sNumConns; i++)
            {
                const uint8_t *conn = sConns + i * 4;
                const uint8_t src = conn[0];
                const uint8_t dst = conn[1];
                if (sRegionOk[src] && !sRegionOk[dst] && evalRule(rd16(conn + 2)))
                {
                    sRegionOk[dst] = 1;
                    changed = true;
                }
            }

            for (uint16_t i = 0; i < sNumLocs; i++)
            {
                if (sLocOk[i])
                    continue;
                const uint8_t *entry = sLocs + i * kLocEntrySize;
                if (entry[9] & 1) // disabled
                    continue;
                if (sRegionOk[entry[4]] && evalRule(rd16(entry + 2)))
                {
                    sLocOk[i] = 1;
                    changed = true;
                }
            }
        }
    }

    // win.tpl image indices appended by the apworld's win.bsdiff4
    static constexpr uint16_t kTexNodeRed = 194;       // out of logic (blink = 193)
    static constexpr uint16_t kTexNodeGreen = 196;     // checks available (blink = 195)
    static constexpr uint16_t kTexNodeGoombella = 198; // tattle virtual node (blink = 197)
    static constexpr uint16_t kTexNodeCookbook = 200;  // cooking virtual node (blink = 199)
    static constexpr uint16_t kTexNodeGrey = 202;      // all locations checked (blink = 201)

    static void updateMarkerTextures()
    {
        const ttyd::win_log::MapMarker *markers =
            reinterpret_cast<const ttyd::win_log::MapMarker *>(ap_map_markers);
        for (int32_t i = 0; i < 95; i++)
        {
            const ttyd::win_log::MapMarker &marker = markers[i];
            if (!marker.isLocation || !marker.map_prefix)
                continue;
            if (strncmp(marker.map_prefix, "tattle", 7) == 0)
            {
                win_map_marker_tex[i] = kTexNodeGoombella;
                continue;
            }
            if (strncmp(marker.map_prefix, "cook", 7) == 0)
            {
                win_map_marker_tex[i] = kTexNodeCookbook;
                continue;
            }
            const NodeInfo info = trackerQueryPrefix(marker.map_prefix);
            if (!info.valid)
                continue; // leave the vanilla orange
            if (info.available > 0)
                win_map_marker_tex[i] = kTexNodeGreen;
            else if (info.total > 0 && info.checked == info.total)
                win_map_marker_tex[i] = kTexNodeGrey; // area fully completed
            else
                win_map_marker_tex[i] = kTexNodeRed; // remaining checks out of logic
        }
    }

    void trackerTick()
    {
        // Panel hints need the solver even when the tracker UI is disabled;
        // trackerReady() keeps the UI itself gated on the tracker option.
        mod::owr::APSettings *settings = mod::owr::gState->apSettings;
        if (!settings->tracker && !settings->panelHints)
            return;
        if (!ensureLoaded())
            return;
        if (sSolveCooldown > 0)
        {
            sSolveCooldown--;
            return;
        }
        solve();
        if (settings->tracker)
            updateMarkerTextures();
        sSolveCooldown = kSolveIntervalFrames;
    }

    bool trackerReady()
    {
        return sBin != nullptr && mod::owr::gState->apSettings->tracker;
    }

    static const uint8_t *findNode(const char *prefix)
    {
        const uint8_t *gorNode = nullptr;
        for (uint16_t i = 0; i < sNumNodes; i++)
        {
            const uint8_t *entry = sNodes + i * kNodeEntrySize;
            if (strncmp(reinterpret_cast<const char *>(entry), prefix, 7) == 0)
                return entry;
            if (strncmp(reinterpret_cast<const char *>(entry), "gor", 7) == 0)
                gorNode = entry;
        }
        // Unknown prefixes fast-travel to Rogueport, so report Rogueport's info
        return gorNode;
    }

    static bool nodeContainsRegion(const uint8_t *node, uint8_t region)
    {
        const uint8_t dispCount = node[8];
        for (uint8_t j = 0; j < dispCount && j < 6; j++)
        {
            if (node[9 + j] == region)
                return true;
        }
        return false;
    }

    NodeInfo trackerQueryPrefix(const char *prefix)
    {
        NodeInfo info = {};
        if (!sBin || !prefix)
            return info;

        const uint8_t *node = findNode(prefix);
        if (!node)
            return info;

        info.valid = true;
        info.gateReachable = node[7] < sNumRegions && sRegionOk[node[7]] != 0;

        const uint8_t dispCount = node[8];
        for (uint16_t i = 0; i < sNumLocs; i++)
        {
            const uint8_t *entry = sLocs + i * kLocEntrySize;
            if (entry[9] & 1)
                continue;
            const uint8_t region = entry[10];
            bool inNode = false;
            for (uint8_t j = 0; j < dispCount && j < 6; j++)
            {
                if (node[9 + j] == region)
                {
                    inNode = true;
                    break;
                }
            }
            if (!inNode)
                continue;
            info.total++;
            if (sLocChecked[i])
                info.checked++;
            else if (sLocOk[i])
                info.available++;
        }
        return info;
    }

    // Tracker-screen list: indices into the location table, snapshotted per node
    static constexpr int32_t kMaxListRows = 512;
    static uint16_t sList[kMaxListRows];
    static int32_t sListCount = 0;

    static uint8_t locationStatus(uint16_t idx)
    {
        if (sLocChecked[idx])
            return LOC_CHECKED;
        return sLocOk[idx] ? LOC_AVAILABLE : LOC_OUT_OF_LOGIC;
    }

    int32_t trackerBuildNodeList(const char *prefix)
    {
        sListCount = 0;
        if (!sBin || !prefix)
            return 0;
        const uint8_t *node = findNode(prefix);
        if (!node)
            return 0;

        // Grouped by status so the actionable rows lead; file order within groups
        for (uint8_t group = LOC_AVAILABLE; sListCount < kMaxListRows;)
        {
            for (uint16_t i = 0; i < sNumLocs && sListCount < kMaxListRows; i++)
            {
                const uint8_t *entry = sLocs + i * kLocEntrySize;
                if (entry[9] & 1)
                    continue;
                if (!nodeContainsRegion(node, entry[10])) // display region
                    continue;
                if (locationStatus(i) == group)
                    sList[sListCount++] = i;
            }
            if (group == LOC_AVAILABLE)
                group = LOC_OUT_OF_LOGIC;
            else if (group == LOC_OUT_OF_LOGIC)
                group = LOC_CHECKED;
            else
                break;
        }
        return sListCount;
    }

    bool trackerGetListRow(int32_t row, const char **name, uint8_t *status)
    {
        if (!sBin || row < 0 || row >= sListCount)
            return false;
        const uint16_t idx = sList[row];
        const uint8_t *entry = sLocs + idx * kLocEntrySize;
        *name = reinterpret_cast<const char *>(sStrings + rd16(entry));
        *status = locationStatus(idx);
        return true;
    }

    uint8_t trackerLocStatusByGswf(uint16_t gswfId)
    {
        if (!sBin || !sLocOk)
            return 0xFF;
        for (uint16_t i = 0; i < sNumLocs; i++)
        {
            const uint8_t *entry = sLocs + i * kLocEntrySize;
            if (entry[5] != 1 || rd16(entry + 6) != gswfId)
                continue;
            if (entry[9] & 1)
                return 0xFF; // disabled this seed
            // Checked is read live so the tint greys out the moment the item
            // is picked up; availability comes from the last solve.
            if (ttyd::swdrv::swGet(gswfId))
                return LOC_CHECKED;
            return sLocOk[i] ? LOC_AVAILABLE : LOC_OUT_OF_LOGIC;
        }
        return 0xFF;
    }

    struct PanelTint
    {
        void *pose;
        void *mobj;
        uint32_t rgba;
    };
    static constexpr int32_t kMaxPanelTints = 16;
    static PanelTint sPanelTints[kMaxPanelTints];
    static int32_t sPanelTintCount = 0;

    // Returns true when this is a new registration or the color changed.
    static bool setPanelTint(void *pose, const gc::color4 &c)
    {
        const uint32_t rgba = (static_cast<uint32_t>(c.r) << 24) | (static_cast<uint32_t>(c.g) << 16) |
                              (static_cast<uint32_t>(c.b) << 8) | c.a;
        for (int32_t i = 0; i < sPanelTintCount; i++)
        {
            if (sPanelTints[i].pose == pose)
            {
                if (sPanelTints[i].rgba == rgba)
                    return false;
                sPanelTints[i].rgba = rgba;
                return true;
            }
        }
        if (sPanelTintCount < kMaxPanelTints)
        {
            sPanelTints[sPanelTintCount].pose = pose;
            sPanelTints[sPanelTintCount].mobj = nullptr;
            sPanelTints[sPanelTintCount].rgba = rgba;
            sPanelTintCount++;
            return true;
        }
        return false;
    }

    static void removePanelTint(void *pose)
    {
        for (int32_t i = 0; i < sPanelTintCount; i++)
        {
            if (sPanelTints[i].pose == pose)
            {
                sPanelTints[i] = sPanelTints[sPanelTintCount - 1];
                sPanelTintCount--;
                return;
            }
        }
    }

    void trackerClearPanelTints()
    {
        sPanelTintCount = 0;
    }

    // Idle panels render nothing (the pose only draws once the flip starts),
    // so the tint is a translucent quad over the panel's floor area.
    extern "C"
    {
        void *camGetPtr(int cameraId);
        void GXLoadPosMtxImm(float *mtx, uint32_t id);
        void GXSetCurrentMtx(uint32_t id);
        void GXClearVtxDesc();
        void GXSetVtxDesc(uint32_t attr, uint32_t type);
        void GXSetVtxAttrFmt(uint32_t fmt, uint32_t attr, uint32_t compCnt, uint32_t compType, uint8_t shift);
        void GXSetNumChans(uint32_t count);
        void GXSetChanCtrl(uint32_t chan, uint8_t enable, uint32_t ambSrc, uint32_t matSrc, uint32_t lightMask,
                           uint32_t diffFn, uint32_t attnFn);
        void GXSetNumTexGens(uint32_t count);
        void GXSetNumTevStages(uint32_t count);
        void GXSetTevOrder(uint32_t stage, uint32_t coord, uint32_t map, uint32_t color);
        void GXSetTevOp(uint32_t stage, uint32_t mode);
        void GXSetBlendMode(uint32_t type, uint32_t srcFactor, uint32_t dstFactor, uint32_t logicOp);
        void GXSetZMode(uint8_t compareEnable, uint32_t func, uint8_t updateEnable);
        void GXSetAlphaCompare(uint32_t comp0, uint8_t ref0, uint32_t aop, uint32_t comp1, uint8_t ref1);
        void GXSetCullMode(uint32_t mode);
        void GXBegin(uint32_t prim, uint32_t vtxfmt, uint16_t nverts);
    }

    namespace
    {
        constexpr float kPanelQuadHalf = 30.f;
        constexpr float kPanelQuadYOffset = 0.8f;
        constexpr uint8_t kPanelQuadAlpha = 0x78;

        void kururingTintDisp(ttyd::dispdrv::CameraId cameraId, void *user)
        {
            (void)cameraId;

            uint8_t *mobj = static_cast<uint8_t *>(user);
            const int32_t poseId = *reinterpret_cast<int32_t *>(mobj + 0x70);

            // Only while the panel sits idle - the flipping panel renders (and
            // is tinted) itself, and the overlay would float over the hole.
            const char *anim = ttyd::animdrv::animPoseGetCurrentAnim(poseId);
            if (!anim || strcmp(anim, "S_1") != 0)
                return;

            void *pose = ttyd::animdrv::animPoseGetAnimPosePtr(poseId);
            uint32_t rgba = 0;
            for (int32_t i = 0; i < sPanelTintCount; i++)
            {
                if (sPanelTints[i].pose == pose)
                {
                    rgba = sPanelTints[i].rgba;
                    break;
                }
            }
            if (!rgba)
                return;
            const uint32_t color = (rgba & 0xFFFFFF00u) | kPanelQuadAlpha;

            const float x = *reinterpret_cast<float *>(mobj + 0x38);
            const float y = *reinterpret_cast<float *>(mobj + 0x3C) + kPanelQuadYOffset;
            const float z = *reinterpret_cast<float *>(mobj + 0x40);

            float *view = reinterpret_cast<float *>(static_cast<uint8_t *>(camGetPtr(4)) + 0x11C);
            GXLoadPosMtxImm(view, 0); // GX_PNMTX0
            GXSetCurrentMtx(0);
            GXClearVtxDesc();
            GXSetVtxDesc(9, 1);  // GX_VA_POS, GX_DIRECT
            GXSetVtxDesc(11, 1); // GX_VA_CLR0, GX_DIRECT
            GXSetVtxAttrFmt(7, 9, 1, 4, 0);  // fmt7 POS XYZ F32
            GXSetVtxAttrFmt(7, 11, 1, 5, 0); // fmt7 CLR0 RGBA8
            GXSetNumChans(1);
            GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2); // COLOR0A0 unlit, vtx color
            GXSetNumTexGens(0);
            GXSetNumTevStages(1);
            GXSetTevOrder(0, 0xFF, 0xFF, 4); // stage0: no tex, COLOR0A0
            GXSetTevOp(0, 4);                // GX_PASSCLR
            GXSetBlendMode(1, 4, 5, 0);      // blend srcAlpha/invSrcAlpha
            GXSetZMode(1, 3, 0);             // z-test LEQUAL, no z-write
            GXSetAlphaCompare(7, 0, 0, 7, 0);
            GXSetCullMode(0); // GX_CULL_NONE

            GXBegin(0x80, 7, 4); // GX_QUADS
            volatile float *wgpF = reinterpret_cast<volatile float *>(0xCC008000);
            volatile uint32_t *wgpU = reinterpret_cast<volatile uint32_t *>(0xCC008000);
            const float xs[4] = {x - kPanelQuadHalf, x + kPanelQuadHalf, x + kPanelQuadHalf, x - kPanelQuadHalf};
            const float zs[4] = {z - kPanelQuadHalf, z - kPanelQuadHalf, z + kPanelQuadHalf, z + kPanelQuadHalf};
            for (int32_t i = 0; i < 4; i++)
            {
                *wgpF = xs[i];
                *wgpF = y;
                *wgpF = zs[i];
                *wgpU = color;
            }
        }
    } // namespace

    KEEP_VAR void (*g_animPoseDrawMtx_trampoline)(void *, void *, int32_t, int32_t, int32_t, float, float) = nullptr;

    KEEP_FUNC void animPoseDrawMtxHook(void *pose, void *mtx, int32_t mode, int32_t a4, int32_t a5, float f1, float f2)
    {
        for (int32_t i = 0; i < sPanelTintCount; i++)
        {
            if (sPanelTints[i].pose != pose)
                continue;
            uint8_t *p = static_cast<uint8_t *>(pose);
            if (*reinterpret_cast<uint32_t *>(p + 0xE8) & 0x40)
                *reinterpret_cast<uint32_t *>(p + 0xF0) = sPanelTints[i].rgba;
            break;
        }
        g_animPoseDrawMtx_trampoline(pose, mtx, mode, a4, a5, f1, f2);
    }

    // The mobj mains freeze during pauses/NPC talk/events, so the overlay is
    // registered from the always-running per-frame update instead.
    KEEP_FUNC void trackerPanelOverlayTick()
    {
        if (ttyd::seqdrv::seqGetSeq() != ttyd::seqdrv::SeqIndex::kGame)
            return;
        for (int32_t i = 0; i < sPanelTintCount; i++)
        {
            if (sPanelTints[i].mobj)
                ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::k3d, 8, 0.f, kururingTintDisp, sPanelTints[i].mobj);
        }
    }

    KEEP_VAR int32_t (*g_mobjKururingFloor_trampoline)(void *mobj) = nullptr;

    KEEP_FUNC int32_t mobjKururingFloorHook(void *mobjPtr)
    {
        const int32_t ret = g_mobjKururingFloor_trampoline(mobjPtr);

        mod::owr::APSettings *settings = mod::owr::gState ? mod::owr::gState->apSettings : nullptr;
        if (!settings || !settings->panelHints)
            return ret;

        static int32_t sTintTickGate = 0;
        if (--sTintTickGate <= 0)
        {
            trackerTick();
            sTintTickGate = 60;
        }

        uint8_t *mobj = static_cast<uint8_t *>(mobjPtr);
        const int32_t poseId = *reinterpret_cast<int32_t *>(mobj + 0x70);
        constexpr uint32_t kEvtGswfBase = 0xF8405B80;
        const uint32_t rawFlag = *reinterpret_cast<uint32_t *>(mobj + 0x1E4);
        const uint32_t gswf = rawFlag - kEvtGswfBase;
        if (poseId < 0 || rawFlag < kEvtGswfBase || gswf >= 0x8000)
            return ret;

        // Matches kTrackerRowColors in the tracker screen.
        gc::color4 tint;
        switch (trackerLocStatusByGswf(static_cast<uint16_t>(gswf)))
        {
            case LOC_CHECKED:
                tint = {0x9A, 0x9A, 0x9A, 0xFF};
                break;
            case LOC_AVAILABLE:
                tint = {0x66, 0xEE, 0x66, 0xFF};
                break;
            case LOC_OUT_OF_LOGIC:
                tint = {0xFA, 0x80, 0x72, 0xFF};
                break;
            default:
                // Unknown: disable the blend stage for pure vanilla rendering.
                ttyd::animdrv::animPoseSetMaterialFlagOff(poseId, 0x40);
                removePanelTint(ttyd::animdrv::animPoseGetAnimPosePtr(poseId));
                return ret;
        }
        ttyd::animdrv::animPoseSetMaterialFlagOn(poseId, 0x40);
        ttyd::animdrv::animPoseSetMaterialEvtColor(poseId, &tint);
        // The registered _animPoseDrawMtx hook re-injects the color where the
        // paper paths white it out.
        const bool changed = setPanelTint(ttyd::animdrv::animPoseGetAnimPosePtr(poseId), tint);
        if (changed)
        {
            const char *cur = ttyd::animdrv::animPoseGetCurrentAnim(poseId);
            if (cur && strcmp(cur, "S_1") == 0)
                ttyd::animdrv::animPoseSetAnim(poseId, "S_1", 1);
        }

        for (int32_t i = 0; i < sPanelTintCount; i++)
        {
            if (sPanelTints[i].pose == ttyd::animdrv::animPoseGetAnimPosePtr(poseId))
                sPanelTints[i].mobj = mobjPtr;
        }
        return ret;
    }
} // namespace mod::tracker
