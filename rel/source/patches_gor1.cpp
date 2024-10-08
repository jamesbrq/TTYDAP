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

extern const int32_t gor_gor_01_gor_01_init_evt[];

EVT_BEGIN(gor_01_init)
USER_FUNC(evt_item::evt_item_entry, PTR("item_00"), ItemType::L_EMBLEM, 490, 0, 245, 16, GSWF(5570), 0)
RETURN()
EVT_END()

void DoPatches(OSModuleInfo* new_module) {
	switch (new_module->id) {
	case ModuleId::GOR:
		patch::writePatch(reinterpret_cast<void*>(gor_gor_01_gor_01_init_evt[276]), gor_01_init, sizeof(gor_01_init));
	}
}
