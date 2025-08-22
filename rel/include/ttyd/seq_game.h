#pragma once

#include "seqdrv.h"

namespace ttyd::seq_game
{
    extern "C"
    {
        void seq_gameInit(seqdrv::SeqInfo *info);
        // seq_gameMain
        // seq_gameExit
    }
} // namespace ttyd::seq_game
