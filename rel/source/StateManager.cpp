#include "StateManager.h"

using namespace mod::owr;

StateManager *gState = nullptr;

StateManager::StateManager()
{
    gState = this;
    gState->Init();
}

void StateManager::Init()
{
    gState->apSettings = reinterpret_cast<APSettings *>(0x80003210);
}
