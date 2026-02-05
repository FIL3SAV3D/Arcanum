#pragma once

#include "ArcEngine/Util/JobSystem/JobSystem.h"
#include "ArcEngine/OpenGL/Window.h"
#include "ArcEngine/OpenGL/InputHandler.h"
#include <ArcEngine/Util/Clock.h>
#include <ArcEngine/ECS/Coordinator.h>
#include "ArcEngine/Renderer/IRenderer.h"

#include "Framework/Managers/ModeManager.h"
#include "Framework/ECS/Systems/UIRenderSystem.h"

#include <memory>

#include <ArcEngine/Asset/AssetManager.h>

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

struct GLFWwindow;

class IApplication
{
public:
	IApplication(const ApplicationSpecification& _Spec);
	~IApplication();

	virtual void Initialize() {};
	virtual void Deinitialize() {};

	template<typename T>
	void RegisterMode(const std::string& _Name)
	{
		modeManger->RegisterMode<T>(_Name);
	}

	template<typename T>
	void RegisterDefaultMode(const std::string& _Name)
	{
		modeManger->RegisterDefaultMode<T>(_Name);
	}

	void SetMode();

	void Run();

	bool IsQuitting();

	static void FrameBufferSizeCallback(GLFWwindow* _window, int _width, int _height);

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
	std::shared_ptr<ArcEngine::AssetManager> assetManager;

	std::shared_ptr<Coordinator> coordinator;

	std::shared_ptr<Window> window;

	std::unique_ptr<ModeManager> modeManger;
	std::shared_ptr<InputHandler> inputHandler;

	std::unique_ptr<Clock> clock;

	std::weak_ptr<UIRenderSystem> UI;
};