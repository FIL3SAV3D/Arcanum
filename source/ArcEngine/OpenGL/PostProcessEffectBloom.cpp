#include "PostProcessEffectBloom.h"

#include <glad/glad.h>

#include "Shader.h"

PostProcessEffectBloom::PostProcessEffectBloom()
{
	m_PPSBrightnessThreshold	= std::make_shared<Shader>("DSPostProcessBase.vert", "DSBloomBrightnessThreshold.frag");
	m_PPSHorizontalBlur			= std::make_shared<Shader>("DSPostProcessBase.vert", "DSBloomHorizontalBlur.frag");
	m_PPSVerticalBlur			= std::make_shared<Shader>("DSPostProcessBase.vert", "DSBloomVerticalBlur.frag");
	m_PPSAdditiveBlend			= std::make_shared<Shader>("DSPostProcessBase.vert", "DSBloomAdditiveBlend.frag");

	m_PPSBrightnessThreshold->Use();
	m_PPSBrightnessThreshold->setInt("gScene", 0);
	m_PPSBrightnessThreshold->setInt("customInput01", 1);
	//m_PPSBrightnessThreshold->setInt("customInput02", 2);

	m_PPSHorizontalBlur->Use();
	m_PPSHorizontalBlur->setInt("gScene", 0);
	m_PPSHorizontalBlur->setInt("customInput01", 1);
	//m_PPSHorizontalBlur->setInt("customInput02", 2);

	m_PPSVerticalBlur->Use();
	m_PPSVerticalBlur->setInt("gScene", 0);
	m_PPSVerticalBlur->setInt("customInput01", 1);
	//m_PPSVerticalBlur->setInt("customInput02", 2);

	m_PPSAdditiveBlend->Use();
	m_PPSAdditiveBlend->setInt("gScene", 0);
	m_PPSAdditiveBlend->setInt("customInput01", 1);
	//m_PPSAdditiveBlend->setInt("customInput02", 2);
}

PostProcessEffectBloom::~PostProcessEffectBloom()
{
	m_PPSVerticalBlur.reset();
	m_PPSHorizontalBlur.reset();

	m_PPSVerticalBlur = nullptr;
	m_PPSHorizontalBlur = nullptr;
}

void PostProcessEffectBloom::ApplyPostProcess(unsigned int& _QuadVAO, std::function<void()> _PingPongCallback)
{
	auto quad = glm::mat4x4(1.0f);

	// Extract Brightness
	_PingPongCallback();
	m_PPSBrightnessThreshold->Use();
	m_PPSBrightnessThreshold->setMat4("quad", quad);
	quad = glm::mat4x4(1.0f);
	glBindVertexArray(_QuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenerateTextureMipmap(0);

	for (unsigned int i = 0; i < 5; i++)
	{
		// Horizontal Blur
		_PingPongCallback();
		m_PPSHorizontalBlur->Use();
		m_PPSHorizontalBlur->setMat4("quad", quad);
		glBindVertexArray(_QuadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Vertical Blur
		_PingPongCallback();
		m_PPSVerticalBlur->Use();
		m_PPSVerticalBlur->setMat4("quad", quad);
		quad = glm::mat4x4(1.0f);
		glBindVertexArray(_QuadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Additive Blend
	_PingPongCallback();
	m_PPSAdditiveBlend->Use();
	m_PPSAdditiveBlend->setMat4("quad", quad);
	quad = glm::mat4x4(1.0f);
	glBindVertexArray(_QuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
