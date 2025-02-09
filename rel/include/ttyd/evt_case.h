#pragma once

#include <cstdint>

#include "evtmgr.h"

namespace ttyd::evt_case {

	extern "C"
	{
		EVT_DECLARE_USER_FUNC(evt_run_case_evt, 6)
	}

}