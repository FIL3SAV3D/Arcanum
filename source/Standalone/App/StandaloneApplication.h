#pragma once

#include "App/Application.h"

#include "Layer/GameLayer.h"

class StandaloneApplication : public Application
{
public:
	StandaloneApplication():
		Application{ ApplicationSpecification{ "TEST GAME", glm::vec2(1440, 780), false} }
	{
		PushLayer<GameLayer>(GetWindow());
	}
};