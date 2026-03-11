#pragma once

#include "IRenderer.h"

#include <memory>
#include "ArcEngine/Asset/Types/ComputeShader.h"
#include "ArcEngine/Asset/Types/MeshShader.h"
#include <ArcEngine/Asset/Types/ClusterModel.h>

class MeshShaderRenderer : public IRenderer
{
public:
    // Inherited via IRenderer
    void Create(std::shared_ptr<ArcEngine::Window> _Window) override;
    void Destroy() override;
    void BeginRender(const RenderParams& _Params) const override;
    void RenderMesh(const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, const glm::mat4& objectToWorld) override;
    void RenderMeshInstanced(const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, std::vector<glm::mat4> _InstanceData, const int& _InstanceCount) override;
    void EndRender(const RenderParams& _RParams) const override;
    void Blit() const override;
    void Resize(std::shared_ptr<ArcEngine::Window> _Window) override;

    void RenderClusterMesh(const RenderParams& _RParams, const ClusterModel& _Mesh, const int& _SubMeshIndex, const glm::mat4& objectToWorld);

    struct uniforms
    {
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;
    };

private:
    float deltaTime = 0.0f; // time between current frame and last frame
    float lastFrame = 0.0f; // time of last frame
    int fCounter = 0;

    unsigned int ssbo;

    unsigned int quadVAO;
    unsigned int quadVBO;

    unsigned int texture;

    std::shared_ptr<ArcEngine::Window> window;

    std::unique_ptr<Shader> screenQuad;
    std::unique_ptr<ComputeShader> computeShader;
    std::unique_ptr<MeshShader> debugmeshShader;
    std::unique_ptr<MeshShader> meshShader;
};