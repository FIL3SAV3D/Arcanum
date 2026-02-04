#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DeferredRenderer.h"

#include "OpenGL/Window.h"

void DeferredRenderer::Create(std::shared_ptr<Window> _Window)
{
    m_GBuffer = std::make_unique<GBuffer>();
    m_GBuffer->Create(_Window->GetScreenSize());

    m_GBufferPass = std::make_unique<Shader>("Deferred\\PBR\\DeferredPBRGeometryPass.vert", "Deferred\\PBR\\DeferredPBRGeometryPass.frag");
    m_ScreenPass  = std::make_unique<Shader>("Deferred\\PBR\\DeferredPBRScreenPass.vert", "Deferred\\PBR\\DeferredPBRScreenPass.frag");
}

void DeferredRenderer::Destroy()
{
    m_GBufferPass.reset();
    m_ScreenPass.reset();

    m_GBuffer->Destroy();
}

void DeferredRenderer::BeginRender(const RenderParams& _camera) const
{
    const glm::uvec2& bufferSize = m_GBuffer->GetBufferSize();
    glViewport(0, 0, bufferSize.x, bufferSize.y);
    
    m_GBuffer->BindForWriting();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_GBufferPass->Use();
    //m_GBufferPass->setMat4("view", _camera.GetViewMatrix());
    //m_GBufferPass->setMat4("projection", _camera.GetProjectionMatrix());
}

void DeferredRenderer::RenderModel(const Model& model)
{
    for (const Mesh& mesh : model.meshes)
    {
        RenderMesh(mesh);
    }
}

void DeferredRenderer::RenderMesh(const Mesh& mesh)
{
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void DeferredRenderer::RenderMeshInstanced(const Mesh& mesh)
{
}

void DeferredRenderer::EndRender(std::shared_ptr<Window> _Window) const
{
    m_ScreenPass->Use();

    glfwSwapBuffers(_Window->GetNativeWindow());
}

void DeferredRenderer::Blit() const
{
}

void DeferredRenderer::Resize(std::shared_ptr<Window> _Window) const
{
    m_GBuffer->Destroy();
    m_GBuffer->Create(_Window->GetScreenSize());
}