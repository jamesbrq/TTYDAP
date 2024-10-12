#include <ttyd/evt_mobj.h>
#include <ttyd/item_data.h>
#include <ttyd/evt_item.h>
#include "common_types.h"
#include "patch.h"

#include <gc/OSLink.h>
#include "evt_cmd.h"

using namespace ttyd;
using namespace mod;
using namespace ttyd::item_data;

using gc::OSLink::OSModuleInfo;

const int32_t test = ItemType::ALL_OR_NOTHING;
extern int32_t gor_gor_01_gor_01_init_evt[];

EVT_BEGIN(gor_01_init)
	USER_FUNC(evt_item::evt_item_entry, PTR("item_00"), ItemType::ALL_OR_NOTHING, 490, 0, 245, 16, GSWF(5570), 0)
EVT_END()

void DoPatches(OSModuleInfo* new_module)
{
	switch (new_module->id)
	{
	case ModuleId::GOR:
		patch::writePatch(&gor_gor_01_gor_01_init_evt[224], gor_01_init, sizeof(gor_01_init));
	}
}
