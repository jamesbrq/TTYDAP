#pragma once

#include <cstdint>

namespace mod::tracker
{
    struct NodeInfo
    {
        bool valid;         // tracker.bin loaded and node resolved
        bool gateReachable; // fast-travel gate region is in logic
        uint16_t total;     // enabled locations across the node's display regions
        uint16_t checked;
        uint16_t available; // reachable and not yet checked
    };

    static constexpr int32_t kEverFlagBase = 6600;
    static constexpr int32_t kEverGswStarPiece = 1801;

    // Called from pouchGetItemHook on every successful grant.
    void recordItemObtained(int32_t item);

    void trackerTick();

    bool trackerReady();

    NodeInfo trackerQueryPrefix(const char *prefix);

    enum LocStatus : uint8_t
    {
        LOC_CHECKED = 0,
        LOC_AVAILABLE = 1,
        LOC_OUT_OF_LOGIC = 2,
    };

    int32_t trackerBuildNodeList(const char *prefix);

    bool trackerGetListRow(int32_t row, const char **name, uint8_t *status);

    uint8_t trackerLocStatusByGswf(uint16_t gswfId);

    // Kururing (flip-panel) tint: hooked over the DOL's mobj_kururing_floor
    // per-frame handler; colors the panel model by its check's tracker status.
    extern int32_t (*g_mobjKururingFloor_trampoline)(void *mobj);
    int32_t mobjKururingFloorHook(void *mobj);

    // Re-injects registered panel tints that the idle paper-composite draw
    // whites out (hooked over the DOL's _animPoseDrawMtx).
    extern void (*g_animPoseDrawMtx_trampoline)(void *, void *, int32_t, int32_t, int32_t, float, float);
    void animPoseDrawMtxHook(void *pose, void *mtx, int32_t mode, int32_t a4, int32_t a5, float f1, float f2);

    // Drops all registered panel tints; called when an area rel is unlinked so
    // reused pose slots can't inherit a stale tint.
    void trackerClearPanelTints();

    // Registers the kururing floor-tint overlays each frame (call from the
    // always-running update; mobj mains freeze during pauses/events).
    void trackerPanelOverlayTick();
} // namespace mod::tracker
