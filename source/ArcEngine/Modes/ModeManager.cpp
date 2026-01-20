#include "ModeManager.h"

ModeManager::ModeManager():
	m_Modes{}
{
}

ModeManager::~ModeManager()
{
}

void ModeManager::Update(const float& _DeltaTime)
{
	currentMode->Update(_DeltaTime);
}



void ModeManager::PopMode()
{
}
