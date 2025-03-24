#include "evt_cmd.h"
#include <AP/end.h>

extern int32_t evt_carry_up_bridge[];
extern int32_t init_powan_toron_konari_papa_gurume[];
extern int32_t talk_ekiin1[];
extern int32_t init_ekiin2[];
extern int32_t talk_ekiin2[];
extern int32_t hom_evt_lock[];
extern int32_t evt_train_door[];
extern int32_t hom_evt_train[];
extern int32_t evt_hom_touchaku[];
extern int32_t hom_00_init_evt[];
extern int32_t hom_10_evt_resha_start_hom_00[];


void ApplyHomPatches(OSModuleInfo* module_info)
{
	evt_carry_up_bridge[82] = GSW(1720);
	evt_carry_up_bridge[83] = 2;

	init_powan_toron_konari_papa_gurume[1] = GSW(1720);
	init_powan_toron_konari_papa_gurume[3] = 8;
	init_powan_toron_konari_papa_gurume[12] = GSW(1720);
	init_powan_toron_konari_papa_gurume[13] = 1;

	talk_ekiin1[1] = GSW(1720);
	talk_ekiin1[3] = 2;
	talk_ekiin1[7] = 8;
	talk_ekiin1[15] = 9;

	init_ekiin2[1] = GSW(1720);
	init_ekiin2[3] = 8;

	talk_ekiin2[1] = GSW(1720);
	talk_ekiin2[3] = 8;
	talk_ekiin2[11] = 9;

	hom_evt_lock[6] = GSW(1720);
	hom_evt_lock[7] = 3;

	evt_train_door[1] = GSW(1720);
	evt_train_door[3] = 8;

	hom_evt_train[252] = GSW(1706);
	hom_evt_train[253] = 30;

	evt_hom_touchaku[452] = GSW(1720);
	evt_hom_touchaku[453] = 1;

	hom_00_init_evt[75] = GSW(1706);
	hom_00_init_evt[76] = 43;
	hom_00_init_evt[165] = GSW(1720);
	hom_00_init_evt[166] = 1;
	hom_00_init_evt[180] = GSW(1706);
	hom_00_init_evt[181] = 44;
	hom_00_init_evt[198] = GSW(1720);
	hom_00_init_evt[199] = 3;
	hom_00_init_evt[218] = GSW(1720);
	hom_00_init_evt[219] = 8;
	hom_00_init_evt[227] = GSW(1720);
	hom_00_init_evt[228] = 8;

	hom_10_evt_resha_start_hom_00[49] = GSW(1706);
	hom_10_evt_resha_start_hom_00[50] = 43;
}