#pragma once

#include "ArcEngine/Graphics/Graphics.h"
#include "ArcEngine/Platform/Window.h"

namespace ArcEngine
{
    class UIGraphics
    {
    public:
        UIGraphics()  = default;
        ~UIGraphics() = default;

    public:
        void Create(const Graphics::GraphicsAPI _GraphicsAPI, const Window& _Window);
        void Destroy(const Window& _Window);

        void FrameStart(const Window& _Window);
        void FrameEnd  (const Window& _Window);

    private:
        Graphics::GraphicsAPI m_graphicsAPI{};
    };
}