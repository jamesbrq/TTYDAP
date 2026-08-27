#include "MirrorMode.h"

#include "OWR.h"
#include "StateManager.h"
#include "patch.h"
#include "visibility.h"

#include <ttyd/dispdrv.h>
#include <ttyd/mario.h>
#include <ttyd/mariost.h>
#include <ttyd/seq_mapchange.h>
#include <ttyd/system.h>
#include <gc/mtx.h>

#include <cstdint>
#include <cstring>

extern "C"
{
    void *camGetPtr(int cameraId);
    void camMain();
    void marioGetStick(float *angle, float *sens);
    void GXSetCullMode(int32_t mode);
    void test_kururing_mapdisp(int32_t camId);
    void camShiftMain(void *cam, void *player, void *arg);
    void mot_plane(void *a, void *b, void *c);
    void mot_plane_post(void *a, void *b, void *c);
    void mot_roll(void *a, void *b, void *c);
    void yoshi_use(void *partyNpc, void *b, void *c);
    void GXLoadPosMtxImm(float (*mtx)[4], uint32_t id);
    void animPoseDrawMtx(int32_t poseId, float (*mtx)[4], int32_t mode, float f1, float f2);
    void btlDispTexPlane(uint32_t texId, void *params, float x, float y, float z, float sclX, float sclY);
    void btlDispTexPlane2(gc::mat3x4 *m, uint32_t a, void *b, uint32_t c, uint32_t d);
    int32_t btlevtcmd_BtlStageObjSetRotate(void *evt, int32_t firstCall);
    int32_t btlevtcmd_BtlStageObjGetRotate(void *evt, int32_t firstCall);
    float evtGetFloat(void *evt, uint32_t v);
    void evtSetFloat(void *evt, uint32_t v, float value);
    int32_t evtGetValue(void *evt, uint32_t v);
    extern uint8_t *_battleWorkPointer;
    void iconNumberDispGx3D(gc::mat3x4 *m, int32_t number, int32_t noFixedWidth, void *color);
    void iconDispGxCol(gc::mat3x4 *m, uint32_t flags, uint32_t iconId, void *color);
    extern uint32_t main_cullingDlTable[]; // mapdrv culling$678
}

namespace mod::mirror
{
    namespace
    {
        using CameraId = ttyd::dispdrv::CameraId;

        bool g_effCullNoSwapOnce = false;
        // Scoped variant for whole animdrv draws.
        int32_t g_effCullNoSwapScope = 0;

        using EvtFn = int32_t (*)(void *evt, int32_t firstCall);
        using MotFn = void (*)(void *, void *, void *);

        void (*g_camMain_trampoline)() = nullptr;
        void (*g_marioGetStick_trampoline)(float *, float *) = nullptr;
        void (*g_GXSetCullMode_trampoline)(int32_t) = nullptr;
        void (*g_testKururingMapdisp_trampoline)(int32_t) = nullptr;
        void (*g_camShiftMain_trampoline)(void *, void *, void *) = nullptr;
        EvtFn g_btlStageObjSetRotate_trampoline = nullptr;
        EvtFn g_btlStageObjGetRotate_trampoline = nullptr;
        MotFn g_motPlane_trampoline = nullptr;
        MotFn g_motPlanePost_trampoline = nullptr;
        MotFn g_motRoll_trampoline = nullptr;
        MotFn g_yoshiUse_trampoline = nullptr;
        void (*g_dispDraw_trampoline)(CameraId) = nullptr;

        // Camera whose disp callbacks are currently rendering; 0xFF outside
        // dispDraw (e.g. during display-list recording at map load).
        uint8_t g_curDispCam = 0xFF;

        bool active()
        {
            if (!(owr::gState && owr::gState->apSettings && owr::gState->apSettings->mirrorMode))
            {
                return false;
            }

            // Never mirror the title-screen demo reel (and the intro), which
            // runs on the dmo area rel.
            const OSModuleInfo *rel = ::_globalWorkPtr ? ::_globalWorkPtr->relocationBase : nullptr;
            if (rel && rel->id == ttyd::seq_mapchange::RelId::DMO)
            {
                return false;
            }
            return true;
        }

        void syncCullingDlTable(bool on)
        {
            uint32_t *cullingDlTable = main_cullingDlTable;
            cullingDlTable[0] = on ? 2u : 1u; // FRONT slot
            cullingDlTable[1] = on ? 1u : 2u; // BACK slot
        }

        bool mirroredCamera(uint8_t id)
        {
            // k3d, both 3D effect cams, and kBackground (bgdrv draws through
            // cam 3's view mtx); k3dImage and kShadow stay unmirrored.
            return id == static_cast<uint8_t>(CameraId::k3d) || id == static_cast<uint8_t>(CameraId::k3dEffectA) ||
                   id == static_cast<uint8_t>(CameraId::k3dEffectB) ||
                   id == static_cast<uint8_t>(CameraId::kBackground);
        }
    } // namespace

    KEEP_FUNC void camMainHook()
    {
        g_camMain_trampoline();

        const bool on = active();
        syncCullingDlTable(on);

        if (!on)
        {
            return;
        }

        for (uint8_t id = 0; id < 13; id++)
        {
            if (!mirroredCamera(id))
            {
                continue;
            }
            float *view = reinterpret_cast<float *>(static_cast<uint8_t *>(camGetPtr(id)) + 0x11C);

            const float *r1 = view + 4;
            const float *r2 = view + 8;
            const float det = view[0] * (r1[1] * r2[2] - r1[2] * r2[1]) - view[1] * (r1[0] * r2[2] - r1[2] * r2[0]) +
                              view[2] * (r1[0] * r2[1] - r1[1] * r2[0]);
            if (det <= 0.f)
            {
                continue;
            }

            view[0] = -view[0];
            view[1] = -view[1];
            view[2] = -view[2];
            view[3] = -view[3];

            // The bgdrv backdrop lives in [0..608] screen-space coordinates,
            // so its mirror pivot is x=304, not the camera axis.
            if (id == static_cast<uint8_t>(CameraId::kBackground))
            {
                view[3] += 608.f;
            }
        }
    }

    KEEP_FUNC void marioGetStickHook(float *angle, float *sens)
    {
        g_marioGetStick_trampoline(angle, sens);
        if (!active())
        {
            return;
        }

        ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
        // Evt-forced movement returns a world-space angle with no stick/camera
        // component; leave it alone.
        if (player->flags1 & 0x20)
        {
            return;
        }

        float mirrored = 2.f * player->unk_19c - *angle;
        while (mirrored < 0.f)
        {
            mirrored += 360.f;
        }
        while (mirrored >= 360.f)
        {
            mirrored -= 360.f;
        }
        *angle = mirrored;
    }

    KEEP_FUNC void testKururingMapdispHook(int32_t camId)
    {
        if (!active())
        {
            g_testKururingMapdisp_trampoline(camId);
            return;
        }

        float *view = reinterpret_cast<float *>(static_cast<uint8_t *>(camGetPtr(camId)) + 0x11C);
        view[0] = -view[0];
        view[1] = -view[1];
        view[2] = -view[2];
        view[3] = -view[3];

        const uint8_t savedCam = g_curDispCam;
        g_curDispCam = static_cast<uint8_t>(CameraId::k3d);
        g_testKururingMapdisp_trampoline(camId);
        g_curDispCam = savedCam;

        // Restore the capture's own view for the rest of kururing_capture
        // (it restores the camera from a backup afterwards regardless).
        view[0] = -view[0];
        view[1] = -view[1];
        view[2] = -view[2];
        view[3] = -view[3];
    }

    KEEP_FUNC void dispDrawHook(CameraId cameraId)
    {
        g_curDispCam = static_cast<uint8_t>(cameraId);
        g_effCullNoSwapOnce = false; // never leak a latch across cameras
        g_dispDraw_trampoline(cameraId);
        g_curDispCam = 0xFF;
        g_effCullNoSwapOnce = false;
    }

    KEEP_FUNC void mirrorEffLoadPosMtx(float (*mtx)[4], uint32_t id)
    {
        if (!active())
        {
            GXLoadPosMtxImm(mtx, id);
            return;
        }

        float m[3][4];
        for (int32_t r = 0; r < 3; r++)
        {
            m[r][0] = -mtx[r][0];
            m[r][1] = mtx[r][1];
            m[r][2] = mtx[r][2];
            m[r][3] = mtx[r][3];
        }
        GXLoadPosMtxImm(m, id);
        g_effCullNoSwapOnce = true;
    }

    // btlDispTexPlane(texId, params, x, y, z, sclX, sclY) builds its matrix
    // internally as Trans * Scale; the reflection about battle center is
    // x -> -x plus a mirrored face via sclX -> -sclX.
    KEEP_FUNC void mirrorLvupTexPlane(uint32_t texId, void *params, float x, float y, float z, float sclX, float sclY)
    {
        if (active())
        {
            x = -x;
            sclX = -sclX;
            g_effCullNoSwapOnce = true;
        }
        btlDispTexPlane(texId, params, x, y, z, sclX, sclY);
    }

    // btlDispTexPlane2 takes the model matrix in r3 (verified: PSMTXCopy of
    // its first arg); reflect via row 0.
    KEEP_FUNC void mirrorLvupTexPlane2(gc::mat3x4 *m, uint32_t a, void *b, uint32_t c, uint32_t d)
    {
        if (!active())
        {
            btlDispTexPlane2(m, a, b, c, d);
            return;
        }
        gc::mat3x4 refl = *m;
        refl.a[0] = -refl.a[0];
        refl.a[1] = -refl.a[1];
        refl.a[2] = -refl.a[2];
        refl.a[3] = -refl.a[3];
        g_effCullNoSwapOnce = true;
        btlDispTexPlane2(&refl, a, b, c, d);
    }

    // Stat values and icon under each picker board: matrix in r3, same
    // world reflection as the boards.
    // Level-up "spotlight" stage object: SetRotate stores mirrored rotY/rotZ
    // (honoring the -250000000 leave-unchanged sentinel), and GetRotate
    // un-mirrors its reads so the evt keeps interpolating in vanilla space.
    KEEP_FUNC int32_t btlStageObjGetRotateHook(void *evt, int32_t firstCall)
    {
        const int32_t ret = g_btlStageObjGetRotate_trampoline(evt, firstCall);
        if (!active())
        {
            return ret;
        }

        uint32_t *args = *reinterpret_cast<uint32_t **>(static_cast<uint8_t *>(evt) + 0x18);
        const char *name = reinterpret_cast<const char *>(evtGetValue(evt, args[0]));
        if (!name || strcmp(name, "spotlight") != 0)
        {
            return ret;
        }

        evtSetFloat(evt, args[2], -evtGetFloat(evt, args[2])); // rot y
        evtSetFloat(evt, args[3], -evtGetFloat(evt, args[3])); // rot z
        return ret;
    }

    KEEP_FUNC int32_t btlStageObjSetRotateHook(void *evt, int32_t firstCall)
    {
        const int32_t ret = g_btlStageObjSetRotate_trampoline(evt, firstCall);
        if (!active() || !_battleWorkPointer)
        {
            return ret;
        }

        uint32_t *args = *reinterpret_cast<uint32_t **>(static_cast<uint8_t *>(evt) + 0x18);
        const char *name = reinterpret_cast<const char *>(evtGetValue(evt, args[0]));
        if (!name || strcmp(name, "spotlight") != 0)
        {
            return ret;
        }

        constexpr uint32_t kEvtNoChange = 0xF1194D80; // -250000000
        const bool setY = args[2] != kEvtNoChange;
        const bool setZ = args[3] != kEvtNoChange;
        if (!setY && !setZ)
        {
            return ret;
        }

        for (int32_t i = 0; i < 32; i++)
        {
            uint8_t *obj = _battleWorkPointer + 0x1715C + i * 0x7C;
            if (*reinterpret_cast<int32_t *>(obj) <= 0)
                continue;
            const char *objName = **reinterpret_cast<const char ***>(obj + 0x64);
            if (!objName || strcmp(objName, "spotlight") != 0)
                continue;
            if (setY)
            {
                float *ry = reinterpret_cast<float *>(obj + 0x20);
                *ry = -*ry;
            }
            if (setZ)
            {
                float *rz = reinterpret_cast<float *>(obj + 0x24);
                *rz = -*rz;
            }
            break;
        }
        return ret;
    }

    KEEP_FUNC void mirrorLvupIconNumber(gc::mat3x4 *m, int32_t number, int32_t noFixedWidth, void *color)
    {
        if (!active())
        {
            iconNumberDispGx3D(m, number, noFixedWidth, color);
            return;
        }
        gc::mat3x4 refl = *m;
        refl.a[0] = -refl.a[0];
        refl.a[1] = -refl.a[1];
        refl.a[2] = -refl.a[2];
        refl.a[3] = -refl.a[3];
        g_effCullNoSwapOnce = true;
        iconNumberDispGx3D(&refl, number, noFixedWidth, color);
    }

    KEEP_FUNC void mirrorLvupIconCol(gc::mat3x4 *m, uint32_t flags, uint32_t iconId, void *color)
    {
        if (!active())
        {
            iconDispGxCol(m, flags, iconId, color);
            return;
        }
        gc::mat3x4 refl = *m;
        refl.a[0] = -refl.a[0];
        refl.a[1] = -refl.a[1];
        refl.a[2] = -refl.a[2];
        refl.a[3] = -refl.a[3];
        g_effCullNoSwapOnce = true;
        iconDispGxCol(&refl, flags, iconId, color);
    }


    KEEP_FUNC void mirrorEffConcatReflect(gc::mat3x4 *a, gc::mat3x4 *b, gc::mat3x4 *dst)
    {
        gc::mtx::PSMTXConcat(a, b, dst);
        if (!active())
        {
            return;
        }
        dst->a[0] = -dst->a[0]; // column 0 of each row: elements 0, 4, 8
        dst->a[4] = -dst->a[4];
        dst->a[8] = -dst->a[8];
        g_effCullNoSwapOnce = true;
    }

    KEEP_FUNC void mirrorEffAnimPoseDrawMtx(int32_t poseId, float (*mtx)[4], int32_t mode, float f1, float f2)
    {
        if (!active())
        {
            animPoseDrawMtx(poseId, mtx, mode, f1, f2);
            return;
        }

        float m[3][4];
        for (int32_t r = 0; r < 3; r++)
        {
            m[r][0] = -mtx[r][0];
            m[r][1] = mtx[r][1];
            m[r][2] = mtx[r][2];
            m[r][3] = mtx[r][3];
        }
        g_effCullNoSwapScope++;
        animPoseDrawMtx(poseId, m, mode, f1, f2);
        g_effCullNoSwapScope--;
    }

    KEEP_FUNC void GXSetCullModeHook(int32_t mode)
    {
        if (active() && mirroredCamera(g_curDispCam))
        {
            // A counter-mirrored effect draw keeps its vanilla cull mode.
            if (g_effCullNoSwapOnce || g_effCullNoSwapScope > 0)
            {
                g_effCullNoSwapOnce = false;
                g_GXSetCullMode_trampoline(mode);
                return;
            }
            if (mode == 1) // GX_CULL_FRONT
            {
                mode = 2; // GX_CULL_BACK
            }
            else if (mode == 2)
            {
                mode = 1;
            }
        }
        g_GXSetCullMode_trampoline(mode);
    }

    namespace
    {
        void callWithMirroredStickX(MotFn real, void *a, void *b, void *c)
        {
            ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
            if (!active() || !player)
            {
                real(a, b, c);
                return;
            }

            int8_t *stickX = reinterpret_cast<int8_t *>(reinterpret_cast<uint8_t *>(player) + 0x252);
            const int8_t saved = *stickX;
            *stickX = (saved == -128) ? 127 : static_cast<int8_t>(-saved);
            real(a, b, c);
            *stickX = saved;
        }
    } // namespace

    KEEP_FUNC void motPlaneHook(void *a, void *b, void *c)
    {
        callWithMirroredStickX(g_motPlane_trampoline, a, b, c);
    }

    KEEP_FUNC void motPlanePostHook(void *a, void *b, void *c)
    {
        callWithMirroredStickX(g_motPlanePost_trampoline, a, b, c);
    }

    KEEP_FUNC void motRollHook(void *a, void *b, void *c)
    {
        callWithMirroredStickX(g_motRoll_trampoline, a, b, c);
    }

    // Yoshi riding reads the party NPC's own buffered stick at +0x158/+0x159
    // (never marioGetStick or player+0x252), so it needs its own X negation.
    KEEP_FUNC void yoshiUseHook(void *partyNpc, void *b, void *c)
    {
        if (!active() || !partyNpc)
        {
            g_yoshiUse_trampoline(partyNpc, b, c);
            return;
        }

        int8_t *stickX = reinterpret_cast<int8_t *>(static_cast<uint8_t *>(partyNpc) + 0x158);
        const int8_t saved = *stickX;
        *stickX = (saved == -128) ? 127 : static_cast<int8_t>(-saved);
        g_yoshiUse_trampoline(partyNpc, b, c);
        *stickX = saved;
    }

    KEEP_FUNC int32_t mirrorKeyGetStickX(int32_t padId)
    {
        const int32_t v = ttyd::system::keyGetStickX(static_cast<uint32_t>(padId));
        if (!active())
            return v;
        const int8_t b = static_cast<int8_t>(v);
        return (b == -128) ? 127 : -b;
    }

    KEEP_FUNC void camShiftMainHook(void *cam, void *player, void *arg)
    {
        if (!active() || !player)
        {
            g_camShiftMain_trampoline(cam, player, arg);
            return;
        }

        int8_t *stickX = reinterpret_cast<int8_t *>(static_cast<uint8_t *>(player) + 0x252);
        const int8_t saved = *stickX;
        *stickX = (saved == -128) ? 127 : static_cast<int8_t>(-saved);
        g_camShiftMain_trampoline(cam, player, arg);
        *stickX = saved;
    }

    KEEP_FUNC void SyncCullingTable(uint32_t relId)
    {
        const bool flagOn = owr::gState && owr::gState->apSettings && owr::gState->apSettings->mirrorMode;
        syncCullingDlTable(flagOn && relId != static_cast<uint32_t>(ttyd::seq_mapchange::RelId::DMO));
    }

    KEEP_FUNC void InstallMirrorModeHooks()
    {
        g_camMain_trampoline = patch::hookFunction(camMain, camMainHook);
        g_marioGetStick_trampoline =
            patch::hookFunction(marioGetStick, marioGetStickHook);
        g_dispDraw_trampoline = patch::hookFunction(ttyd::dispdrv::dispDraw, dispDrawHook);
        g_GXSetCullMode_trampoline =
            patch::hookFunction(GXSetCullMode, GXSetCullModeHook);
        g_testKururingMapdisp_trampoline =
            patch::hookFunction(test_kururing_mapdisp, testKururingMapdispHook);
        g_camShiftMain_trampoline =
            patch::hookFunction(camShiftMain, camShiftMainHook);
        g_motPlane_trampoline = patch::hookFunction(mot_plane, motPlaneHook);
        g_motPlanePost_trampoline =
            patch::hookFunction(mot_plane_post, motPlanePostHook);
        g_motRoll_trampoline = patch::hookFunction(mot_roll, motRollHook);
        g_yoshiUse_trampoline = patch::hookFunction(yoshi_use, yoshiUseHook);
        g_btlStageObjSetRotate_trampoline =
            patch::hookFunction(btlevtcmd_BtlStageObjSetRotate, btlStageObjSetRotateHook);
        g_btlStageObjGetRotate_trampoline =
            patch::hookFunction(btlevtcmd_BtlStageObjGetRotate, btlStageObjGetRotateHook);
    }
} // namespace mod::mirror
