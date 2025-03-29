#include "evt_cmd.h"
#include <AP/rel_patch_definitions.h>

extern int32_t door_teresa_n[];
extern int32_t door_teresa_s[];
extern int32_t first_jin00[];
extern int32_t push_evt[];
extern int32_t evt_key_jin[];
extern int32_t evt_unlock_jin[];
extern int32_t tyuou_teresa_init[];
extern int32_t tyuou_teresa_talk[];
extern int32_t oku_teresa_init[];
extern int32_t temae_teresa_init[];
extern int32_t senteresa_evt[];
extern int32_t jin_00_init_evt[];
extern int32_t ki_data_evt_01[];
extern int32_t ranperu_toujou[];
extern int32_t irekawatta_mario[];
extern int32_t ranperu_lastbattle[];
extern int32_t hontonosugata_mario[];
extern int32_t ranperu_init[];
extern int32_t nisemario_init[];
extern int32_t jin_04_jumpstand[];
extern int32_t jin_04_init_evt[];
extern int32_t jin_06_init_evt[];
extern int32_t evt_teresa_box_open[];
extern int32_t jin_07_init_evt[];
extern int32_t evt_open_jin[];
extern int32_t oumu_talk[];
extern int32_t oumu_check[];
extern int32_t jin_08_init_evt[];
extern int32_t evt_yuka[];
extern int32_t jin_11_init_evt[];
extern int32_t evt_usu_kagemario_party_kill_jin[];

// Assembly
extern int32_t jin_evt_kagemario_init[];

void ApplyJinPatches()
{
    door_teresa_n[1] = GSW(1716);
    door_teresa_n[2] = 2;
    door_teresa_n[63] = GSW(1716);
    door_teresa_n[64] = 1;

    door_teresa_s[1] = GSW(1716);
    door_teresa_s[2] = 2;
    door_teresa_s[63] = GSW(1716);
    door_teresa_s[64] = 1;

    first_jin00[58] = GSW(1715);
    first_jin00[59] = 1;

    push_evt[290] = GSWF(6045);
    push_evt[291] = 1;

    evt_key_jin[1] = GSWF(6046);
    evt_key_jin[2] = 1;

    evt_unlock_jin[1] = GSWF(6047);
    evt_unlock_jin[2] = 1;

    tyuou_teresa_init[3] = GSW(1716);
    tyuou_teresa_init[4] = 1;
    tyuou_teresa_init[13] = GSW(1716);
    tyuou_teresa_init[14] = 2;

    tyuou_teresa_talk[387] = GSW(1716);
    tyuou_teresa_talk[388] = 2;

    oku_teresa_init[3] = GSW(1716);
    oku_teresa_init[4] = 2;

    temae_teresa_init[3] = GSW(1716);
    temae_teresa_init[4] = 2;

    senteresa_evt[562] = GSWF(1715);
    senteresa_evt[563] = 3;

    jin_00_init_evt[22] = GSW(1715);
    jin_00_init_evt[23] = 1;
    jin_00_init_evt[46] = GSWF(6046);
    jin_00_init_evt[47] = 1;
    jin_00_init_evt[60] = GSWF(6047);
    jin_00_init_evt[61] = 1;
    jin_00_init_evt[86] = GSW(1716);
    jin_00_init_evt[87] = 1;
    jin_00_init_evt[94] = GSW(1716);
    jin_00_init_evt[95] = 2;
    jin_00_init_evt[137] = GSWF(6045);
    jin_00_init_evt[138] = 1;
    jin_00_init_evt[181] = GSW(1715);
    jin_00_init_evt[182] = 3;

    ki_data_evt_01[15] = GSWF(6107);

    ranperu_toujou[327] = GSW(1715);
    ranperu_toujou[328] = 2;

    irekawatta_mario[442] = EVT_HELPER_CMD(1, 4);
    irekawatta_mario[443] = EVT_HELPER_OP(&irekawatta_mario[454]);
    irekawatta_mario[481] = GSW(1715);
    irekawatta_mario[482] = 3;

    ranperu_lastbattle[370] = GSW(1715);
    ranperu_lastbattle[371] = 6;

    hontonosugata_mario[929] = GSW(1715);
    hontonosugata_mario[930] = 7;
    hontonosugata_mario[932] = GSW(1715);
    hontonosugata_mario[933] = 8;

    ranperu_init[1] = GSW(1715);
    ranperu_init[3] = 2;

    nisemario_init[1] = GSW(1715);
    nisemario_init[3] = 2;

    jin_04_jumpstand[4] = GSW(1715);
    jin_04_jumpstand[6] = 1;
    jin_04_jumpstand[8] = 3;

    jin_04_init_evt[1] = GSW(1715);
    jin_04_init_evt[3] = 2;
    jin_04_init_evt[56] = GSW(1715);
    jin_04_init_evt[58] = 2;
    jin_04_init_evt[68] = 5;
    jin_04_init_evt[80] = 2;
    jin_04_init_evt[89] = 6;

    jin_06_init_evt[157] = GSWF(6045);
    jin_06_init_evt[158] = 1;
    jin_06_init_evt[160] = GSWF(6045);
    jin_06_init_evt[161] = 1;

    evt_teresa_box_open[556] = GSW(1716);
    evt_teresa_box_open[557] = 1;

    jin_07_init_evt[107] = GSW(1716);
    jin_07_init_evt[108] = 1;

    evt_open_jin[64] = GSWF(6049);
    evt_open_jin[65] = 1;

    oumu_talk[7] = GSW(1715);
    oumu_talk[9] = 7;

    oumu_check[114] = GSW(1715);
    oumu_check[115] = 0;
    oumu_check[117] = GSW(1715);
    oumu_check[118] = 5;

    jin_08_init_evt[26] = GSWF(6049);
    jin_08_init_evt[27] = 1;

    evt_yuka[1] = GSWF(6048);
    evt_yuka[2] = 1;

    jin_11_init_evt[40] = GSWF(6048);
    jin_11_init_evt[41] = 1;

    evt_usu_kagemario_party_kill_jin[1] = GSW(1715); // Unused

    jin_evt_kagemario_init[2] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
    jin_evt_kagemario_init[6] = 0x2C030003; // cmpwi r3, 0x3
    jin_evt_kagemario_init[8] = 0x2C030007; // cmpwi r3, 0x7
}
