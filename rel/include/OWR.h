#pragma once
#include <cstdint>
#include <gc/OSModule.h>
#include <StateManager.h>

namespace mod::owr
{
    class OWR
    {
       public:
        OWR();
        void Init();
        void Update();
        void OnModuleLoaded(OSModuleInfo *module_info);
        void SequenceInit();
        void DrawString(const char *data, float x, float y, uint32_t color, float scale = 1.0f);
        void HomewardWarp();
        void RecieveItems();

        StateManager state;
    };

    extern OWR *gSelf;
} // namespace mod::owr
