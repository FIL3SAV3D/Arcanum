#pragma once

#include <memory>

#include "Modes/ModeManager.h"

#include "OpenGL/Window.h"
#include "OpenGL/InputHandler.h"
#include <Misc/Clock.h>
#include <OpenGL/IRenderer.h>

struct ApplicationSpecification
{
	std::string name;
	glm::vec2 windowSize;
	bool fullscreen;

	ApplicationSpecification(
		std::string _Name,
		glm::vec2 _WindowSize,
		bool _Fullscreen)
	{
		name = _Name;
		windowSize = _WindowSize;
		fullscreen = _Fullscreen;
	}
};

class Application
{
public:
	Application(const ApplicationSpecification& _Spec);
	~Application();

	void Create();
	void Destroy();

	void Run();

	void Update();

	void Render();

	void Render3D();
	void Render2D();
	void RenderUI();

	bool IsQuitting();


	std::shared_ptr<Window> GetWindow() { return window; }

	template<typename T>
	std::shared_ptr<IMode> PushMode() { return modeManger->PushMode<T>(); }

	template<typename T, typename ...Args>
	std::shared_ptr<ILayer> PushLayer(Args... _Args) { return m_LayerStack->PushLayer<T>(_Args...); }

private:
	std::shared_ptr<IRenderer> renderer;

	std::shared_ptr<Window> window;

	std::unique_ptr<LayerStack> m_LayerStack;
	std::unique_ptr<ModeManager> modeManger;
	std::unique_ptr<InputHandler> inputHandler;

	std::unique_ptr<Clock> clock;
};