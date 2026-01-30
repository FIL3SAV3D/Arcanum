#pragma once

#include "Util/JobSystem/JobSystem.h"

#include <memory>

#include "Managers/ModeManager.h"

#include "OpenGL/Window.h"
#include "OpenGL/InputHandler.h"
#include <Util/Clock.h>
#include <ECS/Coordinator.h>

#include "ECS/Systems/UIRenderSystem.h"
#include "Renderer/IRenderer.h"

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

	void Run();

	bool IsQuitting();

public:
	// Application Being Created
	void OnCreate();
	// Before Application First Run
	void OnStart();
	// 
	void OnEnable();

	// Update
	void OnInput();
	void OnUpdate(const float& _DeltaTime);
	void OnLateUpdate(const float& _DeltaTime);
	void OnRender();
	void OnRenderUI();
	void OnApplicationPause();
	void OnCheckForDisabled();

	// End
	void OnDisable();
	void OnQuit();
	void OnDestroy();

public:
	std::shared_ptr<Window> GetWindow() { return window; }

private:
	std::shared_ptr<>


	std::shared_ptr<Camera> camera;

	std::shared_ptr<Coordinator> coordinator;

	std::shared_ptr<IRenderer> renderer;

	std::shared_ptr<Window> window;

	std::unique_ptr<ModeManager> modeManger;
	std::shared_ptr<InputHandler> inputHandler;

	std::unique_ptr<Clock> clock;

	std::weak_ptr<UIRenderSystem> UI;
};