#include "subrel_gor.h"
#include "evt_cmd.h"
#include "patch.h"
#include "AP/rel_patch_definitions.h"
#include "ttyd/evt_cam.h"
#include "ttyd/evt_item.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_npc.h"

#include <cstdint>

using namespace mod;
using namespace ttyd;

extern int32_t luigi_gor_first_talk[];
extern int32_t gor_10_init_evt[];
extern int32_t gor_12_init_evt[];
extern int32_t gor_11_init_evt[];
extern int32_t christine_nakama[];

// Assembly
extern int32_t irai_init_func[];
extern int32_t keijiban_data_make[];
extern int32_t monosiri_check[];
extern int32_t evt_exchange_msg_set[];
extern int32_t exchange_ret_tbl_no[];
extern int32_t exchange_ryokin_medal[];

const char goombella[] = "\x83\x4C\x83\x6D\x82\xB6\x82\xA2";

// clang-format off
EVT_BEGIN(party_evt)
	USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
	USER_FUNC(evt_item::evt_item_entry, PTR("item01"), LW(3), LW(0), LW(1), LW(2), 16, GSWF(6076), 0)
	USER_FUNC(evt_item::evt_item_get_item, PTR("item01"))
	WAIT_MSEC(800)
	SET(GSW(1700), 7)
	USER_FUNC(evt_npc::evt_npc_set_position, PTR(&goombella), 0, -1000, 0)
	USER_FUNC(evt_cam::evt_cam3d_evt_off, 500, 11)
	USER_FUNC(evt_mario::evt_mario_key_onoff, 1)
	RETURN()
EVT_END()
// clang-format on

void ApplyGorMiscPatches()
{
    christine_nakama[707] = EVT_HELPER_CMD(2, 50);
    christine_nakama[708] = EVT_HELPER_OP(LW(3));
    patch::writePatch(&christine_nakama[710], party_evt, sizeof(party_evt));

    luigi_gor_first_talk[18] = GSW(1708);
    luigi_gor_first_talk[19] = 17;
    luigi_gor_first_talk[44] = GSW(1700);
    luigi_gor_first_talk[46] = 1;
    luigi_gor_first_talk[47] = 1;
    luigi_gor_first_talk[52] = 2;
    luigi_gor_first_talk[53] = 2;
    luigi_gor_first_talk[58] = 3;
    luigi_gor_first_talk[59] = 3;
    luigi_gor_first_talk[64] = 4;
    luigi_gor_first_talk[65] = 4;
    luigi_gor_first_talk[70] = 5;
    luigi_gor_first_talk[71] = 5;
    luigi_gor_first_talk[76] = 6;
    luigi_gor_first_talk[77] = 6;
    luigi_gor_first_talk[82] = 7;
    luigi_gor_first_talk[83] = 7;
    luigi_gor_first_talk[88] = 8;

    gor_10_init_evt[19] = GSW(1700);

    gor_11_init_evt[19] = GSW(1708);
    gor_11_init_evt[20] = 18;
    gor_12_init_evt[24] = GSW(1708);
    gor_12_init_evt[25] = 18;

    irai_init_func[33] = 0x386006A4;  // li r3, 0x6A4 (GSW(1700))
    irai_init_func[43] = 0x2C030000;  // cmpwi r3, 0x0
    irai_init_func[56] = 0x2C030000;  // cmpwi r3, 0x0
    irai_init_func[64] = 0x2C030000;  // cmpwi r3, 0x0
    irai_init_func[72] = 0x2C030000;  // cmpwi r3, 0x0
    irai_init_func[80] = 0x2C030000;  // cmpwi r3, 0x0
    irai_init_func[86] = 0x2C030000;  // cmpwi r3, 0x0
    irai_init_func[90] = 0x2C030000;  // cmpwi r3, 0x0
    irai_init_func[98] = 0x2C030000;  // cmpwi r3, 0x0
    irai_init_func[106] = 0x2C030000; // cmpwi r3, 0x0

    patch::writeBranchBL(&keijiban_data_make[11], reinterpret_cast<void *>(bJohoyaSeqAddition));
}
