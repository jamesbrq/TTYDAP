#include "util.h"
#include "gc/pad.h"
#include "ttyd/system.h"
#include "ttyd/fontmgr.h"

#include <cstdint>
#include <cstring>

// TTYD doesn't use this so we reimplement it.
// TODO: Not the greatest solution in the long run
char* strstr(const char* container, const char* pattern)
{
	int pattern_len = strlen(pattern);
	for (const char* start = container; *start; ++start)
	{
		bool mismatch = false;
		for (int i = 0; i < pattern_len; ++i)
		{
			if (!container[i] || start[i] != pattern[i])
			{
				mismatch = true;
				break;
			}
		}
		if (!mismatch)
		{
			// Cast away the const because C demands it
			return (char*)start;
		}
	}
	return nullptr;
}

namespace mod::util
{

	bool checkButtonCombo(uint32_t combo)
	{
		if ((ttyd::system::keyGetButton(gc::pad::PadId::CONTROLLER_ONE) & combo) != combo)
		{
			return false;
		}

		return ttyd::system::keyGetButtonTrg(gc::pad::PadId::CONTROLLER_ONE) & combo;
	}

	bool checkButtonsEveryFrame(uint32_t buttons)
	{
		return ttyd::system::keyGetButton(gc::pad::PadId::CONTROLLER_ONE) & buttons;
	}

	bool checkButtonComboEveryFrame(uint32_t combo)
	{
		return (ttyd::system::keyGetButton(gc::pad::PadId::CONTROLLER_ONE) & combo) == combo;
	}

	uint32_t ptrIsValid(void* ptr)
	{
		const uint32_t ptrRaw = reinterpret_cast<uint32_t>(ptr);

		// Cached memory
		if ((ptrRaw >= 0x80000000) && (ptrRaw < 0x81800000))
		{
			return mod::util::PointerVerificationType::PTR_CACHED;
		}

		// Uncached memory
		if ((ptrRaw >= 0xC0000000) && (ptrRaw < 0xC1800000))
		{
			return mod::util::PointerVerificationType::PTR_UNCACHED;
		}

		return mod::util::PointerVerificationType::PTR_INVALID;
	}

	__attribute__((noinline)) float intToFloat(int32_t value)
	{
		return static_cast<float>(value);
	}

	float getTextMultilineIncrement(const char* text, float scale, uint32_t additionalLines)
	{
		uint16_t numLines;
		ttyd::fontmgr::FontGetMessageWidthLine(text, &numLines);

		const float lineCount = intToFloat(numLines + additionalLines);
		return lineCount * (32.f * scale);
	}

}
