#pragma once
namespace ttyd::battle_database_common { struct BattleGroupSetup; }
namespace mod::field_enemy {
void InstallHooks();
void RegisterGroup(ttyd::battle_database_common::BattleGroupSetup *group);
void ResetGroups();
void ResetMap();
void Update();
}
