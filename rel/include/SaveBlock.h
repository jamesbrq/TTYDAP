#pragma once
#include <ttyd/evtmgr.h>

namespace mod::save_block
{
    void Init();
    void ResetMap();
    constexpr int SetItemSaveActive_parameter_count = 1;
    int32_t SetItemSaveActive(ttyd::evtmgr::EvtEntry *evt, bool firstCall);
}
