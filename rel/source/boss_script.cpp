#include "boss_script.h"

#include <cstdint>
#include "ttyd/battle_unit.h"
#include "ttyd/battle_event_cmd.h"
#include "evt_cmd.h"

extern int32_t unit_boss_gold_chorobon_damage_event[];
extern int32_t unit_chorobon_gundan_damage_event[];
extern int32_t unit_boss_gonbaba_negotiation_event[];
extern int32_t unit_boss_magnum_battender_damage_event[];
extern int32_t unit_faker_mario_phase_event_fmario[];
extern int32_t unit_boss_kanbu3_damage_event[];
extern int32_t unit_boss_kanbu3_dead_event[];
extern int32_t unit_boss_magnum_battender_mkII_damage_event[];

void GoldChorobonPatches(int scaledHp)
{
    unit_boss_gold_chorobon_damage_event[99] = (scaledHp * 70) / 100;
}

namespace {
EVT_BEGIN_KEEP(HordeScaledDamageEvent)
    USER_FUNC(ttyd::battle_event_cmd::btlevtcmd_GetHp, -2, LW(13))
    EVT_HELPER_CMD(2, 91), 0, LW(15), // Native living-Fuzzy count call.
    USER_FUNC(ttyd::battle_event_cmd::btlevtcmd_GetMaxHp, -2, LW(14))
    IF_LARGE(LW(14), 0)
        IF_SMALL(LW(13), 0)
            SET(LW(13), 0)
        END_IF()
        // Round remaining HP up to whole groups of 50 Fuzzies.
        MUL(LW(13), 20)
        ADD(LW(13), LW(14))
        SUB(LW(13), 1)
        DIV(LW(13), LW(14))
        MUL(LW(13), 50)
        SUB(LW(15), LW(13))
        IF_LARGE(LW(15), 0)
            ADD(LW(15), 49)
            DIV(LW(15), 50)
        ELSE()
            SET(LW(15), 0)
        END_IF()
    ELSE()
        SET(LW(15), 0)
    END_IF()
    RETURN()
EVT_END()
}

void FuzzyHordePatches(int scaledHp)
{
    HordeScaledDamageEvent[5] = unit_chorobon_gundan_damage_event[15];
    // Replace the four-word GetDamage call without moving the rest of the event.
    unit_chorobon_gundan_damage_event[0] = EVT_HELPER_CMD(1, 94); // RUN_CHILD_EVT
    unit_chorobon_gundan_damage_event[1] = PTR(HordeScaledDamageEvent);
    unit_chorobon_gundan_damage_event[2] = EVT_HELPER_CMD(1, 10); // WAIT_MSEC
    unit_chorobon_gundan_damage_event[3] = 0;
    ttyd::battle_unit::unit_chorobon_gundan.max_hp = scaledHp;
}

void HooktailPatches(int scaledHp)
{
    unit_boss_gonbaba_negotiation_event[927] = (scaledHp * 50) / 100;
    unit_boss_gonbaba_negotiation_event[951] = (scaledHp * 50) / 100;
}

void MagnumBattenderPatches(int scaledHp)
{
    unit_boss_magnum_battender_damage_event[41] = (scaledHp * 23) / 100;
}

void FakerMarioPatches(int scaledHp)
{
    // Boss scaling can repatch the shared rematch event after area setup.
    // Keep Vivian's comeback IF permanently false (0 == 1), at every turn.
    unit_faker_mario_phase_event_fmario[11] = 0;
    unit_faker_mario_phase_event_fmario[12] = 1;
}

void Kanbu3Patches(int scaledHp)
{
    unit_boss_kanbu3_damage_event[100] = (scaledHp * 50) / 100;
    unit_boss_kanbu3_dead_event[166] = (scaledHp * 20) / 100;
    unit_boss_kanbu3_dead_event[184] = (scaledHp * 20) / 100;
    unit_boss_kanbu3_dead_event[202] = (scaledHp * 20) / 100;
    unit_boss_kanbu3_dead_event[220] = (scaledHp * 20) / 100;
    unit_boss_kanbu3_dead_event[238] = (scaledHp * 20) / 100;
}

void MagnumBattenderMKIIPatches(int scaledHp)
{
    unit_boss_magnum_battender_mkII_damage_event[41] = (scaledHp * 20) / 100;
}

namespace {
EVT_BEGIN_KEEP(CrumpNoTutorialPhase)
    RETURN()
EVT_END()

int32_t SkipCrumpTutorialCall(ttyd::evtmgr::EvtEntry *, bool)
{
    return 2;
}
}

KEEP_FUNC void TutorialCrumpPatches(ttyd::battle_database_common::BattleUnitKind *kind)
{
    if (!kind || kind->unit_type != 0x06 || !kind->init_evt_code) return;
    auto *init = static_cast<int32_t *>(kind->init_evt_code);
    // Preserve command sizes; skip the tutorial-only NPC's spawn, stored ID,
    // placement and home position. Crump's normal attack/wait setup remains.
    init[19] = PTR(CrumpNoTutorialPhase);
    const int npcCalls[] = {25, 30, 35, 41};
    for (int word : npcCalls)
        init[word] = PTR(SkipCrumpTutorialCall);
    if (!kind->data_table) return;
    for (auto *entry = kind->data_table; entry->id; ++entry)
    {
        if (entry->id != 0x31 || !entry->data) continue;
        auto *dead = static_cast<int32_t *>(entry->data);
        // After death effects, coins/EXP and WaitAttackEnd, kill Crump directly.
        // Omit the Goombella conversation and Mario's turns toward her.
        for (int i = 0; i < 4; ++i) dead[173 + i] = dead[270 + i];
        dead[177] = EVT_HELPER_CMD(0, 2); // RETURN
        dead[178] = EVT_HELPER_CMD(0, 1); // END
        break;
    }
}
