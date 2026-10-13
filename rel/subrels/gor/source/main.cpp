#include "subrel_gor.h"
#include "boss_script.h"
#include "ttyd/battle_database_common.h"

#include <cstdint>

extern ttyd::battle_database_common::BattleUnitKind gor_tutorial_crump_kind;

namespace mod
{
    void main()
    {
        TutorialCrumpPatches(&gor_tutorial_crump_kind);
        ApplyGor00Patches();
        ApplyGor01Patches();
        ApplyGor02Patches();
        ApplyGor03Patches();
        ApplyGor04Patches();
        ApplyGorMiscPatches();
    }

    void exit() {}
} // namespace mod
