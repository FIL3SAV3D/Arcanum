#pragma once
#include <vector>
#include <Modes/IMode.h>

#include <map>
#include <unordered_map>

class ModeManager
{
public:
	template<typename T, typename ...Args>
	std::shared_ptr<T> RegisterMode(Args ..._Args)
	{
		const char* typeName = typeid(T).name();

		assert(m_ModeMap.find(typeName) == m_ModeMap.end() && "Registering mode more than once.");

		std::shared_ptr<T> mode = std::make_shared<T>(_Args...);
		m_ModeMap.insert({ typeName, mode });

		return mode;
	}

	// Application Being Created
	void OnCreate();
	void OnStart();
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

private:
	std::unordered_map<const char*, std::shared_ptr<IMode>> m_ModeMap{};
};