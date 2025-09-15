#pragma once

#include "ttyd/win_log.h"
#include "ttyd/win_main.h"

#include <cstdint>
#include <cstring>

namespace mod::custom_warp
{
    enum class WarpType : int32_t
    {
        RETURN_PIPE = 0,
        FAST_TRAVEL,
    };

    struct FastTravelPair
    {
        const char *map_name;
        const char *bero;

        static FastTravelPair getFastTravelPair()
        {
            const char *map_prefix = ttyd::win_log::mapMarkers[ttyd::win_main::main_winGetPtr()->mapCursorIdx].map_prefix;

            if (strcmp(map_prefix, "tik") == 0)
                return {"tik_01", "dokan_2"};

            if (strcmp(map_prefix, "hei") == 0)
                return {"hei_00", "dokan_2"};

            if (strcmp(map_prefix, "nok") == 0)
                return {"nok_00", "w_bero"};

            if (strcmp(map_prefix, "gon") == 0)
                return {"gon_00", "w_bero"};

            if (strcmp(map_prefix, "win") == 0)
                return {"win_06", "dokan1"}; // not a typo

            if (strcmp(map_prefix, "mri") == 0)
                return {"mri_00", "w_bero"};

            if (strcmp(map_prefix, "hou") == 0)
                return {"win_04", "w_bero"};

            if (strcmp(map_prefix, "tou") == 0)
                return {"tou_01", nullptr}; // triggers the blimp entrance by default

            if (strcmp(map_prefix, "usu") == 0)
                return {"usu_00", "dokan_1"};

            if (strcmp(map_prefix, "gra") == 0)
                return {"gra_00", "w_bero"};

            if (strcmp(map_prefix, "jin") == 0)
                return {"gra_06", "sw_bero"};

            if (strcmp(map_prefix, "muj") == 0)
                return {"muj_00", "e_bero"};

            if (strcmp(map_prefix, "dou") == 0)
                return {"muj_05", "w_bero"};

            if (strcmp(map_prefix, "eki") == 0)
                return {"hom_00", "n_bero_1"};

            if (strcmp(map_prefix, "rsh") == 0)
                return {"rsh_01_a", "s_bero"};

            if (strcmp(map_prefix, "pik") == 0)
                return {"pik_00", "n_bero"};

            if (strcmp(map_prefix, "sin") == 0)
                return {"pik_02", "s_bero"};

            if (strcmp(map_prefix, "bom") == 0)
                return {"bom_02", "w_bero"};

            if (strcmp(map_prefix, "moo") == 0)
                return {"moo_00", nullptr}; // first time landing on the moon also uses null

            if (strcmp(map_prefix, "aji") == 0)
                return {"aji_00", "w_bero"};

            if (strcmp(map_prefix, "las") == 0)
                return {"las_00", "w_bero"};

            if (strcmp(map_prefix, "las_09") == 0)
                return {"las_09", "w_bero"};

            return {"gor_01", "s_bero"};
        }
    };

} // namespace mod::custom_warp
