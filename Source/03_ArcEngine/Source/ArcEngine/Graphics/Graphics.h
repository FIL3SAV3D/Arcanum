#pragma once

#include "ArcEngine/Platform/Window.h"

#include <memory>

#include "ArcEngine/Graphics/Interface/IGraphics.h"
#include "CameraData.h"


struct SDL_Window;

namespace ArcEngine
{
    class Graphics
    {
    public:
        enum GraphicsAPI
        {
            VULKAN,
            OPENGL
        };

    public:
        void Create(const GraphicsAPI& _GraphicsAPI, const Window& _Window);
        void Destroy();

        void UpdateCameraData(const glm::vec4& _Position, const glm::mat4& _View, const glm::mat4& _Projection);

        void FrameStart(const Window& _Window);
        void FrameEnd  (const Window& _Window);

        void Resize(const glm::uvec2& _Size);

        void Blit();
        void RenderMesh(std::shared_ptr<Model> _Model, const glm::mat4x4& _ObjectToWorld);
        void RenderMeshInstanced();
        void RenderMeshIndirect();

    protected:
        void CreateShader();
        void DestroyShader();

        void CreateMesh(MeshCreationInfo& _Info);
        void DestroyMesh();

        void CreateMeshlet();
        void DestroyMeshlet();

    private:
        std::unique_ptr<IGraphics> graphics;

        friend class MeshletFactory;
    };
}