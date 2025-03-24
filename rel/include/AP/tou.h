#pragma once

#include <gc/OSLink.h>
#include <ttyd/evtmgr.h>

using namespace gc::OSLink;


extern "C" {
	EVT_DECLARE_USER_FUNC(evt_tou_get_ranking, 1)
}

void ApplyTouPatches(OSModuleInfo* module_info);
