#pragma once

#include "App/Application.h"

#include "Layer/EditorLayer.h"
#include "Layer/TestLayer.h"

class EditorApplication : public Application
{
public:
	EditorApplication():
		Application{}
	{
		PushLayer<EditorLayer>(GetWindow());
	}
};