#include "evt_cmd.h"
#include <AP/rel_patch_definitions.h>

extern int32_t unk_evt_end_00004654[];
extern int32_t unk_evt_end_00004718[];


void ApplyEndPatches()
{
	unk_evt_end_00004654[17] = GSW(1708);
	unk_evt_end_00004654[18] = 20;

	unk_evt_end_00004718[59] = GSW(1708);
	unk_evt_end_00004718[60] = 20;
}