#pragma once

#include "IRenderer.h"

#include "ArcEngine/Graphics/OpenGL-4.6/GBuffer.h"
#include "ArcEngine/Graphics/OpenGL-4.6/Shader.h"

class DeferredRenderer : public IRenderer
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

private:
    std::shared_ptr<ArcEngine::Window> window;

    unsigned int quadVAO;
    unsigned int quadVBO;

    std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;

    std::unique_ptr<GBuffer> m_GBuffer;

    std::unique_ptr<Shader> m_GBufferPass;
    std::unique_ptr<Shader> m_ScreenPass;

    std::unique_ptr<Shader> m_LightBoxShader;
};