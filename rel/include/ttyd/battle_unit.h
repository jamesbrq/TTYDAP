#pragma once

#define NUM_BATTLE_GROUPS 532
#define UNIT_KIND_TABLE_SIZE 0xE7

#include <ttyd/battle_database_common.h>
#include <ttyd/seq_mapchange.h>
#include <ttyd/system.h>

using namespace ttyd::battle_database_common;
using namespace ttyd::seq_mapchange;

using ttyd::system::irand;

namespace ttyd::battle_unit
{
    extern "C"
    {
        extern BattleUnitKind unit_gundan_zako;
        extern BattleUnitKind unit_barriern_z_satellite;
        extern BattleUnitKind unit_barriern_z;
        extern BattleUnitKind unit_gundan_zako_elite;
        extern BattleUnitKind unit_gundan_zako_magician;
        extern BattleUnitKind unit_barriern_custom;
        extern BattleUnitKind unit_barriern_custom_satellite;
        extern BattleUnitKind unit_barriern;
        extern BattleUnitKind unit_barriern_satellite;
        extern BattleUnitKind unit_bubble;
        extern BattleUnitKind unit_killer;
        extern BattleUnitKind unit_killer_cannon;
        extern BattleUnitKind unit_heavy_bom;
        extern BattleUnitKind unit_hermos;
        extern BattleUnitKind unit_patamet;
        extern BattleUnitKind unit_sambo;
        extern BattleUnitKind unit_sambo_mummy;
        extern BattleUnitKind unit_patatogemet;
        extern BattleUnitKind unit_kurokumorn;
        extern BattleUnitKind unit_honenoko;
        extern BattleUnitKind unit_red_honenoko;
        extern BattleUnitKind unit_nokonoko;
        extern BattleUnitKind unit_patakuri;
        extern BattleUnitKind unit_patapata;
        extern BattleUnitKind unit_togekuri;
        extern BattleUnitKind unit_hyper_kuriboo;
        extern BattleUnitKind unit_hyper_patakuri;
        extern BattleUnitKind unit_hyper_togekuri;
        extern BattleUnitKind unit_hyper_sinemon;
        extern BattleUnitKind unit_pansy;
        extern BattleUnitKind unit_twinkling_pansy;
        extern BattleUnitKind unit_chorobon;
        extern BattleUnitKind unit_kuriboo;
        extern BattleUnitKind unit_sinemon;
        extern BattleUnitKind unit_sinnosuke;
        extern BattleUnitKind unit_togedaruma;
        extern BattleUnitKind unit_monochrome_sinemon;
        extern BattleUnitKind unit_met;
        extern BattleUnitKind unit_togemet;
        extern BattleUnitKind unit_basabasa;
        extern BattleUnitKind unit_teresa;
        extern BattleUnitKind unit_hannya;
        extern BattleUnitKind unit_hinnya;
        extern BattleUnitKind unit_yami_kuriboo;
        extern BattleUnitKind unit_yami_patakuri;
        extern BattleUnitKind unit_monochrome_kurokumorn;
        extern BattleUnitKind unit_yami_togekuri;
        extern BattleUnitKind unit_borodo;
        extern BattleUnitKind unit_jyugem;
        extern BattleUnitKind unit_bomhei;
        extern BattleUnitKind unit_togezo;
        extern BattleUnitKind unit_yami_noko;
        extern BattleUnitKind unit_flower_chorobon;
        extern BattleUnitKind unit_yami_pata;
        extern BattleUnitKind unit_yamitogedaruma;
        extern BattleUnitKind unit_wanwan;
        extern BattleUnitKind unit_togenoko_ace;
        extern BattleUnitKind unit_super_mahorn;
        extern BattleUnitKind unit_super_mahorn_bunsin;
        extern BattleUnitKind unit_purple_teresa;
        extern BattleUnitKind unit_phantom;
        extern BattleUnitKind unit_pakkun_flower;
        extern BattleUnitKind unit_mahorn_custom;
        extern BattleUnitKind unit_mahorn_custom_bunsin;
        extern BattleUnitKind unit_mahorn;
        extern BattleUnitKind unit_ice_pakkun;
        extern BattleUnitKind unit_hyper_jyugem;
        extern BattleUnitKind unit_hennya;
        extern BattleUnitKind unit_giant_bomb;
        extern BattleUnitKind unit_dokugassun;
        extern BattleUnitKind unit_dark_keeper;
        extern BattleUnitKind unit_burst_wanwan;
        extern BattleUnitKind unit_bllizard;
        extern BattleUnitKind unit_basabasa_chururu;
        extern BattleUnitKind unit_badge_borodo;
        extern BattleUnitKind unit_piders;
        extern BattleUnitKind unit_hyper_togezo;
        extern BattleUnitKind unit_churantalar;
        extern BattleUnitKind unit_black_karon;
        extern BattleUnitKind unit_karon;
        extern BattleUnitKind unit_super_killer;
        extern BattleUnitKind unit_super_killer_cannon;
        extern BattleUnitKind unit_green_chorobon;
        extern BattleUnitKind unit_poison_pakkun;
        extern BattleUnitKind unit_hammer_bros;
        extern BattleUnitKind unit_kamec;
        extern BattleUnitKind unit_kamec_bunsin;
        extern BattleUnitKind unit_togenoko;
        extern BattleUnitKind unit_boomerang_bros;
        extern BattleUnitKind unit_fire_bros;
        extern BattleUnitKind unit_kurikuri;
        extern BattleUnitKind unit_ura_noko;
        extern BattleUnitKind unit_ura_pata;
        extern BattleUnitKind unit_hyper_sinnosuke;
        extern BattleUnitKind unit_kamec_green;
        extern BattleUnitKind unit_kamec_green_bunsin;
        extern BattleUnitKind unit_kamec_red;
        extern BattleUnitKind unit_kamec_red_bunsin;
        extern BattleUnitKind unit_kamec_white;
        extern BattleUnitKind unit_kamec_white_bunsin;
        extern BattleUnitKind unit_borodo_king;
        extern BattleUnitKind unit_nokonoko_fighter;
        extern BattleUnitKind unit_patapata_fighter;
        extern BattleUnitKind unit_crimson_togemet;
        extern BattleUnitKind unit_monochrome_pakkun;

        // Use the same ID space as unitKindTable[] indices.
        static inline BattleUnitKind *GetUnitKindById(uint8_t id)
        {
            switch (id)
            {
                case 0x01:
                    return &unit_kuriboo;
                case 0x02:
                    return &unit_patakuri;
                case 0x03:
                    return &unit_togekuri;
                case 0x04:
                    return &unit_hinnya;
                case 0x05:
                    return &unit_hannya;

                case 0x0B:
                    return &unit_togenoko;
                case 0x0C:
                    return &unit_kamec;
                case 0x0D:
                    return &unit_kamec_bunsin;
                case 0x0E:
                    return &unit_nokonoko;
                case 0x0F:
                    return &unit_patapata;

                case 0x10:
                    return &unit_chorobon;
                case 0x11:
                    return &unit_honenoko;
                case 0x12:
                    return &unit_sinnosuke;
                case 0x13:
                    return &unit_togedaruma;

                case 0x16:
                    return &unit_red_honenoko;

                case 0x18:
                    return &unit_monochrome_kurokumorn;
                case 0x19:
                    return &unit_monochrome_pakkun;
                case 0x1A:
                    return &unit_monochrome_sinemon;
                case 0x1B:
                    return &unit_piders;

                case 0x1C:
                    return &unit_gundan_zako;
                case 0x1D:
                    return &unit_barriern;
                case 0x1E:
                    return &unit_barriern_satellite;

                case 0x24:
                    return &unit_kurikuri;
                case 0x25:
                    return &unit_nokonoko_fighter;
                case 0x26:
                    return &unit_patapata_fighter;
                case 0x27:
                    return &unit_sambo;
                case 0x28:
                    return &unit_jyugem;
                case 0x29:
                    return &unit_togezo;
                case 0x2A:
                    return &unit_hyper_sinnosuke;
                case 0x2B:
                    return &unit_bomhei;
                case 0x2C:
                    return &unit_borodo;
                case 0x2D:
                    return &unit_borodo_king;
                case 0x2E:
                    return &unit_crimson_togemet;
                case 0x2F:
                    return &unit_ura_noko;
                case 0x30:
                    return &unit_ura_pata;

                case 0x31:
                    return &unit_kamec_red;
                case 0x32:
                    return &unit_kamec_red_bunsin;
                case 0x33:
                    return &unit_kamec_white;
                case 0x34:
                    return &unit_kamec_white_bunsin;
                case 0x35:
                    return &unit_kamec_green;
                case 0x36:
                    return &unit_kamec_green_bunsin;

                case 0x37:
                    return &unit_dark_keeper;
                case 0x38:
                    return &unit_hammer_bros;
                case 0x39:
                    return &unit_boomerang_bros;
                case 0x3A:
                    return &unit_fire_bros;
                case 0x3B:
                    return &unit_burst_wanwan;
                case 0x3C:
                    return &unit_togenoko_ace;

                case 0x42:
                    return &unit_hyper_kuriboo;
                case 0x43:
                    return &unit_hyper_patakuri;
                case 0x44:
                    return &unit_hyper_togekuri;
                case 0x45:
                    return &unit_pansy;
                case 0x46:
                    return &unit_twinkling_pansy;
                case 0x47:
                    return &unit_hyper_sinemon;

                case 0x48:
                    return &unit_met;
                case 0x49:
                    return &unit_togemet;
                case 0x4A:
                    return &unit_basabasa;
                case 0x4B:
                    return &unit_teresa;

                case 0x54:
                    return &unit_hermos;
                case 0x55:
                    return &unit_bubble;
                case 0x56:
                    return &unit_green_chorobon;
                case 0x57:
                    return &unit_flower_chorobon;
                case 0x58:
                    return &unit_poison_pakkun;
                case 0x59:
                    return &unit_patamet;
                case 0x5A:
                    return &unit_killer_cannon;
                case 0x5B:
                    return &unit_killer;
                case 0x5C:
                    return &unit_heavy_bom;

                case 0x67:
                    return &unit_kurokumorn;
                case 0x68:
                    return &unit_sambo_mummy;
                case 0x69:
                    return &unit_patatogemet;
                case 0x6A:
                    return &unit_purple_teresa;

                case 0x70:
                    return &unit_bllizard;
                case 0x71:
                    return &unit_ice_pakkun;
                case 0x72:
                    return &unit_sinemon;

                case 0x73:
                    return &unit_barriern_z;
                case 0x74:
                    return &unit_barriern_z_satellite;
                case 0x75:
                    return &unit_barriern_custom;
                case 0x76:
                    return &unit_barriern_custom_satellite;
                case 0x77:
                    return &unit_gundan_zako_magician;
                case 0x78:
                    return &unit_gundan_zako_elite;

                case 0x7B:
                    return &unit_basabasa_chururu;
                case 0x7C:
                    return &unit_phantom;
                case 0x7D:
                    return &unit_super_killer_cannon;
                case 0x7E:
                    return &unit_super_killer;
                case 0x7F:
                    return &unit_wanwan;

                case 0x80:
                    return &unit_super_mahorn;
                case 0x81:
                    return &unit_super_mahorn_bunsin;

                case 0x82:
                    return &unit_karon;
                case 0x83:
                    return &unit_black_karon;

                case 0x99:
                    return &unit_yami_kuriboo;
                case 0x9A:
                    return &unit_yami_patakuri;
                case 0x9B:
                    return &unit_yami_togekuri;
                case 0x9C:
                    return &unit_yami_noko;
                case 0x9D:
                    return &unit_yami_pata;
                case 0x9E:
                    return &unit_badge_borodo;
                case 0x9F:
                    return &unit_hyper_jyugem;
                case 0xA0:
                    return &unit_hyper_togezo;

                case 0xA1:
                    return &unit_mahorn;
                case 0xA2:
                    return &unit_pakkun_flower;
                case 0xA3:
                    return &unit_hennya;
                case 0xA4:
                    return &unit_churantalar;

                case 0xA5:
                    return &unit_yamitogedaruma;
                case 0xA6:
                    return &unit_dokugassun;

                case 0xA7:
                    return &unit_basabasa_chururu;

                case 0xA8:
                    return &unit_giant_bomb;
                case 0xA9:
                    return &unit_mahorn_custom;
                case 0xAA:
                    return &unit_mahorn_custom_bunsin;

                default:
                    return nullptr;
            }
        }

        static inline float GetEnemyYPosition(uint8_t id)
        {
            switch (id)
            {
                // Paragoombas: Paragoomba, H. Paragoomba, Paragloomba
                case 0x02:
                case 0x43:
                case 0x9A:
                // Paratroopas: Paratroopa, KP Paratroopa, S. Paratroopa, D. Paratroopa
                case 0x0F:
                case 0x26:
                case 0x30:
                case 0x9D:
                // Lakitus: Lakitu, Dark Lakitu
                case 0x28:
                case 0x9F:
                // Parabuzzies: Parabuzzy, S. Parabuzzy
                case 0x59:
                case 0x69:
                // Pider
                case 0x1B:
                // Puffs: Dark Puff, Ruff Puff, Ice Puff, Poison Puff
                case 0x18:
                case 0x67:
                case 0x70:
                case 0xA6:
                // Lava Bubble / Embers: Ember, Lava Bubble, Phantom Ember
                case 0x54:
                case 0x55:
                case 0x7C:
                // Yuxes: Yux, Z-Yux, X-Yux
                case 0x1D:
                case 0x73:
                case 0x75:
                    return 40.0f;

                // Swoopers: Swooper, Swoopula, Swampire
                case 0x4A:
                case 0x7B:
                case 0xA7:
                    return 130.0f;

                default:
                    return 0.0f;
            }
        }


        // Battle group setups
        extern BattleGroupSetup btlgrp_aji_aji_01_01_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_01_02_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_01_03_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_01_04_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_01_05_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_01_06_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_01_07_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_01_08_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_04_01_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_06_01_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_08_01_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_09_01_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_10_01_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_13_01_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_13_02_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_15_01_off_1;
        extern BattleGroupSetup btlgrp_aji_aji_mbmkII;
        extern BattleGroupSetup btlgrp_aji_aji_zako;
        extern BattleGroupSetup btlgrp_bom_bom_00_01_off_1;
        extern BattleGroupSetup btlgrp_bom_bom_03_01_off_1;
        extern BattleGroupSetup btlgrp_bom_bom_03_01_off_2;
        extern BattleGroupSetup btlgrp_bom_bom_03_03_off_1;
        extern BattleGroupSetup btlgrp_bom_bom_03_03_off_2;
        extern BattleGroupSetup btlgrp_bom_bom_04_01_off_1;
        extern BattleGroupSetup btlgrp_bom_bom_04_01_off_2;
        extern BattleGroupSetup btlgrp_bom_bom_04_02_off_1;
        extern BattleGroupSetup btlgrp_bom_bom_04_02_off_2;
        extern BattleGroupSetup btlgrp_bom_bom_04_03_off_1;
        extern BattleGroupSetup btlgrp_dou_dou_00_01_off_1;
        extern BattleGroupSetup btlgrp_dou_dou_01_01_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_02_01_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_02_02_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_03_01_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_03_02_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_03_03_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_03_99_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_05_01_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_05_02_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_06_01_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_06_02_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_06_03_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_07_01_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_08_01_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_09_01_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_09_02_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_12_01_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_12_99_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_13_01_off_01;
        extern BattleGroupSetup btlgrp_dou_dou_13_99_off_01;
        extern BattleGroupSetup btlgrp_eki_eki_02_01_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_02_02_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_02_03_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_03_01_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_03_02_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_03_03_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_03_05_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_03_07_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_04_01_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_04_02_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_04_03_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_04_04_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_04_05_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_04_06_off_1;
        extern BattleGroupSetup btlgrp_eki_eki_06_01_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_02_02_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_02_03_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_02_04_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_03_01_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_04_01_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_04_02_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_04_03_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_06_03_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_06_04_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_08_01_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_08_03_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_08_05_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_09_01_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_09_02_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_09_04_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_10_01_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_10_02_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_10_03_off_1;
        extern BattleGroupSetup btlgrp_gon_gon_11_01_off_1;
        extern BattleGroupSetup btlgrp_gor_gor_00_01_off_1;
        extern BattleGroupSetup btlgrp_gor_gor_02_01_off_1;
        extern BattleGroupSetup btlgrp_gor_gor_02_ac_lecture;
        extern BattleGroupSetup btlgrp_gor_gor_02_sac_lecture;
        extern BattleGroupSetup btlgrp_gra_gra_00_01_off_1;
        extern BattleGroupSetup btlgrp_gra_gra_01_01_off_1;
        extern BattleGroupSetup btlgrp_gra_gra_01_01_on_1;
        extern BattleGroupSetup btlgrp_gra_gra_01_01_on_2;
        extern BattleGroupSetup btlgrp_gra_gra_01_01_on_3;
        extern BattleGroupSetup btlgrp_gra_gra_01_03_off_1;
        extern BattleGroupSetup btlgrp_gra_gra_01_03_on_1;
        extern BattleGroupSetup btlgrp_gra_gra_01_03_on_2;
        extern BattleGroupSetup btlgrp_gra_gra_01_03_on_3;
        extern BattleGroupSetup btlgrp_gra_gra_01_04_off_1;
        extern BattleGroupSetup btlgrp_gra_gra_01_04_on_1;
        extern BattleGroupSetup btlgrp_gra_gra_01_04_on_2;
        extern BattleGroupSetup btlgrp_gra_gra_01_04_on_3;
        extern BattleGroupSetup btlgrp_gra_gra_03_01_off_1;
        extern BattleGroupSetup btlgrp_gra_gra_03_01_off_2;
        extern BattleGroupSetup btlgrp_gra_gra_03_04_off_1;
        extern BattleGroupSetup btlgrp_gra_gra_03_04_off_2;
        extern BattleGroupSetup btlgrp_gra_gra_03_04_off_3;
        extern BattleGroupSetup btlgrp_gra_gra_03_05_off_1;
        extern BattleGroupSetup btlgrp_gra_gra_03_05_off_2;
        extern BattleGroupSetup btlgrp_gra_gra_03_05_off_3;
        extern BattleGroupSetup btlgrp_gra_gra_04_02_off_1;
        extern BattleGroupSetup btlgrp_gra_gra_04_02_off_2;
        extern BattleGroupSetup btlgrp_gra_gra_04_02_off_3;
        extern BattleGroupSetup btlgrp_gra_gra_05_01_off_1;
        extern BattleGroupSetup btlgrp_gra_gra_05_01_off_2;
        extern BattleGroupSetup btlgrp_gra_gra_05_03_off_1;
        extern BattleGroupSetup btlgrp_gra_gra_05_03_off_2;
        extern BattleGroupSetup btlgrp_gra_gra_05_03_off_3;
        extern BattleGroupSetup btlgrp_hei_hei_01_01_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_02_02_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_02_03_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_02_04_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_03_01_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_04_01_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_04_01_off_2;
        extern BattleGroupSetup btlgrp_hei_hei_04_02_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_04_02_off_2;
        extern BattleGroupSetup btlgrp_hei_hei_05_01_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_06_02_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_06_03_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_06_03_off_2;
        extern BattleGroupSetup btlgrp_hei_hei_07_01_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_08_01_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_09_01_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_09_01_off_2;
        extern BattleGroupSetup btlgrp_hei_hei_09_03_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_09_03_off_2;
        extern BattleGroupSetup btlgrp_hei_hei_10_01_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_11_01_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_11_01_off_2;
        extern BattleGroupSetup btlgrp_hei_hei_11_03_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_11_03_off_2;
        extern BattleGroupSetup btlgrp_hei_hei_12_01_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_13_02_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_13_02_on_1;
        extern BattleGroupSetup btlgrp_hei_hei_13_02_on_2;
        extern BattleGroupSetup btlgrp_hei_hei_13_02_on_3;
        extern BattleGroupSetup btlgrp_hei_hei_13_03_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_13_03_on_1;
        extern BattleGroupSetup btlgrp_hei_hei_13_03_on_2;
        extern BattleGroupSetup btlgrp_hei_hei_13_04_off_1;
        extern BattleGroupSetup btlgrp_hei_hei_13_04_on_1;
        extern BattleGroupSetup btlgrp_hei_hei_13_04_on_2;
        extern BattleGroupSetup btlgrp_jin_jin_00_01_off_1;
        extern BattleGroupSetup btlgrp_jin_jin_00_01_off_2;
        extern BattleGroupSetup btlgrp_jin_jin_00_01_off_3;
        extern BattleGroupSetup btlgrp_jin_jin_00_01_off_4;
        extern BattleGroupSetup btlgrp_jin_jin_00_atmic_teresa;
        extern BattleGroupSetup btlgrp_jin_jin_01_01_off_1;
        extern BattleGroupSetup btlgrp_jin_jin_01_01_off_2;
        extern BattleGroupSetup btlgrp_jin_jin_01_01_off_3;
        extern BattleGroupSetup btlgrp_jin_jin_01_03_off_1;
        extern BattleGroupSetup btlgrp_jin_jin_01_03_off_2;
        extern BattleGroupSetup btlgrp_jin_jin_01_03_off_3;
        extern BattleGroupSetup btlgrp_jin_jin_01_faker_mario;
        extern BattleGroupSetup btlgrp_jin_jin_03_01_off_1;
        extern BattleGroupSetup btlgrp_jin_jin_03_02_off_1;
        extern BattleGroupSetup btlgrp_jin_jin_04_ramper;
        extern BattleGroupSetup btlgrp_jin_jin_07_01_off_1;
        extern BattleGroupSetup btlgrp_jin_jin_07_01_off_2;
        extern BattleGroupSetup btlgrp_jin_jin_07_02_off_1;
        extern BattleGroupSetup btlgrp_jin_jin_07_02_off_2;
        extern BattleGroupSetup btlgrp_jin_jin_09_01_off_1;
        extern BattleGroupSetup btlgrp_jin_jin_10_01_off_1;
        extern BattleGroupSetup btlgrp_jin_jin_10_02_off_1;
        extern BattleGroupSetup btlgrp_jin_jin_10_03_off_1;
        extern BattleGroupSetup btlgrp_jin_jin_10_04_off_1;
        extern BattleGroupSetup btlgrp_jin_jin_10_05_off_1;
        extern BattleGroupSetup btlgrp_jin_jin_10_06_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_01_01_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_01_01_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_01_01_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_01_02_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_01_02_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_01_02_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_01_03_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_01_03_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_01_03_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_01_04_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_01_04_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_01_04_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_01_05_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_01_05_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_01_05_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_100_01_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_11_01_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_11_01_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_11_01_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_11_02_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_11_02_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_11_02_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_11_03_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_11_03_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_11_03_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_11_04_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_11_04_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_11_04_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_11_05_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_11_05_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_11_05_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_21_01_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_21_01_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_21_01_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_21_02_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_21_02_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_21_02_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_21_03_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_21_03_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_21_03_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_21_04_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_21_04_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_21_04_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_21_05_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_21_05_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_21_05_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_31_01_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_31_01_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_31_01_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_31_02_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_31_02_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_31_02_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_31_03_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_31_03_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_31_03_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_31_04_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_31_04_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_31_04_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_31_05_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_31_05_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_31_05_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_41_01_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_41_01_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_41_01_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_41_02_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_41_02_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_41_02_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_41_03_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_41_03_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_41_03_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_41_04_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_41_04_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_41_04_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_41_05_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_41_05_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_41_05_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_51_01_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_51_01_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_51_01_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_51_02_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_51_02_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_51_02_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_51_03_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_51_03_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_51_03_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_51_04_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_51_04_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_51_04_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_51_05_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_51_05_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_51_05_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_61_01_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_61_01_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_61_01_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_61_02_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_61_02_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_61_02_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_61_03_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_61_03_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_61_03_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_61_04_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_61_04_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_61_04_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_61_05_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_61_05_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_61_05_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_71_01_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_71_01_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_71_01_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_71_02_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_71_02_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_71_02_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_71_03_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_71_03_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_71_03_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_71_04_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_71_04_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_71_04_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_71_05_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_71_05_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_71_05_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_81_01_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_81_01_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_81_01_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_81_02_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_81_02_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_81_02_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_81_03_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_81_03_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_81_03_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_81_04_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_81_04_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_81_04_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_81_05_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_81_05_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_81_05_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_91_01_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_91_01_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_91_01_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_91_02_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_91_02_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_91_02_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_91_03_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_91_03_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_91_03_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_91_04_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_91_04_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_91_04_off_3;
        extern BattleGroupSetup btlgrp_jon_jon_91_05_off_1;
        extern BattleGroupSetup btlgrp_jon_jon_91_05_off_2;
        extern BattleGroupSetup btlgrp_jon_jon_91_05_off_3;
        extern BattleGroupSetup btlgrp_las_las_01_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_01_01_off_2;
        extern BattleGroupSetup btlgrp_las_las_01_02_off_1;
        extern BattleGroupSetup btlgrp_las_las_01_02_off_2;
        extern BattleGroupSetup btlgrp_las_las_01_03_off_1;
        extern BattleGroupSetup btlgrp_las_las_01_03_off_2;
        extern BattleGroupSetup btlgrp_las_las_02_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_02_02_off_1;
        extern BattleGroupSetup btlgrp_las_las_02_03_off_1;
        extern BattleGroupSetup btlgrp_las_las_02_04_off_1;
        extern BattleGroupSetup btlgrp_las_las_02_05_off_1;
        extern BattleGroupSetup btlgrp_las_las_02_06_off_1;
        extern BattleGroupSetup btlgrp_las_las_02_99_off_1;
        extern BattleGroupSetup btlgrp_las_las_03_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_03_01_off_2;
        extern BattleGroupSetup btlgrp_las_las_03_02_off_1;
        extern BattleGroupSetup btlgrp_las_las_03_02_off_2;
        extern BattleGroupSetup btlgrp_las_las_03_03_off_1;
        extern BattleGroupSetup btlgrp_las_las_03_03_off_2;
        extern BattleGroupSetup btlgrp_las_las_04_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_05_01;
        extern BattleGroupSetup btlgrp_las_las_06_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_06_02_off_1;
        extern BattleGroupSetup btlgrp_las_las_06_03_off_1;
        extern BattleGroupSetup btlgrp_las_las_06_04_off_1;
        extern BattleGroupSetup btlgrp_las_las_06_05_off_1;
        extern BattleGroupSetup btlgrp_las_las_06_06_off_1;
        extern BattleGroupSetup btlgrp_las_las_06_99_off_1;
        extern BattleGroupSetup btlgrp_las_las_07_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_07_02_off_1;
        extern BattleGroupSetup btlgrp_las_las_07_03_off_1;
        extern BattleGroupSetup btlgrp_las_las_09_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_09_02_off_1;
        extern BattleGroupSetup btlgrp_las_las_09_rampell;
        extern BattleGroupSetup btlgrp_las_las_17_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_17_02_off_1;
        extern BattleGroupSetup btlgrp_las_las_17_03_off_1;
        extern BattleGroupSetup btlgrp_las_las_17_04_off_1;
        extern BattleGroupSetup btlgrp_las_las_19_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_19_02_off_1;
        extern BattleGroupSetup btlgrp_las_las_19_03_off_1;
        extern BattleGroupSetup btlgrp_las_las_20_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_21_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_21_02_off_1;
        extern BattleGroupSetup btlgrp_las_las_22_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_22_02_off_1;
        extern BattleGroupSetup btlgrp_las_las_22_03_off_1;
        extern BattleGroupSetup btlgrp_las_las_24_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_25_01_off_1;
        extern BattleGroupSetup btlgrp_las_las_25_02_off_1;
        extern BattleGroupSetup btlgrp_las_las_25_03_off_1;
        extern BattleGroupSetup btlgrp_las_las_25_04_off_1;
        extern BattleGroupSetup btlgrp_las_las_25_05_off_1;
        extern BattleGroupSetup btlgrp_las_las_25_06_off_1;
        extern BattleGroupSetup btlgrp_las_las_28_batten_leader;
        extern BattleGroupSetup btlgrp_las_las_28_koopa;
        extern BattleGroupSetup btlgrp_las_las_29_black_peach_1;
        extern BattleGroupSetup btlgrp_las_las_29_black_peach_2;
        extern BattleGroupSetup btlgrp_las_las_bunbaba;
        extern BattleGroupSetup btlgrp_moo_moo_00_01_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_01_01_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_01_02_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_01_02_off_2;
        extern BattleGroupSetup btlgrp_moo_moo_01_03_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_01_04_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_01_04_off_2;
        extern BattleGroupSetup btlgrp_moo_moo_01_04_off_3;
        extern BattleGroupSetup btlgrp_moo_moo_01_05_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_02_01_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_02_02_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_02_03_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_02_04_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_02_05_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_05_01_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_05_01_off_2;
        extern BattleGroupSetup btlgrp_moo_moo_05_01_off_3;
        extern BattleGroupSetup btlgrp_moo_moo_05_02_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_05_03_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_05_03_off_2;
        extern BattleGroupSetup btlgrp_moo_moo_05_04_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_05_05_off_1;
        extern BattleGroupSetup btlgrp_moo_moo_05_05_off_2;
        extern BattleGroupSetup btlgrp_moo_moo_05_05_off_3;
        extern BattleGroupSetup btlgrp_mri_mri_01_01;
        extern BattleGroupSetup btlgrp_mri_mri_04_01;
        extern BattleGroupSetup btlgrp_mri_mri_05_01;
        extern BattleGroupSetup btlgrp_mri_mri_06_2F_02;
        extern BattleGroupSetup btlgrp_mri_mri_06_2F_03;
        extern BattleGroupSetup btlgrp_mri_mri_12_01;
        extern BattleGroupSetup btlgrp_mri_mri_12_02;
        extern BattleGroupSetup btlgrp_mri_mri_15_01;
        extern BattleGroupSetup btlgrp_mri_mri_17_01;
        extern BattleGroupSetup btlgrp_mri_mri_18_02;
        extern BattleGroupSetup btlgrp_mri_mri_18_03;
        extern BattleGroupSetup btlgrp_mri_mri_19_01;
        extern BattleGroupSetup btlgrp_mri_mri_19_02;
        extern BattleGroupSetup btlgrp_mri_mri_19_04;
        extern BattleGroupSetup btlgrp_mri_mri_20_02;
        extern BattleGroupSetup btlgrp_mri_mri_20_03;
        extern BattleGroupSetup btlgrp_mri_mri_mb;
        extern BattleGroupSetup btlgrp_muj_muj_00_01;
        extern BattleGroupSetup btlgrp_muj_muj_01_01;
        extern BattleGroupSetup btlgrp_muj_muj_02_01_off_01;
        extern BattleGroupSetup btlgrp_muj_muj_02_01_off_02;
        extern BattleGroupSetup btlgrp_muj_muj_02_01_off_03;
        extern BattleGroupSetup btlgrp_muj_muj_02_01_off_04;
        extern BattleGroupSetup btlgrp_muj_muj_02_04_off_01;
        extern BattleGroupSetup btlgrp_muj_muj_02_04_off_02;
        extern BattleGroupSetup btlgrp_muj_muj_02_04_off_03;
        extern BattleGroupSetup btlgrp_muj_muj_02_07_off_01;
        extern BattleGroupSetup btlgrp_muj_muj_02_07_off_02;
        extern BattleGroupSetup btlgrp_muj_muj_02_07_off_03;
        extern BattleGroupSetup btlgrp_muj_muj_03_01_off_01;
        extern BattleGroupSetup btlgrp_muj_muj_03_01_off_02;
        extern BattleGroupSetup btlgrp_muj_muj_03_01_off_03;
        extern BattleGroupSetup btlgrp_muj_muj_03_02_off_01;
        extern BattleGroupSetup btlgrp_muj_muj_03_02_off_02;
        extern BattleGroupSetup btlgrp_muj_muj_03_02_off_03;
        extern BattleGroupSetup btlgrp_muj_muj_03_04_off_01;
        extern BattleGroupSetup btlgrp_muj_muj_03_04_off_02;
        extern BattleGroupSetup btlgrp_muj_muj_03_04_off_03;
        extern BattleGroupSetup btlgrp_muj_muj_05_01;
        extern BattleGroupSetup btlgrp_muj_muj_05_02;
        extern BattleGroupSetup btlgrp_muj_muj_cortez;
        extern BattleGroupSetup btlgrp_muj_muj_kanbu;
        extern BattleGroupSetup btlgrp_nok_hei_ep;
        extern BattleGroupSetup btlgrp_pik_pik_06_01_off_1;
        extern BattleGroupSetup btlgrp_pik_pik_06_02_off_1;
        extern BattleGroupSetup btlgrp_pik_pik_06_03_off_1;
        extern BattleGroupSetup btlgrp_pik_pik_06_04_off_1;
        extern BattleGroupSetup btlgrp_pik_pik_06_05_off_1;
        extern BattleGroupSetup btlgrp_pik_pik_06_06_off_1;
        extern BattleGroupSetup btlgrp_pik_pik_06_07_off_1;
        extern BattleGroupSetup btlgrp_pik_pik_06_08_off_1;
        extern BattleGroupSetup btlgrp_pik_pik_06_09_off_1;
        extern BattleGroupSetup btlgrp_rsh_rsh_06_01_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_01_01_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_01_02_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_04_01_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_04_01_on_1;
        extern BattleGroupSetup btlgrp_tik_tik_04_01_on_2;
        extern BattleGroupSetup btlgrp_tik_tik_04_02_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_04_02_on_1;
        extern BattleGroupSetup btlgrp_tik_tik_04_02_on_2;
        extern BattleGroupSetup btlgrp_tik_tik_04_03_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_04_03_off_2;
        extern BattleGroupSetup btlgrp_tik_tik_04_03_off_3;
        extern BattleGroupSetup btlgrp_tik_tik_04_04_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_04_04_off_2;
        extern BattleGroupSetup btlgrp_tik_tik_04_04_off_3;
        extern BattleGroupSetup btlgrp_tik_tik_07_01_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_07_01_off_2;
        extern BattleGroupSetup btlgrp_tik_tik_07_02_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_07_02_off_2;
        extern BattleGroupSetup btlgrp_tik_tik_07_03_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_07_03_off_2;
        extern BattleGroupSetup btlgrp_tik_tik_07_03_off_3;
        extern BattleGroupSetup btlgrp_tik_tik_07_04_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_07_04_off_2;
        extern BattleGroupSetup btlgrp_tik_tik_07_04_off_3;
        extern BattleGroupSetup btlgrp_tik_tik_15_01_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_15_02_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_15_03_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_15_04_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_15_04_off_2;
        extern BattleGroupSetup btlgrp_tik_tik_15_04_off_3;
        extern BattleGroupSetup btlgrp_tik_tik_15_05_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_15_05_off_2;
        extern BattleGroupSetup btlgrp_tik_tik_15_05_off_3;
        extern BattleGroupSetup btlgrp_tik_tik_18_01_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_18_01_off_2;
        extern BattleGroupSetup btlgrp_tik_tik_18_02_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_18_02_off_2;
        extern BattleGroupSetup btlgrp_tik_tik_18_03_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_18_03_off_2;
        extern BattleGroupSetup btlgrp_tik_tik_18_03_off_3;
        extern BattleGroupSetup btlgrp_tik_tik_18_04_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_18_04_off_2;
        extern BattleGroupSetup btlgrp_tik_tik_18_04_off_3;
        extern BattleGroupSetup btlgrp_tik_tik_20_01_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_20_02_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_20_03_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_20_04_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_20_05_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_20_06_off_1;
        extern BattleGroupSetup btlgrp_tik_tik_gesso;
        extern BattleGroupSetup btlgrp_tou_tou_add_1;
        extern BattleGroupSetup btlgrp_tou_tou_add_2;
        extern BattleGroupSetup btlgrp_tou_tou_boss;
        extern BattleGroupSetup btlgrp_tou_tou_champ;
        extern BattleGroupSetup btlgrp_tou_tou_koopa;
        extern BattleGroupSetup btlgrp_tou_tou_rank_1;
        extern BattleGroupSetup btlgrp_tou_tou_rank_10;
        extern BattleGroupSetup btlgrp_tou_tou_rank_11;
        extern BattleGroupSetup btlgrp_tou_tou_rank_12;
        extern BattleGroupSetup btlgrp_tou_tou_rank_13;
        extern BattleGroupSetup btlgrp_tou_tou_rank_14;
        extern BattleGroupSetup btlgrp_tou_tou_rank_15;
        extern BattleGroupSetup btlgrp_tou_tou_rank_16;
        extern BattleGroupSetup btlgrp_tou_tou_rank_17;
        extern BattleGroupSetup btlgrp_tou_tou_rank_18;
        extern BattleGroupSetup btlgrp_tou_tou_rank_19;
        extern BattleGroupSetup btlgrp_tou_tou_rank_2;
        extern BattleGroupSetup btlgrp_tou_tou_rank_3;
        extern BattleGroupSetup btlgrp_tou_tou_rank_4;
        extern BattleGroupSetup btlgrp_tou_tou_rank_5;
        extern BattleGroupSetup btlgrp_tou_tou_rank_6;
        extern BattleGroupSetup btlgrp_tou_tou_rank_7;
        extern BattleGroupSetup btlgrp_tou_tou_rank_8;
        extern BattleGroupSetup btlgrp_tou_tou_rank_9;
        extern BattleGroupSetup btlgrp_win_win_00_01_off_1;
        extern BattleGroupSetup btlgrp_win_win_00_01_on_1;
        extern BattleGroupSetup btlgrp_win_win_00_01_on_2;
        extern BattleGroupSetup btlgrp_win_win_00_01_on_3;
        extern BattleGroupSetup btlgrp_win_win_00_02_off_1;
        extern BattleGroupSetup btlgrp_win_win_00_02_on_1;
        extern BattleGroupSetup btlgrp_win_win_00_02_on_2;
        extern BattleGroupSetup btlgrp_win_win_00_03_off_1;
        extern BattleGroupSetup btlgrp_win_win_00_03_on_1;
        extern BattleGroupSetup btlgrp_win_win_00_03_on_2;
        extern BattleGroupSetup btlgrp_win_win_00_04_off_1;
        extern BattleGroupSetup btlgrp_win_win_02_01_off_1;
        extern BattleGroupSetup btlgrp_win_win_02_01_on_1;
        extern BattleGroupSetup btlgrp_win_win_02_01_on_2;
        extern BattleGroupSetup btlgrp_win_win_02_01_on_3;
        extern BattleGroupSetup btlgrp_win_win_02_02_off_1;
        extern BattleGroupSetup btlgrp_win_win_02_02_on_1;
        extern BattleGroupSetup btlgrp_win_win_02_02_on_2;
        extern BattleGroupSetup btlgrp_win_win_02_02_on_3;
        extern BattleGroupSetup btlgrp_win_win_03_01_off_1;
        extern BattleGroupSetup btlgrp_win_win_03_01_on_1;
        extern BattleGroupSetup btlgrp_win_win_03_01_on_2;
        extern BattleGroupSetup btlgrp_win_win_03_01_on_3;
        extern BattleGroupSetup btlgrp_win_win_03_01_on_4;

        inline BattleGroupSetup *battleGroupList[NUM_BATTLE_GROUPS] = {
            &btlgrp_aji_aji_zako,         &btlgrp_aji_aji_01_01_off_1,  &btlgrp_aji_aji_01_02_off_1,
            &btlgrp_aji_aji_01_03_off_1,  &btlgrp_aji_aji_01_04_off_1,  &btlgrp_aji_aji_01_05_off_1,
            &btlgrp_aji_aji_01_06_off_1,  &btlgrp_aji_aji_01_07_off_1,  &btlgrp_aji_aji_01_08_off_1,
            &btlgrp_aji_aji_04_01_off_1,  &btlgrp_aji_aji_06_01_off_1,  &btlgrp_aji_aji_08_01_off_1,
            &btlgrp_aji_aji_09_01_off_1,  &btlgrp_aji_aji_10_01_off_1,  &btlgrp_aji_aji_13_01_off_1,
            &btlgrp_aji_aji_13_02_off_1,  &btlgrp_aji_aji_15_01_off_1,  &btlgrp_bom_bom_00_01_off_1,
            &btlgrp_bom_bom_03_01_off_1,  &btlgrp_bom_bom_03_01_off_2,  &btlgrp_bom_bom_03_03_off_1,
            &btlgrp_bom_bom_03_03_off_2,  &btlgrp_bom_bom_04_01_off_1,  &btlgrp_bom_bom_04_01_off_2,
            &btlgrp_bom_bom_04_02_off_1,  &btlgrp_bom_bom_04_02_off_2,  &btlgrp_bom_bom_04_03_off_1,
            &btlgrp_dou_dou_00_01_off_1,  &btlgrp_dou_dou_01_01_off_01, &btlgrp_dou_dou_02_01_off_01,
            &btlgrp_dou_dou_02_02_off_01, &btlgrp_dou_dou_03_01_off_01, &btlgrp_dou_dou_03_99_off_01,
            &btlgrp_dou_dou_03_02_off_01, &btlgrp_dou_dou_03_03_off_01, &btlgrp_dou_dou_05_01_off_01,
            &btlgrp_dou_dou_05_02_off_01, &btlgrp_dou_dou_06_01_off_01, &btlgrp_dou_dou_06_02_off_01,
            &btlgrp_dou_dou_06_03_off_01, &btlgrp_dou_dou_07_01_off_01, &btlgrp_dou_dou_08_01_off_01,
            &btlgrp_dou_dou_09_01_off_01, &btlgrp_dou_dou_09_02_off_01, &btlgrp_dou_dou_12_01_off_01,
            &btlgrp_dou_dou_12_99_off_01, &btlgrp_dou_dou_13_01_off_01, &btlgrp_dou_dou_13_99_off_01,
            &btlgrp_eki_eki_06_01_off_1,  &btlgrp_eki_eki_02_01_off_1,  &btlgrp_eki_eki_02_02_off_1,
            &btlgrp_eki_eki_02_03_off_1,  &btlgrp_eki_eki_03_01_off_1,  &btlgrp_eki_eki_03_02_off_1,
            &btlgrp_eki_eki_03_03_off_1,  &btlgrp_eki_eki_03_05_off_1,  &btlgrp_eki_eki_03_07_off_1,
            &btlgrp_eki_eki_04_01_off_1,  &btlgrp_eki_eki_04_02_off_1,  &btlgrp_eki_eki_04_06_off_1,
            &btlgrp_eki_eki_04_03_off_1,  &btlgrp_eki_eki_04_04_off_1,  &btlgrp_eki_eki_04_05_off_1,
            &btlgrp_gon_gon_02_02_off_1,  &btlgrp_gon_gon_02_03_off_1,  &btlgrp_gon_gon_02_04_off_1,
            &btlgrp_gon_gon_03_01_off_1,  &btlgrp_gon_gon_04_01_off_1,  &btlgrp_gon_gon_04_02_off_1,
            &btlgrp_gon_gon_04_03_off_1,  &btlgrp_gon_gon_06_03_off_1,  &btlgrp_gon_gon_06_04_off_1,
            &btlgrp_gon_gon_08_01_off_1,  &btlgrp_gon_gon_08_03_off_1,  &btlgrp_gon_gon_08_05_off_1,
            &btlgrp_gon_gon_09_01_off_1,  &btlgrp_gon_gon_09_02_off_1,  &btlgrp_gon_gon_09_04_off_1,
            &btlgrp_gon_gon_10_01_off_1,  &btlgrp_gon_gon_10_02_off_1,  &btlgrp_gon_gon_10_03_off_1,
            &btlgrp_gra_gra_01_01_off_1,  &btlgrp_gra_gra_01_01_on_1,   &btlgrp_gra_gra_01_01_on_2,
            &btlgrp_gra_gra_01_01_on_3,   &btlgrp_gra_gra_01_03_off_1,  &btlgrp_gra_gra_01_03_on_1,
            &btlgrp_gra_gra_01_03_on_2,   &btlgrp_gra_gra_01_03_on_3,   &btlgrp_gra_gra_01_04_off_1,
            &btlgrp_gra_gra_01_04_on_1,   &btlgrp_gra_gra_01_04_on_2,   &btlgrp_gra_gra_01_04_on_3,
            &btlgrp_gra_gra_03_01_off_1,  &btlgrp_gra_gra_03_01_off_2,  &btlgrp_gra_gra_03_04_off_1,
            &btlgrp_gra_gra_03_04_off_2,  &btlgrp_gra_gra_03_04_off_3,  &btlgrp_gra_gra_03_05_off_1,
            &btlgrp_gra_gra_03_05_off_2,  &btlgrp_gra_gra_03_05_off_3,  &btlgrp_gra_gra_04_02_off_1,
            &btlgrp_gra_gra_04_02_off_2,  &btlgrp_gra_gra_04_02_off_3,  &btlgrp_gra_gra_05_01_off_1,
            &btlgrp_gra_gra_05_01_off_2,  &btlgrp_gra_gra_05_03_off_1,  &btlgrp_gra_gra_05_03_off_2,
            &btlgrp_gra_gra_05_03_off_3,  &btlgrp_hei_hei_01_01_off_1,  &btlgrp_hei_hei_02_02_off_1,
            &btlgrp_hei_hei_02_03_off_1,  &btlgrp_hei_hei_02_04_off_1,  &btlgrp_hei_hei_03_01_off_1,
            &btlgrp_hei_hei_04_01_off_1,  &btlgrp_hei_hei_04_01_off_2,  &btlgrp_hei_hei_04_02_off_1,
            &btlgrp_hei_hei_04_02_off_2,  &btlgrp_hei_hei_05_01_off_1,  &btlgrp_hei_hei_06_02_off_1,
            &btlgrp_hei_hei_06_03_off_1,  &btlgrp_hei_hei_06_03_off_2,  &btlgrp_hei_hei_07_01_off_1,
            &btlgrp_hei_hei_08_01_off_1,  &btlgrp_hei_hei_09_01_off_1,  &btlgrp_hei_hei_09_01_off_2,
            &btlgrp_hei_hei_09_03_off_1,  &btlgrp_hei_hei_09_03_off_2,  &btlgrp_hei_hei_11_01_off_1,
            &btlgrp_hei_hei_11_01_off_2,  &btlgrp_hei_hei_11_03_off_1,  &btlgrp_hei_hei_11_03_off_2,
            &btlgrp_hei_hei_12_01_off_1,  &btlgrp_hei_hei_13_02_off_1,  &btlgrp_hei_hei_13_02_on_1,
            &btlgrp_hei_hei_13_02_on_2,   &btlgrp_hei_hei_13_02_on_3,   &btlgrp_hei_hei_13_03_off_1,
            &btlgrp_hei_hei_13_03_on_1,   &btlgrp_hei_hei_13_03_on_2,   &btlgrp_hei_hei_13_04_off_1,
            &btlgrp_hei_hei_13_04_on_1,   &btlgrp_hei_hei_13_04_on_2,   &btlgrp_jin_jin_00_01_off_1,
            &btlgrp_jin_jin_00_01_off_2,  &btlgrp_jin_jin_00_01_off_3,  &btlgrp_jin_jin_00_01_off_4,
            &btlgrp_jin_jin_01_01_off_1,  &btlgrp_jin_jin_01_01_off_2,  &btlgrp_jin_jin_01_01_off_3,
            &btlgrp_jin_jin_01_03_off_1,  &btlgrp_jin_jin_01_03_off_2,  &btlgrp_jin_jin_01_03_off_3,
            &btlgrp_jin_jin_03_01_off_1,  &btlgrp_jin_jin_03_02_off_1,  &btlgrp_jin_jin_07_01_off_1,
            &btlgrp_jin_jin_07_01_off_2,  &btlgrp_jin_jin_07_02_off_1,  &btlgrp_jin_jin_07_02_off_2,
            &btlgrp_jin_jin_09_01_off_1,  &btlgrp_jin_jin_10_01_off_1,  &btlgrp_jin_jin_10_02_off_1,
            &btlgrp_jin_jin_10_03_off_1,  &btlgrp_jin_jin_10_04_off_1,  &btlgrp_jin_jin_10_05_off_1,
            &btlgrp_jin_jin_10_06_off_1,  &btlgrp_jon_jon_01_01_off_1,  &btlgrp_jon_jon_01_01_off_2,
            &btlgrp_jon_jon_01_01_off_3,  &btlgrp_jon_jon_01_02_off_1,  &btlgrp_jon_jon_01_02_off_2,
            &btlgrp_jon_jon_01_02_off_3,  &btlgrp_jon_jon_01_03_off_1,  &btlgrp_jon_jon_01_03_off_2,
            &btlgrp_jon_jon_01_03_off_3,  &btlgrp_jon_jon_01_04_off_1,  &btlgrp_jon_jon_01_04_off_2,
            &btlgrp_jon_jon_01_04_off_3,  &btlgrp_jon_jon_01_05_off_1,  &btlgrp_jon_jon_01_05_off_2,
            &btlgrp_jon_jon_01_05_off_3,  &btlgrp_jon_jon_11_01_off_1,  &btlgrp_jon_jon_11_01_off_2,
            &btlgrp_jon_jon_11_01_off_3,  &btlgrp_jon_jon_11_02_off_1,  &btlgrp_jon_jon_11_02_off_2,
            &btlgrp_jon_jon_11_02_off_3,  &btlgrp_jon_jon_11_03_off_1,  &btlgrp_jon_jon_11_03_off_2,
            &btlgrp_jon_jon_11_03_off_3,  &btlgrp_jon_jon_11_04_off_1,  &btlgrp_jon_jon_11_04_off_2,
            &btlgrp_jon_jon_11_04_off_3,  &btlgrp_jon_jon_11_05_off_1,  &btlgrp_jon_jon_11_05_off_2,
            &btlgrp_jon_jon_11_05_off_3,  &btlgrp_jon_jon_21_01_off_1,  &btlgrp_jon_jon_21_01_off_2,
            &btlgrp_jon_jon_21_01_off_3,  &btlgrp_jon_jon_21_02_off_1,  &btlgrp_jon_jon_21_02_off_2,
            &btlgrp_jon_jon_21_02_off_3,  &btlgrp_jon_jon_21_03_off_1,  &btlgrp_jon_jon_21_03_off_2,
            &btlgrp_jon_jon_21_03_off_3,  &btlgrp_jon_jon_21_04_off_1,  &btlgrp_jon_jon_21_04_off_2,
            &btlgrp_jon_jon_21_04_off_3,  &btlgrp_jon_jon_21_05_off_1,  &btlgrp_jon_jon_21_05_off_2,
            &btlgrp_jon_jon_21_05_off_3,  &btlgrp_jon_jon_31_01_off_1,  &btlgrp_jon_jon_31_01_off_2,
            &btlgrp_jon_jon_31_01_off_3,  &btlgrp_jon_jon_31_02_off_1,  &btlgrp_jon_jon_31_02_off_2,
            &btlgrp_jon_jon_31_02_off_3,  &btlgrp_jon_jon_31_03_off_1,  &btlgrp_jon_jon_31_03_off_2,
            &btlgrp_jon_jon_31_03_off_3,  &btlgrp_jon_jon_31_04_off_1,  &btlgrp_jon_jon_31_04_off_2,
            &btlgrp_jon_jon_31_04_off_3,  &btlgrp_jon_jon_31_05_off_1,  &btlgrp_jon_jon_31_05_off_2,
            &btlgrp_jon_jon_31_05_off_3,  &btlgrp_jon_jon_41_01_off_1,  &btlgrp_jon_jon_41_01_off_2,
            &btlgrp_jon_jon_41_01_off_3,  &btlgrp_jon_jon_41_02_off_1,  &btlgrp_jon_jon_41_02_off_2,
            &btlgrp_jon_jon_41_02_off_3,  &btlgrp_jon_jon_41_03_off_1,  &btlgrp_jon_jon_41_03_off_2,
            &btlgrp_jon_jon_41_03_off_3,  &btlgrp_jon_jon_41_04_off_1,  &btlgrp_jon_jon_41_04_off_2,
            &btlgrp_jon_jon_41_04_off_3,  &btlgrp_jon_jon_41_05_off_1,  &btlgrp_jon_jon_41_05_off_2,
            &btlgrp_jon_jon_41_05_off_3,  &btlgrp_jon_jon_51_01_off_1,  &btlgrp_jon_jon_51_01_off_2,
            &btlgrp_jon_jon_51_01_off_3,  &btlgrp_jon_jon_51_02_off_1,  &btlgrp_jon_jon_51_02_off_2,
            &btlgrp_jon_jon_51_02_off_3,  &btlgrp_jon_jon_51_03_off_1,  &btlgrp_jon_jon_51_03_off_2,
            &btlgrp_jon_jon_51_03_off_3,  &btlgrp_jon_jon_51_04_off_1,  &btlgrp_jon_jon_51_04_off_2,
            &btlgrp_jon_jon_51_04_off_3,  &btlgrp_jon_jon_51_05_off_1,  &btlgrp_jon_jon_51_05_off_2,
            &btlgrp_jon_jon_51_05_off_3,  &btlgrp_jon_jon_61_01_off_1,  &btlgrp_jon_jon_61_01_off_2,
            &btlgrp_jon_jon_61_01_off_3,  &btlgrp_jon_jon_61_02_off_1,  &btlgrp_jon_jon_61_02_off_2,
            &btlgrp_jon_jon_61_02_off_3,  &btlgrp_jon_jon_61_03_off_1,  &btlgrp_jon_jon_61_03_off_2,
            &btlgrp_jon_jon_61_03_off_3,  &btlgrp_jon_jon_61_04_off_1,  &btlgrp_jon_jon_61_04_off_2,
            &btlgrp_jon_jon_61_04_off_3,  &btlgrp_jon_jon_61_05_off_1,  &btlgrp_jon_jon_61_05_off_2,
            &btlgrp_jon_jon_61_05_off_3,  &btlgrp_jon_jon_71_01_off_1,  &btlgrp_jon_jon_71_01_off_2,
            &btlgrp_jon_jon_71_01_off_3,  &btlgrp_jon_jon_71_02_off_1,  &btlgrp_jon_jon_71_02_off_2,
            &btlgrp_jon_jon_71_02_off_3,  &btlgrp_jon_jon_71_03_off_1,  &btlgrp_jon_jon_71_03_off_2,
            &btlgrp_jon_jon_71_03_off_3,  &btlgrp_jon_jon_71_04_off_1,  &btlgrp_jon_jon_71_04_off_2,
            &btlgrp_jon_jon_71_04_off_3,  &btlgrp_jon_jon_71_05_off_1,  &btlgrp_jon_jon_71_05_off_2,
            &btlgrp_jon_jon_71_05_off_3,  &btlgrp_jon_jon_81_01_off_1,  &btlgrp_jon_jon_81_01_off_2,
            &btlgrp_jon_jon_81_01_off_3,  &btlgrp_jon_jon_81_02_off_1,  &btlgrp_jon_jon_81_02_off_2,
            &btlgrp_jon_jon_81_02_off_3,  &btlgrp_jon_jon_81_03_off_1,  &btlgrp_jon_jon_81_03_off_2,
            &btlgrp_jon_jon_81_03_off_3,  &btlgrp_jon_jon_81_04_off_1,  &btlgrp_jon_jon_81_04_off_2,
            &btlgrp_jon_jon_81_04_off_3,  &btlgrp_jon_jon_81_05_off_1,  &btlgrp_jon_jon_81_05_off_2,
            &btlgrp_jon_jon_81_05_off_3,  &btlgrp_jon_jon_91_01_off_1,  &btlgrp_jon_jon_91_01_off_2,
            &btlgrp_jon_jon_91_01_off_3,  &btlgrp_jon_jon_91_02_off_1,  &btlgrp_jon_jon_91_02_off_2,
            &btlgrp_jon_jon_91_02_off_3,  &btlgrp_jon_jon_91_03_off_1,  &btlgrp_jon_jon_91_03_off_2,
            &btlgrp_jon_jon_91_03_off_3,  &btlgrp_jon_jon_91_04_off_1,  &btlgrp_jon_jon_91_04_off_2,
            &btlgrp_jon_jon_91_04_off_3,  &btlgrp_jon_jon_91_05_off_1,  &btlgrp_jon_jon_91_05_off_2,
            &btlgrp_jon_jon_91_05_off_3,  &btlgrp_las_las_05_01,        &btlgrp_las_las_17_01_off_1,
            &btlgrp_las_las_17_02_off_1,  &btlgrp_las_las_17_03_off_1,  &btlgrp_las_las_17_04_off_1,
            &btlgrp_las_las_01_01_off_1,  &btlgrp_las_las_01_02_off_1,  &btlgrp_las_las_01_03_off_1,
            &btlgrp_las_las_01_01_off_2,  &btlgrp_las_las_01_02_off_2,  &btlgrp_las_las_01_03_off_2,
            &btlgrp_las_las_02_01_off_1,  &btlgrp_las_las_02_02_off_1,  &btlgrp_las_las_02_03_off_1,
            &btlgrp_las_las_02_04_off_1,  &btlgrp_las_las_02_05_off_1,  &btlgrp_las_las_02_06_off_1,
            &btlgrp_las_las_02_99_off_1,  &btlgrp_las_las_03_01_off_1,  &btlgrp_las_las_03_02_off_1,
            &btlgrp_las_las_03_03_off_1,  &btlgrp_las_las_03_01_off_2,  &btlgrp_las_las_03_02_off_2,
            &btlgrp_las_las_03_03_off_2,  &btlgrp_las_las_04_01_off_1,  &btlgrp_las_las_06_01_off_1,
            &btlgrp_las_las_06_02_off_1,  &btlgrp_las_las_06_03_off_1,  &btlgrp_las_las_06_04_off_1,
            &btlgrp_las_las_06_05_off_1,  &btlgrp_las_las_06_06_off_1,  &btlgrp_las_las_06_99_off_1,
            &btlgrp_las_las_07_01_off_1,  &btlgrp_las_las_07_02_off_1,  &btlgrp_las_las_07_03_off_1,
            &btlgrp_las_las_09_01_off_1,  &btlgrp_las_las_09_02_off_1,  &btlgrp_las_las_19_01_off_1,
            &btlgrp_las_las_19_02_off_1,  &btlgrp_las_las_19_03_off_1,  &btlgrp_las_las_20_01_off_1,
            &btlgrp_las_las_21_01_off_1,  &btlgrp_las_las_21_02_off_1,  &btlgrp_las_las_22_01_off_1,
            &btlgrp_las_las_22_02_off_1,  &btlgrp_las_las_22_03_off_1,  &btlgrp_las_las_24_01_off_1,
            &btlgrp_las_las_25_01_off_1,  &btlgrp_las_las_25_02_off_1,  &btlgrp_las_las_25_03_off_1,
            &btlgrp_las_las_25_04_off_1,  &btlgrp_las_las_25_05_off_1,  &btlgrp_las_las_25_06_off_1,
            &btlgrp_moo_moo_00_01_off_1,  &btlgrp_moo_moo_01_01_off_1,  &btlgrp_moo_moo_01_02_off_1,
            &btlgrp_moo_moo_01_02_off_2,  &btlgrp_moo_moo_01_03_off_1,  &btlgrp_moo_moo_01_04_off_1,
            &btlgrp_moo_moo_01_04_off_2,  &btlgrp_moo_moo_01_04_off_3,  &btlgrp_moo_moo_01_05_off_1,
            &btlgrp_moo_moo_02_01_off_1,  &btlgrp_moo_moo_02_02_off_1,  &btlgrp_moo_moo_02_03_off_1,
            &btlgrp_moo_moo_02_04_off_1,  &btlgrp_moo_moo_02_05_off_1,  &btlgrp_moo_moo_05_01_off_1,
            &btlgrp_moo_moo_05_01_off_2,  &btlgrp_moo_moo_05_01_off_3,  &btlgrp_moo_moo_05_02_off_1,
            &btlgrp_moo_moo_05_03_off_1,  &btlgrp_moo_moo_05_03_off_2,  &btlgrp_moo_moo_05_04_off_1,
            &btlgrp_moo_moo_05_05_off_1,  &btlgrp_moo_moo_05_05_off_2,  &btlgrp_moo_moo_05_05_off_3,
            &btlgrp_mri_mri_01_01,        &btlgrp_mri_mri_04_01,        &btlgrp_mri_mri_05_01,
            &btlgrp_mri_mri_06_2F_02,     &btlgrp_mri_mri_06_2F_03,     &btlgrp_mri_mri_12_01,
            &btlgrp_mri_mri_12_02,        &btlgrp_mri_mri_15_01,        &btlgrp_mri_mri_17_01,
            &btlgrp_mri_mri_18_02,        &btlgrp_mri_mri_18_03,        &btlgrp_mri_mri_19_01,
            &btlgrp_mri_mri_19_02,        &btlgrp_mri_mri_19_04,        &btlgrp_mri_mri_20_02,
            &btlgrp_mri_mri_20_03,        &btlgrp_muj_muj_01_01,        &btlgrp_muj_muj_05_01,
            &btlgrp_muj_muj_05_02,        &btlgrp_muj_muj_00_01,        &btlgrp_muj_muj_02_01_off_01,
            &btlgrp_muj_muj_02_01_off_02, &btlgrp_muj_muj_02_01_off_03, &btlgrp_muj_muj_02_01_off_04,
            &btlgrp_muj_muj_02_04_off_01, &btlgrp_muj_muj_02_04_off_02, &btlgrp_muj_muj_02_04_off_03,
            &btlgrp_muj_muj_02_07_off_01, &btlgrp_muj_muj_02_07_off_02, &btlgrp_muj_muj_02_07_off_03,
            &btlgrp_muj_muj_03_01_off_01, &btlgrp_muj_muj_03_01_off_02, &btlgrp_muj_muj_03_01_off_03,
            &btlgrp_muj_muj_03_02_off_01, &btlgrp_muj_muj_03_02_off_02, &btlgrp_muj_muj_03_02_off_03,
            &btlgrp_muj_muj_03_04_off_01, &btlgrp_muj_muj_03_04_off_02, &btlgrp_muj_muj_03_04_off_03,
            &btlgrp_pik_pik_06_01_off_1,  &btlgrp_pik_pik_06_02_off_1,  &btlgrp_pik_pik_06_03_off_1,
            &btlgrp_pik_pik_06_04_off_1,  &btlgrp_pik_pik_06_05_off_1,  &btlgrp_pik_pik_06_06_off_1,
            &btlgrp_pik_pik_06_07_off_1,  &btlgrp_pik_pik_06_08_off_1,  &btlgrp_pik_pik_06_09_off_1,
            &btlgrp_tik_tik_01_02_off_1,  &btlgrp_tik_tik_04_01_off_1,  &btlgrp_tik_tik_04_01_on_1,
            &btlgrp_tik_tik_04_01_on_2,   &btlgrp_tik_tik_04_02_off_1,  &btlgrp_tik_tik_04_02_on_1,
            &btlgrp_tik_tik_04_02_on_2,   &btlgrp_tik_tik_04_03_off_1,  &btlgrp_tik_tik_04_03_off_2,
            &btlgrp_tik_tik_04_03_off_3,  &btlgrp_tik_tik_04_04_off_1,  &btlgrp_tik_tik_04_04_off_2,
            &btlgrp_tik_tik_04_04_off_3,  &btlgrp_tik_tik_07_01_off_1,  &btlgrp_tik_tik_07_01_off_2,
            &btlgrp_tik_tik_07_02_off_1,  &btlgrp_tik_tik_07_02_off_2,  &btlgrp_tik_tik_07_03_off_1,
            &btlgrp_tik_tik_07_03_off_2,  &btlgrp_tik_tik_07_03_off_3,  &btlgrp_tik_tik_07_04_off_1,
            &btlgrp_tik_tik_07_04_off_2,  &btlgrp_tik_tik_07_04_off_3,  &btlgrp_tik_tik_15_01_off_1,
            &btlgrp_tik_tik_15_02_off_1,  &btlgrp_tik_tik_15_03_off_1,  &btlgrp_tik_tik_15_04_off_1,
            &btlgrp_tik_tik_15_04_off_2,  &btlgrp_tik_tik_15_04_off_3,  &btlgrp_tik_tik_15_05_off_1,
            &btlgrp_tik_tik_15_05_off_2,  &btlgrp_tik_tik_15_05_off_3,  &btlgrp_tik_tik_18_01_off_1,
            &btlgrp_tik_tik_18_01_off_2,  &btlgrp_tik_tik_18_02_off_1,  &btlgrp_tik_tik_18_02_off_2,
            &btlgrp_tik_tik_18_03_off_1,  &btlgrp_tik_tik_18_03_off_2,  &btlgrp_tik_tik_18_03_off_3,
            &btlgrp_tik_tik_18_04_off_1,  &btlgrp_tik_tik_18_04_off_2,  &btlgrp_tik_tik_18_04_off_3,
            &btlgrp_tik_tik_20_01_off_1,  &btlgrp_tik_tik_20_02_off_1,  &btlgrp_tik_tik_20_03_off_1,
            &btlgrp_tik_tik_20_04_off_1,  &btlgrp_tik_tik_20_05_off_1,  &btlgrp_tik_tik_20_06_off_1,
            &btlgrp_tou_tou_rank_19,      &btlgrp_tou_tou_rank_18,      &btlgrp_tou_tou_rank_17,
            &btlgrp_tou_tou_rank_16,      &btlgrp_tou_tou_rank_15,      &btlgrp_tou_tou_rank_14,
            &btlgrp_tou_tou_rank_13,      &btlgrp_tou_tou_rank_12,      &btlgrp_tou_tou_rank_11,
            &btlgrp_tou_tou_rank_9,       &btlgrp_tou_tou_rank_8,       &btlgrp_tou_tou_rank_7,
            &btlgrp_tou_tou_rank_6,       &btlgrp_tou_tou_rank_5,       &btlgrp_tou_tou_rank_4,
            &btlgrp_tou_tou_rank_3,       &btlgrp_tou_tou_rank_2,       &btlgrp_tou_tou_rank_1,
            &btlgrp_tou_tou_add_1,        &btlgrp_tou_tou_add_2,        &btlgrp_win_win_00_01_off_1,
            &btlgrp_win_win_00_01_on_1,   &btlgrp_win_win_00_01_on_2,   &btlgrp_win_win_00_01_on_3,
            &btlgrp_win_win_00_02_off_1,  &btlgrp_win_win_00_02_on_1,   &btlgrp_win_win_00_02_on_2,
            &btlgrp_win_win_00_03_off_1,  &btlgrp_win_win_00_03_on_1,   &btlgrp_win_win_00_03_on_2,
            &btlgrp_win_win_02_01_off_1,  &btlgrp_win_win_02_01_on_1,   &btlgrp_win_win_02_01_on_2,
            &btlgrp_win_win_02_01_on_3,   &btlgrp_win_win_02_02_off_1,  &btlgrp_win_win_02_02_on_1,
            &btlgrp_win_win_02_02_on_2,   &btlgrp_win_win_02_02_on_3,   &btlgrp_win_win_03_01_off_1,
            &btlgrp_win_win_03_01_on_1,   &btlgrp_win_win_03_01_on_2,   &btlgrp_win_win_03_01_on_3,
            &btlgrp_win_win_03_01_on_4,
        };

        struct BattleGroupIndexRange
        {
            int start;
            int end;
        };

        static_assert(sizeof(BattleGroupIndexRange) == 0x8);

        inline constexpr BattleGroupIndexRange kBtlGrpRange_aji_aji {0, 16};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_bom_bom {17, 26};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_dou_dou {27, 47};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_eki_eki {48, 62};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_gon_gon {63, 80};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_gra_gra {81, 108};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_hei_hei {109, 142};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_jin_jin {143, 165};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_jon_jon {166, 315};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_las_las {316, 368};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_moo_moo {369, 392};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_mri_mri {393, 408};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_muj_muj {409, 431};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_pik_pik {432, 440};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_tik_tik {441, 488};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_tou_tou {489, 508};
        inline constexpr BattleGroupIndexRange kBtlGrpRange_win_win {509, 531};

        struct MovementSoundControl
        {
            const char *sound_name_left;
            const char *sound_name_right;
            int16_t unk_08;
            int16_t unk_0a;
            int16_t initial_wait_timer;
            int16_t repeat_wait_timer_left;
            int16_t repeat_wait_timer_right;
            int16_t current_timer;
            int16_t sound_to_play_next;
            int16_t pad_16;
            int32_t psnd_sfx_idx;
        } __attribute__((__packed__));

        static_assert(sizeof(MovementSoundControl) == 0x1c);

        struct MovementSoundData
        {
            const char *jump_sound_left;
            const char *jump_sound_right;
            const char *run_sound_left;
            const char *run_sound_right;
            int16_t run_initial_wait_timer;
            int16_t run_repeat_wait_timer_left;
            int16_t run_repeat_wait_timer_right;
            int16_t pad_16;
            const char *walk_sound_left;
            const char *walk_sound_right;
            int16_t walk_initial_wait_timer;
            int16_t walk_repeat_wait_timer_left;
            int16_t walk_repeat_wait_timer_right;
            int16_t pad_26;
            const char *dive_sound_left;
            const char *dive_sound_right;
            int16_t dive_initial_wait_timer;
            int16_t dive_repeat_wait_timer_left;
            int16_t dive_repeat_wait_timer_right;
            int16_t pad_36;
        } __attribute__((__packed__));

        static_assert(sizeof(MovementSoundData) == 0x38);

        // See jda_ttyd_utils Github (BattleWorkUnit.0x148 onward) for more details.
        struct MovementParams
        {
            gc::vec3 move_start_position;
            gc::vec3 move_current_position;
            gc::vec3 move_target_position;
            int32_t move_frame_count;
            float move_speed_xz;
            float fall_accel;
            float move_speed_y;
            float move_angle_xz;
            float move_dist_xz;
            float jump_offset_height;
            int8_t move_direction;
            int8_t face_direction;
            int8_t pad_42[2];
            MovementSoundControl sound_control_work;
            // Jump parameters used in btlevtcmd_MarioJumpPosition.
            gc::vec3 jump_move_per_frame;
            float jump_angle_perturbance_angle;
            float jump_angle_perturbance_angle_delta;
            float jump_current_position_y; // ?
            int8_t unk_78[4];
            MovementSoundData sound_data_table;
        } __attribute__((__packed__));

        static_assert(sizeof(MovementParams) == 0xb4);

        struct BadgesEquipped
        {
            int8_t close_call;
            int8_t pretty_lucky;
            int8_t lucky_day;
            int8_t unk_03; // Probably unused
            int8_t power_plus;
            int8_t p_up_d_down;
            int8_t all_or_nothing;
            int8_t mega_rush;

            int8_t power_rush;
            int8_t p_down_d_up;
            int8_t double_pain;
            int8_t last_stand;
            int8_t defend_plus;
            int8_t damage_dodge;
            int8_t happy_heart;
            int8_t happy_flower;

            int8_t return_postage;
            int8_t hp_plus;
            int8_t fp_plus;
            int8_t double_dip;
            int8_t triple_dip;
            int8_t flower_saver;
            int8_t feeling_fine;
            int8_t zap_tap;

            int8_t pity_flower;
            int8_t hp_drain;
            int8_t fp_drain;
            int8_t refund;
            int8_t charge;
            int8_t super_charge;
            int8_t unused_defend_command_badge;
            int8_t jumpman;

            int8_t hammerman;
            int8_t ice_power;
            int8_t spike_shield;
            int8_t super_appeal;
            int8_t lucky_start;
            int8_t simplifier;
            int8_t unsimplifier;
            int8_t auto_command_badge;
        } __attribute__((__packed__));

        static_assert(sizeof(BadgesEquipped) == 0x28);

        struct BattleWorkUnit;
        struct BattleWorkUnitPart;

        struct BattleWorkUnitPart
        {
            BattleWorkUnitPart *next_part;
            battle_database_common::BattleUnitKindPart *kind_part_params;
            const char *part_name;
            gc::vec3 home_position;
            gc::vec3 position;
            gc::vec3 position_offset;
            gc::vec3 display_offset;
            gc::vec3 base_rotation;
            gc::vec3 rotation;
            gc::vec3 rotation_offset;
            gc::vec3 scale;
            gc::vec3 base_scale;
            float unk_078; // some sort of scale

            MovementParams movement_params;
            uint32_t parts_work[16];    // length unknown; at least 3
            gc::vec3 hit_base_position; // ?
            gc::vec3 hit_offset;
            gc::vec3 hit_cursor_base_position; // ?
            gc::vec3 hit_cursor_offset;
            int16_t addl_target_offset_x;
            int16_t unk_1a2;
            int16_t unk_1a4;
            int8_t unk_1a6[6];

            uint32_t part_attribute_flags;
            uint32_t counter_attribute_flags;
            int8_t *defense;
            int8_t *defense_attr;

            void *pose_table;
            int32_t anim_pose_type;
            char anim_pose_name[64]; // length unknown
            uint32_t unk_204;        // anim-related flags
            float unk_208;
            float anim_motion_speed;
            void *unk_210; // some sort of anim-related callback fn
            int8_t unk_214;
            int8_t unk_215; // flags related to anim
            int8_t pad_216[2];
            gc::color4 base_blur_color;
            uint32_t blur_flags;
            gc::color4 blur_colors[2];
            int8_t blur_params[0x44 * 10];
            int8_t pose_sound_params[0x18];

            int32_t unk_4e8;
            BattleWorkUnit *unit_owner;
            gc::color4 color;
            gc::color4 blended_color; // with base alpha, invis status, etc.
            float unk_4f8;            // some sort of z-offset?  Used in btlDispMain
            int8_t unk_4fc;           // axis order to apply rotations?
            int8_t unk_4fd[3];
        } __attribute__((__packed__));

        static_assert(sizeof(BattleWorkUnitPart) == 0x500);

        struct BattleWorkUnit
        {
            int32_t unit_id;
            int32_t true_kind;    // BattleUnitType::e
            int32_t current_kind; // BattleUnitType::e
            int8_t alliance;
            int8_t level;
            int8_t group_index;
            int8_t pad_00f;
            battle_database_common::BattleUnitKind *unit_kind_params;
            BattleWorkUnitPart *parts;
            void *data_table;
            uint32_t unit_flags;
            int8_t move_state; // ?
            int8_t max_move_count;
            int8_t moves_remaining;
            int8_t max_moves_this_turn;
            int8_t active_turns; // ? Used to make Slow status skip turns
            int8_t swallow_chance;
            int8_t swallow_attributes;
            int8_t pad_027;
            int32_t move_priority;
            int32_t attack_phase;

            gc::vec3 home_position;
            gc::vec3 position;
            gc::vec3 offset_position;
            gc::vec3 display_offset;
            gc::vec3 base_rotation;
            gc::vec3 rotation;
            gc::vec3 rotation_offset;
            gc::vec3 center_offset;
            gc::vec3 scale;
            gc::vec3 base_scale;
            gc::vec3 toge_offset; // Used for speech bubble spike?
            gc::vec3 held_item_base_offset;
            gc::vec3 possession_item_offset;
            int16_t width;
            int16_t height;
            int16_t status_icon_offset[2];
            int16_t hp_gauge_offset[2];
            gc::vec3 cut_base_offset;
            float cut_width;
            float cut_height;
            gc::vec3 binta_hit_offset;
            gc::vec3 kiss_hit_offset;

            uint32_t attribute_flags;
            int16_t max_hp;
            int16_t base_max_hp; // no badges
            int16_t current_hp;
            int16_t max_fp;
            int16_t base_max_fp; // no badges
            int16_t current_fp;
            float unk_scale;

            int8_t sleep_turns;
            int8_t unk_119; // something to do with waking up from sleep?
            int8_t stop_turns;
            int8_t dizzy_turns;
            int8_t poison_turns;
            int8_t poison_strength;
            int8_t confusion_turns;
            int8_t electric_turns;

            int8_t dodgy_turns;
            int8_t burn_turns;
            int8_t freeze_turns;
            int8_t size_change_turns;
            int8_t size_change_strength;
            int8_t attack_change_turns;
            int8_t attack_change_strength;
            int8_t defense_change_turns;

            int8_t defense_change_strength;
            int8_t charge_strength;
            int8_t allergic_turns;
            int8_t flipped_turns; // for shelled enemies, etc.
            int8_t invisible_turns;
            int8_t payback_turns;
            int8_t hold_fast_turns;
            int8_t fast_turns;

            int8_t slow_turns;
            int8_t hp_regen_turns;
            int8_t hp_regen_strength;
            int8_t fp_regen_turns;
            int8_t fp_regen_strength;
            int8_t knocked_out; // OHKO strength?
            int8_t unk_136[2];

            uint32_t status_flags;
            int8_t unk_13c[4];
            uint32_t protect_unit_idx; // related to Shell Shield?
            battle_database_common::StatusVulnerability *status_vulnerability;
            MovementParams movement_params;
            int8_t hp_gauge_params[0x1c];
            uint32_t unit_work[16];

            // Parameters used during a single "act" (attack).
            BattleWorkUnitPart *currently_targeted_part;
            uint32_t unk_25c;               // unreferenced?
            int16_t hp_damaging_hits_dealt; // used for Power Bounce decay, etc.
            int16_t pad_262;
            int32_t total_hp_damage_taken;
            int32_t total_fp_damage_taken;
            int32_t total_fp_lost;
            int8_t hp_damage_taken;
            int8_t fp_damage_taken;
            int8_t fp_lost;
            int8_t hits_taken;
            uint32_t damage_pattern;
            uint32_t damage_code;
            uint32_t token_flags;
            int32_t hits_dealt_this_attack;         // Used for Fire Drive decay, etc.
            int32_t total_damage_dealt_this_attack; // Used for Drain badges

            int32_t init_evt_tid;
            void *wait_evt_code;
            int32_t wait_evt_tid;
            void *unison_phase_evt_code;
            void *phase_evt_code;
            int32_t phase_evt_tid;
            void *attack_evt_code;
            void *confuse_evt_code;
            int32_t attack_evt_tid;
            uint32_t unk_2ac; // battle menu state?
            void *damage_evt_code;
            int32_t damage_evt_tid;
            void *entry_evt_code;
            int32_t entry_evt_tid;
            void *ceiling_fall_evt_code;
            int32_t ceiling_fall_evt_tid;
            void *unknown_evt_code;
            int32_t unknown_evt_tid;
            void *hit_evt_code; // ? BattleRunHitEventDirect
            uint32_t talk_body_part_id;
            const char *talk_pose_name;
            const char *stay_pose_name;

            BadgesEquipped badges_equipped;
            int32_t held_item;
            battle_database_common::ItemDropData *held_item_table;

            int8_t misc_310[0x824];
        } __attribute__((__packed__));

        static_assert(sizeof(BattleWorkUnit) == 0xb34);

        struct BattleStatRelValues
        {
            RelId rel;
            int16_t base_hp;
            int8_t level;
        } __attribute__((__packed__));

        extern BattleStatRelValues g_battleStats[];
        extern const int g_battleStatsCount;
        const BattleStatRelValues *GetBattleStats(RelId rel);
        extern void ShuffleBattleStats();

        int BtlUnit_GetUnitId(void* unit_ptr);
        BattleWorkUnit *BtlUnit_Entry(battle_database_common::BattleUnitSetup *unit_setup);
    }
} // namespace ttyd::battle_unit