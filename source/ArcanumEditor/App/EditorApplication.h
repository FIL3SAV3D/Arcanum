#pragma once

#include "App/Application.h"

class EditorApplication : public Application
{
public:
	EditorApplication():
		Application{ ApplicationSpecification{ std::string("ARCANUM EDITOR"), glm::vec2(720, 480), false }}
	{
	}
private:

};