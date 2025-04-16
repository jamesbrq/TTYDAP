#pragma once
#include "evt_cmd.h"
#include "visibility.h"
#include <AP/rel_patch_definitions.h>
#include <gc/OSModule.h>
#include <StateManager.h>
#include <ttyd/seqdrv.h>
#include <ttyd/evtmgr.h>
#include <ttyd/party.h>

#include <cstdint>

using namespace ttyd::seqdrv;
using namespace ttyd::party;
using namespace ttyd::evtmgr;

namespace mod::owr
{
    class OWR
    {
       public:
        OWR();
        ~OWR() {};
        void Update();
        void OnModuleLoaded(OSModuleInfo *module_info);
        void SequenceInit();
        void DrawString(const char *data, float x, float y, uint32_t color, float scale = 1.0f);
        void HomewardWarp();
        void RecieveItems();

        StateManager state;
    };

    extern OWR *gSelf;
    extern StateManager *gState;

    bool OSLinkHook(OSModuleInfo *new_module, void *bss);
    void seqSetSeqHook(SeqIndex seq, const char *map, const char *bero);
    uint32_t pouchGetItemHook(int32_t item);
    void partySetForceMoveHook(PartyEntry *ptr, float x, float z, float speed);
    int32_t evtMarioSetPoseHook(EvtEntry *evt, bool firstCall);
    const char *msgSearchHook(const char *msgKey);
    void logoSkip(SeqInfo *index);

    extern bool (*g_OSLink_trampoline)(OSModuleInfo *, void *);
    extern void (*g_seqSetSeq_trampoline)(SeqIndex seq, const char *map, const char *bero);
    extern void (*gTrampoline_seq_logoMain)(SeqInfo *info);
    extern uint32_t (*g_pouchGetItem_trampoline)(int32_t);
    extern void (*g_partySetForceMove_trampoline)(ttyd::party::PartyEntry *ptr, float x, float z, float speed);
    extern int32_t (*g_evt_mario_set_pose_trampoline)(ttyd::evtmgr::EvtEntry *evt, bool firstCall);
    extern const char *(*g_msgSearch_trampoline)(const char *);

    extern const char *goombellaName;
    extern const char *goombellaDescription;
    extern const char *koopsName;
    extern const char *koopsDescription;
    extern const char *flurrieName;
    extern const char *flurrieDescription;
    extern const char *yoshiName;
    extern const char *yoshiDescription;
    extern const char *vivianName;
    extern const char *vivianDescription;
    extern const char *bobberyName;
    extern const char *bobberyDescription;
    extern const char *mowzName;
    extern const char *mowzDescription;
    extern const char *apItemNameDescription;
    extern const char *tenCoinsNameDescription;
    extern const char *planeModeNameDescription;
    extern const char *paperModeNameDescription;
    extern const char *tubeModeNameDescription;
    extern const char *boatModeNameDescription;

} // namespace mod::owr
