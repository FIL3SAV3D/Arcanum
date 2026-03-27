#pragma once

#include "Framework/Modes/IMode.h"

#include <vector>
#include <unordered_map>
#include <cassert>

#include "Framework/ECS/Coordinator.h"

class SceneManager;

class ModeManager
{
public:
	template<typename T>
	void RegisterMode(const std::string& _Name, std::shared_ptr<Coordinator> _coordinator)
	{
		assert(m_ModeMap.find(_Name) == m_ModeMap.end() && "Registering mode with same name more than once.");

		std::shared_ptr<T> mode = std::make_shared<T>();
		m_ModeMap.insert({ _Name, mode });

		mode->RegisterSystems(_coordinator);
	}

	void UnregisterMode(const std::string& _Name, std::shared_ptr < Coordinator> _Coordinator, std::shared_ptr < SceneManager> _SceneManager)
	{
		assert(m_ModeMap.find(_Name) == m_ModeMap.end() && "No mode found while unregistering!");

		std::shared_ptr<IMode> mode = m_ModeMap.at(_Name);
		mode->UnregisterSystems(_Coordinator);

		m_ModeMap.erase(_Name);
	}

	void SwitchMode(const std::string& _Name, std::shared_ptr<Coordinator> _Coordinator, std::shared_ptr <SceneManager> _SceneManager)
	{
		if (m_CurrentMode != nullptr)
		{
			m_CurrentMode->Destroy(_Coordinator, _SceneManager);
		}

		assert(m_ModeMap.find(_Name) != m_ModeMap.end() && "No mode found!");
		m_CurrentMode = m_ModeMap.at(_Name);

		m_CurrentMode->Create(_Coordinator, _SceneManager);
	}

private:
	std::shared_ptr<IMode> m_CurrentMode;

	std::unordered_map<std::string, std::shared_ptr<IMode>> m_ModeMap{};
};