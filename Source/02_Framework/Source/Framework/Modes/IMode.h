#pragma once

#include <glm/glm.hpp>

class IMode
{
public:
	IMode()  = default;
	~IMode() = default;

public:
	virtual void RegisterSystems() {};
};