#pragma once
#include <cstdint>
#include <ttyd/OSLink.h>

namespace mod::owr
{
	class OWR
	{
	public:
		OWR();

		void Init();
		void Update();
		void Draw();

	private:
		int16_t ReplaceGeneralItem(int16_t id, int32_t collection_expr);
		void OnModuleLoaded(ttyd::oslink::OSModuleInfo* module_info);
		void SequenceInit();
		void DrawString(const char* data, int32_t x, int32_t y, uint32_t color, float scale = 1.0);
		void LZTest();
		void NewFileInit();
	};
}
