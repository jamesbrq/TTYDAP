#pragma once

#include <ttyd/dispdrv.h>

#include <cstdint>

// Forward declarations.
namespace ttyd::effdrv
{
    struct EffEntry;
}
namespace ttyd::evtmgr
{
    struct EvtEntry;
}
namespace ttyd::filemgr
{
    struct File;
}

namespace ttyd::win_root
{
    extern "C"
    {
        struct SortMethodInfo
        {
            const char *type;
            void *func;
        };
        static_assert(sizeof(SortMethodInfo) == 8);

        struct WinTabHeaderInfo
        {
            int32_t id;
            float x;
            float y;
            int32_t state;
            int32_t timer;
            const char *desc;
        };
        static_assert(sizeof(WinTabHeaderInfo) == 0x18);

        struct WinTabBodyInfo
        {
            int32_t id;
            float x;
            float y;
            int32_t state;
            int32_t timer;
        };
        static_assert(sizeof(WinTabBodyInfo) == 0x14);

        struct WinEmailInfo
        {
            uint8_t order;
            uint8_t id;
        };
        static_assert(sizeof(WinEmailInfo) == 2);

        struct WinEquippedBadgeInfo
        {
            int32_t isEquipped;
            int32_t badgeMenuIdx;
            int32_t badgeId;
        };
        static_assert(sizeof(WinEquippedBadgeInfo) == 0xc);

        struct WinLogSubmenuInfo
        {
            int32_t id;
            float x;
            float y;
            float targetX;
            float targetY;
            int32_t state;
            int32_t timer;
            const char *helpMsg;
        };
        static_assert(sizeof(WinLogSubmenuInfo) == 0x20);

        struct WinLogTattleInfo
        {
            uint8_t order;
            uint8_t id;
        };
        static_assert(sizeof(WinLogTattleInfo) == 2);

        struct WinLogTattleMenuWork
        {
            int32_t state;
            int32_t enemyId;
            filemgr::File *winTplFile;
            filemgr::File *monosiriDatFile;
            float x;
            float y;
            float targetX;
            float targetY;
            int32_t animPose;
            int32_t animPose2;
            void *textureObj;
            int32_t alpha;
        };
        static_assert(sizeof(WinLogTattleMenuWork) == 0x30);

        struct WinPauseMenu
        {
            uint16_t flags;
            uint8_t unk_0x0002[2];
            uint32_t buttonsPressed;
            uint32_t buttonsRepeated;
            uint32_t dirsPressed;
            uint32_t dirsRepeated;
            int32_t lectureState;
            evtmgr::EvtEntry *lectureEvtEntry;
            int32_t initialRootMenuId;
            int32_t menuState;
            int32_t rootMenuState;
            filemgr::File *winTplFile;
            filemgr::File *mailTplFile;
            filemgr::File *mapTplFile;
            void *fbufWork;
            int32_t fbufFinished;
            evtmgr::EvtEntry *useItemEvtEntry;
            int32_t rootMenuCursorIdx;
            int32_t rootMenuCursorMax;
            WinTabHeaderInfo tabHeaderInfo[5];
            WinTabBodyInfo tabBodyInfo[5];
            // Used by sort menu, possibly description window as well?
            int32_t parentMenuState;
            float msgWinX;
            float msgWinY;
            int32_t msgWinState;
            int32_t msgWinTimer;
            int32_t msgWinItemId;
            char *msgWinBody;
            char *msgWinNameplate;
            char *msgWinBodyCopy;
            int32_t msgWinLineCur;
            int32_t msgWinLineMax;
            float mainCursorX;
            float mainCursorY;
            float mainCursorTargetX;
            float mainCursorTargetY;

            // "Mario" menu + sort menu.
            int32_t marioMenuState;
            float sortMenuX;
            float sortMenuY;
            float sortMenuTargetX;
            float sortMenuTargetY;
            int32_t sortMenuCursorIdx;
            int32_t sortMenuCursorMax;
            int32_t sortMenuState;
            int32_t hasSorted;
            int32_t sortMenuType;
            int32_t marioAnimPoseId;
            int32_t marioAnimTimer;
            int32_t starPowersListOpen;
            int32_t starPowersCursorIdx;
            int32_t starPowersCount;

            // "Party" menu.
            int32_t isSwappingParty;
            int32_t partyMenuAnimPoseIds[7];
            int32_t partyMenuMemberIds[7];
            int32_t activePartyDataLookupIdx; // Index into winPartyDt table.
            int32_t partyCursorIdx;
            int32_t partyCount;
            float partyCircleRot;
            float partyCircleTargetRot;
            int8_t unk_0x01ec[16];
            int32_t currentFieldPartyIdx;
            int32_t partyMenuState;
            int32_t partyMovesCursorMax;
            int32_t partyMovesCursorIdx;

            // "Item" menu.
            int32_t itemMenuState;
            int32_t itemSubmenuId;
            // Indexed by itemSubmenuId.
            int32_t itemsCursorIdx[2];
            int32_t itemsPageNum[2];
            float itemsOffsetY[2];
            float itemsTargetY[2];
            float superLuigiWinX;
            float superLuigiWinTargetX;
            float superLuigiWinY;
            float superLuigiWinTargetY;
            float mailWinX;
            float mailWinTargetX;
            float mailWinY;
            float mailWinTargetY;
            int32_t emailsCount;
            int32_t emailsCursorIdx;
            int32_t emailsPageNum;
            float emailScrollY;
            float emailScrollTargetY;
            int32_t emailLineCur;
            int16_t emailLineMax;
            int16_t emailIds[42]; // Unsure.
            uint8_t unk_0x02c2[6];
            int32_t superLuigiBookId;
            int32_t superLuigiPageMax;
            int32_t superLuigiPageCur;
            int32_t useItemType;
            int32_t useItemIdx;
            int32_t useItemMenuCursorIdx;
            int32_t useItemTimer;
            int8_t unk_0x02e4[4];
            int16_t keyItemIds[121];
            int16_t keyItemsCount;

            // "Badges" menu.
            int32_t badgeMenuState;
            int32_t badgeSubmenuId;
            // Indexed by badgeSubmenuId.
            int32_t badgesCursorIdx[2];
            int32_t badgesPageNum[2];
            float badgesOffsetY[2];
            float badgesTargetY[2];
            WinEquippedBadgeInfo equippedBadges[200];
            int32_t equippedBadgesCount;

            // "Journal" menu + submenus.
            int32_t logMenuState;
            int32_t logSubmenuCursorIdx;
            int32_t logSubmenuCount;
            WinLogSubmenuInfo logSubmenuInfo[5];
            // Map.
            int32_t mapCursorIdx;
            int32_t mapCurrentLocationAreaIdx;
            int32_t mapAnimPoseIdx;
            float mapCursorX;
            float mapCursorY;
            float mapTargetX;
            float mapCurrentX;
            float mapTargetY;
            float mapCurrentY;
            float mapScrollX;
            float mapScrollY;
            // Crystal Stars.
            int32_t crystalCount;
            effdrv::EffEntry *crystalEffs[7];
            int32_t crystalCursorIdx;
            float unk_0x0e64[2]; // Seemingly unused target-current position pair.
            float crystalStarWinTargetX;
            float crystalStarWinX;
            float crystalStarWinTargetY;
            float crystalStarWinY;
            float crystalStarTargetRot;
            float crystalStarRot;
            // Badge log.
            float badgeLogWinX;
            float badgeLogWinTargetX;
            float badgeLogWinY;
            float badgeLogWinTargetY;
            int32_t badgeLogTotalCount;
            int32_t badgeLogObtainedCount;
            int32_t badgeLogCursorIdx;
            int32_t badgeLogPageNum;
            float badgeLogScrollY;
            float badgeLogScrollTargetY;
            float badgeLogShowcaseX;
            float badgeLogShowcaseTargetX;
            float badgeLogShowcaseY;
            float badgeLogShowcaseTargetY;
            int16_t badgeLogIds[100];
            // Recipe log.
            float recipeLogWinX;
            float recipeLogWinTargetX;
            float recipeLogWinY;
            float recipeLogWinTargetY;
            int32_t recipeLogTotalCount;
            int32_t recipeLogObtainedCount;
            int32_t recipeLogCursorIdx;
            int32_t recipeLogPageNum;
            float recipeLogScrollY;
            float recipeLogScrollTargetY;
            float recipeLogShowcaseX;
            float recipeLogShowcaseTargetX;
            float recipeLogShowcaseY;
            float recipeLogShowcaseTargetY;
            int16_t recipeLogIds[57];
            uint8_t unk_0x102e[2];
            // Tattle log.
            float unk_0x1030[4]; // Seemingly unused target-current X/Y pairs.
            int32_t tattleLogTotalCount;
            int32_t tattleLogObtainedCount;
            int32_t tattleLogCursorIdx;
            int32_t tattleLogPageNum;
            float unk_0x1050;
            float unk_0x1054;
            WinLogTattleInfo tattleLogs[216];
            WinLogTattleMenuWork *tattleLogMenuWork;
            int8_t unk_0x120c[4];

            // WinMgrEntry indices for additional window dialogs
            // (used when choosing an item to use in the "Items" menu).
            int32_t winmgr_entry_1;
            int32_t winmgr_entry_2;
        };
        static_assert(sizeof(WinPauseMenu) == 0x1218);

        // winMailDisp
        void winZClear();
        void winKageGX(float x, float y, float z, float scale, WinPauseMenu *menu, uint32_t *color);
        uint32_t winSortWait(WinPauseMenu *menu);
        void winSortEntry(float x, float y, WinPauseMenu *menu, int sortType);
        // winSortGX
        // winSortMain
        // sort_8_2_func
        // compare_func5_r
        // compare_func5
        // sort_8_1_func
        // compare_func4_r
        // compare_func4
        void sort_7_3_func(WinPauseMenu *menu);
        // compare_func3_r
        // compare_func3
        // sort_7_2_func
        // compare_func2_r
        // compare_func2
        // sort_7_1_func
        // compare_func1_r
        // compare_func1
        // sort_6_2_func
        // sort_6_1_func
        int32_t compare_func6_2_r(int16_t *param_1, int16_t *param_2);
        int32_t compare_func6_2(int16_t *param_1, int16_t *param_2);
        int32_t compare_func6_1_r(int16_t *param_1, int16_t *param_2);
        int32_t compare_func6_1(int16_t *param_1, int16_t *param_2);
        // sort_5_3_func
        // sort_5_2_func
        // sort_5_1_func
        int32_t compare_func5_2_r(int16_t *param_1, int16_t *param_2);
        int32_t compare_func5_2(int16_t *param_1, int16_t *param_2);
        int32_t compare_func5_1_r(int16_t *param_1, int16_t *param_2);
        int32_t compare_func5_1(int16_t *param_1, int16_t *param_2);
        // sort_4_3_func
        // sort_4_2_func
        // sort_4_1_func
        // N_compare_func4_2_r
        // N_compare_func4_2
        // N_compare_func4_1_r
        // N_compare_func4_1
        // sort_3_3_func
        // sort_3_2_func
        // sort_3_1_func
        // sort_2_2_func
        // sort_2_1_func
        // sort_1_2_func
        // sort_1_1_func
        void winWazaGX(float x, float y, float w, float h, WinPauseMenu *menu, int32_t type);
        void winHakoGX(float x, float y, WinPauseMenu *menu, int32_t type);
        // winMailGX
        // winHalfBookGX
        void winBookGX(float x, float y, WinPauseMenu *menu, int32_t type);
        void winNameGX(float x, float y, float w, float h, WinPauseMenu *menu, int32_t type);
        void winKirinukiGX(float x, float y, float w, float h, WinPauseMenu *menu, int32_t type);
        void winMsgEntry(WinPauseMenu *menu, int32_t item_id, const char *msg, const char *name);
        // winMsgDisp
        // winMsgMain
        void winBgGX(float x, float y, WinPauseMenu *menu, int32_t type);
        // winBgMain
        void winRootDisp(ttyd::dispdrv::CameraId camera, ttyd::win_root::WinPauseMenu *menu);
        // winRootMain
    }

} // namespace ttyd::win_root