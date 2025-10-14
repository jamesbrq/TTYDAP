#include "StateManager.h"
#include "OWR.h"

#include <cstdint>

using namespace mod::owr;

StateManager::StateManager()
{
    gState = this;
    gState->Init();
    for (int i = 0; i < 8; i++) gState->state_msgWork[i] = ttyd::msgdrv::msgWork[i]; // msgWork.entries[0] && msgWork.entries[1]
    gState->state_msgWork[16] = ttyd::msgdrv::msgWork[8]; // msgWork.animBase
}

void StateManager::Init()
{
    gState->apSettings = reinterpret_cast<APSettings *>(0x80003220);
    gState->tattleItems = reinterpret_cast<uint16_t *>(0x80003B00);
    gState->maniacItems = reinterpret_cast<uint16_t *>(0x80003400);
}
