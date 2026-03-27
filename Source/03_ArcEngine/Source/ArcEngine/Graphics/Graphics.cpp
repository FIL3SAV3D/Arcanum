#include "Graphics.h"

#include "ArcEngine/Graphics/Vulkan-1.4/VulkanGraphics.h"
#include "ArcEngine/Graphics/OpenGL-4.6/OpenGLGraphics.h"

#include "ArcEngine/Graphics/Interface/IGraphics.h"

#include <cassert>
#include <ArcEngine/Asset/Types/Model.h>

void ArcEngine::Graphics::Create(const GraphicsAPI& _GraphicsAPI, const Window& _Window)
{
    switch (_GraphicsAPI)
    {
    case Graphics::VULKAN:
    {
        graphics = std::make_unique<VulkanGraphics>();
        break;
    }
    case Graphics::OPENGL:
    {
        graphics = std::make_unique<OpenGLGraphics>();
        break;
    }
    default:
    {
        assert(false && "No API supported");
        break;
    }
    }

    graphics->Create(_Window);
}

void ArcEngine::Graphics::Destroy()
{
    graphics->Destroy();

    graphics.reset();
}

void ArcEngine::Graphics::FrameStart(const Window& _Window)
{
    graphics->FrameStart(_Window);
}

void ArcEngine::Graphics::FrameEnd(const Window& _Window)
{
    graphics->FrameEnd(_Window);
}

void ArcEngine::Graphics::Resize(const glm::uvec2& _Size)
{
    graphics->Resize(_Size);
}

void ArcEngine::Graphics::Blit()
{
    graphics->Blit();
}

void ArcEngine::Graphics::UpdateCameraData(const glm::vec4& _Position, const glm::mat4& _View, const glm::mat4& _Projection)
{
    graphics->UpdateCameraData(_Position, _View, _Projection);
}

void ArcEngine::Graphics::RenderMesh(std::shared_ptr<Model> _Model, const glm::mat4x4& _ObjectToWorld)
{
    graphics->RenderMesh(_Model, _ObjectToWorld);
}

void ArcEngine::Graphics::RenderMeshInstanced()
{
    graphics->RenderMeshIndirect();
}

void ArcEngine::Graphics::RenderMeshIndirect()
{
    graphics->RenderMeshInstanced();
}

void ArcEngine::Graphics::CreateShader()
{

}

void ArcEngine::Graphics::DestroyShader()
{

}

void ArcEngine::Graphics::CreateMesh(MeshCreationInfo& _Info)
{
    graphics->CreateMesh(_Info);
}