#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DeferredRenderer.h"

#include "ArcEngine/OpenGL/Window.h"
#include <random>

void DeferredRenderer::Create(std::shared_ptr<Window> _Window)
{
    window = _Window;

    m_GBuffer = std::make_unique<GBuffer>();
    m_GBuffer->Create(window->GetScreenSize());

    m_GBufferPass = std::make_unique<Shader>("Deferred\\PBR\\DeferredPBRGeometryPassNoTextures.vert", "Deferred\\PBR\\DeferredPBRGeometryPassNoTextures.frag");
    m_ScreenPass  = std::make_unique<Shader>("Deferred\\PBR\\DeferredPBRScreenPass.vert", "Deferred\\PBR\\DeferredPBRScreenPass.frag");
    m_ScreenPass->Use();
    m_ScreenPass->setInt("gPosition", 0);
    m_ScreenPass->setInt("gNormal", 1);
    m_ScreenPass->setInt("gAlbedo", 2);
    m_ScreenPass->setInt("gORM", 3);
    m_ScreenPass->setInt("gDepth", 4);
    m_ScreenPass->setInt("shadowMap", 5);
    m_ScreenPass->setInt("irradianceMap", 6);

    m_LightBoxShader = std::make_unique<Shader>("ShaderLightBox.vert", "ShaderLightBox.frag");

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

    const unsigned int NR_LIGHTS = 128;
	
    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
    std::uniform_real_distribution<float> randColor(1.0f, 100.0f);

	for (unsigned int i = 0; i < NR_LIGHTS; i++)
	{
		lightPositions.push_back(glm::vec3(randPosition(generator), randPosition(generator), randPosition(generator)));
		lightColors.push_back(glm::vec3(randColor(generator), randColor(generator), randColor(generator)));
	}
}

void DeferredRenderer::Destroy()
{
    m_GBufferPass.reset();
    m_ScreenPass.reset();

    m_GBuffer->Destroy();
}

void DeferredRenderer::BeginRender(const RenderParams& _RParams) const
{
    const glm::uvec2& bufferSize = m_GBuffer->GetBufferSize();
    glViewport(0, 0, bufferSize.x, bufferSize.y);
    
    m_GBuffer->BindForWriting();

    glDepthMask(GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Camera* camera = _RParams.camera;

    m_GBufferPass->Use();
    m_GBufferPass->setMat4("projection", camera->GetProjectionMatrix());
    m_GBufferPass->setMat4("view", camera->GetViewMatrix());
}

void DeferredRenderer::RenderMesh(const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, const glm::mat4& objectToWorld)
{
    m_GBufferPass->setMat4("model", objectToWorld);
    m_GBufferPass->setVec4("color", glm::vec4(1.0f));
    m_GBufferPass->setVec3("ORM", glm::vec3(0.5f, 0.5f, 0.5f));

    glBindVertexArray(_Mesh.VAO);
    glDrawElements(GL_TRIANGLES, _Mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void DeferredRenderer::RenderMeshInstanced(const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, std::vector<glm::mat4> _InstanceData, const int& _InstanceCount)
{
}

void DeferredRenderer::EndRender(const RenderParams& _RParams) const
{
    //	glDepthMask(GL_FALSE);
//
//	glDepthFunc(GL_LEQUAL);
//	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
//
//	m_shaderSkyBox->Use();
//	m_shaderSkyBox->setMat4("projection", projection);
//	m_shaderSkyBox->setMat4("view", glm::mat4(glm::mat3(view)));
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap->envCubemap);
//
//	m_invertedCube->Draw(m_shaderSkyBox.get());
//
//	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
//	glDepthFunc(GL_LESS);
//
    glDisable(GL_DEPTH_TEST);

    m_GBuffer->BindForReading();
    glClear(GL_COLOR_BUFFER_BIT);

    m_ScreenPass->Use();

    for (unsigned int i = 0; i < lightPositions.size(); i++)
	{
        m_ScreenPass->setVec3(std::string("lights[" + std::to_string(i) + "].Position").c_str(), lightPositions[i]);
        m_ScreenPass->setVec3(std::string("lights[" + std::to_string(i) + "].Color").c_str(), lightColors[i]);
		// update attenuation parameters and calculate radius
		const float linear = 1.0f;
		const float quadratic = 1.8f;
        m_ScreenPass->setFloat(std::string("lights[" + std::to_string(i) + "].Linear").c_str(), linear);
        m_ScreenPass->setFloat(std::string("lights[" + std::to_string(i) + "].Quadratic").c_str(), quadratic);
	}
    m_ScreenPass->setVec3("viewPos", _RParams.camera->GetPosition());

    m_ScreenPass->setMat4("quad", glm::mat4{1.0f});
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    auto& size = m_GBuffer->GetBufferSize();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer->GetFBO());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(
		0, 0, size.x, size.y,
		0, 0, size.x, size.y,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Light Rendering Using Forward Rendering
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
    m_LightBoxShader->Use();
    m_LightBoxShader->setMat4("projection", _RParams.camera->GetProjectionMatrix());
    m_LightBoxShader->setMat4("view", _RParams.camera->GetViewMatrix());
	auto model = glm::mat4x4(1.0f);
	for (unsigned int i = 0; i < lightPositions.size(); i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPositions[i]);
		model = glm::scale(model, glm::vec3(0.25f));
        m_LightBoxShader->setMat4("model", model);
        m_LightBoxShader->setVec3("lightColor", lightColors[i]);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
	}

    //auto& bufferSize = m_GBuffer->GetBufferSize();

    //glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer->GetFBO());
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    //glBlitFramebuffer(
    //0, 0, bufferSize.x, bufferSize.y,
    //0, 0, bufferSize.x, bufferSize.y,
    //GL_COLOR_BUFFER_BIT, GL_NEAREST);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glfwSwapBuffers(window->GetNativeWindow());
}

void DeferredRenderer::Blit() const
{
}

void DeferredRenderer::Resize(std::shared_ptr<Window> _Window)
{
    m_GBuffer->Destroy();
    m_GBuffer->Create(_Window->GetScreenSize());
}