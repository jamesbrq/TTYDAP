#pragma once

#include <cstdint>

#include "evtmgr.h"

namespace ttyd::evt_nannpc
{
	extern "C"
	{
		EVT_DECLARE_USER_FUNC(evt_nannpc_init, 5)
		EVT_DECLARE_USER_FUNC(evt_nannpc_set_subfunc, 2)
	}
}
