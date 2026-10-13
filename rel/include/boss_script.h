#pragma once

void GoldChorobonPatches(int scaledHp);
void FuzzyHordePatches(int scaledHp);
void HooktailPatches(int scaledHp);
void MagnumBattenderPatches(int scaledHp);
void FakerMarioPatches(int scaledHp);
void Kanbu3Patches(int scaledHp);
void MagnumBattenderMKIIPatches(int scaledHp);
namespace ttyd::battle_database_common { struct BattleUnitKind; }
void TutorialCrumpPatches(ttyd::battle_database_common::BattleUnitKind *kind);
