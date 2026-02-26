#pragma once

#include <ArcEngine/Platform/Window.h>

#include <memory>

#include "ArcEngine/Asset/Types/Mesh.h"

namespace ArcEngine
{
    class IGraphics
    {
    public:
        virtual void Create(std::shared_ptr<Window> _window) = 0;
        virtual void Destroy() = 0;
        virtual void Resize(const glm::uvec2& _Size) = 0;

        virtual void Blit() = 0;
        virtual void RenderMesh() = 0;
        virtual void RenderMeshInstanced() = 0;
        virtual void RenderMeshIndirect() = 0;

        virtual void CreateMesh(MeshCreationInfo& _Info)  {};
        virtual void DestroyMesh() {};

        virtual void CreateTexture() {};
        virtual void DestroyTexture() {};
    };
}