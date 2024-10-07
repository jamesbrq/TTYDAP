#include <ttyd/evt_bero.h>
#include <ttyd/evt_mobj.h>
#include <ttyd/item_data.h>
#include <ttyd/evt_item.h>
#include "common_types.h"
#include "patch.h"

#include <ttyd/OSLink.h>

#include "evt_cmd.h"

using namespace ttyd;
using namespace mod;
using namespace ttyd::item_data;

using ::ttyd::oslink::OSModuleInfo;

extern const int32_t gor_01_init_evt = 0x805F6F20;

EVT_BEGIN(gor_01_init)
USER_FUNC(evt_item::evt_item_entry, PTR("item_00"), ItemType::L_EMBLEM, 490, 0, 245, 16, GSWF(5570), 0)
EVT_END()

void DoPatches(OSModuleInfo* new_module) {
	switch (new_module->id) {
	case ModuleId::GOR:
		patch::writePatch(reinterpret_cast<void*>(gor_01_init_evt),
			gor_01_init, sizeof(gor_01_init));
	}
}
