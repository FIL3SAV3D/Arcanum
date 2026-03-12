#pragma once

#include <ArcEngine/Platform/Window.h>

#include <memory>

#include "ArcEngine/Asset/Types/Model.h"

namespace ArcEngine
{
    class IGraphics
    {
    public:
        virtual void Create(const Window& _Window) = 0;
        virtual void Destroy() = 0;
        virtual void Resize(const glm::uvec2& _Size) = 0;

        virtual void FrameStart(const Window& _Window) = 0;
        virtual void FrameEnd  (const Window& _Window) = 0;

        virtual void Blit() = 0;
        virtual void RenderMesh(const Model& _Model, const glm::mat4x4& _ObjectToWorld) = 0;
        virtual void RenderMeshInstanced() = 0;
        virtual void RenderMeshIndirect() = 0;

        virtual void CreateMesh(MeshCreationInfo& _Info)  {};
        virtual void DestroyMesh() {};

        virtual void CreateTexture() {};
        virtual void DestroyTexture() {};
    };
}