#include "DeferredRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

#include "DeferredPostProcessChain.h"
#include "Cubemap.h"

#include "GBuffer.h"

#include "Shader.h"
#include "Model.h"
#include "HDRCubemap.h"

void DeferredRenderer::Initialize(const glm::vec2& _size)
{
	m_cubemap = std::make_shared<HDRCubemap>("qwantani_sunset_1k.hdr", _size.x, _size.y);

	glViewport(0, 0, static_cast<int>(_size.x), _size.y);

	m_gBuffer = std::make_shared<GBuffer>();
	m_gBuffer->Create(_size);

	m_gBufferShader = std::make_shared<Shader>("DSGeometry");
	m_gBufferShader->Use();
	m_gBufferShader->setInt("texture_diffuse1", 0);
	m_gBufferShader->setInt("texture_normal1", 1);
	m_gBufferShader->setInt("texture_ORM1", 2);

	m_houseShader = std::make_shared<Shader>("DSGeometry.vert", "DSHouseGEO.frag");
	m_houseShader->Use();
	m_houseShader->setInt("texture_diffuse1", 0);

	m_screenShader = std::make_shared<Shader>("DSToScreen.vert", "DSToScreen.frag");
	m_screenDebugShader = std::make_shared<Shader>("DSToScreen.vert", "DSToScreenDEBUG.frag");

	m_shaderLightBox = std::make_shared<Shader>("ShaderLightBox");

	m_shaderSkyBox = std::make_shared<Shader>("skybox");
	m_shaderSkyBox->Use();
	m_shaderSkyBox->setInt("skybox", 0);

	m_screenDebugShader->Use();
	m_screenDebugShader->setInt("gPosition", 0);
	m_screenDebugShader->setInt("gNormal", 1);
	m_screenDebugShader->setInt("gAlbedo", 2);
	m_screenDebugShader->setInt("gORM", 3);
	m_screenDebugShader->setInt("gDepth", 4);
	m_screenDebugShader->setInt("gCombined", 5);

	m_screenShader->Use();
	m_screenShader->setInt("gPosition", 0);
	m_screenShader->setInt("gNormal", 1);
	m_screenShader->setInt("gAlbedo", 2);
	m_screenShader->setInt("gORM", 3);
	m_screenShader->setInt("gDepth", 4);
	m_screenShader->setInt("irradianceMap", 5);

	m_postProcessChain = std::make_shared<DeferredPostProcessChain>();
	m_postProcessChain->Create(m_gBuffer);

	m_RatModel = std::make_shared<Model>(std::string("D:\\PersonalProjects\\Arcanum\\Data\\Models\\GLB_Models\\Rat.glb").c_str());
	m_cubeModel = std::make_shared<Model>(std::string("D:\\PersonalProjects\\Arcanum\\Source\\ArcEngine\\Models\\obj_models\\cube.obj").c_str());

	m_houseModel = std::make_shared<Model>(std::string("D:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\american_house_MARK_2.fbx").c_str());

	m_invertedCube = std::make_shared<Model>(std::string("D:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\InvertedCube.fbx").c_str());

	m_ratHat = std::make_shared<Model>(std::string("D:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\Ratsmas_Hat.fbx").c_str());
	

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	const unsigned int NR_LIGHTS = 32;
	
	srand(9);
	for (unsigned int i = 0; i < NR_LIGHTS - 1; i++)
	{
		// calculate slightly random offsets
		float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
		float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
		float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// also calculate random color
		float rColor = static_cast<float>((rand() % 100) / 100.0f); // between 0.5 and 1.0
		float gColor = static_cast<float>((rand() % 100) / 100.0f); // between 0.5 and 1.0
		float bColor = static_cast<float>((rand() % 100) / 100.0f); // between 0.5 and 1.0
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}

	lightPositions.push_back(glm::vec3(0.0f, 10.0f, 0.0f));
	lightColors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	BC = TextureFromFile("T_Rat_BC.png", "D:\\PersonalProjects\\Arcanum\\Data\\Models\\Src_Images\\");
	N = TextureFromFile("T_Rat_N.png", "D:\\PersonalProjects\\Arcanum\\Data\\Models\\Src_Images\\");
	ORM = TextureFromFile("T_Rat_ORM.png", "D:\\PersonalProjects\\Arcanum\\Data\\Models\\Src_Images\\");

	BC_HOUSE = TextureFromFile("american_house_DIFF.png", "D:\\PersonalProjects\\Arcanum\\Data\\Models\\Src_Images\\");
	ORM_HOUSE = TextureFromFile("T_Rat_ORM.png", "D:\\PersonalProjects\\Arcanum\\Data\\Models\\Src_Images\\");


	N_HOUSE = TextureFromFile("T_Ratsmashat_BC.png", "D:\\PersonalProjects\\Arcanum\\Data\\Models\\Src_Images\\");

	srand(10);
	for (unsigned int i = 0; i < NR_OF_RATS; i++)
	{
		// calculate slightly random offsets
		float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
		float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
		float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
		ratPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// also calculate random color
		float rColor = static_cast<float>((rand() % 180)); // between 0.5 and 1.0
		float gColor = static_cast<float>((rand() % 180)); // between 0.5 and 1.0
		float bColor = static_cast<float>((rand() % 180)); // between 0.5 and 1.0
		ratRotation.push_back(glm::vec3(rColor, gColor, bColor));
	}

}

void DeferredRenderer::RenderSceneCB(const glm::mat4& projection, const glm::mat4x4& view, const glm::vec3& cameraPosition, const bool& _DebugMode, const glm::vec2& _DepthRange)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_gBuffer->BindForWriting();
	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);

	m_gBufferShader->Use();
	m_gBufferShader->setMat4("view", view);
	m_gBufferShader->setMat4("projection", projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, BC);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, N);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, ORM);
	
	for (int i = 0; i < NR_OF_RATS; i++)
	{
		auto model = glm::mat4x4(1.0f);
		model = glm::translate(model, ratPositions[i]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		m_gBufferShader->setMat4("model", model);
		m_gBufferShader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		m_RatModel->Draw(*m_gBufferShader);

	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, N_HOUSE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	for (int i = 0; i < NR_OF_RATS; i++)
	{
		auto model = glm::mat4x4(1.0f);
		model = glm::translate(model, glm::vec3(0, 0.025f, 0.35f));
		model = glm::translate(model, ratPositions[i]);
		model = glm::rotate(model, glm::radians(8.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.015f));
		m_gBufferShader->setMat4("model", model);
		m_gBufferShader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		m_ratHat->Draw(*m_gBufferShader);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, BC_HOUSE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_houseShader->Use();
	m_houseShader->setMat4("view", view);
	m_houseShader->setMat4("projection", projection);

	auto houseModel = glm::mat4x4(1.0f);
	houseModel = glm::translate(houseModel, glm::vec3(0, 0, 15.0f));
	houseModel = glm::rotate(houseModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	houseModel = glm::scale(houseModel, glm::vec3(0.01f));
	m_houseShader->setMat4("model", houseModel);
	m_houseShader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(houseModel))));

	m_houseModel->Draw(*m_houseShader);

	glDepthMask(GL_FALSE);

	glDisable(GL_DEPTH_TEST);

	m_gBuffer->BindForReading();
	glClear(GL_COLOR_BUFFER_BIT);

	m_screenShader->Use();
	for (unsigned int i = 0; i < lightPositions.size(); i++)
	{
		m_screenShader->setVec3(std::string("lights[" + std::to_string(i) + "].Position").c_str(), lightPositions[i]);
		m_screenShader->setVec3(std::string("lights[" + std::to_string(i) + "].Color").c_str(), lightColors[i]);
		// update attenuation parameters and calculate radius
		const float linear = 1.0f;
		const float quadratic = 1.8f;
		m_screenShader->setFloat(std::string("lights[" + std::to_string(i) + "].Linear").c_str(), linear);
		m_screenShader->setFloat(std::string("lights[" + std::to_string(i) + "].Quadratic").c_str(), quadratic);
	}
	m_screenShader->setVec3("viewPos", cameraPosition);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap->irradianceMap);

	auto quad = glm::mat4x4(1.0f);
	m_screenShader->setMat4("quad", quad);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer->m_gBuffer_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(
		0, 0, static_cast<int>(m_gBuffer->bufferSize.x), static_cast<int>(m_gBuffer->bufferSize.y),
		0, 0, static_cast<int>(m_gBuffer->bufferSize.x), static_cast<int>(m_gBuffer->bufferSize.y),
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Light Rendering Using Forward Rendering
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	m_shaderLightBox->Use();
	m_shaderLightBox->setMat4("projection", projection);
	m_shaderLightBox->setMat4("view", view);
	auto model = glm::mat4x4(1.0f);
	for (unsigned int i = 0; i < lightPositions.size(); i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPositions[i]);
		model = glm::scale(model, glm::vec3(0.25f));
		m_shaderLightBox->setMat4("model", model);
		m_shaderLightBox->setVec3("lightColor", lightColors[i]);
		m_cubeModel->Draw(*m_shaderLightBox);
	}

	glDepthFunc(GL_LEQUAL);
	m_shaderSkyBox->Use();
	m_shaderSkyBox->setMat4("projection", projection);
	m_shaderSkyBox->setMat4("view", glm::mat4(glm::mat3(view)));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap->irradianceMap);

	m_invertedCube->Draw(*m_shaderSkyBox);

	glDepthFunc(GL_LESS);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	// Copy the Framebuffer to texture
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_gBuffer->m_gBuffer_fbo);
	m_gBuffer->SetActiveTexture(GBuffer::GBUFFER_TEXTURE_TYPE_COMBINED);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 0, 0, static_cast<int>(m_gBuffer->bufferSize.x), static_cast<int>(m_gBuffer->bufferSize.y), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Make a initializer to be able to bypass PPS

	// Post Process
	m_postProcessChain->ApplyPostProcessChain();

	// Render to Screen
	m_postProcessChain->FinalBlitToScreen();

	if (_DebugMode)
	{
		DebugQuads(_DepthRange);
	}

}

void DeferredRenderer::Resize(const glm::vec2& _size)
{
	throw "FIX YO SHIT AND RESIZE EVERYTHING";

	m_gBuffer->Destroy();
	m_postProcessChain->Destroy();
	m_gBuffer->Create(_size);
	m_postProcessChain->Create(m_gBuffer);
}

void DeferredRenderer::DebugQuads(const glm::vec2& _DepthRange)
{
	// Debug View
	m_gBuffer->BindForReading();

	const float screenwidth = m_gBuffer->bufferSize.x;
	const float screenheight = m_gBuffer->bufferSize.y;

	float bufferSegments = (sizeof(m_gBuffer->m_gBuffer_textures) / sizeof(m_gBuffer->m_gBuffer_textures[0]));
	float segment = (screenwidth / bufferSegments) / screenwidth;
	float segmentHeight = (screenheight * segment) / screenheight;

	auto quad = glm::mat4x4(1.0f);

	for (unsigned int i = 0; i < bufferSegments; i++)
	{
		m_screenDebugShader->Use();
		m_screenDebugShader->setFloat("DebugPosition", i == 0);
		m_screenDebugShader->setFloat("DebugNormal", i == 1);
		m_screenDebugShader->setFloat("DebugAlbedo", i == 2);
		m_screenDebugShader->setFloat("DebugORM", i == 3);
		m_screenDebugShader->setFloat("DebugDepth", i == 4);
		m_screenDebugShader->setFloat("DebugCombined", i == 5);

		m_screenDebugShader->setVec2("DebugDepthRange", _DepthRange);

		quad = glm::mat4x4(1.0f);
		quad = glm::translate(quad, glm::vec3(segment + (segment * 2.0f * (i)) - 1.0f, segmentHeight - 1.0f, 0));
		quad = glm::scale(quad, glm::vec3(segment));
		m_screenDebugShader->setMat4("quad", quad);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}
