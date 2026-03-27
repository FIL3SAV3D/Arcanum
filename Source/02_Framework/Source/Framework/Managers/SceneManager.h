#pragma once

#include <string>
#include <Framework/Scenes/IScene.h>
#include <unordered_map>
#include <cassert>
#include <memory>

class SceneManager
{
public:
	template<typename T>
	void RegisterScene(const std::string& _Name, std::shared_ptr<Coordinator> _Coordinator)
	{
		assert(m_SceneMap.find(_Name) == m_SceneMap.end() && "Registering scene with same name more than once.");

		std::shared_ptr<IScene> scene = std::static_pointer_cast<IScene>(std::make_shared<T>());
		m_SceneMap.insert({ _Name, scene });
	}

	void UnregisterScene(const std::string& _Name, std::shared_ptr<Coordinator> _Coordinator)
	{
		assert(m_SceneMap.find(_Name) == m_SceneMap.end() && "No scene found while unregistering!");

		std::shared_ptr<IScene> scene = m_SceneMap.at(_Name);

		m_SceneMap.erase(_Name);
	}

	void SwitchScene(const std::string& _Name, std::shared_ptr<Coordinator> _Coordinator)
	{
		if (m_CurrentScene != nullptr)
		{
			m_CurrentScene->Destroy(_Coordinator);
		}

		assert(m_SceneMap.find(_Name) != m_SceneMap.end() && "No scene found!");
		m_CurrentScene = m_SceneMap.at(_Name);

		m_CurrentScene->Create(_Coordinator);
	}

private:
	std::shared_ptr<IScene> m_CurrentScene;

    std::unordered_map<std::string, std::shared_ptr<IScene>> m_SceneMap;
};