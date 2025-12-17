#pragma once

#include "IDeferredPostProcess.h"

class Shader;

class PostProcessEffectBloom : public IDeferredPostProcess
{
public:
	PostProcessEffectBloom();
	~PostProcessEffectBloom();

	void ApplyPostProcess(unsigned int& _QuadVAO, std::function<void()> _PingPongCallback) override;

private:
	std::shared_ptr<Shader> m_PPSBrightnessThreshold;
	std::shared_ptr<Shader> m_PPSVerticalBlur;
	std::shared_ptr<Shader> m_PPSHorizontalBlur;
	std::shared_ptr<Shader> m_PPSAdditiveBlend;
};