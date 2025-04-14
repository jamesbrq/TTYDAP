#include "subrel_gor.h"
#include "evt_cmd.h"
#include "AP/rel_patch_definitions.h"
#include "OWR.h"
#include <cstdint>

using namespace ::mod::owr;

extern int32_t gor_hom_10_evt_resha_start_04[];
extern int32_t gor_evt_train[];
extern int32_t gor_evt_train2[];
extern int32_t gor_peach_mail_04[];
extern int32_t gor_jugemu_a_init[];
extern int32_t gor_jugemu_a_talk[];
extern int32_t gor_kinopiko_b_talk_04[];
extern int32_t gor_luigi_init_04[];
extern int32_t gor_luigi_npcEnt_04[];
extern int32_t gor_04_init_evt[];

void ApplyGor04Patches()
{
    gor_hom_10_evt_resha_start_04[49] = GSW(1706);
    gor_hom_10_evt_resha_start_04[50] = 43;

    gor_evt_train[240] = GSW(1706);
    gor_evt_train[241] = 49;
    gor_evt_train[270] = GSW(1706);
    gor_evt_train[271] = 38;

    gor_evt_train2[223] = GSW(1706);
    gor_evt_train2[224] = 49;
    gor_evt_train2[226] = GSW(1706);
    gor_evt_train2[227] = 49;
    gor_evt_train2[280] = GSW(1706);
    gor_evt_train2[281] = 50;

    gor_peach_mail_04[225] = GSW(1706);
    gor_peach_mail_04[226] = 50;

    gor_jugemu_a_init[1] = GSW(1715);
    gor_jugemu_a_init[2] = 10;

    gor_jugemu_a_talk[1] = GSW(1710); // HOOK
    gor_jugemu_a_talk[3] = 2;
    gor_jugemu_a_talk[11] = 3;
    gor_jugemu_a_talk[19] = 4;
    gor_jugemu_a_talk[27] = 5;
    gor_jugemu_a_talk[35] = 6;
    gor_jugemu_a_talk[43] = 7;
    gor_jugemu_a_talk[51] = 8;

    gor_kinopiko_b_talk_04[1] = GSW(1706); // HOOK
    gor_kinopiko_b_talk_04[3] = 2;
    gor_kinopiko_b_talk_04[11] = 3;
    gor_kinopiko_b_talk_04[19] = 4;
    gor_kinopiko_b_talk_04[27] = 5;
    gor_kinopiko_b_talk_04[35] = 6;
    gor_kinopiko_b_talk_04[43] = 7;
    gor_kinopiko_b_talk_04[51] = 8;

    gor_luigi_init_04[1] = GSW(1710); // HOOK
    gor_luigi_init_04[3] = 6;
    gor_luigi_init_04[4] = 7;

    gor_luigi_npcEnt_04[1] = GSW(1710); // HOOK
    gor_luigi_npcEnt_04[3] = 6;
    gor_luigi_npcEnt_04[4] = 7;

    gor_04_init_evt[1] = GSW(1715);
    gor_04_init_evt[2] = 10;
    gor_04_init_evt[49] = GSW(1715);
    gor_04_init_evt[50] = 10;
    gor_04_init_evt[185] = GSW(1706);
    gor_04_init_evt[186] = 49;
    gor_04_init_evt[191] = GSW(1715);
    gor_04_init_evt[192] = 10;
    gor_04_init_evt[222] = GSW(1706);
    gor_04_init_evt[223] = 48;
}
