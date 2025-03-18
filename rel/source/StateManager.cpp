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
		int value = 0x149 - i;
		if(value < 0)
			value = 1;;
		gState->itemEntries[i] = value;
	}
}
