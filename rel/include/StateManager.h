#pragma once

#include <cstdint>

namespace mod::owr {

	class StateManager
	{
	public:
		int8_t rChapters = 7;
		int16_t itemEntries[255];
		void Init();
	};
}