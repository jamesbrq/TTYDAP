#pragma once
#include "evt_cmd.h"
#include "visibility.h"
#include <AP/rel_patch_definitions.h>
#include <gc/OSModule.h>
#include <gc/OS.h>
#include <gc/types.h>
#include <StateManager.h>
#include <ttyd/battle_database_common.h>
#include <ttyd/battle_unit.h>
#include <ttyd/evtmgr.h>
#include <ttyd/msgdrv.h>
#include <ttyd/npcdrv.h>
#include <ttyd/party.h>
#include <ttyd/seqdrv.h>
#include <ttyd/win_root.h>
#include <ttyd/windowdrv.h>

#include <cstdint>
#include <cstring>

using namespace ttyd::seqdrv;
using namespace ttyd::party;
using namespace ttyd::evtmgr;
using namespace ttyd::msgdrv;
using namespace ttyd::battle_unit;
using namespace ttyd::battle_database_common;
using namespace gc;

namespace mod::owr
{
    class OWR
    {
       public:
        OWR();
        ~OWR() {};
        void Update();
        void OnModuleLoaded(OSModuleInfo *module_info);
        void SequenceInit();
        void DrawString(const char *data, float x, float y, uint32_t color, float scale = 1.0f);
        void RecieveItems();

        StateManager state;
    };

    struct NumericInputData
    {
        int32_t window_id;
        int16_t initialValue;
        int16_t currentValue;
        int16_t selectedValue;
        int16_t minValue;
        int16_t maxValue;
        uint8_t stepSize;
        bool active;

        void clearState() { memset(this, 0, sizeof(NumericInputData)); }
    };

    extern OWR *gSelf;
    extern StateManager *gState;
    extern NumericInputData g_numericInput;

    bool OSLinkHook(OSModuleInfo *new_module, void *bss);
    void seqSetSeqHook(SeqIndex seq, const char *map, const char *bero);
    uint32_t pouchGetItemHook(int32_t item);
    void partySetForceMoveHook(PartyEntry *ptr, float x, float z, float speed);
    int32_t evtMarioSetPoseHook(EvtEntry *evt, bool firstCall);
    const char *msgSearchHook(const char *msgKey);
    void logoSkip(SeqInfo *index);
    void seq_gameInitHook(SeqInfo *index);
    void DisplayStarPowerNumber();
    void DisplayStarPowerOrbs(float x, float y, int32_t star_power);
    void SetMaxSP(int32_t star);
    int32_t WinItemMainHook(ttyd::win_root::WinPauseMenu *menu);
    int32_t WinLogMainHook(ttyd::win_root::WinPauseMenu *menu);
    void MsgAnalizeHook(ttyd::memory::SmartAllocationData *smartAlloc, const char *text);
    int msgWindow_Entry_Hook(const char *message, int unk1, int windowType);
    void _load_Hook(const char *mapName, const char *entranceName, const char *beroName);
    int numericWindow_Main(ttyd::windowdrv::Window *window);
    void numericWindow_Disp(ttyd::dispdrv::CameraId cameraId, void *user);
    void replaceMultipleCharacters(ttyd::memory::SmartAllocationData *smartData, uint32_t startIndex, int value);
    BattleWorkUnit *BtlUnit_Entry_Hook(BattleUnitSetup *setup);
    void ExecAllUnitBattleEndEvent_Hook();
    void RegisterOriginalKind(BattleUnitSetup *setup, BattleUnitKind *orig);
    void ScaleUnitStats(BattleUnitKind *unit, RelId rel);
    int main__psndSFXOnHook(int idOrName, int vol, int pan, int a4, const void *pos, int a6, int a7, int a8);
    int psndSFXOffHook(int channel);
    void npcSetupBattleInfoHook(::NpcEntry *npc, void *info);
    int32_t pouchRemoveItemHook(int32_t item);
    void swSetHook(int gswf);
    void DrainReceivedFlags();
    int32_t AlterDamageCalculation(BattleWorkUnit *attacker,
                                   BattleWorkUnit *target,
                                   BattleWorkUnitPart *target_part,
                                   BattleWeapon *weapon,
                                   uint32_t *unk0,
                                   uint32_t unk1);
    int32_t InterruptStopHook(ttyd::evtmgr::EvtEntry *evt, bool isFirstCall);
    int32_t BattleCheckConcludedHook(void *battleWork);

    extern const char *kChampStageGlobalDir;
    extern const char *kChampStageCurrentDir;
    extern BattleStageObjectData kChampStageProps[];
    constexpr int32_t kChampNumProps = 8;
    extern const char *kZakoStageGlobalDir;
    extern const char *kZakoStageCurrentDir;
    extern BattleStageObjectData kZakoStageProps[];
    extern BattleUnitSetup gKanbuPartyUnits[2];
    extern BattleGroupSetup gKanbuGroup;
    constexpr int32_t kZakoNumProps = 9;

    inline void ApplyBossGroups(const BattleGroupIndexRange &range)
    {
        if (!gState->apSettings->bossRandomizer)
            return;
        for (int i = range.start; i <= range.end; i++)
        {
            BattleGroupSetup *bossGroup = bossGroupList[i];
            EnemyLoadout &loadout = gState->bossLoadouts[i];

            bool kanbuHere = false;
            for (int32_t j = 0; j < bossGroup->num_enemies; j++)
                if (loadout.enemyIds[j] == 0x63)
                    kanbuHere = true;

            // Lord Crump is a two-member party: group-1 X-Naut (0x64) in slot 0,
            // Crump (0x63) in slot 1. Our random party only ever writes 0x63, so
            // build the full vanilla-shaped party and swap it in whole.
            if (kanbuHere)
            {
                gKanbuPartyUnits[0].unit_kind_params = GetUnitKindById(0x64);
                gKanbuPartyUnits[0].alliance = 0x01;
                gKanbuPartyUnits[0].attack_phase = 0x04000004;
                gKanbuPartyUnits[0].position = {80.0f, 0.0f, -20.0f};
                gKanbuPartyUnits[0].addl_target_offset_x = 0;
                gKanbuPartyUnits[0].unit_work[0] = 0;
                gKanbuPartyUnits[0].unit_work[1] = 0;
                gKanbuPartyUnits[0].unit_work[2] = 0;
                gKanbuPartyUnits[0].unit_work[3] = 0;
                gKanbuPartyUnits[0].item_drop_table = bossGroup->enemy_data[0].item_drop_table;

                gKanbuPartyUnits[1].unit_kind_params = GetUnitKindById(0x63);
                gKanbuPartyUnits[1].alliance = 0x01;
                gKanbuPartyUnits[1].attack_phase = 0x04000004;
                gKanbuPartyUnits[1].position = {160.0f, 0.0f, 0.0f};
                gKanbuPartyUnits[1].addl_target_offset_x = 0;
                gKanbuPartyUnits[1].unit_work[0] = 0;
                gKanbuPartyUnits[1].unit_work[1] = 0;
                gKanbuPartyUnits[1].unit_work[2] = 0;
                gKanbuPartyUnits[1].unit_work[3] = 0;
                gKanbuPartyUnits[1].item_drop_table = bossGroup->enemy_data[(bossGroup->num_enemies > 1 ? 1 : 0)].item_drop_table;

                gKanbuGroup.num_enemies = 2;
                gKanbuGroup.enemy_data = gKanbuPartyUnits;
                gKanbuGroup.held_item_weight = bossGroup->held_item_weight;
                gKanbuGroup.random_item_weight = bossGroup->random_item_weight;
                gKanbuGroup.no_item_weight = bossGroup->no_item_weight;
                gKanbuGroup.hp_drop_table = bossGroup->hp_drop_table;
                gKanbuGroup.fp_drop_table = bossGroup->fp_drop_table;
                gKanbuGroup.unk_1c = bossGroup->unk_1c;

                RegisterOriginalKind(&gKanbuPartyUnits[0], bossGroup->enemy_data[0].unit_kind_params);
                RegisterOriginalKind(&gKanbuPartyUnits[1], bossGroup->enemy_data[(bossGroup->num_enemies > 1 ? 1 : 0)].unit_kind_params);

                if (bossSetupList[i])
                    bossSetupList[i]->group_data = &gKanbuGroup;

                if (bossSetupList[i] && bossSetupList[i]->stage_data && kZakoStageGlobalDir && kZakoStageCurrentDir)
                {
                    BattleStageData *st = bossSetupList[i]->stage_data;
                    st->global_stage_data_dir = kZakoStageGlobalDir;
                    st->current_stage_data_dir = kZakoStageCurrentDir;
                    st->num_props = kZakoNumProps;
                    st->props = kZakoStageProps;
                }
                continue;
            }

            // Every other boss encounter collapses to a single enemy. The vanilla
            // group may have >1 slot (e.g. the real kanbu group); process slot 0
            // and force the count to 1 so stale slots are never spawned.
            bool championHere = false;
            BattleUnitSetup &unit = bossGroup->enemy_data[0];
            RegisterOriginalKind(&unit, unit.unit_kind_params);
            unit.position.x = GetEnemyXPosition(loadout.enemyIds[0], unit.position.x);
            unit.position.y = GetEnemyYPosition(loadout.enemyIds[0]);
            unit.unit_kind_params = GetUnitKindById(loadout.enemyIds[0]);
            if (loadout.enemyIds[0] == 0x40)
                championHere = true;
            bossGroup->num_enemies = 1;

            // champion needs the Glitz Pit stage; rewrite this loaded encounter
            if (championHere && bossSetupList[i] && bossSetupList[i]->stage_data && kChampStageGlobalDir &&
                kChampStageCurrentDir)
            {
                BattleStageData *st = bossSetupList[i]->stage_data;
                st->global_stage_data_dir = kChampStageGlobalDir;
                st->current_stage_data_dir = kChampStageCurrentDir;
                st->num_props = kChampNumProps;
                st->props = kChampStageProps;
            }
        }
    }

    extern const char *const goods[];
    extern const int goodsCount;

    extern bool (*g_OSLink_trampoline)(OSModuleInfo *, void *);
    extern void (*g_seqSetSeq_trampoline)(SeqIndex, const char *, const char *);
    extern void (*g_seq_logoMain_trampoline)(SeqInfo *);
    extern void (*g_seq_gameInit_trampoline)(SeqInfo *);
    extern uint32_t (*g_pouchGetItem_trampoline)(int32_t);
    extern void (*g_partySetForceMove_trampoline)(ttyd::party::PartyEntry *, float, float, float);
    extern int32_t (*g_evt_mario_set_pose_trampoline)(ttyd::evtmgr::EvtEntry *, bool);
    extern const char *(*g_msgSearch_trampoline)(const char *);
    extern void (*g_statusWinDisp_trampoline)(void);
    extern void (*g_pouchGetStarstone_trampoline)(int32_t);
    extern int32_t (*g_winItemMain_trampoline)(ttyd::win_root::WinPauseMenu *);
    extern int32_t (*g_winLogMain_trampoline)(ttyd::win_root::WinPauseMenu *);
    extern void (*g_msgAnalize_trampoline)(ttyd::memory::SmartAllocationData *, const char *);
    extern int (*g_msgWindow_Entry_trampoline)(const char *, int, int);
    extern void (*g__load_trampoline)(const char *, const char *, const char *);
    extern BattleWorkUnit *(*g_BtlUnit_Entry_trampoline)(BattleUnitSetup *);
    extern void (*g_ExecAllUnitBattleEndEvent_trampoline)();
    extern int (*g_main__psndSFXOn_trampoline)(int, int, int, int, const void *, int, int, int);
    extern int (*g_psndSFXOff_trampoline)(int);
    extern void (*g_npcSetupBattleInfo_trampoline)(::NpcEntry *, void *);
    extern int32_t (*g_pouchRemoveItem_trampoline)(int32_t);
    extern void (*g_swSet_trampoline)(int);
    extern int32_t (*g_BattleCalculateDamage_trampoline)(BattleWorkUnit *,
                                                         BattleWorkUnit *,
                                                         BattleWorkUnitPart *,
                                                         BattleWeapon *,
                                                         uint32_t *,
                                                         uint32_t);
    extern int32_t (*g_InterruptStop_trampoline)(ttyd::evtmgr::EvtEntry *, bool);
    extern int32_t (*g_BattleCheckConcluded_trampoline)(void *);

    extern const char *goombellaName;
    extern const char *goombellaDescription;
    extern const char *koopsName;
    extern const char *koopsDescription;
    extern const char *flurrieName;
    extern const char *flurrieDescription;
    extern const char *yoshiName;
    extern const char *yoshiDescription;
    extern const char *vivianName;
    extern const char *vivianDescription;
    extern const char *bobberyName;
    extern const char *bobberyDescription;
    extern const char *mowzName;
    extern const char *mowzDescription;
    extern const char *apItemName;
    extern const char *apItemDescription;
    extern const char *tenCoinsName;
    extern const char *tenCoinsDescription;
    extern const char *returnPipeName;
    extern const char *returnPipeDescription;
    extern const char *planeModeName;
    extern const char *planeModeDescription;
    extern const char *paperModeName;
    extern const char *paperModeDescription;
    extern const char *tubeModeName;
    extern const char *tubeModeDescription;
    extern const char *boatModeName;
    extern const char *boatModeDescription;
    extern const char *elevatorKeyStationName;
    extern const char *cardKey1Name;
    extern const char *cardKey2Name;
    extern const char *cardKey3Name;
    extern const char *cardKey4Name;
    extern const char *palaceKeyTowerName;
    extern const char *elevatorKey1Name;
    extern const char *elevatorKey2Name;
    extern const char *stationKey1Name;
    extern const char *stationKey2Name;
    extern const char *storageKey1Name;
    extern const char *storageKey2Name;
    extern const char *blackKeyPlaneName;
    extern const char *blackKeyPaperName;
    extern const char *blackKeyTubeName;
    extern const char *blackKeyBoatName;
    extern const char *progressiveBootsName;
    extern const char *progressiveHammerName;
    extern const char *rshNode;
    extern const char *las_09Node;
    extern const char *walrusWhiskersName;
    extern const char *walrusWhiskersDescription;
    extern const char *shellPackName;
    extern const char *shellPackDescription;
    extern const char *trunkPackName;
    extern const char *trunkPackDescription;
} // namespace mod::owr