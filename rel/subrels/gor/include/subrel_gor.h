#pragma once

#include <ttyd/evtmgr.h>

void ApplyGor00Patches();
void ApplyGor01Patches();
void ApplyGor02Patches();
void ApplyGor03Patches();
void ApplyGor04Patches();
void ApplyGorMiscPatches();

EVT_DECLARE_USER_FUNC(checkChapterClears, 1)
