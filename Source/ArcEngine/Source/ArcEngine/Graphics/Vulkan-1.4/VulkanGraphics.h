#pragma once

#include "ArcEngine/Graphics/IGraphics.h"

class VulkanGraphics : public IGraphics
{
public:
    void Create() override;
    void Destroy() override;
    void Blit() override;
    void RenderMesh() override;
    void RenderMeshInstanced() override;
    void RenderMeshIndirect() override;
};