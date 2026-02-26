#pragma once

#include "ArcEngine/Platform/Window.h"

#include <memory>

#include "ArcEngine/Graphics/Interface/IGraphics.h"


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
        void Create(const GraphicsAPI& _graphicsAPI, std::shared_ptr<Window> _window);
        void Destroy();
        void Resize(const glm::uvec2& _Size);

        void Blit();
        void RenderMesh();
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