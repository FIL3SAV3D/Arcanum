#pragma once

#include "ArcEngine/Graphics/Interface/IGraphics.h"

#include "ArcEngine/Graphics/Vulkan-1.4/VulkanSwapchain.h"

namespace ArcEngine
{
    class VulkanGraphics : public IGraphics
    {
    public:
        void Create() override;
        void Destroy() override;
        void Blit() override;
        void RenderMesh() override;
        void RenderMeshInstanced() override;
        void RenderMeshIndirect() override;

    private:
        VulkanSwapchain swapchain;
    };
}