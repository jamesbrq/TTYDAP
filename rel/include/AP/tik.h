#pragma once

#include <gc/OSLink.h>
#include <ttyd/evtmgr.h>

using namespace gc::OSLink;

EVT_DECLARE_USER_FUNC(checkChapterRequirements, 1)

void ApplyTikPatches(OSModuleInfo* module_info);
