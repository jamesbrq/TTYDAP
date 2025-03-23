#pragma once

extern "C" {
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
}

void ApplyMainAssemblyPatches();
void ApplyMainScriptPatches();
void ApplyItemDataTablePatches();