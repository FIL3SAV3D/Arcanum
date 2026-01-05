#pragma once

#include "IRenderer.h"

#include <memory>
#include <vector>

class DeferredPostProcessChain;
class HDRCubemap;
class GBuffer;
class Model;
class Shader;

class DeferredRenderer : public IRenderer
{
public:
	void Initialize(const glm::vec2& _size);

	std::shared_ptr<GBuffer> m_gBuffer;

	std::shared_ptr<Shader> m_shaderGeometryPass;

	std::shared_ptr<Shader> m_shaderGeometryPassNoTextures;
	
	std::shared_ptr<Shader> m_shaderScreenPass;

	std::shared_ptr<Shader> m_shaderSkyBox;

	std::shared_ptr<Shader> m_screenDebugShader;

	std::shared_ptr<Shader> m_shaderLightBox;

	std::shared_ptr<Shader> m_shadowMap;


	std::shared_ptr<Model> m_RatModel;
	std::shared_ptr<Model> m_cubeModel;

	std::shared_ptr<Model> m_invertedCube;

	std::shared_ptr<Model> m_ratHat;

	std::shared_ptr<Model> m_plane;

	std::shared_ptr<HDRCubemap> m_cubemap;

	std::shared_ptr<DeferredPostProcessChain> m_postProcessChain;

	unsigned int quadVAO, quadVBO;

	unsigned int skyboxVAO, skyboxVBO;

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthMapFBO;
	unsigned int depthMap;



	unsigned int BC;
	unsigned int N;
	unsigned int ORM;

	unsigned int BC_HOUSE;
	unsigned int N_HOUSE;
	unsigned int ORM_HOUSE;

	const unsigned int NR_OF_RATS = 100;
	
	std::vector<glm::vec3> ratPositions;
	std::vector<glm::vec3> ratRotation;



	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;

	// Inherited via IRenderer
	void RenderSceneCB(const glm::mat4& projection, const glm::mat4x4& view, const glm::vec3& cameraPosition, const bool& _DebugMode, const glm::vec2& _DepthRange) override;

	// Inherited via IRenderer
	void Resize(const glm::vec2& _size) override;

	void DebugQuads(const glm::vec2& _DepthRange);
};