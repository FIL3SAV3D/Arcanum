#pragma once

#include <memory>

class Coordinator;
class SceneManager;

class IMode
{
public:
	IMode()  = default;
	~IMode() = default;

public:
	virtual void Create (std::shared_ptr < Coordinator> _Coordinator, std::shared_ptr < SceneManager> _SceneManager)  = 0;
	virtual void Destroy(std::shared_ptr < Coordinator> _Coordinator, std::shared_ptr < SceneManager> _SceneManager) = 0;

	virtual void RegisterSystems  (std::shared_ptr < Coordinator> _Coordinator) {};
	virtual void UnregisterSystems(std::shared_ptr < Coordinator> _Coordinator) {};
};