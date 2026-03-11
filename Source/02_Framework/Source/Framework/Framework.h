#pragma once

#include <ArcEngine/ArcEngine.h>

#include "Framework/Managers/ModeManager.h"
#include "Framework/ECS/Coordinator.h"

#include <zpp_bits.h>
#include <memory>

struct ApplicationSpecification
{
	//constexpr static auto serialize(auto& archive, auto& self)
	//{
	//	return archive(self.name, self.windowSizeX, self.windowSizeY, self.fullscreen);
	//}

	std::string name{};
	int windowSizeX{};
	int windowSizeY{};
	bool fullscreen = false;
};

struct GLFWwindow;

class Framework
{
public:
	Framework()  = default;
	~Framework() = default;

	void Create();
	void Destroy();

	template<typename T>
	void RegisterMode(const std::string& _Name)
	{
		modeManger.RegisterMode<T>(_Name, coordinator);
	}

	void SetMode();

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
	bool bQuit = false;
	bool stop_rendering = false;
private:
	ModeManager modeManger;
	ArcEngine::ArcEngine m_ArcEngine;

	Coordinator coordinator;

	//std::shared_ptr<InputHandler> inputHandler;

	//std::unique_ptr<Clock> clock;

	//std::weak_ptr<UIRenderSystem> UI;

};