#pragma once

#include <ttyd/evtmgr.h>

void ApplyGor00Patches();
void ApplyGor01Patches();
void ApplyGor02Patches();
void ApplyGor03Patches();
void ApplyGor04Patches();
void ApplyGorMiscPatches();

EVT_DECLARE_USER_FUNC(checkChapterClears, 1)
EVT_DECLARE_USER_FUNC(iraiSetStartFlag, 1)
EVT_DECLARE_USER_FUNC(iraiGetStartFlag, 2)
EVT_DECLARE_USER_FUNC(iraiClearStartFlag, 1)
EVT_DECLARE_USER_FUNC(getMonosiriItem, 2)

// AP cooking natives (gor_01.cpp), pointed to from cooking_evt via pokes
EVT_DECLARE_USER_FUNC(apMakeIngredientTbl, 2)
EVT_DECLARE_USER_FUNC(apCookChk, 3)
EVT_DECLARE_USER_FUNC(apCookChk2, 3)
EVT_DECLARE_USER_FUNC(apCookingFlag, 1)
EVT_DECLARE_USER_FUNC(apCookRemoveNop, 3)
