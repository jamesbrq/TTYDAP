#include "subrel_gra.h"
#include "evt_cmd.h"
#include "patch.h"
#include "AP/rel_patch_definitions.h"
#include "ttyd/evt_cam.h"
#include "ttyd/evt_msg.h"
#include "ttyd/evt_pouch.h"

#include <cstdint>

using namespace ttyd;

extern int32_t gra_evt_machibuse[];
extern int32_t gra_evt_nameent[];
extern int32_t gra_00_init_evt[];
extern int32_t gra_01_init_evt[];
extern int32_t gra_shopmaster_pig_init[];
extern int32_t gra_shopmaster_talk[];
extern int32_t gra02_get_key[];
extern int32_t gra02_straw_blow_evt[];
extern int32_t gra_02_init_evt[];
extern int32_t gra_04_tree_open_evt[];
extern int32_t gra_04_init_evt[];
extern int32_t gra_05_futa_open_evt[];
extern int32_t gra_evt_gra_05iwa_move_init[];
extern int32_t gra_evt_gra_05iwa_move_evt[];
extern int32_t gra_evt_break_floor[];
extern int32_t gra_05_init_evt[];
extern int32_t gra_first_gra06jin[];
extern int32_t gra_06_init_evt[];
extern int32_t gra_evt_usu_kagemario_party_kill[];
extern int32_t gra_03_init_evt[];

// Assembly
extern int32_t gra_evt_kagemario_init[];

const char doopliss[] = "\x82\xC9\x82\xB9\x83\x7D\x83\x8A\x83\x49";

// clang-format off
EVT_BEGIN(evt_machibuse_evt)
	IF_SMALL(GSWF(6044), 1)
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_gra_03"), 0, PTR(&doopliss))
	ELSE()
		USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg4_gra_09"), 0, PTR(&doopliss))
	END_IF()
	GOTO(&gra_evt_machibuse[423])
EVT_PATCH_END()

EVT_BEGIN(evt_nameent_evt)
	USER_FUNC(evt_cam::evt_cam_letter_box_onoff, 0, 0)
    USER_FUNC(evt_pouch::evt_pouch_check_item, 68, LW(1))
    IF_EQUAL(LW(1), 0)
        SET(LW(0), 2)
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN(evt_nameent_hook)
    RUN_CHILD_EVT(evt_nameent_evt)
    GOTO(&gra_evt_nameent[40])
EVT_PATCH_END()
// clang-format on

namespace mod
{
    void main()
    {
        gra_evt_machibuse[78] = 99; // Unused
        gra_evt_machibuse[291] = GSWF(6044);
        gra_evt_machibuse[292] = 1;
        patch::writePatch(&gra_evt_machibuse[358], evt_machibuse_evt, sizeof(evt_machibuse_evt));
        gra_evt_machibuse[426] = 0; // Unused
        gra_evt_machibuse[476] = GSW(1715);
        gra_evt_machibuse[477] = 5;
        gra_evt_machibuse[538] = GSWF(6044);
        gra_evt_machibuse[540] = 1;

        patch::writePatch(&gra_evt_nameent[36], evt_nameent_hook, sizeof(evt_nameent_hook));

        gra_00_init_evt[43] = 0;
        gra_00_init_evt[47] = 1;
        gra_00_init_evt[65] = GSW(1715);
        gra_00_init_evt[67] = 4;
        gra_00_init_evt[68] = 4;

        gra_01_init_evt[84] = GSW(1715); // Unused
        gra_01_init_evt[77] = 1;

        gra_shopmaster_pig_init[1] = GSW(1715);
        gra_shopmaster_pig_init[3] = 2;

        gra_shopmaster_talk[1] = GSW(1715);
        gra_shopmaster_talk[3] = 2;

        gra02_get_key[1] = GSWF(6041);
        gra02_get_key[2] = 1;

        gra02_straw_blow_evt[19] = GSW(1714);
        gra02_straw_blow_evt[20] = 1;

        gra_02_init_evt[43] = GSWF(6041);
        gra_02_init_evt[44] = 1;
        gra_02_init_evt[57] = GSW(1714);
        gra_02_init_evt[58] = 1;

        gra_03_init_evt[15] = 1;

        gra_04_tree_open_evt[37] = GSW(1714);
        gra_04_tree_open_evt[38] = 2;

        gra_04_init_evt[20] = GSW(1714);
        gra_04_init_evt[21] = 2;

        gra_05_futa_open_evt[24] = GSW(1714);
        gra_05_futa_open_evt[25] = 3;

        gra_evt_gra_05iwa_move_init[5] = GSW(1714);
        gra_evt_gra_05iwa_move_init[6] = 5;

        gra_evt_gra_05iwa_move_evt[176] = GSW(1714);
        gra_evt_gra_05iwa_move_evt[177] = 5;

        gra_evt_break_floor[1] = GSW(1714);
        gra_evt_break_floor[2] = 4;

        gra_05_init_evt[15] = 1;
        gra_05_init_evt[39] = GSW(1714);
        gra_05_init_evt[40] = 3;
        gra_05_init_evt[81] = GSW(1714);
        gra_05_init_evt[82] = 4;
        gra_05_init_evt[98] = GSW(1714);
        gra_05_init_evt[99] = 5;

        gra_first_gra06jin[129] = GSW(1714);
        gra_first_gra06jin[130] = 6;

        gra_06_init_evt[36] = GSW(1714);
        gra_06_init_evt[37] = 6;
        gra_06_init_evt[132] = GSW(1714);
        gra_06_init_evt[133] = 6;

        gra_evt_usu_kagemario_party_kill[1] = GSW(1714); // Unused

        // These are swByteGet
        gra_evt_kagemario_init[2] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
        gra_evt_kagemario_init[6] = 0x2C030003; // cmpwi r3, 0x3
        gra_evt_kagemario_init[8] = 0x2C030007; // cmpwi r3, 0x7
    }

    void exit() {}
} // namespace mod
