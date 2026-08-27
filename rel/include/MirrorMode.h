#pragma once

#include <cstdint>
#include <gc/types.h>

namespace mod::mirror
{
    void InstallMirrorModeHooks();

    void SyncCullingTable(uint32_t relId);

    void mirrorEffLoadPosMtx(float (*mtx)[4], uint32_t id);
    // keyGetStickX negated while mirrored; bl-retarget over screen-relative
    // raw-stick checks in map code (gra_05 rock push).
    int32_t mirrorKeyGetStickX(int32_t padId);
    void mirrorEffAnimPoseDrawMtx(int32_t poseId, float (*mtx)[4], int32_t mode, float f1, float f2);
    void mirrorEffConcatReflect(gc::mat3x4 *a, gc::mat3x4 *b, gc::mat3x4 *dst);
    // Level-up stat-picker board draws (world reflection about battle center).
    void mirrorLvupTexPlane(uint32_t texId, void *params, float x, float y, float z, float sclX, float sclY);
    void mirrorLvupTexPlane2(gc::mat3x4 *m, uint32_t a, void *b, uint32_t c, uint32_t d);
    void mirrorLvupIconNumber(gc::mat3x4 *m, int32_t number, int32_t noFixedWidth, void *color);
    void mirrorLvupIconCol(gc::mat3x4 *m, uint32_t flags, uint32_t iconId, void *color);
} // namespace mod::mirror
