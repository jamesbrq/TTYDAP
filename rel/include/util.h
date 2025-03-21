#pragma once

#include <gc/os.h>

#include <cstdint>

#define MOD_ARRAYSIZE(x) \
	(sizeof((x)) / sizeof((x)[0]))

#define MOD_CONCAT_IMPL(s1, s2) s1##s2
#define MOD_CONCAT(s1, s2) MOD_CONCAT_IMPL(s1, s2)
#define MOD_ANONYMOUS(str) MOD_CONCAT(str, __LINE__)

#define MOD_ASSERT(x) \
	do \
	{ \
		if (!(x)) \
		{ \
			gc::os::OSPanic(__FILE__, __LINE__, "Mod assertion failed: %s\n", #x); \
		} \
	} \
	while(false)

namespace mod::util {

	// Used to specifiy what region of GameCube memory a pointer points to.
	enum PointerVerificationType
	{
		PTR_INVALID = 0, // Does not point to valid GameCube memory
		PTR_CACHED,      // 0x80000000 through 0x817FFFFF
		PTR_UNCACHED,    // 0xC0000000 through 0xC17FFFFF
	};

	inline int64_t GetTbRate()
	{
		return *(int32_t*)0x800000f8 / 4;
	}

	bool checkButtonCombo(uint32_t combo);
	bool checkButtonsEveryFrame(uint32_t buttons);
	bool checkButtonComboEveryFrame(uint32_t combo);

	uint32_t ptrIsValid(void* ptr);
	float intToFloat(int32_t value);
	float getTextMultilineIncrement(const char* text, float scale, uint32_t additionalLines);

	template<typename Ptr>
	uint32_t ptrIsValid(Ptr ptr)
	{
		return ptrIsValid(reinterpret_cast<void*>(ptr));
	}

}