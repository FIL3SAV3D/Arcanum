#pragma once

#include "IRenderer.h"

#include <memory>
#include "ArcEngine/Asset/Types/ComputeShader.h"

class MeshShaderRenderer : public IRenderer
{
    // Inherited via IRenderer
    void Create(std::shared_ptr<Window> _Window) override;
    void Destroy() override;
    void BeginRender(const RenderParams& _Params) const override;
    void RenderMesh(const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, const glm::mat4& objectToWorld) override;
    void RenderMeshInstanced(const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, std::vector<glm::mat4> _InstanceData, const int& _InstanceCount) override;
    void EndRender(const RenderParams& _RParams) const override;
    void Blit() const override;
    void Resize(std::shared_ptr<Window> _Window) const override;

private:
    unsigned int quadVAO;
    unsigned int quadVBO;

    unsigned int texture;

    std::shared_ptr<Window> window;

    const unsigned int TEXTURE_WIDTH = 512, TEXTURE_HEIGHT = 512;
    std::unique_ptr<Shader> screenQuad;
    std::unique_ptr<ComputeShader> computeShader;
};