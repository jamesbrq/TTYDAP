#pragma once

#include "gc/types.h"

#include <cstdint>

namespace gc::mtx
{
    extern "C"
    {
        void PSMTXIdentity(mat3x4 *m);
        void PSMTXCopy(mat3x4 *src, mat3x4 *dst);
        void PSMTXConcat(mat3x4 *lhs, mat3x4 *rhs, mat3x4 *dst);
        void PSMTXInverse(mat3x4 *src, mat3x4 *dst);
        void PSMTXInvXpose(mat3x4 *src, mat3x4 *dst);
        void PSMTXRotRad(mat3x4 *dst, char axis, float radians);
        void PSMTXRotTrig(mat3x4 *dst, char axis, float sin, float cos);
        // local: __PSMTXRotAxisRadInternal
        void PSMTXRotAxisRad(mat3x4 *dst, gc::vec3 *axis);
        void PSMTXTrans(mtx34 *dst, float tx, float ty, float tz);
        void PSMTXTransApply(mat3x4 *src, mat3x4 *dst, float tx, float ty, float tz);
        void PSMTXScale(mat3x4 *dst, float sx, float sy, float sz);
        void PSMTXScaleApply(mat3x4 *src, mat3x4 *dst, float sx, float sy, float sz);

        void C_MTXLookAt(mat3x4 *dst, gc::vec3 *pos, gc::vec3 *up, gc::vec3 *targetPos);

        void C_MTXLightFrustum(mat3x4 *dst,
                               float top,
                               float bottom,
                               float left,
                               float right,
                               float near,
                               float su,
                               float sv,
                               float tu,
                               float tv);

        void C_MTXLightPerspective(mat3x4 *dst, float fovY, float aspectRatio, float su, float sv, float tu, float tv);

        void C_MTXLightOrtho(mat3x4 *dst,
                             float top,
                             float bottom,
                             float left,
                             float right,
                             float su,
                             float sv,
                             float tu,
                             float tv);

        void PSMTXMultVec(mat3x4 *m, gc::vec3 *src, gc::vec3 *dst);
        void PSMTXMultVecSR(mat3x4 *m, gc::vec3 *src, gc::vec3 *dst);

        void C_MTXFrustum(mat4x4 *m, float top, float bottom, float left, float right, float near, float far);
        void C_MTXPerspective(mat4x4 *m, float fovY, float aspectRatio, float near, float far);
        void C_MTXOrtho(mat4x4 *m, float top, float bottom, float left, float right, float near, float far);

        void PSMTX44Copy(mat4x4 *src, mat4x4 *dst);
        void PSMTX44Concat(mat4x4 *lhs, mat4x4 *rhs, mat4x4 *dst);
        void PSMTX44Trans(mat4x4 *dst, float tx, float ty, float tz);
        void PSMTX44Scale(mat4x4 *dst, float sx, float sy, float sz);
        void PSMTX44MultVec(mat4x4 *m, gc::vec3 *src, gc::vec3 *dst);

        void PSVECAdd(gc::vec3 *lhs, gc::vec3 *rhs, gc::vec3 *dst);
        void PSVECSubtract(gc::vec3 *lhs, gc::vec3 *rhs, gc::vec3 *dst);
        void PSVECScale(gc::vec3 *src, gc::vec3 *dst, float scale);
        void PSVECNormalize(gc::vec3 *src, gc::vec3 *dst);
        float PSVECSquareMag(gc::vec3 *v);
        float PSVECMag(gc::vec3 *v);
        float PSVECDotProduct(gc::vec3 *lhs, gc::vec3 *rhs);
        void PSVECCrossProduct(gc::vec3 *lhs, gc::vec3 *rhs, gc::vec3 *dst);
        void C_VECHalfAngle(gc::vec3 *lhs, gc::vec3 *rhs, gc::vec3 *dst);
        void C_VECReflect(gc::vec3 *incident, gc::vec3 *normal, gc::vec3 *dst);
        float PSVECSquareDistance(gc::vec3 *lhs, gc::vec3 *rhs);
        float PSVECDistance(gc::vec3 *lhs, gc::vec3 *rhs);
    }
} // namespace gc::mtx
