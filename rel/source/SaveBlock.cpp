#include "SaveBlock.h"
#include "patch.h"
#include "visibility.h"
#include <ttyd/evtmgr_cmd.h>
#include <ttyd/mario.h>
#include <ttyd/mariost.h>
#include <cstring>

namespace mod::save_block
{
    using ttyd::evtmgr::EvtEntry;
    using vec3 = gc::Vec3;
    using EventFn = int32_t (*)(EvtEntry *, bool);
    static EventFn doorEntryTrampoline;
    static EventFn doorParamTrampoline;

    // The two vanilla door systems use different definition layouts.
    struct Interior
    {
        const uint32_t *definition;
        const float *runtime;
        const char *group;
    };
    static Interior interiors[64];
    static uint32_t interiorCount;
    static bool itemSaveActive;

    static const char *nameAt(const uint32_t *definition, uint32_t offset)
    {
        return reinterpret_cast<const char *>(definition[offset / 4]);
    }

    static void registerInterior(const uint32_t *definition, const float *runtime, const char *group)
    {
        if (!definition || !group) return;
        for (uint32_t i = 0; i < interiorCount; ++i)
            if (interiors[i].definition == definition) return;
        if (interiorCount < sizeof(interiors) / sizeof(interiors[0]))
            interiors[interiorCount++] = {definition, runtime, group};
    }

    KEEP_FUNC void ResetMap()
    {
        interiorCount = 0;
        itemSaveActive = false;
    }

    static int32_t doorEntryHook(EvtEntry *evt, bool firstCall)
    {
        const int32_t output = evt->evtArguments[1];
        const int32_t result = doorEntryTrampoline(evt, firstCall);
        auto *runtime = reinterpret_cast<const uint32_t *>(ttyd::evtmgr_cmd::evtGetValue(evt, output));
        if (runtime)
        {
            auto *definition = reinterpret_cast<const uint32_t *>(runtime[0]);
            registerInterior(definition, reinterpret_cast<const float *>(runtime), nameAt(definition, 0x2c));
        }
        return result;
    }

    static int32_t doorParamHook(EvtEntry *evt, bool firstCall)
    {
        if (ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[1]) == 3)
        {
            auto *definition = reinterpret_cast<const uint32_t *>(
                ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]));
            if (definition) registerInterior(definition, nullptr, nameAt(definition, 0x20));
        }
        return doorParamTrampoline(evt, firstCall);
    }

    static bool getInteriorExit(vec3 &position)
    {
        auto mapGetMapObj = reinterpret_cast<const uint32_t *(*)(const char *)>(0x8001f878);
        auto hitObjGetPos = reinterpret_cast<void (*)(const char *, vec3 *)>(0x8001449c);
        auto doorPosition = reinterpret_cast<void (*)(uint32_t, const char *, const char *, const char *,
                                                     vec3 *, vec3 *, int32_t)>(0x800e884c);
        for (uint32_t i = 0; i < interiorCount; ++i)
        {
            const auto &interior = interiors[i];
            const auto *object = mapGetMapObj(interior.group);
            // Door initialization hides the interior; entering clears flag 1.
            if (!object || (object[0] & 1)) continue;
            const auto *definition = interior.definition;
            if (interior.runtime)
            {
                const char *outsideHit = nameAt(definition, 0x10);
                if (!outsideHit) continue;
                hitObjGetPos(outsideHit, &position);
                // in_pos is the exterior approach point; out_pos is indoors.
                position.x = interior.runtime[1];
                position.z = interior.runtime[3];
            }
            else
            {
                if (!nameAt(definition, 0x10)) continue;
                vec3 inside = {};
                // Use the same exterior point as evt_door_param mode 4.
                doorPosition(definition[1], nameAt(definition, 0x0c), nameAt(definition, 0x10),
                             nameAt(definition, 0x14), &position, &inside, 0);
            }
            return true;
        }
        return false;
    }

    KEEP_FUNC int32_t SetItemSaveActive(EvtEntry *evt, bool)
    {
        itemSaveActive = ttyd::evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]) != 0;
        return 2;
    }

    // Replaces only cardWrite's GlobalWork copy, before its checksums are calculated.
    // Live Mario coordinates and normal save blocks are unaffected.
    static void *copySaveWork(void *destination, const void *source, uint32_t size)
    {
        std::memcpy(destination, source, size);
        vec3 exit = {};
        if (itemSaveActive)
        {
            auto *savedWork = static_cast<GlobalWork *>(destination);
            if (std::strcmp(savedWork->currentMapName, "mri_03") == 0)
            {
                // The cage room must reload at its pipe, even when no house door is active.
                // dokan is the room's pipe entrance hit object (to mri_20).
                auto hitObjGetPos = reinterpret_cast<void (*)(const char *, vec3 *)>(0x8001449c);
                hitObjGetPos("dokan", &exit);
                exit.y += 1.0f; // Match vanilla pipe arrival's surface clearance.
                savedWork->savePlayerPos = exit;
            }
            else if (getInteriorExit(exit))
            {
                savedWork->savePlayerPos = exit;
            }
        }
        return destination;
    }

    KEEP_FUNC void Init()
    {
        doorEntryTrampoline = patch::hookFunction(reinterpret_cast<EventFn>(0x800e946c), doorEntryHook);
        doorParamTrampoline = patch::hookFunction(reinterpret_cast<EventFn>(0x800e81f4), doorParamHook);
        patch::writeBranchBL(reinterpret_cast<void *>(0x800b1ffc), copySaveWork);
    }
}
