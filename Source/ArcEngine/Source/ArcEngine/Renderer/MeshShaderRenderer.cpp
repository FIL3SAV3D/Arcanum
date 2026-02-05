#include "MeshShaderRenderer.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "ArcEngine/OpenGL/Shader.h"


void MeshShaderRenderer::Create(std::shared_ptr<Window> _Window)
{
    screenQuad = std::make_unique<Shader>("Debug\\ScreenQuad.vert", "Debug\\ScreenQuad.frag");

    // texture size
    computeShader = std::make_unique<ComputeShader>("D:\\PersonalProjects\\Arcanum\\Source\\Shaders\\Source\\Shaders\\Compute\\TestCompute.comp");

    window = _Window;

    screenQuad->Use();
    screenQuad->setInt("tex", 0);

    // Create texture for opengl operation
    // -----------------------------------
    unsigned int texture;

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);


    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    
}

void MeshShaderRenderer::Destroy()
{
}

void MeshShaderRenderer::BeginRender(const RenderParams& _Params) const
{
}

void MeshShaderRenderer::RenderMesh(const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, const glm::mat4& objectToWorld)
{
    computeShader->use();

    auto& size = window->GetScreenSize();

    glDispatchCompute(size.x, size.y, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    screenQuad->Use();

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void MeshShaderRenderer::RenderMeshInstanced(const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, std::vector<glm::mat4> _InstanceData, const int& _InstanceCount)
{
}

void MeshShaderRenderer::EndRender(const RenderParams& _RParams) const
{
    glfwSwapBuffers(window->GetNativeWindow());
}

void MeshShaderRenderer::Blit() const
{
}

void MeshShaderRenderer::Resize(std::shared_ptr<Window> _Window) const
{
}
