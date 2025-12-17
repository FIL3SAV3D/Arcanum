#pragma once

#include "IRenderer.h"

#include "GBuffer.h"
#include <memory>

#include "Shader.h"
#include "Model.h"

class DeferredPostProcessChain;

class DeferredRenderer : public IRenderer
{
public:
	void Initialize(const glm::vec2& _size);

	std::shared_ptr<GBuffer> m_gBuffer;

	std::shared_ptr<Shader> m_gBufferShader;
	std::shared_ptr<Shader> m_houseShader;
	std::shared_ptr<Shader> m_screenShader;
	std::shared_ptr<Shader> m_screenDebugShader;

	std::shared_ptr<Shader> m_shaderLightBox;

	std::shared_ptr<Model> m_RatModel;
	std::shared_ptr<Model> m_cubeModel;
	std::shared_ptr<Model> m_houseModel;

	std::shared_ptr<DeferredPostProcessChain> m_postProcessChain;

	unsigned int quadVAO, quadVBO;

	unsigned int BC;
	unsigned int N;
	unsigned int ORM;

	unsigned int BC_HOUSE;
	unsigned int N_HOUSE;
	unsigned int ORM_HOUSE;

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;

	// Inherited via IRenderer
	void RenderSceneCB(const glm::mat4& projection, const glm::mat4x4& view, const glm::vec3& cameraPosition, const bool& _DebugMode, const glm::vec2& _DepthRange) override;

	// Inherited via IRenderer
	void Resize(const glm::vec2& _size) override;

	void DebugQuads(const glm::vec2& _DepthRange);
};