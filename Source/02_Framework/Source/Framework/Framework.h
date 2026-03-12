#pragma once

#include <ArcEngine/ArcEngine.h>

#include "Framework/Managers/ModeManager.h"
#include "Framework/Managers/AssetManager.h"
#include "Framework/ECS/Coordinator.h"

#include <zpp_bits.h>
#include <memory>
#include <ArcEngine/Util/Clock.h>

struct ApplicationSpecification
{
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
		modeManger.RegisterMode<T>(_Name, m_ECSCoordinator);
	}

	void SwitchMode(const std::string& _Name)
	{
		modeManger.SwitchMode(_Name, m_ECSCoordinator);
	}

	void Start();

	void Run();

	bool IsQuitting();

private:
	// Application Being Created
	void OnCreate();
	// Before Application First Run
	void OnStart();
	// 
	void OnEnable();

	// Update
	void OnInput();
	void OnUpdate(State& _State, const float& _DeltaTime);
	void OnLateUpdate(const float& _DeltaTime);

	void OnRender(State& _State);

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
	Coordinator m_ECSCoordinator;
	AssetManager m_AssetManager;
	Clock clock;

	ArcEngine::ArcEngine    m_ArcEngine;
};