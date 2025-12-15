#pragma once

#include "IRenderer.h"

#include "GBuffer.h"
#include <memory>

#include "Shader.h"
#include "Model.h"



class DeferredRenderer : public IRenderer
{
public:
	void Initialize(const int& screen_width, const int& screen_height);

	std::shared_ptr<GBuffer> m_gBuffer;

	std::shared_ptr<Shader> m_gBufferShader;
	std::shared_ptr<Shader> m_screenShader;

	std::shared_ptr<Shader> m_debugScreenNormalShader;
	std::shared_ptr<Shader> m_debugScreenPositionShader;
	std::shared_ptr<Shader> m_debugScreenDiffuseShader;

	std::shared_ptr<Model> m_RatModel;

	unsigned int quadVAO, quadVBO;

	unsigned int BC;

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;

	// Inherited via IRenderer
	void RenderSceneCB(const glm::mat4& projection, const glm::mat4x4& view, const glm::vec3& cameraPosition) override;
};