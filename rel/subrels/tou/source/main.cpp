#include "AP/rel_patch_definitions.h"
#include "evt_cmd.h"
#include "patch.h"
#include "subrel_tou.h"
#include "ttyd/evt_bero.h"
#include "ttyd/evt_case.h"
#include "ttyd/evt_eff.h"
#include "ttyd/evt_hit.h"
#include "ttyd/evt_item.h"
#include "ttyd/evt_map.h"
#include "ttyd/evt_mario.h"
#include "ttyd/evt_msg.h"
#include "ttyd/evt_npc.h"
#include "ttyd/evt_pouch.h"

#include <cstdint>

using namespace ttyd;

extern int32_t tou_evt_open_tou[];
extern int32_t tou_talk_gardman[];
extern int32_t tou_evt_tou_match_make_default_sub[];
extern int32_t tou_evt_tou_match_make_default[];
extern int32_t tou_evt_mail_1[];
extern int32_t tou_evt_mail_3[];
extern int32_t tou_evt_mail_5[];
extern int32_t tou_evt_tou_match_after_default[];
extern int32_t tou_00_init_evt[];
extern int32_t tou_talk_sakaba[];
extern int32_t tou_talk_hot[];
extern int32_t tou_init_papa_01[];
extern int32_t tou_talk_papa_01[];
extern int32_t tou_init_mama_01[];
extern int32_t tou_talk_mama_01[];
extern int32_t tou_init_konari_01[];
extern int32_t tou_talk_konari_01[];
extern int32_t tou_talk_musume[];
extern int32_t tou_init_musume_1[];
extern int32_t tou_init_musume_2[];
extern int32_t tou_init_musume_3[];
extern int32_t tou_talk_kino_1[];
extern int32_t tou_talk_kino_2[];
extern int32_t tou_talk_roten_1[];
extern int32_t tou_talk_roten_2[];
extern int32_t tou_talk_jugem_1[];
extern int32_t tou_talk_jugem_2[];
extern int32_t tou_talk_bijyo[];
extern int32_t tou_init_kiza[];
extern int32_t tou_talk_kiza[];
extern int32_t tou_init_sarary_01[];
extern int32_t tou_evt_uron[];
extern int32_t tou_evt_tamago_sleep[];
extern int32_t tou_evt_tamago_move[];
extern int32_t tou_evt_tamago[];
extern int32_t tou_talk_tamago[];
extern int32_t tou_evt_gondora[];
extern int32_t tou_evt_gondora2[];
extern int32_t tou_evt_peach_after[];
extern int32_t tou_evt_key_01[];
extern int32_t tou_evt_hotdog[];
extern int32_t tou_01_init_evt[];
extern int32_t tou_evt_move_g[];
extern int32_t tou_init_gard1[];
extern int32_t tou_talk_gard1[];
extern int32_t tou_init_gard2[];
extern int32_t tou_talk_gard2[];
extern int32_t tou_init_madam_02[];
extern int32_t tou_talk_madam_02[];
extern int32_t tou_talk_repo[];
extern int32_t tou_talk_cam1[];
extern int32_t tou_init_cam2[];
extern int32_t tou_talk_cam2[];
extern int32_t tou_talk_kino4[];
extern int32_t tou_talk_kino5[];
extern int32_t tou_talk_kinopiko[];
extern int32_t tou_talk_kinopio_f[];
extern int32_t tou_init_chu[];
extern int32_t tou_talk_chu[];
extern int32_t tou_init_champ[];
extern int32_t tou_talk_champ[];
extern int32_t tou_init_shikowa[];
extern int32_t tou_talk_shikowa[];
extern int32_t tou_init_uranoko[];
extern int32_t tou_talk_uranoko_02[];
extern int32_t tou_init_ironA[];
extern int32_t tou_talk_ironA[];
extern int32_t tou_init_ironB[];
extern int32_t tou_talk_ironB[];
extern int32_t tou_init_borodo[];
extern int32_t tou_talk_borodo[];
extern int32_t tou_evt_tougi2[];
extern int32_t tou_evt_momikucha[];
extern int32_t tou_evt_key_02[];
extern int32_t tou_evt_poster_8[];
extern int32_t tou_02_init_evt[];
extern int32_t tou_init_g_1[];
extern int32_t tou_talk_g_1[];
extern int32_t tou_move_g_1[];
extern int32_t tou_init_g_2[];
extern int32_t tou_talk_g_2[];
extern int32_t tou_move_g_2[];
extern int32_t tou_init_g_4[];
extern int32_t tou_talk_g_4[];
extern int32_t tou_move_g_4[];
extern int32_t tou_talk_g_7[];
extern int32_t tou_talk_g_8[];
extern int32_t tou_talk_g_9[];
extern int32_t tou_init_g_10[];
extern int32_t tou_talk_g_10[];
extern int32_t tou_init_g_11[];
extern int32_t tou_talk_g_11[];
extern int32_t tou_init_brots_04[];
extern int32_t tou_evt_gardman[];
extern int32_t tou_evt_lock_04[];
extern int32_t tou_04_init_evt[];
extern int32_t tou_evt_takurami[];
extern int32_t tou_evt_hiki_open[];
extern int32_t tou_init_gans[];
extern int32_t tou_talk_gans[];
extern int32_t tou_init_kinoshikowa[];
extern int32_t tou_talk_kinoshikowa[];
extern int32_t tou_evt_sensyu[];
extern int32_t tou_evt_sensyu2[];
extern int32_t tou_evt_nozoki[];
extern int32_t tou_05_init_evt[];
extern int32_t tou_evt_kontena[];
extern int32_t tou_evt_kaidan[];
extern int32_t tou_evt_block_broken_06[];
extern int32_t tou_evt_lock_06[];
extern int32_t tou_evt_chuchu[];
extern int32_t tou_06_init_evt[];
extern int32_t tou_evt_champion[];
extern int32_t tou_evt_dakuto_07[];
extern int32_t tou_07_init_evt[];
extern int32_t tou_evt_toile[];
extern int32_t tou_talk_togenoko[];
extern int32_t tou_talk_togemet[];
extern int32_t tou_talk_hammerbros[];
extern int32_t tou_talk_uranoko_08[];
extern int32_t tou_evt_1st_leagu[];
extern int32_t tou_evt_toile_door[];
extern int32_t tou_08_init_evt[];
extern int32_t tou_09_init_evt[];
extern int32_t tou_init_kino[];
extern int32_t tou_talk_kino[];
extern int32_t tou_init_michel_10[];
extern int32_t tou_talk_michel[];
extern int32_t tou_talk_octo[];
extern int32_t tou_init_brots_10[];
extern int32_t tou_talk_brots[];
extern int32_t tou_talk_gangan[];
extern int32_t tou_init_poku[];
extern int32_t tou_talk_poku[];
extern int32_t tou_init_iron[];
extern int32_t tou_talk_iron[];
extern int32_t tou_init_iron2[];
extern int32_t tou_talk_iron2[];
extern int32_t tou_evt_saisyo_after[];
extern int32_t tou_evt_tou_chibi_yoshi[];
extern int32_t tou_evt_okorareru[];
extern int32_t tou_evt_block_10[];
extern int32_t tou_10_evt_1st_leagu[];
extern int32_t tou_10_init_evt[];
extern int32_t tou_11_init_evt[];
extern int32_t tou_evt_block_broken_12[];
extern int32_t tou_init_michel_12[];
extern int32_t tou_init_michel_dummy[];
extern int32_t tou_init_brots_12[];
extern int32_t tou_init_brots_dummy[];
extern int32_t tou_12_init_evt[];
extern int32_t tou_evt_dakuto_13[];
extern int32_t tou_evt_nusumi[];
extern int32_t tou_evt_first[];
extern int32_t tou_13_init_evt[];
extern int32_t tou_all_party_lecture[];

// Assembly
extern int32_t tou_disp_proc[];
extern int32_t tou_evt_tou_get_fighter_battle_msg[];
extern int32_t tou_evt_tou_get_fighter_info[];
extern int32_t tou_evt_tou_get_fighter_name2[];
extern int32_t tou_evt_tou_get_rule_info[];
extern int32_t tou_evt_tou_get_rule_msg2[];
extern int32_t tou_gans_tex[];
extern int32_t tou_make_monitor[];
extern int32_t tou_rankingInit[];
extern int32_t tou_chk[];

const char jolene[] = "\x83\x4C\x83\x6D\x83\x56\x83\x52\x83\x8F";

// clang-format off
EVT_BEGIN(talk_sakaba_evt)
    IF_EQUAL(GSW(1707), 3)
        IF_EQUAL(GSWF(3877), 1)
            SET(GSWF(3879), 1)
            USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg7_tou_01"), 0, 0)
        ELSE()
            USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg3_tou_06"), 0, 0)
        END_IF()
        RETURN()
    END_IF()
    USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg3_tou_297_09"), 0, 0)
    RETURN()
EVT_END()

EVT_BEGIN(talk_sakaba_hook)
    RUN_CHILD_EVT(talk_sakaba_evt)
    RETURN()
EVT_END()

EVT_BEGIN(jolene_egg_evt)
    WAIT_MSEC(300)
    IF_EQUAL(GSWF(6075), 1)
        GOTO(99)
    END_IF()
    USER_FUNC(evt_npc::evt_npc_set_ry, PTR(&jolene), 90)
    WAIT_MSEC(300)
    USER_FUNC(evt_eff::evt_eff_fukidashi, 1, 0, PTR(&jolene), 0, 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_msg::evt_msg_print, 0, PTR("jolene_fukidashi"), 0, PTR(&jolene))
    WAIT_MSEC(300)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR(&jolene), PTR("M_1"))
    WAIT_MSEC(600)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR(&jolene), PTR("S_1"))
    INLINE_EVT()
    USER_FUNC(evt_npc::evt_npc_set_ry, PTR(&jolene), 270)
    WAIT_MSEC(600)
    USER_FUNC(evt_npc::evt_npc_move_position, PTR(&jolene), -90, -60, 0, FLOAT(80.0), 1)
    USER_FUNC(evt_npc::evt_npc_move_position, PTR(&jolene), -130, -60, 0, FLOAT(80.0), 1)
    USER_FUNC(evt_npc::evt_npc_set_ry, PTR(&jolene), 90)
    WAIT_MSEC(300)
    END_INLINE()
    RUN_CHILD_EVT(&tou_evt_tou_chibi_yoshi)
    WAIT_MSEC(300)
    USER_FUNC(evt_npc::evt_npc_move_position, PTR(&jolene), 10, -60, 0, FLOAT(80.0), 1)
    USER_FUNC(evt_npc::evt_npc_move_position, PTR(&jolene), 10, -95, 0, FLOAT(80.0), 1)
    USER_FUNC(evt_npc::evt_npc_set_ry, PTR(&jolene), 270)
    WAIT_MSEC(300)
    USER_FUNC(evt_msg::evt_msg_print, 0, PTR("jolene_fukidashi_end"), 0, PTR(&jolene))
    WAIT_MSEC(300)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR(&jolene), PTR("M_1"))
    WAIT_MSEC(600)
    USER_FUNC(evt_npc::evt_npc_set_anim, PTR(&jolene), PTR("S_1"))
    USER_FUNC(evt_npc::evt_npc_status_onoff, 1, PTR(&jolene), 2)
    LBL(99)
    USER_FUNC(evt_npc::evt_npc_move_position, PTR(&jolene), 0, -130, 0, FLOAT(80.0), 1)
    RETURN()
EVT_END()

EVT_BEGIN(jolene_egg_hook)
    RUN_CHILD_EVT(jolene_egg_evt)
    GOTO(&tou_10_evt_1st_leagu[173])
EVT_PATCH_END()

EVT_BEGIN(yoshi_gswf_evt)
    SET(GSWF(6075), 1)
    GOTO(&tou_evt_tou_chibi_yoshi[102])
EVT_PATCH_END()

EVT_BEGIN(tou_04_init_evt_evt)
    SET(LW(0), 5)
    RUN_CHILD_EVT(evt_bero::bero_case_switch_on)
    SET(LW(0), 7)
    RUN_CHILD_EVT(evt_bero::bero_case_switch_on)
    USER_FUNC(tou_evt_tou_get_ranking, LW(0))
    IF_SMALL(LW(0), 11)
        SET(LW(0), 6)
        RUN_CHILD_EVT(evt_bero::bero_case_switch_on)
    END_IF()
    USER_FUNC(tou_evt_tou_get_ranking, LW(0))
    IF_SMALL(LW(0), 1)
        SET(LW(0), 3)
        RUN_CHILD_EVT(evt_bero::bero_case_switch_on)
    END_IF()
    RETURN()
EVT_PATCH_END()

EVT_BEGIN(tou_04_init_evt_hook)
    RUN_CHILD_EVT(tou_04_init_evt_evt)
    GOTO(&tou_04_init_evt[425])
EVT_PATCH_END()

EVT_BEGIN(tou_party_evt)
    USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    USER_FUNC(evt_item::evt_item_entry, PTR("item01"), LW(3), LW(0), LW(1), LW(2), 16, GSWF(6079), 0)
    USER_FUNC(evt_item::evt_item_get_item, PTR("item01"))
    WAIT_MSEC(800)
    RETURN()
EVT_END()

EVT_BEGIN(tou_party_evt_hook)
    RUN_CHILD_EVT(tou_party_evt)
    RETURN()
EVT_END()

EVT_BEGIN(hotdog_item)
	IF_SMALL(GSWF(6105), 1)
		USER_FUNC(evt_mario::evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
		USER_FUNC(evt_item::evt_item_entry, PTR("item01"), LW(3), LW(0), LW(1), LW(2), 16, GSWF(6105), 0)
		USER_FUNC(evt_item::evt_item_get_item, PTR("item01"))
		WAIT_MSEC(800)
		USER_FUNC(evt_pouch::evt_pouch_add_coin, -10)
		USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg3_tou_14"))
		RETURN()
	ELSE()
		USER_FUNC(evt_pouch::evt_pouch_add_item, 159, LW(0))
		IF_EQUAL(LW(0), 0)
			USER_FUNC(evt_pouch::evt_pouch_add_coin, -10)
			USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg3_tou_14"))
			RETURN()
		ELSE()
			USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg3_shop_18"))
			RETURN()
		END_IF()
	END_IF()
	RETURN()
EVT_END()

EVT_BEGIN(hotdog_item_hook)
	RUN_CHILD_EVT(hotdog_item)
	GOTO(&tou_evt_hotdog[78])
EVT_PATCH_END()

EVT_BEGIN(tou_evt_tougi2_evt)
	USER_FUNC(evt_msg::evt_msg_select, 0, PTR("stg3_tou_33_yn"))
    IF_EQUAL(LW(0), 0)
        USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(1)
                USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg3_tou_34_kur"))
            CASE_EQUAL(2)
                USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg3_tou_34_nok"))
            CASE_EQUAL(5)
                USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg3_tou_34_win"))
            CASE_ETC()
                USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg3_tou_34_kur"))
        END_SWITCH()
    ELSE()
        USER_FUNC(evt_mario::evt_mario_get_party, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(1)
                USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg3_tou_34_kur"))
            CASE_EQUAL(2)
                USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg3_tou_34_nok"))
            CASE_EQUAL(5)
                USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg3_tou_34_win"))
            CASE_ETC()
                USER_FUNC(evt_msg::evt_msg_print_add, 0, PTR("stg3_tou_34_kur"))
        END_SWITCH()
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN(tou_evt_tougi2_hook)
	RUN_CHILD_EVT(tou_evt_tougi2_evt)
	GOTO(&tou_evt_tougi2[239])
EVT_PATCH_END()

EVT_BEGIN(babyyoshi_select_evt)
    IF_EQUAL(GSWF(6079), 1)
        SET(LW(1), 1)
        RETURN()
    END_IF()
    SWITCH(LW(0))
        CASE_EQUAL(0)
            SET(LW(0), PTR("c_babyyoshi"))
        CASE_EQUAL(1)
            SET(LW(0), PTR("c_babyyoshi2"))
        CASE_EQUAL(2)
            SET(LW(0), PTR("c_babyyoshi3"))
        CASE_EQUAL(3)
            SET(LW(0), PTR("c_babyyoshi4"))
        CASE_EQUAL(4)
            SET(LW(0), PTR("c_babyyoshi5"))
        CASE_EQUAL(5)
            SET(LW(0), PTR("c_babyyoshi6"))
        CASE_EQUAL(6)
            SET(LW(0), PTR("c_babyyoshi7"))
    END_SWITCH()
    SET(LW(1), 0)
    RETURN()
EVT_END()

EVT_BEGIN(babyyoshi_select_hook)
	RUN_CHILD_EVT(babyyoshi_select_evt)
    IF_EQUAL(LW(1), 1)
        RETURN()
    END_IF()
	GOTO(&tou_evt_tou_chibi_yoshi[43])
EVT_PATCH_END()

EVT_BEGIN(tou_05_init_evt_evt1)
    USER_FUNC(evt_bero::evt_bero_get_entername, LW(0))
    IF_EQUAL(GSW(1703), 4)
        IF_STR_EQUAL(LW(0), PTR("fall"))
            USER_FUNC(evt_case::evt_run_case_evt, 9, 1, PTR("a_p_tukue"), 0, PTR(&tou_talk_gans), 0)
            RETURN()
        END_IF()
    END_IF()
    USER_FUNC(evt_map::evt_mapobj_flag_onoff, 1, 1, PTR("kami_1"), 1)
    RETURN()
EVT_END()

EVT_BEGIN(tou_05_init_evt_hook1)
	RUN_CHILD_EVT(tou_05_init_evt_evt1)
	GOTO(&tou_05_init_evt[193])
EVT_PATCH_END()

EVT_BEGIN(tou_05_init_evt_evt2)
    USER_FUNC(evt_bero::evt_bero_get_entername, LW(0))
    IF_SMALL(GSW(1703), 4)
        IF_STR_NOT_EQUAL(LW(0), PTR("fall"))
            SET(LW(0), 1)
            IF_EQUAL(GSWF(2375), 0)
                RUN_EVT(&tou_evt_sensyu)
            ELSE()
                SET(GSWF(2375), 0)
                RUN_EVT(&tou_evt_sensyu2)
            END_IF()
        END_IF()
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN(tou_05_init_evt_hook2)
	RUN_CHILD_EVT(tou_05_init_evt_evt2)
	GOTO(&tou_05_init_evt[274])
EVT_PATCH_END()

EVT_BEGIN(tou_05_init_evt_evt3)
    SET(LW(0), PTR("hikidasi_N"))
    SET(LW(1), PTR("a_hikidasi_N"))
    USER_FUNC(evt_hit::evt_hit_bind_mapobj, LW(1), LW(0))
    USER_FUNC(evt_case::evt_run_case_evt, 9, 1, LW(1), 0, PTR(&tou_evt_hiki_open), 0)
    IF_LARGE_EQUAL(GSW(1703), 18)
        SET(LW(0), PTR("hikidasi_H"))
        SET(LW(1), PTR("a_hikidasi_H"))
        USER_FUNC(evt_hit::evt_hit_bind_mapobj, LW(1), LW(0))
        USER_FUNC(evt_case::evt_run_case_evt, 9, 1, LW(1), 0, PTR(&tou_evt_hiki_open), 0)
    END_IF()
    RETURN()
EVT_END()

EVT_BEGIN(tou_05_init_evt_hook3)
	RUN_CHILD_EVT(tou_05_init_evt_evt3)
	GOTO(&tou_05_init_evt[171])
EVT_PATCH_END()
// clang-format on

namespace mod
{
    void main()
    {
        tou_evt_open_tou[1] = GSW(1703);
        tou_evt_open_tou[2] = 6;

        tou_talk_gardman[15] = GSW(1703);
        tou_talk_gardman[16] = 11;
        tou_talk_gardman[18] = GSW(1703);
        tou_talk_gardman[19] = 12;
        tou_talk_gardman[173] = GSW(1703);
        tou_talk_gardman[174] = 28;

        tou_evt_tou_match_make_default_sub[147] = GSW(1703);
        tou_evt_tou_match_make_default_sub[148] = 28;
        tou_evt_tou_match_make_default_sub[225] = GSW(1703);
        tou_evt_tou_match_make_default_sub[226] = 28;
        tou_evt_tou_match_make_default_sub[312] = GSW(1703);
        tou_evt_tou_match_make_default_sub[313] = 28;
        tou_evt_tou_match_make_default_sub[371] = 99; // UNUSED
        tou_evt_tou_match_make_default_sub[388] = GSW(1703);
        tou_evt_tou_match_make_default_sub[389] = 28;

        tou_evt_tou_match_make_default[117] = GSW(1703);
        tou_evt_tou_match_make_default[119] = 0;
        tou_evt_tou_match_make_default[120] = 0;
        tou_evt_tou_match_make_default[142] = 0;
        tou_evt_tou_match_make_default[158] = 0;
        tou_evt_tou_match_make_default[159] = 0;
        tou_evt_tou_match_make_default[164] = 0;
        tou_evt_tou_match_make_default[165] = 0;
        tou_evt_tou_match_make_default[170] = 14;
        tou_evt_tou_match_make_default[171] = 19;
        tou_evt_tou_match_make_default[176] = 0;

        tou_evt_tou_match_after_default[3] = EVT_HELPER_CMD(2, 28);
        tou_evt_tou_match_after_default[31] = GSW(1703);
        tou_evt_tou_match_after_default[32] = 11;
        tou_evt_tou_match_after_default[34] = GSW(1703);
        tou_evt_tou_match_after_default[35] = 12;
        tou_evt_tou_match_after_default[1192] = EVT_HELPER_CMD(1, 4);
        tou_evt_tou_match_after_default[1193] = EVT_HELPER_OP(&tou_evt_tou_match_after_default[1204]);
        tou_evt_tou_match_after_default[1316] = GSW(1703);
        tou_evt_tou_match_after_default[1317] = 28;
        tou_evt_tou_match_after_default[1380] = GSW(1703);
        tou_evt_tou_match_after_default[1381] = 11;
        tou_evt_tou_match_after_default[1394] = GSW(1703);
        tou_evt_tou_match_after_default[1395] = 11;
        tou_evt_tou_match_after_default[1524] = 99; // UNUSED
        tou_evt_tou_match_after_default[1535] = 99; // UNUSED
        tou_evt_tou_match_after_default[1574] = 99; // UNUSED
        tou_evt_tou_match_after_default[1585] = 99; // UNUSED
        tou_evt_tou_match_after_default[1600] = 99; // UNUSED

        tou_00_init_evt[7] = GSW(1715);
        tou_00_init_evt[8] = 10;
        tou_00_init_evt[68] = GSW(1703);
        tou_00_init_evt[69] = 1;

        tou_talk_sakaba[23] = GSWF(6026);
        tou_talk_sakaba[25] = 0; // UNUSED
        tou_talk_sakaba[33] = 0;
        tou_talk_sakaba[268] = GSWF(6026);
        tou_talk_sakaba[269] = 1;
        tou_talk_sakaba[275] = EVT_HELPER_CMD(0, 49);
        patch::writePatch(&tou_talk_sakaba[276], talk_sakaba_hook, sizeof(talk_sakaba_hook));

        tou_init_papa_01[1] = GSW(1706);
        tou_init_papa_01[3] = 1;

        tou_talk_papa_01[1] = GSW(1703);
        tou_talk_papa_01[3] = 5;
        tou_talk_papa_01[11] = 20;

        tou_init_mama_01[1] = GSW(1706);
        tou_init_mama_01[3] = 1;

        tou_talk_mama_01[1] = GSW(1703);
        tou_talk_mama_01[3] = 5;
        tou_talk_mama_01[11] = 20;

        tou_init_konari_01[1] = GSW(1706);
        tou_init_konari_01[3] = 1;

        tou_talk_konari_01[1] = GSW(1703);
        tou_talk_konari_01[3] = 5;
        tou_talk_konari_01[11] = 20;

        tou_talk_musume[1] = GSW(1703);
        tou_talk_musume[3] = 5;
        tou_talk_musume[23] = 20;

        tou_init_musume_1[1] = GSW(1703);
        tou_init_musume_1[3] = 1;
        tou_init_musume_1[4] = 20;

        tou_init_musume_2[1] = GSW(1703);
        tou_init_musume_2[3] = 1;
        tou_init_musume_2[4] = 20;

        tou_init_musume_3[1] = GSW(1703);
        tou_init_musume_3[3] = 1;
        tou_init_musume_3[4] = 20;

        tou_talk_kino_1[1] = GSW(1703);
        tou_talk_kino_1[3] = 5;
        tou_talk_kino_1[11] = 20;

        tou_talk_kino_2[1] = GSW(1703);
        tou_talk_kino_2[3] = 5;
        tou_talk_kino_2[11] = 20;

        tou_talk_roten_1[1] = GSW(1703);
        tou_talk_roten_1[3] = 5;
        tou_talk_roten_1[11] = 20;

        tou_talk_roten_2[1] = GSW(1703);
        tou_talk_roten_2[3] = 5;
        tou_talk_roten_2[11] = 20;

        tou_talk_jugem_1[1] = GSW(1703);
        tou_talk_jugem_1[3] = 5;
        tou_talk_jugem_1[11] = 20;

        tou_talk_jugem_2[1] = GSW(1703);
        tou_talk_jugem_2[3] = 5;
        tou_talk_jugem_2[11] = 20;

        tou_talk_bijyo[1] = GSW(1703);
        tou_talk_bijyo[3] = 5;
        tou_talk_bijyo[11] = 20;

        tou_init_kiza[1] = GSW(1706);
        tou_init_kiza[3] = 2;
        tou_init_kiza[4] = 20;

        tou_talk_kiza[1] = GSW(1703);
        tou_talk_kiza[3] = 5;
        tou_talk_kiza[11] = 20;

        tou_init_sarary_01[1] = GSW(1706);
        tou_init_sarary_01[2] = 49;

        tou_evt_uron[191] = GSW(1703);
        tou_evt_uron[192] = 1;

        tou_evt_tamago_sleep[3] = GSW(1703);
        tou_evt_tamago_sleep[4] = 10;

        tou_evt_tamago_move[240] = GSW(1703);
        tou_evt_tamago_move[241] = 10;

        tou_evt_tamago[93] = GSW(1703);
        tou_evt_tamago[94] = 9;

        tou_talk_tamago[24] = GSW(1703);
        tou_talk_tamago[25] = 11;

        tou_evt_gondora[159] = GSW(1703);
        tou_evt_gondora[160] = 1;

        tou_evt_gondora2[161] = GSW(1703);
        tou_evt_gondora2[162] = 28;

        tou_evt_peach_after[82] = GSW(1703);
        tou_evt_peach_after[83] = 28;

        tou_evt_key_01[1] = GSWF(6027);
        tou_evt_key_01[2] = 1;

        tou_evt_key_02[10] = GSWF(6028);
        tou_evt_key_02[11] = 1;

        tou_evt_hotdog[51] = EVT_HELPER_CMD(2, 50);
        tou_evt_hotdog[52] = EVT_HELPER_OP(LW(3));
        patch::writePatch(&tou_evt_hotdog[54], hotdog_item_hook, sizeof(hotdog_item_hook));
        tou_evt_hotdog[65] = 0;
        tou_evt_hotdog[70] = 0;

        tou_01_init_evt[107] = GSW(1708);
        tou_01_init_evt[108] = 16;
        tou_01_init_evt[211] = GSWF(6027);
        tou_01_init_evt[212] = 0;
        tou_01_init_evt[313] = GSW(1703);
        tou_01_init_evt[314] = 10;
        tou_01_init_evt[333] = GSW(1703);
        tou_01_init_evt[334] = 11;
        tou_01_init_evt[1422] = GSW(1708);
        tou_01_init_evt[1423] = 16;
        tou_01_init_evt[1466] = GSW(1703);
        tou_01_init_evt[1468] = 9;
        tou_01_init_evt[1485] = 10;
        tou_01_init_evt[1516] = 11;
        tou_01_init_evt[1589] = GSW(1703);
        tou_01_init_evt[1590] = 27;
        tou_01_init_evt[1615] = GSW(1708);
        tou_01_init_evt[1616] = 16;

        tou_evt_move_g[51] = GSW(1703);
        tou_evt_move_g[52] = 3;
        tou_evt_move_g[54] = GSW(1703);
        tou_evt_move_g[55] = 3;

        tou_init_gard1[1] = GSW(1703);
        tou_init_gard1[3] = 3;
        tou_init_gard1[11] = 27;

        tou_talk_gard1[1] = GSW(1703);
        tou_talk_gard1[3] = 2;
        tou_talk_gard1[11] = 3;
        tou_talk_gard1[21] = 5;
        tou_talk_gard1[29] = 27;

        tou_init_gard2[1] = GSW(1703);
        tou_init_gard2[3] = 3;
        tou_init_gard2[11] = 27;

        tou_talk_gard2[1] = GSW(1703);
        tou_talk_gard2[3] = 2;
        tou_talk_gard2[11] = 3;
        tou_talk_gard2[21] = 5;
        tou_talk_gard2[29] = 27;

        tou_init_madam_02[1] = GSW(1703);
        tou_init_madam_02[3] = 28;
        tou_init_madam_02[11] = 28;

        tou_talk_madam_02[1] = GSW(1703);
        tou_talk_madam_02[3] = 5;
        tou_talk_madam_02[11] = 20;

        tou_talk_repo[1] = GSW(1703);
        tou_talk_repo[3] = 5;
        tou_talk_repo[11] = 20;

        tou_talk_cam1[1] = GSW(1703);
        tou_talk_cam1[3] = 5;
        tou_talk_cam1[11] = 20;

        tou_init_cam2[1] = GSW(1703);
        tou_init_cam2[2] = 2;

        tou_talk_cam2[1] = GSW(1703);
        tou_talk_cam2[3] = 5;
        tou_talk_cam2[11] = 20;

        tou_talk_kino4[1] = GSW(1703);
        tou_talk_kino4[3] = 5;
        tou_talk_kino4[11] = 20;

        tou_talk_kino5[4] = GSW(1703);
        tou_talk_kino5[6] = 5;
        tou_talk_kino5[14] = 20;

        tou_talk_kinopiko[1] = GSW(1703);
        tou_talk_kinopiko[3] = 5;
        tou_talk_kinopiko[11] = 20;

        tou_talk_kinopio_f[1] = GSW(1703);
        tou_talk_kinopio_f[3] = 5;
        tou_talk_kinopio_f[11] = 20;

        tou_init_chu[1] = GSW(1703);
        tou_init_chu[3] = 13;

        tou_talk_chu[1] = GSW(1703);
        tou_talk_chu[3] = 5;
        tou_talk_chu[11] = 20;

        tou_init_champ[1] = GSW(1703);
        tou_init_champ[3] = 27;

        tou_talk_champ[1] = GSW(1703);
        tou_talk_champ[3] = 27;

        tou_init_shikowa[1] = GSW(1703);
        tou_init_shikowa[3] = 27;

        tou_talk_shikowa[1] = GSW(1703);
        tou_talk_shikowa[3] = 27;

        tou_init_uranoko[1] = GSW(1703);
        tou_init_uranoko[3] = 27;

        tou_talk_uranoko_02[1] = GSW(1703);
        tou_talk_uranoko_02[3] = 27;

        tou_init_ironA[7] = GSW(1703);
        tou_init_ironA[9] = 27;

        tou_talk_ironA[1] = GSW(1703);
        tou_talk_ironA[3] = 27;

        tou_init_ironB[7] = GSW(1703);
        tou_init_ironB[9] = 27;

        tou_talk_ironB[1] = GSW(1703);
        tou_talk_ironB[3] = 27;

        tou_init_borodo[1] = GSW(1703);
        tou_init_borodo[3] = 27;

        tou_talk_borodo[1] = GSW(1703);
        tou_talk_borodo[3] = 27;

        patch::writePatch(&tou_evt_tougi2[182], tou_evt_tougi2_hook, sizeof(tou_evt_tougi2_hook));
        tou_evt_tougi2[273] = GSW(1703);
        tou_evt_tougi2[274] = 2;

        tou_evt_momikucha[7] = GSW(1703);
        tou_evt_momikucha[8] = 7;

        tou_evt_poster_8[4] = GSWF(6028);
        tou_evt_poster_8[5] = 0;

        tou_02_init_evt[1] = GSW(1703);
        tou_02_init_evt[3] = 1;
        tou_02_init_evt[4] = 11;
        tou_02_init_evt[17] = 21;
        tou_02_init_evt[79] = GSW(1703);
        tou_02_init_evt[80] = 3;
        tou_02_init_evt[93] = GSW(1703);
        tou_02_init_evt[94] = 28;
        tou_02_init_evt[111] = GSW(1703);
        tou_02_init_evt[112] = 27;
        tou_02_init_evt[131] = GSW(1703);
        tou_02_init_evt[132] = 27;
        tou_02_init_evt[283] = GSWF(6028);
        tou_02_init_evt[285] = 1;
        tou_02_init_evt[599] = GSWF(6028);
        tou_02_init_evt[601] = 1;
        tou_02_init_evt[897] = GSWF(6028);
        tou_02_init_evt[898] = 0;

        tou_init_g_1[17] = EVT_HELPER_CMD(0, 2);

        tou_talk_g_1[1] = GSW(1703);
        tou_talk_g_1[3] = 7;
        tou_talk_g_1[11] = 0;
        tou_talk_g_1[12] = 0;
        tou_talk_g_1[26] = 19;
        tou_talk_g_1[34] = 28;
        tou_talk_g_1[40] = 0; // Unused
        tou_talk_g_1[71] = 0; // Unused

        tou_move_g_1[6] = GSW(1703);
        tou_move_g_1[8] = 5;
        tou_move_g_1[25] = 0;
        tou_move_g_1[41] = 0;

        tou_init_g_2[1] = GSW(1703);
        tou_init_g_2[2] = 19;

        tou_talk_g_2[1] = GSW(1703);
        tou_talk_g_2[3] = 7;
        tou_talk_g_2[11] = 19;
        tou_talk_g_2[19] = 13;

        tou_move_g_2[31] = GSW(1703);
        tou_move_g_2[32] = 13;
        tou_move_g_2[45] = GSW(1703);
        tou_move_g_2[46] = 19;

        tou_init_g_4[1] = GSW(1703);
        tou_init_g_4[2] = 28;

        tou_talk_g_4[1] = GSW(1703);
        tou_talk_g_4[3] = 4;
        tou_talk_g_4[91] = 5;
        tou_talk_g_4[99] = 19;
        tou_talk_g_4[107] = 28;

        tou_move_g_4[28] = GSW(1703);
        tou_move_g_4[30] = 4;
        tou_move_g_4[32] = 99; // Unused
        tou_move_g_4[39] = 18;
        tou_move_g_4[71] = 19;
        tou_move_g_4[72] = 27;

        tou_talk_g_7[1] = GSW(1703);
        tou_talk_g_7[3] = 7;
        tou_talk_g_7[11] = 19;

        tou_talk_g_8[1] = GSW(1703);
        tou_talk_g_8[3] = 7;
        tou_talk_g_8[11] = 13;

        tou_talk_g_9[1] = GSW(1703);
        tou_talk_g_9[3] = 7;
        tou_talk_g_9[11] = 19;
        tou_talk_g_9[19] = 28;

        tou_init_g_10[1] = GSW(1703);
        tou_init_g_10[3] = 13;
        tou_init_g_10[15] = 19;

        tou_talk_g_10[1] = GSW(1703);
        tou_talk_g_10[3] = 13;
        tou_talk_g_10[15] = 19;

        tou_init_g_11[1] = GSW(1703);
        tou_init_g_11[3] = 13;
        tou_init_g_11[15] = 19;

        tou_talk_g_11[1] = GSW(1703);
        tou_talk_g_11[3] = 13;
        tou_talk_g_11[15] = 19;

        tou_init_brots_04[1] = GSW(1703);
        tou_init_brots_04[2] = 28;

        tou_evt_gardman[159] = GSW(1703);
        tou_evt_gardman[161] = 6;
        tou_evt_gardman[163] = GSW(1703);
        tou_evt_gardman[164] = 6;

        tou_evt_lock_04[1] = GSWF(6029);
        tou_evt_lock_04[2] = 1;

        tou_04_init_evt[123] = GSW(1703);
        tou_04_init_evt[124] = 19;
        tou_04_init_evt[136] = GSW(1703);
        tou_04_init_evt[138] = 1;
        tou_04_init_evt[139] = 11;
        tou_04_init_evt[152] = 21;
        tou_04_init_evt[388] = GSW(1703);
        tou_04_init_evt[390] = 4;
        tou_04_init_evt[392] = 5;
        patch::writePatch(&tou_04_init_evt[402], tou_04_init_evt_hook, sizeof(tou_04_init_evt_hook));
        tou_04_init_evt[437] = GSW(1703);
        tou_04_init_evt[438] = 0; // Unused
        tou_04_init_evt[446] = GSW(1703);
        tou_04_init_evt[447] = 0; // Unused
        tou_04_init_evt[455] = GSW(1703);
        tou_04_init_evt[456] = 13;
        tou_04_init_evt[494] = GSW(1703);
        tou_04_init_evt[495] = 19;
        tou_04_init_evt[513] = GSW(1703);
        tou_04_init_evt[514] = 19;
        tou_04_init_evt[578] = GSWF(6029);
        tou_04_init_evt[579] = 1;
        tou_04_init_evt[616] = GSWF(6029);
        tou_04_init_evt[617] = 1;
        tou_04_init_evt[625] = GSW(1703);
        tou_04_init_evt[626] = 0; // Unused
        tou_04_init_evt[631] = GSW(1703);
        tou_04_init_evt[632] = 28;

        tou_evt_takurami[337] = GSW(1703);
        tou_evt_takurami[338] = 19;

        tou_evt_hiki_open[13] = GSW(1703);
        tou_evt_hiki_open[14] = 19;
        tou_evt_hiki_open[135] = GSW(1703);
        tou_evt_hiki_open[136] = 19;

        tou_init_gans[1] = GSW(1703);
        tou_init_gans[3] = 4;
        tou_init_gans[15] = 16;

        tou_talk_gans[172] = GSW(1703);
        tou_talk_gans[173] = 4;

        tou_init_kinoshikowa[1] = GSW(1703);
        tou_init_kinoshikowa[3] = 20;

        tou_talk_kinoshikowa[373] = GSW(1703);
        tou_talk_kinoshikowa[375] = 20;

        tou_evt_sensyu2[194] = GSW(1703);
        tou_evt_sensyu2[195] = 5;

        tou_evt_nozoki[7] = GSWF(6030);
        tou_evt_nozoki[8] = 1;
        tou_evt_nozoki[161] = GSWF(6030);
        tou_evt_nozoki[162] = 1;
        tou_evt_nozoki[691] = GSWF(6030);
        tou_evt_nozoki[692] = 1;

        patch::writePatch(&tou_05_init_evt[131], tou_05_init_evt_hook3, sizeof(tou_05_init_evt_hook3));
        tou_05_init_evt[135] = 0;
        tou_05_init_evt[136] = 0;
        patch::writePatch(&tou_05_init_evt[174], tou_05_init_evt_hook1, sizeof(tou_05_init_evt_hook1));
        tou_05_init_evt[178] = 0;
        tou_05_init_evt[181] = 0;
        tou_05_init_evt[183] = 0;
        tou_05_init_evt[194] = GSWF(6030);
        tou_05_init_evt[195] = 1;
        patch::writePatch(&tou_05_init_evt[255], tou_05_init_evt_hook2, sizeof(tou_05_init_evt_hook2));
        tou_05_init_evt[298] = GSW(1703);
        tou_05_init_evt[299] = 19;
        tou_05_init_evt[311] = GSW(1703);
        tou_05_init_evt[313] = 1;
        tou_05_init_evt[314] = 11;
        tou_05_init_evt[327] = 21;
        tou_05_init_evt[352] = GSW(1703);
        tou_05_init_evt[354] = 18;
        tou_05_init_evt[359] = 28;
        tou_05_init_evt[411] = GSW(1703);
        tou_05_init_evt[412] = 28;

        tou_evt_kontena[17] = GSWF(6031);
        tou_evt_kontena[18] = 1;

        tou_evt_kaidan[80] = GSWF(6032);
        tou_evt_kaidan[81] = 1;

        tou_evt_block_broken_06[1] = GSWF(6033);
        tou_evt_block_broken_06[2] = 1;

        tou_evt_lock_06[1] = GSWF(6034);
        tou_evt_lock_06[2] = 1;

        tou_evt_chuchu[616] = GSWF(6035);
        tou_evt_chuchu[617] = 1;

        tou_06_init_evt[19] = GSWF(6031);
        tou_06_init_evt[20] = 1;
        tou_06_init_evt[46] = GSWF(6031);
        tou_06_init_evt[47] = 1;
        tou_06_init_evt[269] = GSWF(6032);
        tou_06_init_evt[270] = 1;
        tou_06_init_evt[312] = GSWF(6032);
        tou_06_init_evt[313] = 1;
        tou_06_init_evt[330] = GSWF(6032);
        tou_06_init_evt[331] = 1;
        tou_06_init_evt[486] = GSWF(6035);
        tou_06_init_evt[487] = 1;
        tou_06_init_evt[492] = GSW(1703);
        tou_06_init_evt[493] = 19;
        tou_06_init_evt[502] = GSW(1703);
        tou_06_init_evt[504] = 1;
        tou_06_init_evt[505] = 11;
        tou_06_init_evt[518] = 21;

        tou_evt_champion[568] = GSW(1703);
        tou_evt_champion[569] = 14;

        tou_evt_dakuto_07[25] = GSW(1703);
        tou_evt_dakuto_07[26] = 15;
        tou_evt_dakuto_07[28] = GSW(1703);
        tou_evt_dakuto_07[29] = 15;

        tou_07_init_evt[27] = GSW(1703);
        tou_07_init_evt[28] = 14;
        tou_07_init_evt[30] = GSW(1703);
        tou_07_init_evt[31] = 14;
        tou_07_init_evt[68] = GSW(1703);
        tou_07_init_evt[70] = 15;
        tou_07_init_evt[75] = 28;
        tou_07_init_evt[149] = GSW(1703);
        tou_07_init_evt[151] = 1;
        tou_07_init_evt[152] = 11;
        tou_07_init_evt[165] = 21;
        tou_07_init_evt[188] = GSW(1703);
        tou_07_init_evt[189] = 14;
        tou_07_init_evt[195] = GSW(1703);
        tou_07_init_evt[196] = 14;
        tou_07_init_evt[227] = GSW(1703);
        tou_07_init_evt[228] = 19;
        tou_07_init_evt[237] = GSW(1703);
        tou_07_init_evt[239] = 1;
        tou_07_init_evt[240] = 12;

        tou_evt_toile[1] = GSW(1703);
        tou_evt_toile[2] = 13;
        tou_evt_toile[31] = GSW(1703);
        tou_evt_toile[32] = 13;

        tou_talk_togenoko[1] = GSW(1703);
        tou_talk_togenoko[2] = 28;
        tou_talk_togenoko[4] = GSW(1703);
        tou_talk_togenoko[5] = 13;

        tou_talk_togemet[1] = GSW(1703);
        tou_talk_togemet[2] = 28;
        tou_talk_togemet[4] = GSW(1703);
        tou_talk_togemet[5] = 13;

        tou_talk_hammerbros[1] = GSW(1703);
        tou_talk_hammerbros[2] = 28;
        tou_talk_hammerbros[4] = GSW(1703);
        tou_talk_hammerbros[5] = 13;

        tou_talk_uranoko_08[1] = GSW(1703);
        tou_talk_uranoko_08[2] = 28;
        tou_talk_uranoko_08[4] = GSW(1703);
        tou_talk_uranoko_08[5] = 13;

        tou_evt_1st_leagu[743] = GSW(1703);
        tou_evt_1st_leagu[744] = 12;

        tou_evt_toile_door[22] = GSW(1703);
        tou_evt_toile_door[23] = 13;
        tou_evt_toile_door[39] = GSW(1703);
        tou_evt_toile_door[40] = 28;

        tou_08_init_evt[6] = 0;
        tou_08_init_evt[15] = 0;
        tou_08_init_evt[24] = 0;
        tou_08_init_evt[33] = 0;
        tou_08_init_evt[48] = 0;
        tou_08_init_evt[49] = 0;
        tou_08_init_evt[50] = 0;
        tou_08_init_evt[58] = 0;
        tou_08_init_evt[59] = 0;
        tou_08_init_evt[60] = 0;
        tou_08_init_evt[68] = 0;
        tou_08_init_evt[69] = 0;
        tou_08_init_evt[70] = 0;
        tou_08_init_evt[188] = GSW(1703);
        tou_08_init_evt[190] = 4;
        tou_08_init_evt[202] = GSW(1703);
        tou_08_init_evt[204] = 1;
        tou_08_init_evt[205] = 11;
        tou_08_init_evt[218] = 21;
        tou_08_init_evt[220] = GSW(1708);
        tou_08_init_evt[221] = 18;
        tou_08_init_evt[277] = GSW(1708);
        tou_08_init_evt[278] = 18;

        tou_09_init_evt[1] = GSW(1703);
        tou_09_init_evt[3] = 1;
        tou_09_init_evt[4] = 11;
        tou_09_init_evt[17] = 21;

        tou_init_kino[1] = GSW(1703);
        tou_init_kino[3] = 6;

        tou_talk_kino[1] = GSW(1703);
        tou_talk_kino[3] = 6;

        tou_init_michel_10[1] = GSW(1703);
        tou_init_michel_10[3] = 28;

        tou_talk_michel[1] = GSW(1703);
        tou_talk_michel[2] = 28;

        tou_talk_octo[1] = GSW(1703);
        tou_talk_octo[2] = 28;
        tou_talk_octo[90] = GSW(1703);
        tou_talk_octo[92] = 0;
        tou_talk_octo[93] = 0;

        tou_init_brots_10[1] = GSW(1703);
        tou_init_brots_10[2] = 28;

        tou_talk_brots[1] = GSW(1703);
        tou_talk_brots[2] = 28;

        tou_talk_gangan[1] = GSW(1703);
        tou_talk_gangan[2] = 28;
        tou_talk_gangan[90] = GSW(1703);
        tou_talk_gangan[92] = 0;
        tou_talk_gangan[93] = 0;

        tou_init_poku[6] = GSW(1703);
        tou_init_poku[8] = 28;
        tou_init_poku[20] = 28;

        tou_talk_poku[23] = GSW(1703);
        tou_talk_poku[25] = 0;
        tou_talk_poku[26] = 0;

        tou_init_iron[1] = GSW(1703);
        tou_init_iron[2] = 28;
        tou_init_iron[6] = EVT_HELPER_CMD(2, 29);
        tou_init_iron[7] = GSW(1703);
        tou_init_iron[8] = 12;

        tou_talk_iron[1] = GSW(1703);
        tou_talk_iron[2] = 28;

        tou_init_iron2[1] = GSW(1703);
        tou_init_iron2[2] = 28;
        tou_init_iron2[6] = EVT_HELPER_CMD(2, 29);
        tou_init_iron2[7] = GSW(1703);
        tou_init_iron2[8] = 12;

        tou_talk_iron2[1] = GSW(1703);
        tou_talk_iron2[2] = 28;

        tou_evt_saisyo_after[723] = GSW(1703);
        tou_evt_saisyo_after[724] = 8;

        tou_evt_okorareru[283] = GSWF(6036);
        tou_evt_okorareru[284] = 1;

        tou_evt_block_10[1] = GSWF(6037);
        tou_evt_block_10[2] = 1;

        patch::writePatch(&tou_10_evt_1st_leagu[148], jolene_egg_hook, sizeof(jolene_egg_hook));

        tou_evt_tou_chibi_yoshi[0] = 0;
        tou_evt_tou_chibi_yoshi[1] = 0;
        patch::writePatch(&tou_evt_tou_chibi_yoshi[5], babyyoshi_select_hook, sizeof(babyyoshi_select_hook));
        patch::writePatch(&tou_evt_tou_chibi_yoshi[73], yoshi_gswf_evt, sizeof(yoshi_gswf_evt));
        tou_evt_tou_chibi_yoshi[396] = EVT_HELPER_CMD(2, 50);
        tou_evt_tou_chibi_yoshi[397] = EVT_HELPER_OP(LW(3));
        patch::writePatch(&tou_evt_tou_chibi_yoshi[399], tou_party_evt_hook, sizeof(tou_party_evt_hook));

        tou_10_init_evt[1] = GSW(1703);
        tou_10_init_evt[3] = 1;
        tou_10_init_evt[4] = 11;
        tou_10_init_evt[17] = 21;
        tou_10_init_evt[43] = GSW(1703);
        tou_10_init_evt[44] = 11;
        tou_10_init_evt[46] = GSW(1703);
        tou_10_init_evt[47] = 11;
        tou_10_init_evt[82] = GSW(1703);
        tou_10_init_evt[83] = 7;
        tou_10_init_evt[105] = GSWF(6037);
        tou_10_init_evt[106] = 1;
        tou_10_init_evt[123] = GSW(1703);
        tou_10_init_evt[125] = 6;
        tou_10_init_evt[135] = EVT_HELPER_CMD(1, 4);
        tou_10_init_evt[136] = EVT_HELPER_OP(&tou_10_init_evt[159]);
        tou_10_init_evt[176] = GSW(1703);
        tou_10_init_evt[177] = 28;
        tou_10_init_evt[197] = GSW(1703);
        tou_10_init_evt[198] = 28;
        tou_10_init_evt[222] = GSWF(6036);
        tou_10_init_evt[223] = 1;
        tou_10_init_evt[236] = GSW(1703);
        tou_10_init_evt[237] = 11;

        tou_11_init_evt[138] = GSW(1703);
        tou_11_init_evt[140] = 1;
        tou_11_init_evt[141] = 11;
        tou_11_init_evt[154] = 21;

        tou_evt_block_broken_12[53] = GSWF(6038);
        tou_evt_block_broken_12[54] = 1;

        tou_init_michel_12[1] = GSW(1703);
        tou_init_michel_12[2] = 20;

        tou_init_michel_dummy[1] = GSW(1703);
        tou_init_michel_dummy[2] = 20;

        tou_init_brots_12[1] = GSW(1703);
        tou_init_brots_12[2] = 20;

        tou_init_brots_dummy[1] = GSW(1703);
        tou_init_brots_dummy[2] = 20;

        tou_12_init_evt[1] = GSW(1703);
        tou_12_init_evt[2] = 19;
        tou_12_init_evt[11] = GSW(1703);
        tou_12_init_evt[13] = 1;
        tou_12_init_evt[14] = 11;
        tou_12_init_evt[27] = 21;
        tou_12_init_evt[71] = GSWF(6038);
        tou_12_init_evt[72] = 1;
        tou_12_init_evt[89] = GSW(1703);
        tou_12_init_evt[90] = 20;

        tou_evt_dakuto_13[30] = GSW(1703);
        tou_evt_dakuto_13[31] = 18;
        tou_evt_dakuto_13[33] = GSW(1703);
        tou_evt_dakuto_13[34] = 18;

        tou_evt_nusumi[178] = GSW(1703);
        tou_evt_nusumi[179] = 17;

        tou_evt_first[38] = GSW(1703);
        tou_evt_first[39] = 16;

        tou_13_init_evt[1] = GSW(1703);
        tou_13_init_evt[3] = 2;
        tou_13_init_evt[4] = 11;
        tou_13_init_evt[17] = 21;
        tou_13_init_evt[50] = GSW(1703);
        tou_13_init_evt[52] = 18;
        tou_13_init_evt[57] = 28;
        tou_13_init_evt[104] = GSW(1703);
        tou_13_init_evt[105] = 17;
        tou_13_init_evt[110] = GSW(1703);
        tou_13_init_evt[111] = 16;

        // Assembly
        tou_disp_proc[18] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou_disp_proc[20] = 0x2C03001C; // cmpwi r3, 0x1C

        tou_evt_tou_get_fighter_battle_msg[10] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou_evt_tou_get_fighter_battle_msg[13] = 0x2C03001C; // cmpwi r3, 0x1C

        tou_evt_tou_get_fighter_info[10] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou_evt_tou_get_fighter_info[13] = 0x2C03001C; // cmpwi r3, 0x1C

        tou_evt_tou_get_fighter_name2[10] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou_evt_tou_get_fighter_name2[13] = 0x2C03001C; // cmpwi r3, 0x1C

        tou_evt_tou_get_rule_info[10] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou_evt_tou_get_rule_info[13] = 0x2C03001C; // cmpwi r3, 0x1C

        tou_evt_tou_get_rule_msg2[10] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou_evt_tou_get_rule_msg2[13] = 0x2C03001C; // cmpwi r3, 0x1C

        tou_gans_tex[86] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou_gans_tex[88] = 0x2C03001C; // cmpwi r3, 0x1C

        tou_make_monitor[8] = 0x38850827;  // addi r4, r5, 0x827 GSW(1703)
        tou_make_monitor[10] = 0x2C03001C; // cmpwi r3, 0x1C

        tou_rankingInit[12] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou_rankingInit[14] = 0x2C03001C; // cmpwi r3, 0x1C
        tou_rankingInit[95] = 0x38840827; // addi r4, r4, 0x827 GSW(1703)
        tou_rankingInit[97] = 0x2C03001C; // cmpwi r3, 0x1C

        tou_chk[9] = 0x38800000; // li r4, 0x0
    }

    void exit() {}
} // namespace mod