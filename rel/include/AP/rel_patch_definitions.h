#pragma once
#include <ttyd/evtmgr.h>

extern "C"
{
    EVT_DECLARE_USER_FUNC(evt_tou_get_ranking, 1)
    EVT_DECLARE_USER_FUNC(koburon_get_encount_info, 1)

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
}

EVT_DECLARE_USER_FUNC(checkChapterRequirements, 1)
EVT_DECLARE_USER_FUNC(checkChapterClears, 1)

void ApplyAaaPatches();
void ApplyAjiPatches();
void ApplyBomPatches();
void ApplyDouPatches();
void ApplyEkiPatches();
void ApplyEndPatches();
void ApplyGonPatches();
void ApplyGorPatches();
void ApplyGor00Patches();
void ApplyGor01Patches();
void ApplyGor02Patches();
void ApplyGor03Patches();
void ApplyGor04Patches();
void ApplyGorMiscPatches();
void ApplyGraPatches();
void ApplyHeiPatches();
void ApplyHomPatches();
void ApplyJinPatches();
void ApplyKpaPatches();
void ApplyLasPatches();
void ApplyMainScriptPatches();
void ApplyMainAssemblyPatches();
void ApplyItemDataTablePatches();
void ApplyMooPatches();
void ApplyMriPatches();
void ApplyMujPatches();
void ApplyNokPatches();
void ApplyPikPatches();
void ApplyRshPatches();
void ApplyTikPatches();
void ApplyTouPatches();
void ApplyTou2Patches();
void ApplyUsuPatches();
void ApplyWinPatches();

char* strcpy_String(char* dest, const char* src);
