#pragma once

#include <cstdint>

namespace ttyd::statuswindow
{

    extern "C"
    {
        // .text
        // statusPartyHPBlink
        // statusMarioHPBlink
        // statusFPBlink
        // N_statusClearBlink
        // statusAPBlink
        // statusGetApPos
        // statusWinForceUpdateCoin
        void statusWinForceUpdate();
        // statusWinCheckUpdate
        // statusWinCheck
        // statusWinDispOff
        // statusWinDispOn
        // statusWinForceOff
        // statusWinForceCloseClear
        // statusWinForceClose
        // statusWinForceOpen
        // statusWinClose
        // statusWinOpen
        // valueUpdate
        // valueCheck
        // statusGetValue
        void statusWinDisp();
        void gaugeDisp(float x, float y, int32_t star_power);
        // statusWinMain
        // statusWinReInit
        // statusWinInit

        // .data
        extern uint16_t gauge_back[8];
        extern uint16_t gauge_wakka[16];
        extern void *g_StatusWindowWork;
    }

} // namespace ttyd::statuswindow