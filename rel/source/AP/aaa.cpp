#include "evt_cmd.h"
#include <AP/aaa.h>

extern int32_t evt_prologue2[];
extern int32_t aaa_epilogue_evt2[];
extern int32_t aaa_00_init_evt[];

void ApplyAaaPatches(OSModuleInfo* module_info)
{
	evt_prologue2[188] = GSW(1700);
	evt_prologue2[189] = 2;

	aaa_epilogue_evt2[485] = GSW(1708);
	aaa_epilogue_evt2[486] = 19;

	aaa_00_init_evt[49] = GSW(1708);
	aaa_00_init_evt[50] = 18;
}