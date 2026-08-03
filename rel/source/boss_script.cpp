#include "boss_script.h"

#include <cstdint>

extern int32_t unit_boss_gold_chorobon_damage_event[];
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
    unit_faker_mario_phase_event_fmario[12] = 999;
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
