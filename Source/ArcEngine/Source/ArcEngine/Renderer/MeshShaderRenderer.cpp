#include "MeshShaderRenderer.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "ArcEngine/Graphics/OpenGL-4.6/Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <SDL3/SDL_video.h>


void MeshShaderRenderer::Create(std::shared_ptr<ArcEngine::Window> _Window)
{
    screenQuad = std::make_unique<Shader>("Debug\\ScreenQuad.vert", "Debug\\ScreenQuad.frag");

    // texture size
    //computeShader = std::make_unique<ComputeShader>("D:\\PersonalProjects\\Arcanum\\Source\\Shaders\\Source\\Shaders\\Compute\\TestCompute.comp");
    // 
    debugmeshShader = std::make_unique<MeshShader>(
        "D:\\PersonalProjects\\Arcanum\\Source\\Shaders\\ShadersOpenGL\\Source\\Shaders\\MeshShaders\\TriangleDebug.comp",
        "D:\\PersonalProjects\\Arcanum\\Source\\Shaders\\ShadersOpenGL\\Source\\Shaders\\MeshShaders\\TriangleDebug.frag"
        );

    meshShader = std::make_unique<MeshShader>(
        "D:\\PersonalProjects\\Arcanum\\Source\\Shaders\\ShadersOpenGL\\Source\\Shaders\\MeshShaders\\MeshletShader.comp",
        "D:\\PersonalProjects\\Arcanum\\Source\\Shaders\\ShadersOpenGL\\Source\\Shaders\\MeshShaders\\MeshletShader.frag"
    );


    window = _Window;

    screenQuad->Use();
    screenQuad->setInt("tex", 0);

    auto size = window->GetScreenSize();

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
    
    auto version = glGetString(GL_VERSION);

    std::printf("Version: %s\n", version);

    int x;
    glGetIntegerv(GL_NUM_EXTENSIONS, &x);
    std::printf("NUM OF EXTENSIONS: %i\n", x);

    glGetIntegerv(GL_MAX_MESH_OUTPUT_VERTICES_NV, &x);
    std::printf("MAX MESH OUT VERTS: %i\n", x);
    glGetIntegerv(GL_MAX_MESH_OUTPUT_PRIMITIVES_NV, &x);
    std::printf("MAX MESH OUT PRIM: %i\n", x);

    

    glm::ivec3 data;
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &data.x);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &data.y);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &data.z);
    std::printf("GL_MAX_COMPUTE_WORK_GROUP_SIZE: %i * %i * %i\n", data.x, data.y, data.z);

    glGetIntegeri_v(GL_MAX_MESH_WORK_GROUP_SIZE_EXT, 0, &data.x);
    glGetIntegeri_v(GL_MAX_MESH_WORK_GROUP_SIZE_EXT, 1, &data.y);
    glGetIntegeri_v(GL_MAX_MESH_WORK_GROUP_SIZE_EXT, 2, &data.z);
    std::printf("GL_MAX_MESH_WORK_GROUP_SIZE_EXT: %i * %i * %i\n", data.x, data.y, data.z);

    glGetIntegerv(GL_MESH_VERTICES_OUT_NV, &x);
    std::printf("MAX Vert SIZE: %i\n", x);

    glGetIntegerv(GL_MESH_PRIMITIVES_OUT_NV, &x);
    std::printf("MAX PRIM SIZE: %i\n", x);

    uniforms init{};

    glGenBuffers(1, &ssbo);

    glBindBuffer(GL_UNIFORM_BUFFER, ssbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniforms), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ssbo, 0, sizeof(uniforms));
}

void MeshShaderRenderer::Destroy()
{
    glDeleteBuffers(sizeof(uniforms), &ssbo);
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

    



    
}

void MeshShaderRenderer::RenderMeshInstanced(const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, std::vector<glm::mat4> _InstanceData, const int& _InstanceCount)
{
}

void MeshShaderRenderer::EndRender(const RenderParams& _RParams) const
{
    //glfwSwapBuffers(window->GetNativeWindow());
}

void MeshShaderRenderer::Blit() const
{
}

void MeshShaderRenderer::Resize(std::shared_ptr<ArcEngine::Window> _Window)
{
    auto size = window->GetScreenSize();
    
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

void MeshShaderRenderer::RenderClusterMesh(const RenderParams& _RParams, const ClusterModel& _Mesh, const int& _SubMeshIndex, const glm::mat4& objectToWorld)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glDepthMask(GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    //// Debug
    //debugmeshShader->use();
    //uint32_t num_workgroups = 3;
    //glDrawMeshTasksNV(0, num_workgroups);


    meshShader->use();

    meshShader->setMat4("projection", _RParams.camera->GetProjectionMatrix());
    meshShader->setMat4("view", _RParams.camera->GetViewMatrix());
    meshShader->setMat4("model", objectToWorld);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _Mesh.VBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _Mesh.VBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _Mesh.MBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _Mesh.MBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _Mesh.MVBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, _Mesh.MVBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _Mesh.MTBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, _Mesh.MTBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glDrawMeshTasksNV(0, _Mesh.meshletCount);


}