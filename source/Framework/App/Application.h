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

	std::shared_ptr<Window> GetWindow() { return window; }

	template<typename T>
	std::shared_ptr<IMode> PushMode() { return modeManger->PushMode<T>(); }

	template<typename T, typename ...Args>
	std::shared_ptr<ILayer> PushLayer(Args... _Args) { return m_LayerStack->PushLayer<T>(_Args...); }

private:
	std::shared_ptr<Window> window;

	std::unique_ptr<LayerStack> m_LayerStack;
	std::unique_ptr<ModeManager> modeManger;
	std::unique_ptr<InputHandler> inputHandler;

	std::unique_ptr<Clock> clock;
};