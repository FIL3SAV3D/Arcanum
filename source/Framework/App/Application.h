#pragma once

#include <memory>

#include "Modes/ModeManager.h"

#include "OpenGL/Window.h"
#include "OpenGL/InputHandler.h"
#include <Misc/Clock.h>

class Application
{
public:
	Application();
	~Application();

	void Create();

	void Run();

	void Destroy();

	bool IsQuitting();

	void UseMode(std::string _Name);

	template<typename T>
	void PushMode() { modeManger->PushMode<T>(); }

	void PushLayer();

private:
	std::shared_ptr<Window> window;

	std::unique_ptr<ModeManager> modeManger;
	std::unique_ptr<InputHandler> inputHandler;

	std::unique_ptr<Clock> clock;
};