#include "StateManager.h"
#include "visibility.h"

using namespace mod::owr;

KEEP_VAR StateManager *gState = nullptr;

KEEP_FUNC StateManager::StateManager()
{
    gState = this;
    gState->Init();
}

KEEP_FUNC void StateManager::Init()
{
    gState->apSettings = reinterpret_cast<APSettings *>(0x80003220);
}
