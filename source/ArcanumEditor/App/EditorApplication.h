#pragma once

#include "App/Application.h"

#include "Layer/EditorLayer.h"
#include "Layer/TestLayer.h"

class EditorApplication : public Application
{
public:
	EditorApplication():
		Application{ ApplicationSpecification{ std::string("ARCANUM EDITOR"), glm::vec2(1920, 1080), false }}
	{
	}
};