#pragma once


#include <memory>

namespace ArcEngine
{
    class IGraphics;

    class Graphics
    {
    public:
        enum GraphicsAPI
        {
            VULKAN,
            OPENGL
        };

    public:
        void Create(const GraphicsAPI& _graphicsAPI);
        void Destroy();

        void Blit();
        void RenderMesh();
        void RenderMeshInstanced();
        void RenderMeshIndirect();

    private:
        std::unique_ptr<IGraphics> graphics;
    };
}