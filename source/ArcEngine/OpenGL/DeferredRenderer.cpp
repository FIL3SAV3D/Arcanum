#include "DeferredRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

void DeferredRenderer::Initialize(const int& screen_width, const int& screen_height)
{
	m_gBuffer = std::make_shared<GBuffer>();
	m_gBuffer->Init(screen_width, screen_height);

	m_gBufferShader = std::make_shared<Shader>("DSGeometry");
	m_gBufferShader->Use();
	m_gBufferShader->setInt("texture_diffuse1", 0);

	m_screenShader = std::make_shared<Shader>("DSToScreen.vert", "DSToScreen.frag");

	m_debugScreenNormalShader = std::make_shared<Shader>("DSToScreen.vert", "DSToScreenNormals.frag");
	m_debugScreenDiffuseShader = std::make_shared<Shader>("DSToScreen.vert", "DSToScreenDiffuse.frag");
	m_debugScreenPositionShader = std::make_shared<Shader>("DSToScreen.vert", "DSToScreenPosition.frag");

	m_screenShader->Use();
	m_screenShader->setInt("gPosition", 0);
	m_screenShader->setInt("gNormal", 1);
	m_screenShader->setInt("gAlbedoSpec", 2);

	m_debugScreenNormalShader->Use();
	m_debugScreenNormalShader->setInt("gPosition", 0);
	m_debugScreenNormalShader->setInt("gNormal", 1);
	m_debugScreenNormalShader->setInt("gAlbedoSpec", 2);

	m_debugScreenDiffuseShader->Use();
	m_debugScreenDiffuseShader->setInt("gPosition", 0);
	m_debugScreenDiffuseShader->setInt("gNormal", 1);
	m_debugScreenDiffuseShader->setInt("gAlbedoSpec", 2);
	
	m_debugScreenPositionShader->Use();
	m_debugScreenPositionShader->setInt("gPosition", 0);
	m_debugScreenPositionShader->setInt("gNormal", 1);
	m_debugScreenPositionShader->setInt("gAlbedoSpec", 2);

	m_RatModel = std::make_shared<Model>(std::string("F:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\rock.obj").c_str());

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

	const unsigned int NR_LIGHTS = 32;
	
	srand(13);
	for (unsigned int i = 0; i < NR_LIGHTS; i++)
	{
		// calculate slightly random offsets
		float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
		float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
		float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// also calculate random color
		float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
		float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
		float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}

	BC = TextureFromFile("rock.png", "F:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\");
}

void DeferredRenderer::RenderSceneCB(const glm::mat4& projection, const glm::mat4x4& view, const glm::vec3& cameraPosition)
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
	
	for (int i = 0; i < 4; i++)
	{
		auto model = glm::mat4x4(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, -i));
		model = glm::scale(model, glm::vec3(0.1f));
		m_gBufferShader->setMat4("model", model);
		m_gBufferShader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		m_RatModel->Draw(*m_gBufferShader);
	}
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
		const float linear = 0.7f;
		const float quadratic = 1.8f;
		m_screenShader->setFloat(std::string("lights[" + std::to_string(i) + "].Linear").c_str(), linear);
		m_screenShader->setFloat(std::string("lights[" + std::to_string(i) + "].Quadratic").c_str(), quadratic);
	}
	m_screenShader->setVec3("viewPos", cameraPosition);

	auto quad = glm::mat4x4(1.0f);
	m_screenShader->setMat4("quad", quad);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_debugScreenPositionShader->Use();
	quad = glm::mat4x4(1.0f);
	quad = glm::translate(quad, glm::vec3(((200.0f/800.0f) - 1), (150.0f / 600.0f) - 1.0f, 0));
	quad = glm::scale(quad, glm::vec3(0.25f));
	m_debugScreenPositionShader->setMat4("quad", quad);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_debugScreenNormalShader->Use();
	quad = glm::mat4x4(1.0f);
	quad = glm::translate(quad, glm::vec3((-(200.0f / 800.0f)), (150.0f / 600.0f) - 1.0f, 0));
	quad = glm::scale(quad, glm::vec3(0.25f));
	m_debugScreenNormalShader->setMat4("quad", quad);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_debugScreenDiffuseShader->Use();
	quad = glm::mat4x4(1.0f);
	quad = glm::translate(quad, glm::vec3(((200.0f / 800.0f)), (150.0f / 600.0f) - 1.0f, 0));
	quad = glm::scale(quad, glm::vec3(0.25f));
	m_debugScreenDiffuseShader->setMat4("quad", quad);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_screenShader->Use();
	for (unsigned int i = 0; i < lightPositions.size(); i++)
	{
		m_screenShader->setVec3(std::string("lights[" + std::to_string(i) + "].Position").c_str(), lightPositions[i]);
		m_screenShader->setVec3(std::string("lights[" + std::to_string(i) + "].Color").c_str(), lightColors[i]);
		// update attenuation parameters and calculate radius
		const float linear = 0.7f;
		const float quadratic = 1.8f;
		m_screenShader->setFloat(std::string("lights[" + std::to_string(i) + "].Linear").c_str(), linear);
		m_screenShader->setFloat(std::string("lights[" + std::to_string(i) + "].Quadratic").c_str(), quadratic);
	}
	m_screenShader->setVec3("viewPos", cameraPosition);
	quad = glm::mat4x4(1.0f);
	quad = glm::translate(quad, glm::vec3((1 - (200.0f / 800.0f)), (150.0f / 600.0f) - 1.0f, 0));
	quad = glm::scale(quad, glm::vec3(0.25f));
	m_screenShader->setMat4("quad", quad);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
