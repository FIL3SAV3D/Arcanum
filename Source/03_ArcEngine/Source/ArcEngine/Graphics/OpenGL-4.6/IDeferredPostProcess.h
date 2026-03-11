#pragma once

#include <memory>
#include <functional>

struct FBO;

class IDeferredPostProcess
{
public:
	virtual void ApplyPostProcess(unsigned int& _QuadVAO, std::function<void()> _PingPongCallback) = 0;
};