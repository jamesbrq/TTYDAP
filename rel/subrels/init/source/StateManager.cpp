#include "StateManager.h"
#include "OWR.h"

#include <cstdint>

using namespace mod::owr;

StateManager::StateManager()
{
    gState = this;
    gState->Init();
}

void StateManager::Init()
{
    gState->apSettings = reinterpret_cast<APSettings *>(0x80003220);
}
