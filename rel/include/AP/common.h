#pragma once

#include <ttyd/evtmgr.h>
#include <ttyd/npcdrv.h>

#include <cstdint>

extern "C" {

bool isPointerValid(void* ptr);

}
void* readFileFromDvd(const char* path);

constexpr uint32_t assembleEvtNop(uint16_t nopCount) {
	return 0x00000072 | ((nopCount - 1) << 16);
}

void writeEvtNop(int32_t* instr);
void writeEvtReturn(int32_t* instr);
void writeEvtGoto(int32_t* instr, const int32_t* destination);
void writeEvtBranch(int32_t* instr, const int32_t* destination, bool async = false);