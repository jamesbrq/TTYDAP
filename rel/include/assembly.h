#pragma once

#include <cstdint>

extern "C"
{
    // Assembly overwrite functions
    void asmFixBlooperCrash1();
    void asmFixBlooperCrash2();
    void asmPreventDiaryTextboxSelection();

    void cPreventDiaryTextboxOptionSelection(const char *currentText, int32_t *storeAddress, int32_t selectedOption);

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
