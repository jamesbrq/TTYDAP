#pragma once

#include <cstdint>

#include "evtmgr.h"

namespace ttyd::evt_hit
{
	extern "C"
	{
		EVT_DECLARE_USER_FUNC(evt_hitobj_onoff, 3)
		EVT_DECLARE_USER_FUNC(evt_hit_bind_mapobj, 2)
		EVT_DECLARE_USER_FUNC(evt_hitobj_attr_onoff, 4)
	}
}
