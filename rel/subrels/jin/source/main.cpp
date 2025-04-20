#include "subrel_jin.h"
#include "evt_cmd.h"
#include "patch.h"
#include "AP/rel_patch_definitions.h"

#include <cstdint>

using namespace ttyd;

extern int32_t jin_door_teresa_n[];
extern int32_t jin_door_teresa_s[];
extern int32_t jin_first_jin00[];
extern int32_t jin_push_evt[];
extern int32_t jin_evt_key_jin[];
extern int32_t jin_evt_unlock_jin[];
extern int32_t jin_tyuou_teresa_init[];
extern int32_t jin_tyuou_teresa_talk[];
extern int32_t jin_oku_teresa_init[];
extern int32_t jin_temae_teresa_init[];
extern int32_t jin_senteresa_evt[];
extern int32_t jin_00_init_evt[];
extern int32_t jin_ki_data_evt_01[];
extern int32_t jin_ranperu_toujou[];
extern int32_t jin_irekawatta_mario[];
extern int32_t jin_ranperu_lastbattle[];
extern int32_t jin_hontonosugata_mario[];
extern int32_t jin_ranperu_init[];
extern int32_t jin_nisemario_init[];
extern int32_t jin_04_jumpstand[];
extern int32_t jin_04_init_evt[];
extern int32_t jin_06_init_evt[];
extern int32_t jin_evt_teresa_box_open[];
extern int32_t jin_07_init_evt[];
extern int32_t jin_evt_open_jin[];
extern int32_t jin_oumu_talk[];
extern int32_t jin_oumu_check[];
extern int32_t jin_bero_entry_data_08[];
extern int32_t jin_08_init_evt[];
extern int32_t jin_evt_yuka[];
extern int32_t jin_11_init_evt[];
extern int32_t jin_evt_usu_kagemario_party_kill[];
extern int32_t jin_phase_event_fmario[];

// Assembly
extern int32_t jin_evt_kagemario_init[];

// clang-format off
EVT_BEGIN(jin_08_init_evt_evt)
    SET(LW(0), PTR(&jin_bero_entry_data_08))
    SET(GSWF(2231), 1)
    RETURN()
EVT_END()

EVT_BEGIN(jin_08_init_evt_hook)
    RUN_CHILD_EVT(jin_08_init_evt_evt)
EVT_PATCH_END()
// clang-format on

namespace mod
{
    void main()
    {
        jin_door_teresa_n[1] = GSW(1716);
        jin_door_teresa_n[2] = 2;
        jin_door_teresa_n[63] = GSW(1716);
        jin_door_teresa_n[64] = 1;

        jin_door_teresa_s[1] = GSW(1716);
        jin_door_teresa_s[2] = 2;
        jin_door_teresa_s[63] = GSW(1716);
        jin_door_teresa_s[64] = 1;

        jin_first_jin00[58] = GSW(1715);
        jin_first_jin00[59] = 1;

        jin_push_evt[290] = GSWF(6045);
        jin_push_evt[291] = 1;

        jin_evt_key_jin[1] = GSWF(6046);
        jin_evt_key_jin[2] = 1;

        jin_evt_unlock_jin[1] = GSWF(6047);
        jin_evt_unlock_jin[2] = 1;

        jin_tyuou_teresa_init[3] = GSW(1716);
        jin_tyuou_teresa_init[4] = 1;
        jin_tyuou_teresa_init[13] = GSW(1716);
        jin_tyuou_teresa_init[14] = 2;

        jin_tyuou_teresa_talk[387] = GSW(1716);
        jin_tyuou_teresa_talk[388] = 2;

        jin_oku_teresa_init[3] = GSW(1716);
        jin_oku_teresa_init[4] = 2;

        jin_temae_teresa_init[3] = GSW(1716);
        jin_temae_teresa_init[4] = 2;

        jin_senteresa_evt[562] = GSWF(1715);
        jin_senteresa_evt[563] = 3;

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

        jin_ki_data_evt_01[15] = GSWF(6107);

        jin_ranperu_toujou[327] = GSW(1715);
        jin_ranperu_toujou[328] = 2;

        jin_irekawatta_mario[442] = EVT_HELPER_CMD(1, 4);
        jin_irekawatta_mario[443] = EVT_HELPER_OP(&jin_irekawatta_mario[454]);
        jin_irekawatta_mario[444] = 0;
        jin_irekawatta_mario[481] = GSW(1715);
        jin_irekawatta_mario[482] = 4;

        jin_ranperu_lastbattle[370] = GSW(1715);
        jin_ranperu_lastbattle[371] = 6;

        jin_hontonosugata_mario[913] = EVT_HELPER_CMD(1, 4);
        jin_hontonosugata_mario[914] = EVT_HELPER_OP(&jin_hontonosugata_mario[928]);
        jin_hontonosugata_mario[915] = 0;
        jin_hontonosugata_mario[929] = GSW(1715);
        jin_hontonosugata_mario[930] = 7;
        jin_hontonosugata_mario[932] = GSW(1715);
        jin_hontonosugata_mario[933] = 8;

        jin_ranperu_init[1] = GSW(1715);
        jin_ranperu_init[3] = 2;

        jin_nisemario_init[1] = GSW(1715);
        jin_nisemario_init[3] = 2;

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

        jin_evt_teresa_box_open[556] = GSW(1716);
        jin_evt_teresa_box_open[557] = 1;

        jin_07_init_evt[107] = GSW(1716);
        jin_07_init_evt[108] = 1;

        jin_evt_open_jin[64] = GSWF(6049);
        jin_evt_open_jin[65] = 1;

        jin_oumu_talk[7] = GSW(1715);
        jin_oumu_talk[9] = 7;

        jin_oumu_check[114] = GSW(1715);
        jin_oumu_check[115] = 0;
        jin_oumu_check[117] = GSW(1715);
        jin_oumu_check[118] = 5;

        patch::writePatch(&jin_08_init_evt[13], jin_08_init_evt_hook, sizeof(jin_08_init_evt_hook));
        jin_08_init_evt[15] = 0;
        jin_08_init_evt[26] = GSWF(6049);
        jin_08_init_evt[27] = 1;

        jin_evt_yuka[1] = GSWF(6048);
        jin_evt_yuka[2] = 1;

        jin_11_init_evt[40] = GSWF(6048);
        jin_11_init_evt[41] = 1;

        jin_evt_usu_kagemario_party_kill[1] = GSW(1715); // Unused

        jin_phase_event_fmario[12] = 9999; // Unused

        jin_evt_kagemario_init[2] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
        jin_evt_kagemario_init[6] = 0x2C030003; // cmpwi r3, 0x3
        jin_evt_kagemario_init[8] = 0x2C030007; // cmpwi r3, 0x7
    }

    void exit() {}
} // namespace mod
