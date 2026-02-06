#include "MeshShaderRenderer.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "ArcEngine/OpenGL/Shader.h"


void MeshShaderRenderer::Create(std::shared_ptr<Window> _Window)
{
    screenQuad = std::make_unique<Shader>("Debug\\ScreenQuad.vert", "Debug\\ScreenQuad.frag");

    // texture size
    computeShader = std::make_unique<ComputeShader>("D:\\PersonalProjects\\Arcanum\\Source\\Shaders\\Source\\Shaders\\Compute\\TestCompute.comp");
    debugmeshShader = std::make_unique<MeshShader>(
        "D:\\PersonalProjects\\Arcanum\\Source\\Shaders\\Source\\Shaders\\MeshShaders\\TriangleDebug.comp",
        "D:\\PersonalProjects\\Arcanum\\Source\\Shaders\\Source\\Shaders\\MeshShaders\\TriangleDebug.frag"
        );

    meshShader = std::make_unique<MeshShader>(
        "D:\\PersonalProjects\\Arcanum\\Source\\Shaders\\Source\\Shaders\\MeshShaders\\MeshletShader.comp",
        "D:\\PersonalProjects\\Arcanum\\Source\\Shaders\\Source\\Shaders\\MeshShaders\\MeshletShader.frag"
    );


    window = _Window;

    screenQuad->Use();
    screenQuad->setInt("tex", 0);

    auto& size = window->GetScreenSize();

    // Create texture for opengl operation
    // -----------------------------------

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);


    if (quadVAO == 0)
    {
        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };
        // setup plane VAO
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
    
    int x;
    glGetIntegerv(GL_MAX_MESH_OUTPUT_VERTICES_NV, &x);
    std::printf("MAX MESH OUT VERTS: %i\n", x);
    glGetIntegerv(GL_MAX_MESH_OUTPUT_PRIMITIVES_NV, &x);
    std::printf("MAX MESH OUT PRIM: %i\n", x);

    glGetIntegerv(GL_MAX_MESH_WORK_GROUP_SIZE_NV, &x);
    std::printf("MAX WORK SIZE: %i\n", x);

}

void MeshShaderRenderer::Destroy()
{
}

void MeshShaderRenderer::BeginRender(const RenderParams& _Params) const
{
}

void MeshShaderRenderer::RenderMesh(const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, const glm::mat4& objectToWorld)
{
   /* float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    auto& size = window->GetScreenSize();

    computeShader->use();
    computeShader->setFloat("t", currentFrame);
    computeShader->setFloat("width", size.x);

    glDispatchCompute(size.x/10, size.y/10, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    screenQuad->Use();
    screenQuad->setMat4("quad", glm::mat4{ 1.0f });
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);*/

    
    //// DEbug
    //debugmeshShader->use();
    //uint32_t num_workgroups = 3;
    //glDrawMeshTasksNV(0, num_workgroups);


    glBindBuffer(GL_ARRAY_BUFFER, _Mesh.MSPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _Mesh.MSMeshletBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _Mesh.MSMeshletVerticiesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _Mesh.MSMeshletTrianglesBuffer);

    glDrawMeshTasksNV(0, _Mesh.MSMeshletBufferSize);
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

void MeshShaderRenderer::Resize(std::shared_ptr<Window> _Window)
{
    auto& size = window->GetScreenSize();
    
    glViewport(0, 0, size.x, size.y);

    glDeleteTextures(1, &texture);

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}
