#pragma once

#include "IRenderer.h"
#include "OpenGL/GBuffer.h"

#include <memory.h>

#include "OpenGL/Shader.h"

class DeferredRenderer : public IRenderer
{
public:
    // Inherited via IRenderer
    void Create(std::shared_ptr<Window> _Window) override;
    void Destroy() override;
    void BeginRender(const RenderParams& _Params) const override;
    void RenderModel(const Model& model) override;
    void RenderMesh(const Mesh& mesh) override;
    void RenderMeshInstanced(const Mesh& mesh) override;
    void EndRender(std::shared_ptr<Window> _Window) const override;
    void Blit() const override;
    void Resize(std::shared_ptr<Window> _Window) const override;

private:
    std::unique_ptr<GBuffer> m_GBuffer;

    std::unique_ptr<Shader> m_GBufferPass;
    std::unique_ptr<Shader> m_ScreenPass;
};