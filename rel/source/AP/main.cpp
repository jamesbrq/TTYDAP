#include "AP/rel_patch_definitions.h"
#include "common.h"
#include "evt_cmd.h"
#include "patch.h"
#include "string.h"
#include "ttyd/evtmgr_cmd.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/seq_mapchange.h"
#include "ttyd/seqdrv.h"
#include "ttyd/swdrv.h"
#include "visibility.h"
#include <ttyd/evt_shop.h>

#include <cstdint>
#include <cstring>

using namespace ttyd::evtmgr_cmd;
using namespace ttyd::common;
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
} // namespace mod::owr

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
// clang-format on

static const char *goods[] =
    {"gor_01", "gor_03", "tik_00", "nok_00", "mri_07", "tou_01", "usu_01", "muj_01", "rsh_03", "bom_02"};

EVT_DEFINE_USER_FUNC_KEEP(setShopFlags)
{
    (void)isFirstCall;

    int gswfBase = 6200;
    const char *nextMapPtr = &ttyd::seq_mapchange::_next_map[0];
    constexpr int loopCount = static_cast<int>(sizeof(goods) / sizeof(goods[0]));

    for (int i = 0; i < loopCount; i++)
    {
        if (strncmp(nextMapPtr, goods[i], 6) != 0)
        {
            if (i == loopCount - 1)
                return 2;

            gswfBase += 6;
            continue;
        }
        break;
    }

    char *shopWork = reinterpret_cast<char *>(evtGetValue(evt, evt->evtArguments[0]));
    uint32_t *itemIds = *reinterpret_cast<uint32_t **>(shopWork + 0x08);
    uint16_t *itemFlags = reinterpret_cast<uint16_t *>(shopWork + 0x14);
    uint32_t selectedIndex = *reinterpret_cast<uint32_t *>(shopWork + 0x2C);

    swSet(gswfBase + selectedIndex);

    if (itemIds[selectedIndex * 2] > 125)
        return 2;

    itemFlags[selectedIndex] |= 1;
    return 2;
}

void checkShopFlag(uint32_t item, uint32_t index)
{
    if (item > 125)
        return;

    int gswfBase = 6200;
    const char *nextMapPtr = &ttyd::seq_mapchange::_next_map[0];
    constexpr int loopCount = static_cast<int>(sizeof(goods) / sizeof(goods[0]));

    for (int i = 0; i < loopCount; i++)
    {
        if (strncmp(nextMapPtr, goods[i], 6) != 0)
        {
            if (i == loopCount - 1)
                return;

            gswfBase += 6;
            continue;
        }
        break;
    }

    uintptr_t shopWorkPtr = 0x8041EB60;
    char *shopWork = *reinterpret_cast<char **>(shopWorkPtr);
    uint16_t *itemFlags = reinterpret_cast<uint16_t *>(shopWork + 0x14);

    if (swGet(gswfBase + index))
        itemFlags[index] |= 1;
}
