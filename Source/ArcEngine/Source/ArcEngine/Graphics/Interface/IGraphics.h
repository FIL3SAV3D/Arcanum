#pragma once

namespace ArcEngine
{
    class IGraphics
    {
    public:
        virtual void Create() = 0;
        virtual void Destroy() = 0;

        virtual void Blit() = 0;
        virtual void RenderMesh() = 0;
        virtual void RenderMeshInstanced() = 0;
        virtual void RenderMeshIndirect() = 0;
    };
}