#pragma once

#include "ttyd/party.h"

#include <cstdint>

extern "C"
{
    // Assembly overwrite functions
    void asmFixBlooperCrash1();
    void asmFixBlooperCrash2();
    void asmPreventDiaryTextboxSelection();
    void asmFixEvtMapBlendSetFlagPartnerCrashStart();
    void asmFixEvtMapBlendSetFlagPartnerCrashBranchBack();
    void asmFixEvtMapBlendSetFlagFollowerCrashStart();
    void asmFixEvtMapBlendSetFlagFollowerCrashBranchBack();

    void cPreventDiaryTextboxOptionSelection(const char *currentText, int32_t *storeAddress, int32_t selectedOption);
    PartyEntry *cFixEvtMapSetFlagCrash(PartyEntry *partyPtr, bool shouldSpawnPartner);

#ifdef TTYD_JP
    void asmCrashScreenDecrementYPosStart();
    void asmCrashScreenDecrementYPosBranchBack();
#endif

    // Functions accessed by assembly overwrites
    uint32_t cFixBlooperCrash1(uint32_t unkValue, void *battleUnitPtr);

#ifdef TTYD_JP
    int32_t cCrashScreenDecrementYPos();
#endif
}
