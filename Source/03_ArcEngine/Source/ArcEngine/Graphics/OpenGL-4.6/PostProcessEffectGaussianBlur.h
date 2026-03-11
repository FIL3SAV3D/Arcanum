#pragma once

#include "IDeferredPostProcess.h"

class Shader;

class PostProcessEffectGaussianBlur : public IDeferredPostProcess
{
public:
	PostProcessEffectGaussianBlur();
	~PostProcessEffectGaussianBlur();

	void ApplyPostProcess(unsigned int& _QuadVAO, std::function<void()> _PingPongCallback) override;

private:
	std::shared_ptr<Shader> m_PPSVerticalBlur;
	std::shared_ptr<Shader> m_PPSHorizontalBlur;
};