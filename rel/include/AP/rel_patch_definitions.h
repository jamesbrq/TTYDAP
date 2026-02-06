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
    void bMapGXArrFlagCheck();
    void bMapGXArrFlagCheckReturn();
    void bWinLogArrInject();
    void bWinLogArrInjectReturn();
    void bWinLogArrIncrement();
    void bWinLogArrIncrementReturn();
    void bWinLogArrFlagCheck();
    void bWinLogArrFlagCheckReturn();
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
    void bGreenZoneMaxInit();
    void bGreenZoneMaxInitReturn();
    void bMonosiriItemCheck();
    void bMonosiriItemCheckReturn();
    void bMonosiriBucket();
    void bMonosiriBucketReturn();
    void bMonosiriBucketExtra();
    void bHpColorBack();
    void bHpColorBackReturn();
    void bHpColorFront();
    void bHpColorFrontReturn();
    void bExpMultiplier();
    void bExpMultiplierReturn();
    void bBlockVisibility();
    void bBlockVisibilityReturn();
    void bShopDesc();
    void bShopDescReturn();
    void bItemStarstoneCheck();
    void bItemStarstoneCheckReturn();
    void bStarstoneBgmKeep();
    void bStarstoneBgmKeepReturn();
    void bStarstoneBgmKeepFinal();
    void bStarstoneBgmKeepFinalReturn();
    void bMobjStarstoneRotation();
    void bMobjStarstoneRotationReturn();
    void bStoneRotationPointerCheck();
    void bStoneRotationPointerCheckReturn();
    void bStoneRotationPointerCheck2();
    void bStoneRotationPointerCheckReturn2();
    void bStoneRotationPointerCheck3();
    void bStoneRotationPointerCheckReturn3();
    void bStoneRotationPointerCheck4();
    void bStoneRotationPointerCheckReturn4();
    void bStoneRotationPointerCheck5();
    void bStoneRotationPointerCheckReturn5();
    void bStoneBgPointerCheck();
    void bStoneBgPointerCheckReturn();
    void bStoneBgPointerCheck2();
    void bStoneBgPointerCheckReturn2();
    void bStoneCh2RotationPtr();
    void bStoneCh2RotationPtrReturn();
    void bStoneCh3RotationPtr();
    void bStoneCh3RotationPtrReturn();
    void bPuniMainNullFix();
    void bPuniMainNullFixReturn();
    void checkShopFlag(uint32_t item, uint32_t index);
    void monosiriItemCheck(int unit_id);
    int applyExpMultiplier(int exp);
    int getBlockVisibility(int blockType);
    const char *shopItemDescription(const char *itemDescription);
    int itemHandleStarstone(void *itemPtr);
    void powerupBlkStarstoneRotation(const char *itemName, int itemId);
}

EVT_DECLARE_USER_FUNC(checkChapterRequirements, 1)
EVT_DECLARE_USER_FUNC(doorStarsCheck, 0)
EVT_DECLARE_USER_FUNC(pitCheckpointBuffer, 1)
EVT_DECLARE_USER_FUNC(setShopFlags, 1)
EVT_DECLARE_USER_FUNC(handleIntermissionSkip, 4)
EVT_DECLARE_USER_FUNC(checkIntermission, 1)
EVT_DECLARE_USER_FUNC(coconut_remove, 1)
EVT_DECLARE_USER_FUNC(checkTattleItem, 1)
EVT_DECLARE_USER_FUNC(evt_msg_numselect, 2)
EVT_DECLARE_USER_FUNC(starstoneParamInit, 2)
EVT_DECLARE_USER_FUNC(starstoneParamClean, 0)
EVT_DECLARE_USER_FUNC(lasStarsCheck, 2)
EVT_DECLARE_USER_FUNC(marioGetRot, 1)
EVT_DECLARE_USER_FUNC(getStarstoneName, 1)
EVT_DECLARE_USER_FUNC(pouchStarstoneItem, 3)
EVT_DECLARE_USER_FUNC(setIconRenderPriority, 1)
EVT_DECLARE_USER_FUNC(starstoneRunItemEvent, 0)
EVT_DECLARE_USER_FUNC(starstoneCheckGoalComplete, 0)
EVT_DECLARE_USER_FUNC(getManiacItem, 2)
EVT_DECLARE_USER_FUNC(getManiacPrice, 2)
