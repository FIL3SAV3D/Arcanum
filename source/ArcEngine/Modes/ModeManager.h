#pragma once

#include <unordered_map>
#include "IMode.h"

class ModeManager
{
public:
	ModeManager();
	~ModeManager();

	void Update(const float& _DeltaTime);

	template<typename T, typename ...Args>
	void PushMode(Args... _Args)
	{
		std::shared_ptr<IMode> mode = std::static_pointer_cast<IMode>(std::make_shared<T>(_Args...));
		m_Modes.insert({ mode->name, mode });
	}

	void PopMode();

	void SwitchMode(const std::string& _Name)
	{
		currentMode = m_Modes.at(_Name.c_str());
	}

	std::unordered_map<std::string, std::shared_ptr<IMode>> m_Modes;

private:
	std::shared_ptr<IMode> currentMode;
};