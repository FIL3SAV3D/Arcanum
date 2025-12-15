#include "DeferredRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void DeferredRenderer::Initialize(const int& screen_width, const int& screen_height)
{
	m_gBuffer = std::make_shared<GBuffer>();
	m_gBuffer->Init(screen_width, screen_height);

	m_gBufferShader = std::make_shared<Shader>("DSGeometry");
	m_screenShader = std::make_shared<Shader>("DSToScreen");

	m_RatModel = std::make_shared<Model>(std::string("D:\\PersonalProjects\\Arcanum\\Data\\Models\\GLB_Models\\Rat.glb").c_str());

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
}

void DeferredRenderer::RenderSceneCB(const glm::mat4& projection, const glm::mat4x4& view)
{
	m_gBuffer->BindForWriting();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	m_gBufferShader->Use();

	m_gBufferShader->setMat4("view", view);
	m_gBufferShader->setMat4("projection", projection);

	auto model = glm::mat4(1.0f);
	m_gBufferShader->setMat4("model", model);
	m_gBufferShader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));

	m_RatModel->Draw(*m_gBufferShader);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);
	m_gBuffer->BindForReading();

	m_gBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, 800, 600, 0, 0, 400, 300, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	//m_screenShader->Use();
	//glBindVertexArray(quadVAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
}
