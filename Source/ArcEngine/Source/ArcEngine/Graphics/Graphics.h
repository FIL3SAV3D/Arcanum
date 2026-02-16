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

        void Blit();
        void RenderMesh();
        void RenderMeshInstanced();
        void RenderMeshIndirect();

    private:
        std::unique_ptr<IGraphics> graphics;
    };
}