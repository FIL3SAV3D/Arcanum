#pragma once

#include "App/Application.h"

#include "GameLayer.h"

class StandaloneGame : public Application
{
public:
	StandaloneGame(/*const ApplicationSpecification& _Specification*/):
		Application{}
	{
		PushLayer<GameLayer>(GetWindow());
	}
};