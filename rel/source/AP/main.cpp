#include "patch.h"
#include "evt_cmd.h"
#include "AP/rel_patch_definitions.h"
#include "visibility.h"
#include "ttyd/seqdrv.h"
#include "ttyd/evtmgr_cmd.h"
#include "ttyd/swdrv.h"
#include <ttyd/evt_shop.h>
#include "ttyd/mario_pouch.h"

#include <cstdint>

using namespace ttyd::evtmgr_cmd;
using namespace ttyd::swdrv;

namespace mod::owr
{
    KEEP_VAR const char *goombellaName = "goombella";
    KEEP_VAR const char *goombellaDescription = "goombella_desc";
    KEEP_VAR const char *koopsName = "koops";
    KEEP_VAR const char *koopsDescription = "koops_desc";
    KEEP_VAR const char *flurrieName = "flurrie";
    KEEP_VAR const char *flurrieDescription = "flurrie_desc";
    KEEP_VAR const char *yoshiName = "yoshi";
    KEEP_VAR const char *yoshiDescription = "yoshi_desc";
    KEEP_VAR const char *vivianName = "vivian";
    KEEP_VAR const char *vivianDescription = "vivian_desc";
    KEEP_VAR const char *bobberyName = "bobbery";
    KEEP_VAR const char *bobberyDescription = "bobbery_desc";
    KEEP_VAR const char *mowzName = "mowz";
    KEEP_VAR const char *mowzDescription = "mowz_desc";
    KEEP_VAR const char *apItemNameDescription = "ap_item";
    KEEP_VAR const char *tenCoinsNameDescription = "10_coins";
    KEEP_VAR const char *planeModeNameDescription = "plane_mode";
    KEEP_VAR const char *paperModeNameDescription = "paper_mode";
    KEEP_VAR const char *tubeModeNameDescription = "tube_mode";
    KEEP_VAR const char *boatModeNameDescription = "boat_mode";

    // clang-format off
EVT_BEGIN_KEEP(main_buy_evt_evt)
    USER_FUNC(ttyd::evt_shop::get_ptr, LW(0))
    USER_FUNC(setShopFlags, LW(0))
    USER_FUNC(ttyd::evt_shop::get_buy_evt, LW(1))
    IF_NOT_EQUAL(LW(1), 0)
        SET(LW(0), LW(12))
        RUN_CHILD_EVT(LW(1))
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN_KEEP(main_buy_evt_hook)
    RUN_CHILD_EVT(main_buy_evt_evt)
    RETURN()
EVT_END()
    // clang-format on

    KEEP_VAR uint16_t main_buy_evt_hook_size = sizeof(main_buy_evt_hook);
} // namespace mod::owr

const uintptr_t goods[] =
    {0x805f162c, 0x80612510, 0x805c8738, 0x805be5b4, 0x805fb948, 0x805d5874, 0x805d0588, 0x805dc5b8, 0x805de110, 0x805cbb14};

KEEP_FUNC EVT_DEFINE_USER_FUNC(setShopFlags)
{
    (void)isFirstCall;
    char *shopWork = reinterpret_cast<char *>(evtGetValue(evt, evt->evtArguments[0]));

    uint32_t *itemIds = *reinterpret_cast<uint32_t **>(shopWork + 0x08);
    uint16_t *itemFlags = reinterpret_cast<uint16_t *>(shopWork + 0x14);
    uint32_t selectedIndex = *reinterpret_cast<uint32_t *>(shopWork + 0x2C);

    int gswfBase = 6200;
    uintptr_t itemIdsAddress = reinterpret_cast<uintptr_t>(itemIds);
    int loopCount = static_cast<int>(sizeof(goods) / sizeof(uintptr_t));
    for (int i = 0; i < loopCount; i++)
    {
        if (itemIdsAddress != goods[i])
        {
            if (i == loopCount - 1)
                return 2;
            gswfBase += 6;
            continue;
        }
        break;
    }
    swSet(gswfBase + selectedIndex);
    if (itemIds[selectedIndex * 2] > 120)
        return 2;
    itemFlags[selectedIndex] |= 1;
    return 2;
}

EVT_DEFINE_USER_FUNC_KEEP(checkChapterClears)
{
    (void)isFirstCall;

    int8_t count = 0;
    for (int i = 114; i <= 120; i++)
    {
        if (ttyd::mario_pouch::pouchCheckItem(i) > 0)
            count++;
    }
    ttyd::evtmgr_cmd::evtSetValue(evt, evt->evtArguments[0], count);
    return 2;
}

void checkShopFlag(uint32_t item, uint32_t index)
{
    if (item > 120)
        return;

    uintptr_t shopWorkPtr = 0x8041EB60;
    char *shopWork = *reinterpret_cast<char **>(shopWorkPtr);
    uint32_t *itemIds = *reinterpret_cast<uint32_t **>(shopWork + 0x08);
    uint16_t *itemFlags = reinterpret_cast<uint16_t *>(shopWork + 0x14);

    int gswfBase = 6200;
    uintptr_t itemIdsAddress = reinterpret_cast<uintptr_t>(itemIds);
    int loopCount = static_cast<int>(sizeof(goods) / sizeof(uintptr_t));
    for (int i = 0; i < loopCount; i++)
    {
        if (itemIdsAddress != goods[i])
        {
            if (i == loopCount - 1)
                return;
            gswfBase += 6;
            continue;
        }
        break;
    }
    if (swGet(gswfBase + index))
        itemFlags[index] |= 1;
}
