#pragma once

#include <cstdint>
#include "ttyd/evtmgr.h"

struct NpcEntry;

namespace mod::boss_preview
{
    bool IsBossSceneNpc(const NpcEntry *npc);
    bool UsesOriginalDragonScene();
    void InstallHooks();
    void BeforeAutoRelease(int32_t group);
    void Update();
    int32_t SceneCamera(ttyd::evtmgr::EvtEntry *evt, bool firstCall);
    int32_t SceneRelativeCamera(ttyd::evtmgr::EvtEntry *evt, bool firstCall);
    bool HasSmorgReplacement();
    void SetSmorgFormed(bool formed);
    int32_t SmorgCamera(ttyd::evtmgr::EvtEntry *evt, bool firstCall);
    int32_t ArenaCamera(ttyd::evtmgr::EvtEntry *evt, bool firstCall);
    void SetPalaceEncounter(int encounter);
    void SetMagnus2Revealed();
    int32_t Magnus2Camera(ttyd::evtmgr::EvtEntry *evt, bool firstCall);
    int32_t MagnusTransformCamera(ttyd::evtmgr::EvtEntry *evt, bool firstCall);
    int32_t SceneTextAnchor(ttyd::evtmgr::EvtEntry *evt, bool firstCall);
    int32_t SceneDialogue(ttyd::evtmgr::EvtEntry *evt, bool firstCall);
}
