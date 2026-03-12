#pragma once

#include "ArcEngine/Platform/Window.h"
#include "ArcEngine/Graphics/Graphics.h"
#include "ArcEngine/Graphics/UIGraphics.h"

#include "ArcEngine/Asset/Types/Model.h"

namespace ArcEngine
{
    class ArcEngine
    {
    public:
        ArcEngine()  = default;
        ~ArcEngine() = default;

        void Create();
        void Destroy();

        void RenderMesh(const Model& _Model, const glm::mat4x4& _ObjectToWorld);

        void RenderStart();
        void RenderEnd();

        void RenderUIStart();
        void RenderUIEnd();

        void SwapFrame();

    private:
        Window m_Window;
        Graphics m_Graphics;
        UIGraphics m_UIGraphics;
    };
}