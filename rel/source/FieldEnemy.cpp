#include "FieldEnemy.h"
#include "BossPreview.h"
#include "OWR.h"
#include "evt_cmd.h"
#include "ttyd/evt_snd.h"
#include "ttyd/system.h"
#include <algorithm>
#include "patch.h"
#include "visibility.h"
#include "ttyd/animdrv.h"
#include "ttyd/battle_unit.h"
#include "ttyd/battle_database_common.h"
#include "ttyd/evt_npc.h"
#include "ttyd/evtmgr_cmd.h"
#include "ttyd/seq_mapchange.h"
#include "ttyd/seqdrv.h"
#include "ttyd/hitdrv.h"
#include <cstring>

namespace mod::field_enemy {
namespace {
using namespace ttyd;
using SetupFn = int32_t (*)(evtmgr::EvtEntry *, bool);
SetupFn sSetup;
void (*sExecInit)();
bool (*sWaitInit)();
SetupFn sSetTribe;
SetupFn sSlaveEntry;
SetupFn sSetAnim;
void (*sDelete)(NpcEntry *);
void (*sDeleteGroup)(NpcEntry *);
#ifdef TTYD_US
// Native field AI records shared by every area.
struct AiProfile {
    const char *name;
    uint32_t flags;
    void *init;
    void *regular;
    void *dead;
    void *find;
    void *lost;
    void *returning;
    void *blow;
};
static_assert(sizeof(AiProfile) == 0x24);

struct Pending {
    NpcEntry *npc;
    NpcTribeDescription *tribe;
    bool applied;
    const AiProfile *ai;
    uint32_t pose;
    bool replaceAi;
};
// Pending model loads belong to one setup operation. Active records are
// separately reset on map load, before any room NPC addresses can be reused.
Pending sPending[128];
Pending sActors[128];
int sActorCount;
battle_database_common::BattleGroupSetup *sGroups[150];
int sGroupCount;
int sCount;
evtmgr::EvtEntry *sOwner;
bool sBatchDeferred;

EVT_DECLARE_USER_FUNC(PlantPause, 1)
int32_t PlantPause(evtmgr::EvtEntry *evt, bool) {
    evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], 600 + system::irand(1600));
    return 2;
}
EVT_DECLARE_USER_FUNC(PlantDestination, 2)
int32_t PlantDestination(evtmgr::EvtEntry *evt, bool) {
    auto *npc = static_cast<NpcEntry *>(evt->wThisPtr);
    if (!npc) return 2;
    // Stay inside the existing territory, with a small margin at its edges.
    float rx = std::max(0.0f, npc->wTerritoryLoiter.x * 0.45f);
    float rz = std::max(0.0f, npc->wTerritoryLoiter.z * 0.45f);
    float dx = (static_cast<float>(system::irand(2001)) / 1000.0f - 1.0f) * rx;
    float dz = (static_cast<float>(system::irand(2001)) / 1000.0f - 1.0f) * rz;
    if (npc->territoryType == NpcTerritoryType::kNothing) { dx = 0; dz = 0; }
    if (npc->territoryType == NpcTerritoryType::kCircle) {
        const float factor = 0.7071f;
        dx *= factor; dz *= factor;
    }
    evtmgr_cmd::evtSetFloat(evt, evt->evtArguments[0], npc->wTerritoryBase.x + dx);
    evtmgr_cmd::evtSetFloat(evt, evt->evtArguments[1], npc->wTerritoryBase.z + dz);
    return 2;
}
// Use the native plant's burrow/emerge poses, sounds and movement flags.
// The regular event is interrupted by the native detection/attack scripts.
EVT_BEGIN(PlantRoam)
    USER_FUNC(evt_npc::evt_npc_flag_onoff, 0, PTR("me"), 0x200000)
    USER_FUNC(PlantPause, LW(0))
    WAIT_MSEC(LW(0))
    DO(0)
        USER_FUNC(PlantDestination, LW(1), LW(2))
        USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(3), LW(4), LW(5))
        USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_FLD_ENM_PAKKUN_MOVE1"), LW(3), LW(4), LW(5), 0)
        USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), PTR("PKF_E_2"))
        WAIT_FRM(10)
        USER_FUNC(evt_npc::evt_npc_flag_onoff, 1, PTR("me"), 0x20)
        USER_FUNC(evt_npc::evt_npc_move_position, PTR("me"), LW(1), LW(2), 200, 0, 4)
        USER_FUNC(evt_npc::evt_npc_get_position, PTR("me"), LW(3), LW(4), LW(5))
        USER_FUNC(evt_snd::evt_snd_sfxon_3d, PTR("SFX_FLD_ENM_PAKKUN_MOVE2"), LW(3), LW(4), LW(5), 0)
        USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), PTR("PKF_E_1"))
        WAIT_FRM(10)
        USER_FUNC(evt_npc::evt_npc_flag_onoff, 0, PTR("me"), 0x20)
        USER_FUNC(evt_npc::evt_npc_set_anim, PTR("me"), PTR("PKF_S_1"))
        USER_FUNC(PlantPause, LW(0))
        WAIT_MSEC(LW(0))
    WHILE()
    RETURN()
EVT_END()

struct Species { int id; int tribe; int ai; };
// Shared models need distinct tribe records (e.g. winged/spiked Goombas).
// Arena-only species without native roaming AI use the matching walking or
// flying family, with their own tribe's animations and sounds.
constexpr Species kSpecies[] = {
    {0x01, 214, 4},
    {0x02, 216, 6},
    {0x03, 215, 5},
    {0x04, 310, 40},
    {0x05, 309, 40},
    {0x0e, 242, 7},
    {0x0f, 243, 8},
    {0x10, 248, 16},
    {0x11, 223, 14},
    {0x12, 238, 21},
    {0x13, 258, 23},
    {0x16, 36, 14},
    {0x18, 286, 29},
    {0x19, 261, 28},
    {0x1a, 264, 22},
    {0x1b, 266, 27},
    {0x1c, 271, 24},
    {0x1d, 268, 25},
    {0x24, 214, 4},
    {0x25, 246, 7},
    {0x26, 247, 8},
    {0x27, 233, 20},
    {0x28, 280, 6},
    {0x29, 287, 4},
    {0x2a, 288, 21},
    {0x2b, 283, 4},
    {0x2c, 274, 30},
    {0x2d, 275, 30},
    {0x2e, 230, 11},
    {0x2f, 282, 7},
    {0x30, 291, 8},
    {0x31, 314, 42},
    {0x33, 315, 42},
    {0x35, 316, 42},
    {0x37, 308, 45},
    {0x38, 292, 43},
    {0x39, 294, 43},
    {0x3a, 293, 43},
    {0x3b, 306, 44},
    {0x3c, 307, 45},
    {0x42, 217, 4},
    {0x43, 219, 6},
    {0x44, 218, 5},
    {0x45, 252, 34},
    {0x46, 253, 35},
    {0x47, 236, 22},
    {0x48, 225, 9},
    {0x49, 226, 11},
    {0x4a, 239, 32},
    {0x4b, 146, 33},
    {0x54, 159, 36},
    {0x55, 302, 36},
    {0x56, 249, 16},
    {0x57, 250, 16},
    {0x58, 262, 28},
    {0x59, 228, 13},
    {0x5a, 254, 4}, // Launcher model with walking AI; never spawn field projectiles.
    {0x5b, 255, 6},
    {0x5c, 304, 37},
    {0x67, 284, 29},
    {0x68, 234, 20},
    {0x69, 227, 13},
    {0x6a, 147, 33},
    {0x70, 285, 29},
    {0x71, 263, 28},
    {0x72, 235, 22},
    {0x73, 269, 25},
    {0x75, 270, 25},
    {0x77, 273, 39},
    {0x78, 272, 24},
    {0x7b, 240, 32},
    {0x7c, 303, 36},
    {0x7d, 256, 4}, // Golden launcher uses the same safe field fallback.
    {0x7e, 257, 6},
    {0x7f, 301, 44},
    {0x80, 296, 38},
    {0x82, 196, 15},
    {0x83, 197, 15},
    {0x99, 220, 4},
    {0x9a, 222, 6},
    {0x9b, 221, 5},
    {0x9c, 244, 7},
    {0x9d, 245, 8},
    {0x9e, 275, 30},
    {0x9f, 281, 6},
    {0xa1, 295, 38},
    {0xa2, 260, 28},
    {0xa3, 311, 40},
    {0xa4, 267, 27},
    {0xa5, 259, 23},
    {0xa6, 265, 29},
    {0xa7, 241, 32},
    {0xa8, 305, 37},
    {0xa9, 297, 38},
};
const Species *FindSpecies(int id) {
    for (const auto &species : kSpecies) if (species.id == id) return &species;
    return nullptr;
}
Pending *Actor(NpcEntry *npc) {
    if (!npc) return nullptr;
    if (npc->master) npc = npc->master;
    for (int i = 0; i < sActorCount; ++i)
        if (sActors[i].npc == npc && (npc->flags & 1) &&
            npc->tribe == sActors[i].tribe && npc->poseId == sActors[i].pose) return &sActors[i];
    return nullptr;
}
NpcEntry *ArgumentNpc(evtmgr::EvtEntry *evt) {
    auto *name = reinterpret_cast<const char *>(evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]));
    return evt_npc::evtNpcNameToPtr_NoAssert(evt, name);
}
void StopNpc(NpcEntry *npc) {
    if (npc->initEvtId) evtmgr::evtDeleteID(npc->initEvtId);
    if (npc->regularEvtId) evtmgr::evtDeleteID(npc->regularEvtId);
    npc->initEvtId = npc->regularEvtId = 0;
}
bool HasNativeFieldAi(NpcEntry *npc) {
    auto *profiles = reinterpret_cast<const AiProfile *>(0x80338270);
    for (int i = 3; i <= 45; ++i)
        if (npc->regularEvtCode && npc->regularEvtCode == profiles[i].regular &&
            (!npc->initEvtCode || npc->initEvtCode == profiles[i].init)) return true;
    return false;
}
bool IsEncounterEnemy(NpcEntry *npc) {
    // Battle setup IDs can also be populated on friendly NPCs. Classify the
    // original tribe before accepting its encounter configuration.
    if (Actor(npc)) return true;
    auto *tribes = reinterpret_cast<NpcTribeDescription *>(0x803316d0);
    for (const auto &species : kSpecies) {
        if (npc->tribe != &tribes[species.tribe]) continue;
        // These tribes are shared with non-combat characters and scenes.
        if (species.tribe == 36 || species.tribe == 146 ||
            species.tribe == 147 || species.tribe == 159)
            return HasNativeFieldAi(npc);
        return true;
    }
    // Native Magikoopa ground/air variants have separate tribe records.
    for (int i = 312; i <= 321; ++i)
        if (npc->tribe == &tribes[i]) return HasNativeFieldAi(npc);
    return false;
}
void CaptureNpc(NpcEntry *npc) {
    if (!npc || !npc->tribe || !npc->tribe->modelName || npc->master || sCount >= 128) return;
    if (boss_preview::IsBossSceneNpc(npc) || !IsEncounterEnemy(npc)) return;
    auto *group = static_cast<battle_database_common::BattleGroupSetup *>(npc->battleInfo.pConfiguration);
    bool registered = false;
    for (int i = 0; i < sGroupCount; ++i) if (sGroups[i] == group) registered = true;
    if (!registered || !group->enemy_data || group->num_enemies < 1) return;
    auto *kind = group->enemy_data[0].unit_kind_params;
    const auto *species = kind ? FindSpecies(kind->unit_type) : nullptr;
    if (!species) return;
    auto *tribe = reinterpret_cast<NpcTribeDescription *>(0x803316d0) + species->tribe;
    auto *ai = reinterpret_cast<const AiProfile *>(0x80338270) + species->ai;
    const bool replaceAi = HasNativeFieldAi(npc);
    if (auto *actor = Actor(npc))
        if (actor->tribe == tribe && actor->ai == ai &&
            (actor->replaceAi || !replaceAi)) return;
    for (int i = 0; i < sCount; ++i) if (sPending[i].npc == npc) return;
    // A battle configuration alone does not activate field AI. Room scripts
    // install it later for sleeping guards, ambushes and puzzle encounters.
    if (replaceAi) {
        // Stop npcMain from restarting movement while model loading and
        // replacement initialization leave native slave parts unavailable.
        npc->flags &= ~2u;
        StopNpc(npc);
        // Discard the old species' extra parts before initializing the new ones.
        for (int i = 0; i < 4; ++i) {
            auto *slave = npc->slaves[i];
            if (slave) { StopNpc(slave); npcDelete(slave); npc->slaves[i] = nullptr; }
        }
        std::memset(npc->wUnitWork, 0, sizeof(npc->wUnitWork));
    }
    sPending[sCount++] = {npc, tribe, false, ai, npc->poseId, replaceAi};
}
EVT_DECLARE_USER_FUNC(RegularParameters, 2)
int32_t RegularParameters(evtmgr::EvtEntry *evt, bool) {
    auto *actor = Actor(static_cast<NpcEntry *>(evt->wThisPtr));
    void *regular = actor ? actor->ai->regular : nullptr;
    if (actor && actor->ai == reinterpret_cast<const AiProfile *>(0x80338660))
        regular = const_cast<int32_t *>(PlantRoam);
    evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], reinterpret_cast<int32_t>(regular));
    evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], system::irand(900));
    return 2;
}
EVT_BEGIN(RegularEvent)
    USER_FUNC(RegularParameters, LW(14), LW(15))
    WAIT_MSEC(LW(15))
    IF_NOT_EQUAL(LW(14), 0)
        RUN_CHILD_EVT(LW(14))
    END_IF()
    RETURN()
EVT_END()
void PlaceNpc(NpcEntry *npc, const AiProfile *ai) {
    using namespace hitdrv;
    HitCheckQuery query = {};
    query.targetPosition = npc->position;
    query.targetPosition.y += 30.0f;
    query.targetDirection.y = -1.0f;
    query.inOutTargetDistance = 600.0f;
    if (!hitCheckVecFilter(&query, reinterpret_cast<PFN_HitFilterFunction>(0x800915e4)) ||
        query.hitNormal.y <= 0.5f) return;
    int index = ai - reinterpret_cast<const AiProfile *>(0x80338270);
    float hover = 0.0f;
    if (index == 6 || index == 8 || index == 13 || index == 25 ||
        index == 29 || index == 32 || index == 33) hover = 40.0f;
    if (index == 27) hover = 120.0f;
    const float delta = query.hitPosition.y + hover - npc->position.y;
    npc->position.y += delta;
    npc->previousPosition = npc->position;
    npc->wTerritoryBase.y += delta;
}
void Capture(evtmgr::EvtEntry *evt) {
    sCount = 0;
    sOwner = evt;
    auto *setup = reinterpret_cast<const evt_npc::NpcSetupInfo *>(
        evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]));
    for (; setup && setup->name; ++setup)
        CaptureNpc(npcNameToPtr_NoAssert(setup->name));
}
bool Prepare(bool startInit = true) {
    bool ready = true;
    for (int i = 0; i < sCount; ++i) {
        auto &pending = sPending[i];
        if (pending.applied || !pending.npc || !(pending.npc->flags & 1) ||
            pending.npc->poseId != pending.pose) continue;
        if (!animdrv::animGroupBaseAsync(pending.tribe->modelName, 0, nullptr)) {
            ready = false;
            continue;
        }
        int pose = animdrv::animPoseEntry(pending.tribe->modelName, 0);
        if (pose < 0) { ready = false; continue; }
        auto *npc = pending.npc;
        auto *ai = pending.ai;
        if (pending.replaceAi) {
            PlaceNpc(npc, ai);
            npc->flags &= ~(0x10000000u | 0x04000000u | 0x02000000u | 0x08000000u | 0x20000u | 0x400u);
            // Both setup paths must wait for the new init event to finish.
            npc->flags &= ~2u;
        }
        animdrv::animPoseRelease(npc->poseId);
        npc->poseId = pose;
        pending.pose = pose;
        // Match npcEntry: field poses need paper-facing evaluation so their
        // facing direction does not rotate the sprite as a battle model.
        animdrv::animPosePeraOn(pose);
        animdrv::animPoseSetMaterialLightFlagOn(pose, 2);
        // Retain rotationY (movement/facing), not the old species' rig offsets.
        npc->rotation = {0.0f, 0.0f, 0.0f};
        npc->rotationOffset = {0.0f, 0.0f, 0.0f};
        npc->tribe = pending.tribe;
        npc->width = pending.tribe->wWidth;
        npc->height = pending.tribe->height;
        std::strcpy(npc->stayAnimation, pending.tribe->stayAnimation);
        std::strcpy(npc->talkAnimation, pending.tribe->talkAnimation);
        std::strcpy(npc->currentAnimation, pending.tribe->wInitialAnimation);
        animdrv::animPoseSetAnim(pose, npc->currentAnimation, 1);
        animdrv::animPoseSetLocalTime(pose, static_cast<float>(system::irand(30)));
        npc->moveLeftSfxId = pending.tribe->moveLeftSfxId;
        npc->moveRightSfxId = pending.tribe->moveRightSfxId;
        npc->jumpSfxId = pending.tribe->jumpSfxId;
        npc->landingSfxId = pending.tribe->landingSfxId;
        npc->unkSfxId_fromTribe = pending.tribe->unkSfxId;
        npc->wbSoundDataDirty_counter = 1;
        npc->wInitInSetupDataBaseSoundRelated = 0;
        if (pending.replaceAi) {
            // Native dead scripts can depend on species-specific slave parts.
            // Keep room-specific callbacks, but replace ordinary native AI death.
            auto *profiles = reinterpret_cast<const AiProfile *>(0x80338270);
            for (int j = 3; j <= 45; ++j)
                if (npc->deadEvtCode == profiles[j].dead) { npc->deadEvtCode = ai->dead; break; }
            npc->regularEvtCode = const_cast<int32_t *>(RegularEvent);
            npc->findEvtCode = ai->find;
            npc->lostEvtCode = ai->lost;
            npc->returnEvtCode = ai->returning;
            npc->blowEvtCode = ai->blow;
            // Keep room talk scripts and original territory/search ranges.
            npc->initEvtCode = ai->init;
        }
        bool stored = false;
        for (int j = 0; j < sActorCount; ++j)
            if (sActors[j].npc == npc) { sActors[j] = pending; stored = true; break; }
        if (!stored) {
            for (int j = 0; j < sActorCount; ++j)
                if (!sActors[j].npc) { sActors[j] = pending; stored = true; break; }
        }
        if (!stored && sActorCount < 128) sActors[sActorCount++] = pending;
        auto *init = startInit && pending.replaceAi ? evtmgr::evtEntry(ai->init, 0, 0) : nullptr;
        if (init) {
            init->wNpcEventType = 0;
            init->wThisPtr = npc;
            npc->initEvtId = init->threadId;
        }
        pending.applied = true;
    }
    return ready;
}
#endif
void Forget(NpcEntry *npc) {
#ifdef TTYD_US
    for (int i = 0; i < sActorCount; ++i)
        if (sActors[i].npc == npc) sActors[i].npc = nullptr;
    for (int i = 0; i < sCount; ++i)
        if (sPending[i].npc == npc) sPending[i].npc = nullptr;
#endif
}
KEEP_FUNC void DeleteHook(NpcEntry *npc) {
    Forget(npc);
    sDelete(npc);
}
KEEP_FUNC void DeleteGroupHook(NpcEntry *npc) {
    if (npc) Forget(npc->master ? npc->master : npc);
    sDeleteGroup(npc);
}
KEEP_FUNC int32_t TribeHook(evtmgr::EvtEntry *evt, bool first) {
#ifdef TTYD_US
    if (auto *actor = Actor(ArgumentNpc(evt))) {
        int32_t args[2] = {evt->evtArguments[0], reinterpret_cast<int32_t>(actor->tribe->nameJp)};
        auto *saved = evt->evtArguments;
        evt->evtArguments = args;
        int result = sSetTribe(evt, first);
        evt->evtArguments = saved;
        return result;
    }
#endif
    return sSetTribe(evt, first);
}
KEEP_FUNC int32_t SlaveHook(evtmgr::EvtEntry *evt, bool first) {
#ifdef TTYD_US
    if (auto *actor = Actor(ArgumentNpc(evt))) {
        int ai = actor->ai - reinterpret_cast<const AiProfile *>(0x80338270);
        if (ai != 25 && ai != 26 && ai != 28 && ai != 38 && ai != 39 &&
            ai != 41 && ai != 42 && ai != 44) return sSlaveEntry(evt, first);
        // Native AI slaves are the root, shield, spell, chain or other parts
        // of the same model; retain the chosen palette/species for those parts.
        int32_t args[5];
        std::memcpy(args, evt->evtArguments, sizeof(args));
        args[2] = reinterpret_cast<int32_t>(actor->tribe->modelName);
        auto *saved = evt->evtArguments;
        evt->evtArguments = args;
        int result = sSlaveEntry(evt, first);
        evt->evtArguments = saved;
        return result;
    }
#endif
    return sSlaveEntry(evt, first);
}
KEEP_FUNC int32_t AnimHook(evtmgr::EvtEntry *evt, bool first) {
#ifdef TTYD_US
    if (auto *actor = Actor(ArgumentNpc(evt))) {
        const char *anim = reinterpret_cast<const char *>(evtmgr_cmd::evtGetValue(evt, evt->evtArguments[1]));
        const char *initial = actor->tribe->wInitialAnimation;
        // The colored Magikoopa AI spells name the base model's KMK poses.
        if (anim && initial && std::strncmp(anim, "KMK_", 4) == 0 &&
            (std::strncmp(initial, "KMR_", 4) == 0 || std::strncmp(initial, "KMW_", 4) == 0 ||
             std::strncmp(initial, "KMG_", 4) == 0)) {
            char replacement[32];
            std::strncpy(replacement, anim, sizeof(replacement));
            replacement[31] = 0;
            std::memcpy(replacement, initial, 3);
            int32_t args[2] = {evt->evtArguments[0], reinterpret_cast<int32_t>(replacement)};
            auto *saved = evt->evtArguments;
            evt->evtArguments = args;
            int result = sSetAnim(evt, first);
            evt->evtArguments = saved;
            return result;
        }
    }
#endif
    return sSetAnim(evt, first);
}
KEEP_FUNC int32_t SetupHook(evtmgr::EvtEntry *evt, bool first) {
#ifdef TTYD_US
    if (sGroupCount != 0) {
        if (first) {
            const int result = sSetup(evt, first);
            if (result != 0) return result;
            Capture(evt);
        }
        if (sOwner == evt && !Prepare()) return 0;
        if (!first) {
            const int result = sSetup(evt, false);
            if (result == 2 && sOwner == evt) { sOwner = nullptr; sCount = 0; }
            return result;
        }
        return 0;
    }
#endif
    return sSetup(evt, first);
}
KEEP_FUNC void ExecInitHook() {
#ifdef TTYD_US
    sCount = 0;
    sOwner = nullptr;
    sBatchDeferred = false;
    if (sGroupCount != 0) {
        auto *work = npcGetWorkPtr();
        if (work && work->entries)
            for (uint32_t i = 0; i < work->npcMaxCount; ++i)
                if (work->entries[i].flags & 1) CaptureNpc(&work->entries[i]);
        if (!Prepare(false)) { sBatchDeferred = true; return; }
    }
#endif
    sExecInit();
}
KEEP_FUNC bool WaitInitHook() {
#ifdef TTYD_US
    if (sBatchDeferred) {
        if (!Prepare(false)) return false;
        sBatchDeferred = false;
        sCount = 0;
        sExecInit();
    }
#endif
    return sWaitInit();
}

}
KEEP_FUNC void ResetMap() {
#ifdef TTYD_US
    sCount = sActorCount = 0;
    sOwner = nullptr;
    sBatchDeferred = false;
#endif
}
KEEP_FUNC void ResetGroups() {
#ifdef TTYD_US
    ResetMap();
    sGroupCount = 0;
#endif
}
KEEP_FUNC void RegisterGroup(ttyd::battle_database_common::BattleGroupSetup *group) {
#ifdef TTYD_US
    for (int i = 0; i < sGroupCount; ++i) if (sGroups[i] == group) return;
    if (group && sGroupCount < 150) sGroups[sGroupCount++] = group;
#endif
}
KEEP_FUNC void Update() {
#ifdef TTYD_US
    // Event-created enemies may assign their battle information after setup.
    if (seqdrv::seqGetSeq() != seqdrv::SeqIndex::kGame || sOwner || sBatchDeferred || !sGroupCount) return;
    auto *work = npcGetWorkPtr();
    if (!work || !work->entries) return;
    for (int i = 0; i < sActorCount; ++i) {
        auto *npc = sActors[i].npc;
        if (sActors[i].replaceAi && npc && (npc->flags & 1) && npc->tribe == sActors[i].tribe &&
            npc->poseId == sActors[i].pose &&
            npc->initEvtId && !evtmgr::evtCheckID(npc->initEvtId)) {
            npc->initEvtId = 0;
            npc->flags |= 2;
        }
    }
    for (uint32_t i = 0; i < work->npcMaxCount; ++i) {
        auto *npc = &work->entries[i];
        if ((npc->flags & 3) == 3 && !npc->initEvtId) CaptureNpc(npc);
    }
    if (Prepare()) sCount = 0;
#endif
}
KEEP_FUNC void InstallHooks() {
    sSetup = patch::hookFunction(ttyd::evt_npc::evt_npc_setup, SetupHook);
    sExecInit = patch::hookFunction(::npcExecAllInitEvt, ExecInitHook);
    sWaitInit = patch::hookFunction(::npcWaitAllInitEvtEnd, WaitInitHook);
    sSetTribe = patch::hookFunction(ttyd::evt_npc::evt_npc_set_tribe, TribeHook);
    sSlaveEntry = patch::hookFunction(ttyd::evt_npc::evt_npc_slave_entry, SlaveHook);
    sSetAnim = patch::hookFunction(ttyd::evt_npc::evt_npc_set_anim, AnimHook);
    sDelete = patch::hookFunction(::npcDelete, DeleteHook);
    sDeleteGroup = patch::hookFunction(::npcDeleteGroup, DeleteGroupHook);
}
}
