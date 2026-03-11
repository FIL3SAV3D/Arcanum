#pragma once

#include "ArcEngine/Platform/Window.h"

namespace ArcEngine
{
    class ArcEngine
    {
    public:
        ArcEngine()  = default;
        ~ArcEngine() = default;

       void Create();
       void Destroy();

    private:
        Window m_Window;
    };
}