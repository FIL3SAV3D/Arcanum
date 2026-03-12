#pragma once

class Coordinator;

class IMode
{
public:
	IMode()  = default;
	~IMode() = default;

public:
	virtual void Create (Coordinator& _Coordinator)  = 0;
	virtual void Destroy(Coordinator& _Coordinator) = 0;

	virtual void RegisterSystems  (Coordinator& _Coordinator) {};
	virtual void UnregisterSystems(const Coordinator& _Coordinator) {};
};