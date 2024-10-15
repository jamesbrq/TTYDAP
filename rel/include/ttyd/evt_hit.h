#pragma once

#include <cstdint>

#include "evtmgr.h"

namespace ttyd::evt_hit
{
	extern "C"
	{
		EVT_DECLARE_USER_FUNC(evt_hitobj_onoff, 3)
	}
}
