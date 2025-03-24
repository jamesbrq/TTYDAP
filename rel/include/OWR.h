#pragma once
#include <cstdint>
#include <gc/OSLink.h>
#include <StateManager.h>

using gc::OSLink::OSModuleInfo;

namespace mod::owr
{
	class OWR
	{
	public:

		OWR();
		void Init();
		void Update();
		void OnModuleLoaded(OSModuleInfo* module_info);
		void SequenceInit();
		void DrawString(const char* data, float x, float y, uint32_t color, float scale = 1.0f);
		void AAAToGOR();
		void RecieveItems();

		StateManager state_;
	};

	extern OWR* gSelf;
}
