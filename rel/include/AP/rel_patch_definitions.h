#pragma once

#include <ttyd/evtmgr.h>

extern "C"
{
    EVT_DECLARE_USER_FUNC(tou_evt_tou_get_ranking, 1)
    EVT_DECLARE_USER_FUNC(muj_koburon_get_encount_info, 1)

    void bMapGXArrInject();
    void bMapGXArrInjectReturn();
    void bMapGXArrIncrement();
    void bMapGXArrIncrementReturn();
    void bMapGXChSplit();
    void bMapGXChSplitReturn();
    void bWinLogArrInject();
    void bWinLogArrInjectReturn();
    void bWinLogArrIncrement();
    void bWinLogArrIncrementReturn();
    void bChapterClearCheck();
    void bChapterClearCheckReturn();
    void bJohoyaSeqAddition();
    void bPrintPartyErrorFix();
    void bPrintPartyErrorFixReturn();
    void bPrintPartyAddErrorFix();
    void bPrintPartyAddErrorFixReturn();
    void bKururingFloorCapture();
    void bKururingFloorCaptureReturn();
    void bKururingFloorRelease();
    void bKururingFloorReleaseReturn();
    void bPowerupblkCapture();
    void bPowerupblkCaptureReturn();
    void bPowerupblkRelease();
    void bPowerupblkReleaseReturn();
    void bPeachPointer();
    void bPeachReturn();
    void bShopFlagCheck();
    void bShopFlagCheckReturn();
    void bCoconutCheck();
    void bCoconutCheckReturn();
    void checkShopFlag(uint32_t item, uint32_t index);
}

EVT_DECLARE_USER_FUNC(checkChapterRequirements, 1)
EVT_DECLARE_USER_FUNC(setShopFlags, 1)
EVT_DECLARE_USER_FUNC(checkIntermission, 1)
EVT_DECLARE_USER_FUNC(coconut_remove, 1)
