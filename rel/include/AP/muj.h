#pragma once

#include <gc/OSLink.h>
#include <ttyd/evtmgr.h>

using namespace gc::OSLink;
extern "C" {
	EVT_DECLARE_USER_FUNC(koburon_get_encount_info, 1)
}

void ApplyMujPatches(OSModuleInfo* module_info);
