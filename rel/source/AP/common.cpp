#include <ttyd/dvdmgr.h>
#include <ttyd/evtmgr_cmd.h>
#include <ttyd/memory.h>

using namespace ttyd;

bool isPointerValid(void* ptr) {
	const uintptr_t rawPtr = (uintptr_t)ptr;
	return rawPtr >= 0x80000000 && rawPtr <= 0x817FFFFF;
}

void* readFileFromDvd(const char* path) {
	dvdmgr::DvdMgrFile* entry = dvdmgr::DVDMgrOpen(path, 2, 0);
	uint32_t length = *(uint32_t *)((uintptr_t)entry + 0x74);
	length = (length + 31) & ~31; // round up 32b
	void* buf = memory::__memAlloc(0, length);
	dvdmgr::DVDMgrRead(entry, buf, length, 0);
	dvdmgr::DVDMgrClose(entry);
	return buf;
}

// the evt system only reads args it needs, so we can just
// use the original arg count and let it skip over the extra ones
void replaceEvtInstr(int32_t* instr, uint16_t opc) {
	*(uint16_t *)((uintptr_t)instr + sizeof(uint16_t)) = opc;
}

// replaces the instruction and its args with nops
void writeEvtNop(int32_t* instr) {
	replaceEvtInstr(instr, 0x0072);
}

void writeEvtReturn(int32_t* instr) {
	replaceEvtInstr(instr, 0x0002);
}

// the following two require the replaced instruction to have at least 1 arg
void writeEvtGoto(int32_t* instr, const int32_t* destination) {
	replaceEvtInstr(instr, 0x0004);
	instr[1] = (uintptr_t)destination; // arg 1
}

void writeEvtBranch(int32_t* instr, const int32_t* destination, bool async) {
	replaceEvtInstr(instr, async ? 0x5C : 0x5E); // calls{a,s}
	instr[1] = (uintptr_t)destination; // arg 1
}