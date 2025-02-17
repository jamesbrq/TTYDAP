#include <AP/main.h>
#include "patch.h"

using namespace mod;

extern int32_t statusWinDisp[];
extern int32_t winRootMain[];
extern int32_t compare_func3_r[];
extern int32_t compare_func3[];


void ApplyMainAssemblyPatches()
{
	statusWinDisp[425] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	statusWinDisp[487] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))

	winRootMain[40] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))
	winRootMain[82] = 0x386006A4; // li r3, 0x6A4 (GSW(1700))

	compare_func3_r[29] = 0x386006B3; // li r3, 0x6B3 (GSW(1715))
	compare_func3_r[32] = 0x2C030005; // cmpwi r3, 0x5
	compare_func3_r[51] = 0x386006A4; // li r3, 0x6B3 (GSW(1715))
	compare_func3_r[54] = 0x2C030005; // cmpwi r3, 0x5

	compare_func3[109] = 0x386006A4; // li r3, 0x6B3 (GSW(1715))
	compare_func3[32] = 0x2C030005; // cmpwi r3, 0x5
}