#include "StateManager.h"

using namespace mod::owr;

StateManager* gState = nullptr;

StateManager::StateManager()
{
	gState = this;
	gState->Init();
}

void StateManager::Init()
{
	for (int i = 0; i < 255; i++)
	{
		gState->itemEntries[i] = 0x149;
	}
}
