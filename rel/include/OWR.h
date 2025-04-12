#pragma once
#include <cstdint>
#include <gc/OSModule.h>
#include <StateManager.h>
#include <ttyd/seqdrv.h>
#include <ttyd/evtmgr.h>
#include <ttyd/party.h>

using namespace ttyd::seqdrv;
using namespace ttyd::party;
using namespace ttyd::evtmgr;

namespace mod::owr
{
    class OWR
    {
       public:
        OWR();
        void Init();
        void Update();
        void OnModuleLoaded(OSModuleInfo *module_info);
        void SequenceInit();
        void DrawString(const char *data, float x, float y, uint32_t color, float scale = 1.0f);
        void HomewardWarp();
        void RecieveItems();

        StateManager state;
    };

    bool OSLinkHook(OSModuleInfo *new_module, void *bss);
    void seqSetSeqHook(SeqIndex seq, const char *map, const char *bero);
    uint32_t pouchGetItemHook(int32_t item);
    void partySetForceMoveHook(PartyEntry *ptr, float x, float z, float speed);
    int32_t evtMarioSetPoseHook(EvtEntry *evt, bool firstCall);
    const char *msgSearchHook(const char *msgKey);

    extern OWR *gSelf;
} // namespace mod::owr
