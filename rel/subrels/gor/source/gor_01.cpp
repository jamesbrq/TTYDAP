#include "subrel_gor.h"
#include "evt_cmd.h"
#include "patch.h"
#include "OWR.h"
#include "AP/rel_patch_definitions.h"
#include "ttyd/common_types.h"
#include "ttyd/evt_bero.h"
#include "ttyd/evt_hit.h"
#include "ttyd/evt_item.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_msg.h"
#include "ttyd/evt_npc.h"
#include "ttyd/evt_pouch.h"
#include "ttyd/evt_snd.h"
#include "ttyd/evtmgr_cmd.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/swdrv.h"

#include <cstdint>

using namespace mod;
using namespace mod::ap_cooking;
using namespace ttyd;
using namespace ttyd::common;

extern int32_t gor_badgemaster_init[];
extern int32_t gor_badgemaster_talk[];
extern int32_t gor_kinoji_talk[];
extern int32_t gor_marco_init_01[];
extern int32_t gor_marco_talk_01[];
extern int32_t gor_master_init[];
extern int32_t gor_master_talk[];
extern int32_t gor_shoptender_init[];
extern int32_t gor_shoptender_talk[];
extern int32_t gor_roten1_talk[];
extern int32_t gor_roten2_talk[];
extern int32_t gor_kuribo1_talk[];
extern int32_t gor_kuribo2_talk[];
extern int32_t gor_kuribo3_talk[];
extern int32_t gor_chusan1_init[];
extern int32_t gor_chusan1_regl[];
extern int32_t gor_chusan1_naguru[];
extern int32_t gor_chusan1_talk[];
extern int32_t gor_chusan2_talk[];
extern int32_t gor_bomhei_talk[];
extern int32_t gor_bottakuru_init[];
extern int32_t gor_luigi_init_01[];
extern int32_t gor_luigi_npcEnt_01[];
extern int32_t gor_kick2_init[];
extern int32_t gor_borodo2_init_01[];
extern int32_t gor_borodo2_talk_01[];
extern int32_t gor_thief_init[];
extern int32_t gor_thief_talk[];
extern int32_t gor_christine_nakama_before[];
extern int32_t gor_luigi_f1_fade_check[];
extern int32_t gor_luigi_f2_fade_check[];
extern int32_t gor_marco_hitorigoto[];
extern int32_t gor_goods_list;
extern int32_t gor_01_shop_setup[];
extern int32_t gor_christine_nakama[];
extern int32_t gor_01_koopa_evt[];
extern int32_t gor_iri_09_init[];
extern int32_t gor_iri_09_tentyo_talk[];
extern int32_t gor_iri_09_item_tbl_make[];
extern int32_t gor_iri_16_init[];
extern int32_t gor_iri_16_tyutyu_talk[];
extern int32_t gor_01_init_evt[];
extern int32_t gor_cooking_evt[];

// Vanilla cooking lookup natives in gor.rel (.text, resolved via ttyd.us.lst)
extern "C"
{
    int32_t gor_cook_chk(ttyd::evtmgr::EvtEntry *evt, bool isFirstCall);
    int32_t gor_cook_chk2(ttyd::evtmgr::EvtEntry *evt, bool isFirstCall);
}

// Zess T.'s name string in gor.rel .rodata (Shift-JIS), used as evt speaker arg
extern char gor_str_nancy[];
extern int32_t gor_nancy_init[];
extern int32_t gor_nancy_regl[];

// clang-format off
EVT_BEGIN(iri_16_mowz_evt)
	IF_EQUAL(GSWF(6353), 0) // skip the spawn once collected (get_item would hang)
		USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
		USER_FUNC(evt_item::evt_item_entry, PTR("item01"), LW(3), LW(0), LW(1), LW(2), 16, GSWF(6353), 0)
		USER_FUNC(evt_item::evt_item_get_item, PTR("item01"))
		WAIT_MSEC(800)
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(badgemaster_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_024_00"), 0, PTR("me"))
	RETURN()
EVT_END()
// clang-format on

// Wraps the trouble-branch BGM call in tyutyu_talk (word 11, always runs before
// the handover gate): stashes Elusive Badge possession in LW(14) for the gate at
// word 28. Vanilla gated on the Hooktail box-opened flag, but with AP the badge
// can arrive without that chest ever being opened (and opening the chest doesn't
// grant the badge), so possession is the only correct condition.
EVT_DEFINE_USER_FUNC(iri16BadgeGateBgm)
{
    evtmgr_cmd::evtSetValue(
        evt, LW(14), mario_pouch::pouchCheckItem(ItemId::ATTACK_FX_B_KEY_ITEM) > 0 ? 1 : 0);
    return evt_snd::evt_snd_bgmon(evt, isFirstCall);
}

// clang-format off

EVT_BEGIN(marco_init_01_evt)
	IF_LARGE_EQUAL(GSW(1705), 1)
		IF_SMALL_EQUAL(GSW(1717), 26)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(marco_init_01_hook)
	RUN_CHILD_EVT(marco_init_01_evt)
	RETURN()
EVT_END()

EVT_BEGIN(bottakuru_init_evt)
	USER_FUNC(checkChapterClears, LW(0))
	IF_SMALL(LW(0), 1)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), 0, -1000, 0)
		RETURN()
	END_IF()
	USER_FUNC(evt_npc::evt_npc_flag_onoff, 1, PTR("me"), 1073741824)
	USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
	SET(GSWF(1206), 0)
	RETURN()
EVT_END()

EVT_BEGIN(bottakuru_init_hook)
	RUN_CHILD_EVT(bottakuru_init_evt)
	RETURN()
EVT_END()

EVT_BEGIN(kinoji_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_019_02"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(shoptender_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_015_02"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(roten1_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_025_01"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(roten2_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_025_06_2"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(kuribo1_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_026_06"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(kuribo2_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_027_06"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(kuribo3_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_028_08"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(chusan1_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_029_04"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(chusan2_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_030_03"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(bomhei_talk_evt)
	USER_FUNC(evt_msg::evt_msg_print, 0, PTR("gor_01_031_05"), 0, PTR("me"))
	RETURN()
EVT_END()

EVT_BEGIN(gor_master_talk_evt)
	IF_EQUAL(GSWF(6099), 1)
        USER_FUNC(evt_msg::evt_msg_print, 0, PTR("mac_4_076"), 0, PTR("me"))
        SET(LW(3), 1)
    ELSE()
        SET(GF(5), 0)
        USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_SMALL(LW(0), 320)
            SET(GF(5), 1)
        END_IF()
        SET(LW(3), 0)
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN(gor_master_talk_hook)
	RUN_CHILD_EVT(gor_master_talk_evt)
    IF_EQUAL(LW(3), 1)
        RETURN()
    END_IF()
    GOTO(&gor_master_talk[137])
EVT_PATCH_END()

// Replaces nancy_init (its head is repointed here). Three states:
//  - gate  (GSWF 6124 unset): vanilla behavior, she blocks the west gate and the
//    w_bero exit is disabled.
//  - door  (6124 set by OWR when westside is first reached / Open Westside, lens
//    not delivered): she guards her kitchen door instead and the gate stays
//    open. nancy_talk is untouched — delivering the Contact Lens still sets
//    GSWF(1188), walks her inside and unlocks the kitchen/cooking.
//  - delivered (GSWF 1188): vanilla kitchen position. 6124 is also set here so
//    nancy_regl's gate-block check stays off when the lens was delivered at the
//    gate itself.
EVT_BEGIN(ap_nancy_init_evt)
	IF_EQUAL(GSWF(1188), 0)
		IF_EQUAL(GSWF(6124), 0)
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 90)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -573, 0, 56)
			SET(LW(0), PTR("w_bero"))
			RUN_CHILD_EVT(evt_bero::bero_case_switch_off)
			USER_FUNC(evt_hit::evt_hitobj_attr_onoff, 1, 0, PTR("w_bero"), 8388612)
		ELSE()
			USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 270)
			USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -380, 10, 20)
		END_IF()
	ELSE()
		SET(GSWF(6124), 1)
		USER_FUNC(evt_npc::evt_npc_set_ry, PTR("me"), 270)
		USER_FUNC(evt_npc::evt_npc_set_position, PTR("me"), -405, 10, -100)
	END_IF()
	RETURN()
EVT_END()

// Replaces cooking_evt's first make_item_tbl call: asks which ingredients Zess
// should use, stores the mode, then builds the matching select-window table.
// msg keys "ap_cook_mode" (question) and "ap_cook_mode_select" (two options:
// 0 = unlocked ingredient stock, 1 = the player's own items) live in mod.txt.
// An empty table ends the conversation through the parent's vanilla branch,
// whose message apCookEmptyMsg swaps when the pouch-full guard caused it.
EVT_BEGIN(ap_cook_mode_evt)
	USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("ap_cook_mode"))
	USER_FUNC(evt_msg::evt_msg_select, 0, PTR("ap_cook_mode_select"))
	USER_FUNC(apSetCookMode, LW(0))
	USER_FUNC(apMakeIngredientTbl, LW(11), LW(0))
	RETURN()
EVT_END()
// clang-format on

// --- AP cooking: unlock-driven ingredient menu + recipe checks ---
// The ingredient id table and unlock flag mapping live in AP/rel_patch_definitions.h
// (mod::ap_cooking); unlock flags are set on first pickup in pouchGetItemHook (OWR.cpp).

// Select-window table; replaces gor.rel's .bss item_tbl (only 20 slots) at
// cooking_evt words 506/575.
static int32_t apCookIngredientTbl[kIngredientCount + 1];

// Recipe index carried from apCookChk/apCookChk2 to apCookingFlag so the check flag
// is only set once the result item has actually been received.
static int32_t sPendingRecipe = -1;

// Cooking mode, chosen by the player per cook via ap_cook_mode_evt:
// 0 = unlocked ingredient stock: nothing is consumed, each recipe yields its AP
//     item exactly once; repeats and reversion recipes always produce a Mistake.
// 1 = real inventory: pure vanilla cooking (real ingredients consumed, vanilla
//     outputs, no checks) — the only source of farmable dishes/reversions, so the
//     vanilla ingredient economy bounds any farming.
static int32_t sCookMode = 0;

// Set when the unlock-mode menu came out empty ONLY because every regular item
// slot is taken; apCookEmptyMsg turns it into a dedicated message instead of
// letting the vanilla "no ingredients" line mislead the player.
static bool sPouchFull = false;

// Decide what the cook actually hands out. outVar is the evt arg holding the result.
// Everything cooked is recorded in gCookGiveItem so pouchGetItemHook never treats
// cooked results as ingredient-unlocking pickups; the ONLY exception is a recipe's
// first unlock-mode cook, whose AP item is a real acquisition and must unlock
// normally even offline.
static void applyRecipeReplacement(evtmgr::EvtEntry *evt, int32_t outVar)
{
    sPendingRecipe = -1;
    gCookGiveItem = -1;

    int32_t out = static_cast<int32_t>(evtmgr_cmd::evtGetValue(evt, outVar));
    if (out == 0)
        out = ItemId::MISTAKE; // cooking_evt itself turns 0 into Mistake after this

    if (sCookMode == 1)
    {
        // Inventory mode: vanilla cooking, but cooked results still never unlock.
        gCookGiveItem = out;
        return;
    }

    // Unlocked-ingredient mode: reversion recipes (non-dish outputs) belong to
    // inventory mode only, and every recipe pays out its AP item exactly once.
    if (out < kRecipeDishFirst || out > kRecipeDishLast)
    {
        evtmgr_cmd::evtSetValue(evt, outVar, ItemId::MISTAKE);
        gCookGiveItem = ItemId::MISTAKE;
        return;
    }
    const int32_t recipe = out - kRecipeDishFirst;
    if (swdrv::swGet(kRecipeFlagBase + recipe))
    {
        evtmgr_cmd::evtSetValue(evt, outVar, ItemId::MISTAKE);
        gCookGiveItem = ItemId::MISTAKE;
        return;
    }
    const uint16_t romId = *reinterpret_cast<uint16_t *>(kRecipeItemTableAddr + recipe * 2);
    if (romId == 0)
    {
        gCookGiveItem = out;
        return; // table not populated -> vanilla behavior (test builds only)
    }
    evtmgr_cmd::evtSetValue(evt, outVar, romId);
    sPendingRecipe = recipe;
}

EVT_DEFINE_USER_FUNC(apCookChk)
{
    const int32_t ret = gor_cook_chk(evt, isFirstCall);
    // cooking_evt uses result 1 before the cookbook upgrade (GSWF 5392), result 2 after
    applyRecipeReplacement(evt, evt->evtArguments[swdrv::swGet(5392) ? 2 : 1]);
    return ret;
}

EVT_DEFINE_USER_FUNC(apCookChk2)
{
    const int32_t ret = gor_cook_chk2(evt, isFirstCall);
    applyRecipeReplacement(evt, evt->evtArguments[2]);
    return ret;
}

EVT_DEFINE_USER_FUNC(apCookingFlag)
{
    (void)isFirstCall;
    gCookGiveItem = -1; // the cooked item has been received at this point

    if (sPendingRecipe >= 0)
    {
        swdrv::swSet(kRecipeFlagBase + sPendingRecipe);
        sPendingRecipe = -1;
        mod::owr::checkRecipeGoal(); // recipes goal: cooking the Nth recipe warps to the credits
        return 2;
    }

    // Vanilla journal behavior for repeat cooks and reversion outputs
    const int32_t item = static_cast<int32_t>(evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]));
    if (item >= ItemId::GOLD_BAR) // smallest possible vanilla output
        swdrv::swSet(item - 114);
    return 2;
}

EVT_DEFINE_USER_FUNC(apSetCookMode)
{
    (void)isFirstCall;
    sCookMode = evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]) == 1 ? 1 : 0;
    sPendingRecipe = -1;
    gCookGiveItem = -1;
    return 2;
}

EVT_DEFINE_USER_FUNC(apMakeIngredientTbl)
{
    (void)isFirstCall;
    sPendingRecipe = -1;
    gCookGiveItem = -1; // defensive reset at the start of every cooking interaction
    sPouchFull = false;

    // Second menu passes the first pick's table position to exclude (-1 on the first menu)
    const int32_t exclude = static_cast<int32_t>(evtmgr_cmd::evtGetValue(evt, evt->evtArguments[0]));
    int32_t count = 0;

    if (sCookMode == 1)
    {
        // Inventory mode: list the real pouch, vanilla-style (table index == pouch
        // slot index, so the evt's N_evt_pouch_remove_item_index args stay valid)
        const int32_t held = mario_pouch::pouchGetHaveItemCnt();
        for (int32_t i = 0; i < held; i++)
        {
            if (i == exclude)
                continue;
            const int32_t item = mario_pouch::pouchHaveItem(i);
            if (item != 0)
                apCookIngredientTbl[count++] = item;
        }
    }
    else
    {
        // Unlock mode: virtual ingredients, nothing gets consumed, so unlike vanilla
        // the result needs a free pouch slot: offer nothing while the pouch is full.
        const int32_t capacity = mario_pouch::pouchCheckItem(ItemId::STRANGE_SACK) > 0 ? 20 : 10;
        if (mario_pouch::pouchGetHaveItemCnt() < capacity)
        {
            int32_t pos = 0;
            for (int32_t k = 0; k < kIngredientCount; k++)
            {
                if (!swdrv::swGet(kIngredientFlagBase + k))
                    continue;
                if (pos++ == exclude)
                    continue;
                apCookIngredientTbl[count++] = kIngredientIds[k];
            }
        }
        else
        {
            sPouchFull = true;
        }
    }
    apCookIngredientTbl[count] = -1;
    evtmgr_cmd::evtSetValue(evt, evt->evtArguments[1], count);
    return 2;
}

// Wraps the parent's empty-menu evt_msg_print_add (cooking_evt word 491): when the
// unlock menu was hidden only because no result slot is free, explain that instead
// of letting the vanilla "no ingredients" line mislead; the conversation then ends
// through the vanilla goto. Writes the script's own message-arg word, so it must
// set it BOTH ways every call (a swap would otherwise persist in the evt bytecode).
EVT_DEFINE_USER_FUNC(apCookEmptyMsg)
{
    evt->evtArguments[1] = static_cast<int32_t>(
        reinterpret_cast<intptr_t>(sPouchFull ? "ap_cook_full" : "gor_01_cook_005"));
    return ttyd::evt_msg::evt_msg_print_add(evt, isFirstCall);
}

EVT_DEFINE_USER_FUNC(apCookRemove)
{
    // Inventory mode consumes the real ingredients exactly like vanilla; unlock
    // mode cooks from the virtual stock and removes nothing.
    if (sCookMode == 1)
        return ttyd::evt_pouch::N_evt_pouch_remove_item_index(evt, isFirstCall);
    (void)evt;
    (void)isFirstCall;
    return 2;
}

void ApplyGor01Patches()
{
    // Hide the badge shop keeper while Chuchulina runs the counter (trouble 16
    // active); vanilla keyed this on the board's GSW(1420), which is never set.
    // The post-completion hide (GSWF 5390, word 10) stays dead on purpose: our
    // Chuchulina leaves after the trouble, so the keeper should come back.
    gor_badgemaster_init[1] = GSW(1746);
    gor_badgemaster_init[2] = 1;

    gor_badgemaster_init[21] = GSW(1703);
    gor_badgemaster_init[22] = 1;
    gor_badgemaster_init[24] = GSW(1703);
    gor_badgemaster_init[25] = 13;

    patch::writePatch(&gor_badgemaster_talk[0], badgemaster_talk_evt, sizeof(badgemaster_talk_evt));

    patch::writePatch(&gor_kinoji_talk[0], kinoji_talk_evt, sizeof(kinoji_talk_evt));

    patch::writePatch(&gor_marco_init_01[0], marco_init_01_hook, sizeof(marco_init_01_hook));

    gor_marco_talk_01[6] = GSW(1705);
    gor_marco_talk_01[7] = EVT_HELPER_CMD(1, 39);
    gor_marco_talk_01[8] = 99; // Never should be reachable
    gor_marco_talk_01[22] = 0;
    gor_marco_talk_01[483] = GSW(1705);
    gor_marco_talk_01[484] = 1;
    gor_marco_talk_01[485] = EVT_HELPER_CMD(0, 42);
    gor_marco_talk_01[486] = EVT_HELPER_CMD(0, 0);
    gor_marco_talk_01[493] = EVT_HELPER_CMD(0, 49);
    gor_marco_talk_01[494] = EVT_HELPER_CMD(0, 2);
    gor_marco_talk_01[495] = EVT_HELPER_CMD(0, 1);

    gor_master_init[1] = GSW(1705);
    gor_master_init[2] = 1;

    gor_master_talk[99] = GSW(1705);
    gor_master_talk[101] = 1;
    gor_master_talk[109] = 2;
    gor_master_talk[119] = EVT_HELPER_CMD(1, 41);
    gor_master_talk[120] = 3;
    patch::writePatch(&gor_master_talk[121], gor_master_talk_hook, sizeof(gor_master_talk_hook));
    gor_master_talk[483] = GSWF(6099);
    gor_master_talk[544] = GSW(1705);
    gor_master_talk[545] = 4;
    gor_master_talk[556] = 4;
    gor_master_talk[563] = EVT_HELPER_CMD(0, 42);
    gor_master_talk[564] = EVT_HELPER_CMD(0, 0);
    gor_master_talk[571] = EVT_HELPER_CMD(0, 49);
    gor_master_talk[572] = EVT_HELPER_CMD(0, 2);
    gor_master_talk[573] = EVT_HELPER_CMD(0, 1);

    gor_shoptender_init[0] = EVT_HELPER_CMD(0, 0);
    gor_shoptender_init[1] = EVT_HELPER_CMD(0, 0);
    gor_shoptender_init[2] = EVT_HELPER_CMD(0, 0);

    patch::writePatch(&gor_shoptender_talk[189], shoptender_talk_evt, sizeof(shoptender_talk_evt));

    patch::writePatch(&gor_roten1_talk[0], roten1_talk_evt, sizeof(roten1_talk_evt));
    patch::writePatch(&gor_roten2_talk[0], roten2_talk_evt, sizeof(roten2_talk_evt));

    gor_kuribo1_talk[1] = GSW(1734);
    gor_kuribo1_talk[2] = 1;
    gor_kuribo1_talk[4] = GSW(1764);
    gor_kuribo1_talk[13] = GSW(1764);
    patch::writePatch(&gor_kuribo1_talk[25], kuribo1_talk_evt, sizeof(kuribo1_talk_evt));

    patch::writePatch(&gor_kuribo2_talk[0], kuribo2_talk_evt, sizeof(kuribo2_talk_evt));

    gor_kuribo3_talk[1] = GSW(1732);
    gor_kuribo3_talk[2] = 1;
    gor_kuribo3_talk[3] = EVT_HELPER_CMD(2, 26);
    gor_kuribo3_talk[4] = GSW(1762);
    gor_kuribo3_talk[5] = 2;
    gor_kuribo3_talk[7] = GSW(1762);
    gor_kuribo3_talk[33] = GSW(1762);
    gor_kuribo3_talk[50] = EVT_HELPER_CMD(2, 91);
    gor_kuribo3_talk[51] = EVT_HELPER_OP(&irai_complete_item_get);
    gor_kuribo3_talk[57] = GSW(1732);
    gor_kuribo3_talk[58] = 2;
    patch::writePatch(&gor_kuribo3_talk[68], kuribo3_talk_evt, sizeof(kuribo3_talk_evt));
    
    gor_chusan1_init[11] = GSW(1735);
    gor_chusan1_init[12] = 1;

    gor_chusan1_regl[1] = GSW(1735);
    gor_chusan1_regl[2] = 1;

    gor_chusan1_naguru[46] = EVT_HELPER_CMD(2, 91);
    gor_chusan1_naguru[47] = EVT_HELPER_OP(&irai_complete_item_get);
    gor_chusan1_naguru[50] = GSW(1735);
    gor_chusan1_naguru[51] = 2;

    gor_chusan1_talk[1] = GSW(1735);
    gor_chusan1_talk[2] = 1;

    patch::writePatch(&gor_chusan1_talk[104], chusan1_talk_evt, sizeof(chusan1_talk_evt));
    patch::writePatch(&gor_chusan2_talk[0], chusan2_talk_evt, sizeof(chusan2_talk_evt));
    patch::writePatch(&gor_bomhei_talk[0], bomhei_talk_evt, sizeof(bomhei_talk_evt));

    patch::writePatch(&gor_bottakuru_init[0], bottakuru_init_hook, sizeof(bottakuru_init_hook));

    gor_luigi_init_01[1] = GSW(1710);
    gor_luigi_init_01[2] = EVT_HELPER_CMD(1, 36);
    gor_luigi_init_01[3] = 2;
    gor_luigi_init_01[4] = EVT_HELPER_CMD(0, 0);
    gor_luigi_init_01[15] = EVT_HELPER_CMD(1, 36);
    gor_luigi_init_01[16] = 4;
    gor_luigi_init_01[17] = EVT_HELPER_CMD(0, 0);
    gor_luigi_init_01[28] = EVT_HELPER_CMD(1, 41);
    gor_luigi_init_01[29] = 7;

    gor_luigi_npcEnt_01[1] = GSW(1710);
    gor_luigi_npcEnt_01[2] = EVT_HELPER_CMD(1, 36);
    gor_luigi_npcEnt_01[3] = 2;
    gor_luigi_npcEnt_01[4] = EVT_HELPER_CMD(0, 0);
    gor_luigi_npcEnt_01[16] = EVT_HELPER_CMD(1, 36);
    gor_luigi_npcEnt_01[17] = 4;
    gor_luigi_npcEnt_01[18] = EVT_HELPER_CMD(0, 0);
    gor_luigi_npcEnt_01[30] = EVT_HELPER_CMD(1, 41);
    gor_luigi_npcEnt_01[31] = 7;

    gor_kick2_init[1] = GSW(1710);
    gor_kick2_init[2] = EVT_HELPER_CMD(1, 36);
    gor_kick2_init[3] = 7;
    gor_kick2_init[4] = EVT_HELPER_CMD(0, 0);

    gor_borodo2_init_01[1] = GSW(1734);
    gor_borodo2_init_01[2] = 1;
    gor_borodo2_init_01[4] = GSW(1764);
    gor_borodo2_init_01[5] = 3;
    gor_borodo2_init_01[7] = GSW(1764);
    gor_borodo2_init_01[8] = 3;
    gor_borodo2_init_01[10] = GSW(1764);
    gor_borodo2_init_01[11] = 3;
    gor_borodo2_init_01[13] = GSW(1764);
    gor_borodo2_init_01[14] = 3;

    gor_borodo2_talk_01[58] = EVT_HELPER_CMD(2, 91);
    gor_borodo2_talk_01[59] = EVT_HELPER_OP(&irai_complete_item_get);
    gor_borodo2_talk_01[264] = GSW(1764);
    gor_borodo2_talk_01[265] = 4;
    gor_borodo2_talk_01[270] = GSW(1734);
    gor_borodo2_talk_01[271] = 2;

    gor_thief_init[1] = GSW(1700);

    gor_thief_talk[1] = GSW(1700);
    gor_thief_talk[4] = 17;
    gor_thief_talk[89] = EVT_HELPER_CMD(0, 42);
    gor_thief_talk[90] = EVT_HELPER_CMD(0, 0);
    gor_thief_talk[99] = EVT_HELPER_CMD(0, 49);
    gor_thief_talk[100] = EVT_HELPER_CMD(0, 2);
    gor_thief_talk[101] = EVT_HELPER_CMD(0, 1);

    gor_christine_nakama_before[1] = GSW(1700);

    gor_luigi_f1_fade_check[1] = GSW(1710);
    gor_luigi_f1_fade_check[3] = 2;
    gor_luigi_f1_fade_check[4] = 3;
    gor_luigi_f1_fade_check[6] = 4;
    gor_luigi_f1_fade_check[7] = 5;
    gor_luigi_f1_fade_check[23] = 7;

    gor_luigi_f1_fade_check[1] = GSW(1710);
    gor_luigi_f1_fade_check[3] = 2;
    gor_luigi_f1_fade_check[4] = 3;
    gor_luigi_f1_fade_check[6] = 4;
    gor_luigi_f1_fade_check[7] = 5;
    gor_luigi_f1_fade_check[9] = 7;

    gor_marco_hitorigoto[1] = GSW(1705);
    gor_marco_hitorigoto[2] = 1;

    gor_01_shop_setup[16] = PTR(&gor_goods_list);
    gor_01_shop_setup[124] = PTR(&gor_goods_list);
    gor_01_shop_setup[129] = PTR(&gor_goods_list);
    gor_01_shop_setup[134] = PTR(&gor_goods_list);
    gor_01_shop_setup[139] = PTR(&gor_goods_list);
    gor_01_shop_setup[144] = PTR(&gor_goods_list);

    gor_01_koopa_evt[573] = GSW(1706);
    gor_01_koopa_evt[574] = 46;

    gor_iri_09_init[1] = GSW(1739);
    gor_iri_09_init[2] = 1;

    gor_iri_09_tentyo_talk[1] = GSW(1739);
    gor_iri_09_tentyo_talk[2] = 2;
    gor_iri_09_tentyo_talk[9] = GSWF(6332);
    gor_iri_09_tentyo_talk[18] = GSWF(6332);
    gor_iri_09_tentyo_talk[32] = 90;
    gor_iri_09_tentyo_talk[70] = 90;
    gor_iri_09_tentyo_talk[73] = GSW(1769);
    gor_iri_09_tentyo_talk[74] = 1;
    gor_iri_09_tentyo_talk[76] = GSW(1769);
    gor_iri_09_tentyo_talk[77] = 1;
    gor_iri_09_tentyo_talk[85] = 90;
    gor_iri_09_tentyo_talk[99] = GSW(1769);
    gor_iri_09_tentyo_talk[100] = 1;
    gor_iri_09_tentyo_talk[147] = GSW(1739);
    gor_iri_09_tentyo_talk[148] = 2;

    gor_iri_16_init[1] = GSW(1746);
    gor_iri_16_init[2] = 1;

    gor_iri_16_tyutyu_talk[1] = GSW(1746);
    gor_iri_16_tyutyu_talk[2] = 1;
    gor_iri_16_tyutyu_talk[11] = PTR(&iri16BadgeGateBgm); // bgmon wrapper: LW(14) = has badge
    gor_iri_16_tyutyu_talk[15] = GSW(1776);
    gor_iri_16_tyutyu_talk[24] = GSW(1776);
    gor_iri_16_tyutyu_talk[28] = LW(14); // handover gate: badge possession (was chest flag 6354)
    gor_iri_16_tyutyu_talk[405] = EVT_HELPER_OP(LW(3));
    gor_iri_16_tyutyu_talk[408] = EVT_HELPER_OP(&iri_16_mowz_evt);
    gor_iri_16_tyutyu_talk[413] = GSW(1746);
    gor_iri_16_tyutyu_talk[414] = 2;

    gor_cooking_evt[102] = GSW(1748);
    gor_cooking_evt[103] = 1;
    gor_cooking_evt[105] = GSW(1778);
    gor_cooking_evt[114] = GSW(1778);
    gor_cooking_evt[342] = GSW(1748);
    gor_cooking_evt[343] = 2;
    gor_cooking_evt[384] = GSW(1748);
    gor_cooking_evt[385] = 1;
    gor_cooking_evt[415] = GSW(1748);
    gor_cooking_evt[416] = 1;
    gor_cooking_evt[430] = GSW(1748);
    gor_cooking_evt[431] = 1;
    gor_cooking_evt[454] = GSW(1748);
    gor_cooking_evt[455] = 1;

    gor_master_talk[1] = GSW(1750);
    gor_master_talk[2] = 1;
    gor_master_talk[4] = GSW(1780);
    gor_master_talk[8] = GSW(1780);
    gor_master_talk[9] = 1;
    gor_master_talk[62] = GSW(1780);
    gor_master_talk[63] = 2;

    gor_master_talk[77] = GSW(1755);
    gor_master_talk[78] = 1;
    gor_master_talk[79] = EVT_HELPER_CMD(2, 29);
    gor_master_talk[80] = GSW(1785);
    gor_master_talk[81] = 2;
    gor_master_talk[82] = EVT_HELPER_CMD(2, 26);
    gor_master_talk[83] = GSW(1785);
    gor_master_talk[84] = 4;
    gor_master_talk[92] = GSW(1785);
    gor_master_talk[93] = 3;

    gor_01_init_evt[4] = GSW(1706);
    gor_01_init_evt[5] = 45;
    gor_01_init_evt[14] = GSW(1708);
    gor_01_init_evt[15] = 16;
    gor_01_init_evt[35] = GSW(1708);
    gor_01_init_evt[36] = 18;
    gor_01_init_evt[116] = GSW(1700);
    gor_01_init_evt[122] = GSW(1700);
    gor_01_init_evt[138] = GSW(1710);
    gor_01_init_evt[139] = 1;
    gor_01_init_evt[150] = GSW(1710);
    gor_01_init_evt[151] = 1;
    gor_01_init_evt[255] = GSW(1705);
    gor_01_init_evt[256] = 1;
    gor_01_init_evt[308] = GSW(1705);
    gor_01_init_evt[309] = 1;
    gor_01_init_evt[321] = GSW(1700);
    gor_01_init_evt[322] = 5;
    gor_01_init_evt[374] = EVT_HELPER_CMD(2, 29);
    gor_01_init_evt[375] = GSW(1731);
    gor_01_init_evt[385] = GSWF(6331);
    gor_01_init_evt[444] = GSWF(1195);
    gor_01_init_evt[523] = GSW(1708);
    gor_01_init_evt[524] = 18;

    gor_cooking_evt[412] = GSW(1715);
    gor_cooking_evt[413] = 4;

    // Assembly
    patch::writeIntWithCache(&gor_iri_09_item_tbl_make[12], 0x3800005A);

    // Zess T.: once westside has been reached (GSWF 6124, set in OWR), she guards
    // her kitchen door instead of the west gate — the Contact Lens must still be
    // delivered (GSWF 1188) to open the kitchen and cook. Word indices
    // byte-verified against vanilla gor.rel (nancy_init sec5+0x10B50 starts with a
    // 3-word setii; nancy_regl word 20 is the block-loop's GSWF(1188) arg).
    gor_nancy_init[0] = EVT_HELPER_CMD(1, 94); // RUN_CHILD_EVT
    gor_nancy_init[1] = PTR(ap_nancy_init_evt);
    gor_nancy_init[2] = EVT_HELPER_CMD(0, 2);  // RETURN (rest of the evt is dead)
    gor_nancy_regl[20] = GSWF(6124);           // gate-block physics only while she guards the gate

    // AP cooking: the player picks a mode per cook — the unlocked-ingredient stock
    // (AP checks; nothing consumed; repeats/reversions = Mistake) or the real
    // inventory (pure vanilla cooking, ingredients consumed).
    // Word indices byte-verified against vanilla gor.rel (see rel/misc/cooking_research.md §2).
    // With cooksanity off none of this is installed: Zess T. cooks fully vanilla, and
    // the dol 0x500 recipe table (unwritten in that case) is never read.
    if (mod::owr::gState->apSettings->cooksanity)
    {
        gor_cooking_evt[483] = EVT_HELPER_CMD(1, 94);     // RUN_CHILD_EVT (was: callc make_item_tbl)
        gor_cooking_evt[484] = PTR(ap_cook_mode_evt);     // mode select + 1st menu fill
        gor_cooking_evt[485] = EVT_HELPER_CMD(1, 3);      // LBL(999): no-op filler over the old args
        gor_cooking_evt[486] = 999;
        gor_cooking_evt[491] = PTR(&apCookEmptyMsg);      // empty-menu msg: pouch-full aware
        gor_cooking_evt[506] = PTR(apCookIngredientTbl);  // select-window table #1 (vanilla .bss tbl only fits 20)
        gor_cooking_evt[549] = PTR(&apMakeIngredientTbl); // 2nd ingredient menu (keeps the chosen mode)
        gor_cooking_evt[575] = PTR(apCookIngredientTbl);  // select-window table #2
        gor_cooking_evt[695] = PTR(&apCookChk);           // single-ingredient result
        gor_cooking_evt[700] = PTR(&apCookRemove);        // consume (inventory mode) / no-op (unlock mode)
        gor_cooking_evt[717] = PTR(&apCookChk2);          // two-ingredient result
        gor_cooking_evt[722] = PTR(&apCookRemove);
        gor_cooking_evt[727] = PTR(&apCookRemove);
        gor_cooking_evt[819] = PTR(&apCookingFlag);       // sets the recipe check flag after receipt
    }
}
