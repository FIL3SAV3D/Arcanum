#include "PostProcessEffectGaussianBlur.h"

#include <glad/glad.h>

#include "Shader.h"

PostProcessEffectGaussianBlur::PostProcessEffectGaussianBlur()
{
    m_PPSVerticalBlur = std::make_shared<Shader>("DSPostProcessBase.vert", "DSVerticalBlur.frag");
    m_PPSHorizontalBlur = std::make_shared<Shader>("DSPostProcessBase.vert", "DSHorizontalBlur.frag");

	m_PPSVerticalBlur->Use();
	m_PPSVerticalBlur->setInt("gScene", 0);
	m_PPSVerticalBlur->setInt("customInput01", 1);
	//m_PPSVerticalBlur->setInt("customInput02", 2);

	m_PPSHorizontalBlur->Use();
	m_PPSHorizontalBlur->setInt("gScene", 0);
	m_PPSHorizontalBlur->setInt("customInput01", 1);
	//m_PPSHorizontalBlur->setInt("customInput02", 2);
}

PostProcessEffectGaussianBlur::~PostProcessEffectGaussianBlur()
{
	m_PPSVerticalBlur.reset();
	m_PPSHorizontalBlur.reset();

	m_PPSVerticalBlur = nullptr;
	m_PPSHorizontalBlur = nullptr;
}

void PostProcessEffectGaussianBlur::ApplyPostProcess(unsigned int& _QuadVAO, std::function<void()> _PingPongCallback)
{
	auto quad = glm::mat4x4(1.0f);

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
