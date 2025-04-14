#pragma once

#include "util.h"
#include "OWR.h"
#include "errorHandling.h"
#include <ttyd/dispdrv.h>

#include <cstdint>

namespace mod
{
    class Mod
    {
       public:
        Mod() {}
        ~Mod() {}

        owr::OWR owr_mod_;
        NpcNameToPtrErrorInfo npcNameToPtrErrorInfo;
        AnimPoseMainErrorInfo animPoseMainErrorInfo;
        HeapCorruptionInfo heapCorruptionInfo;
    };

    extern Mod *gMod;
    extern void (*mPFN_marioStMain_trampoline)();

    void updateEarly();
    void draw(ttyd::dispdrv::CameraId layerId, void *user);
} // namespace mod
