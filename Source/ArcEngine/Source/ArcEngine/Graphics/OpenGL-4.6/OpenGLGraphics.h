#pragma once

#include "ArcEngine/Graphics/Interface/IGraphics.h"

#include <SDL3/SDL_video.h>

namespace ArcEngine
{
    class OpenGLGraphics : public IGraphics
    {
    public:
        void Create(std::shared_ptr<Window> _window) override;
        void Destroy() override;
        void Resize(const glm::uvec2& _Size) override;

        void Blit() override;
        void RenderMesh() override;
        void RenderMeshInstanced() override;
        void RenderMeshIndirect() override;

    private:
        SDL_GLContext glContext;

    private:
        std::shared_ptr<Window> window;
    };
}