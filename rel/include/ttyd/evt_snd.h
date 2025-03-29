#pragma once

#include <cstdint>

#include "evtmgr.h"

namespace ttyd::evt_snd
{
    extern "C"
    {
        // evt_snd_sfx_all_stop
        // evt_snd_set_rev_mode
        EVT_DECLARE_USER_FUNC(evt_snd_env_lpf, 2)
        // evt_snd_envoff_f
        EVT_DECLARE_USER_FUNC(evt_snd_envoff, 1)
        // evt_snd_envon_f
        EVT_DECLARE_USER_FUNC(evt_snd_envon, 2)
        // evt_snd_sfx_dist
        // evt_snd_sfx_pos
        // evt_snd_sfx_vol
        // evt_snd_sfx_pit
        // evt_snd_sfxchk
        // evt_snd_sfxoff
        // evt_snd_sfxon_3d_ex
        EVT_DECLARE_USER_FUNC(evt_snd_sfxon_3d, 5)
        // evt_snd_sfxon_
        // evt_snd_sfxon__
        EVT_DECLARE_USER_FUNC(evt_snd_sfxon, 2)
        EVT_DECLARE_USER_FUNC(evt_snd_bgm_scope, 2)
        // evt_snd_bgm_freq
        // evt_snd_bgmoff_f
        EVT_DECLARE_USER_FUNC(evt_snd_bgmoff, 1)
        // unk_JP_US_EU_37_801524c8
        EVT_DECLARE_USER_FUNC(evt_snd_bgmon_f, 3)
        EVT_DECLARE_USER_FUNC(evt_snd_bgmon, 2)
    }
} // namespace ttyd::evt_snd
