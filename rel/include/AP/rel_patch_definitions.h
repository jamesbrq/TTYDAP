#pragma once

#include <ttyd/common_types.h>
#include <ttyd/evtmgr.h>

#include <cstdint>

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
    void bMonosiriMonban();
    void bMonosiriMonbanReturn();
    void bMonosiriBonetail();
    void bMonosiriBonetailReturn();
    void bMonosiriAtomicBoo();
    void bMonosiriAtomicBooReturn();
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
    int monosiriRemapWord1(int idx, int curWord1);
    int monosiriRemapStatic(int vanilla);
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
EVT_DECLARE_USER_FUNC(irai_complete_item_get, 1)
EVT_DECLARE_USER_FUNC(irai_complete_item_delete, 0)

// AP cooking (Zess T. recipe checks + ingredient unlocks) — see rel/misc/cooking_research.md.
// The apworld mirrors all of these values (Data.py flag mappings, Rom.py table writes,
// item classifications, client unlock backfill), so they must not change independently.
namespace mod::ap_cooking
{
    // The 57 journal recipes are exactly item ids 179 (Shroom Fry) .. 235 (Fresh Juice),
    // Mistake included; recipe index k = item id - kRecipeDishFirst.
    constexpr int32_t kRecipeDishFirst = 179;
    constexpr int32_t kRecipeDishLast = 235;
    constexpr int32_t kRecipeFlagBase = 6400; // GSWF(6400+k) = recipe k's check sent

    // Rom.py writes each recipe location's AP item as a u16 rom id at dol 0x500+2k,
    // which the loader maps to 0x80003500+2k (same channel as the Battle Trunks table).
    constexpr uint32_t kRecipeItemTableAddr = 0x80003500;

    // Ingredient unlocks: the FIRST acquisition of one of these vanilla items — from any
    // source (AP pool, enemy drops, trees, shops, cooking output) — permanently unlocks
    // it in Zess T.'s ingredient menu via GSWF(kIngredientFlagBase + k), where k is the
    // index in kIngredientIds. The trigger lives in pouchGetItemHook (OWR.cpp); the menu
    // lives in the gor subrel (gor_01.cpp). The client additionally backfills these
    // flags from ReceivedItems so `has <item>` always implies the unlock in logic.
    constexpr int32_t kIngredientFlagBase = 6460;

    constexpr int16_t kIngredientIds[] = {
        // 43 base ingredient items, ascending by id
        ttyd::common::ItemId::GOLD_BAR,      ttyd::common::ItemId::GOLD_BAR_X3,
        ttyd::common::ItemId::THUNDER_BOLT,  ttyd::common::ItemId::THUNDER_RAGE,
        ttyd::common::ItemId::SHOOTING_STAR, ttyd::common::ItemId::ICE_STORM,
        ttyd::common::ItemId::FIRE_FLOWER,   ttyd::common::ItemId::EARTH_QUAKE,
        ttyd::common::ItemId::BOOS_SHEET,    ttyd::common::ItemId::VOLT_SHROOM,
        ttyd::common::ItemId::REPEL_CAPE,    ttyd::common::ItemId::RUIN_POWDER,
        ttyd::common::ItemId::SLEEPY_SHEEP,  ttyd::common::ItemId::POWER_PUNCH,
        ttyd::common::ItemId::COURAGE_SHELL, ttyd::common::ItemId::MINI_MR_MINI,
        ttyd::common::ItemId::MR_SOFTENER,   ttyd::common::ItemId::MUSHROOM,
        ttyd::common::ItemId::SUPER_SHROOM,  ttyd::common::ItemId::ULTRA_SHROOM,
        ttyd::common::ItemId::LIFE_SHROOM,   ttyd::common::ItemId::DRIED_SHROOM,
        ttyd::common::ItemId::TASTY_TONIC,   ttyd::common::ItemId::HONEY_SYRUP,
        ttyd::common::ItemId::MAPLE_SYRUP,   ttyd::common::ItemId::JAMMIN_JELLY,
        ttyd::common::ItemId::SLOW_SHROOM,   ttyd::common::ItemId::GRADUAL_SYRUP,
        ttyd::common::ItemId::HOT_DOG,       ttyd::common::ItemId::POINT_SWAP,
        ttyd::common::ItemId::WHACKA_BUMP,   ttyd::common::ItemId::COCONUT,
        ttyd::common::ItemId::DRIED_BOUQUET, ttyd::common::ItemId::MYSTIC_EGG,
        ttyd::common::ItemId::GOLDEN_LEAF,   ttyd::common::ItemId::KEEL_MANGO,
        ttyd::common::ItemId::FRESH_PASTA,   ttyd::common::ItemId::CAKE_MIX,
        ttyd::common::ItemId::HOT_SAUCE,     ttyd::common::ItemId::TURTLEY_LEAF,
        ttyd::common::ItemId::HORSETAIL,     ttyd::common::ItemId::PEACHY_PEACH,
        ttyd::common::ItemId::SPITE_POUCH,
        // 10 dish-type ingredients, ascending by id (obtainable via repeat cooks
        // after their recipe check, or as pool items)
        ttyd::common::ItemId::SHROOM_FRY,    ttyd::common::ItemId::SPICY_SOUP,
        ttyd::common::ItemId::ZESS_DINNER,   ttyd::common::ItemId::SNOW_BUNNY,
        ttyd::common::ItemId::COCONUT_BOMB,  ttyd::common::ItemId::EGG_BOMB,
        ttyd::common::ItemId::POISON_SHROOM, ttyd::common::ItemId::MANGO_DELIGHT,
        ttyd::common::ItemId::COUPLES_CAKE,  ttyd::common::ItemId::INKY_SAUCE,
    };
    constexpr int32_t kIngredientCount = sizeof(kIngredientIds) / sizeof(kIngredientIds[0]);
    static_assert(kIngredientCount == 53);

    // Returns the unlock index k for an item id, or -1 if the item is not an ingredient.
    constexpr int32_t ingredientIndex(int32_t item)
    {
        for (int32_t k = 0; k < kIngredientCount; k++)
        {
            if (kIngredientIds[k] == item)
                return k;
        }
        return -1;
    }

    // Item id the cooking evt is currently handing out, or -1. Set by the gor subrel's
    // apCookChk/apCookChk2 and cleared by apCookingFlag; pouchGetItemHook skips the
    // ingredient unlock for this id so cooked results (repeat cooks, reversion recipes)
    // never unlock themselves as ingredients. Defined in OWR.cpp (KEEP_VAR).
    extern int32_t gCookGiveItem;
} // namespace mod::ap_cooking
