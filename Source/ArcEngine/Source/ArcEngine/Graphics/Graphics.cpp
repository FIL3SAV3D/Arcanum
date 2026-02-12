#include "Graphics.h"

#include "ArcEngine/Graphics/Vulkan-1.4/VulkanGraphics.h"
#include "ArcEngine/Graphics/OpenGL-4.6/OpenGLGraphics.h"

#include <cassert>

void ArcEngine::Graphics::Create(const GraphicsAPI& _graphicsAPI)
{
    switch (_graphicsAPI)
    {
    case Graphics::VULKAN:
    {
        graphics = std::make_unique<VulkanGraphics>();
        break;
    }
    case Graphics::OPENGL:
    {
        assert(false && "Not Yet Implemented");
        break;
    }
    default:
    {
        assert(false && "No API supported");
        break;
    }
    }

    graphics->Create();
}

void ArcEngine::Graphics::Destroy()
{
    graphics->Destroy();

    graphics.reset();
}

void ArcEngine::Graphics::Blit()
{
    graphics->Blit();
}

void ArcEngine::Graphics::RenderMesh()
{
    graphics->RenderMesh();
}

void ArcEngine::Graphics::RenderMeshInstanced()
{
    graphics->RenderMeshIndirect();
}

void ArcEngine::Graphics::RenderMeshIndirect()
{
    graphics->RenderMeshInstanced();
}