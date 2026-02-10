#pragma once

#include "ArcEngine/Platform/Interface/IWindow.h"

namespace ArcEngine
{
    class Window : public IWindow
    {
    public:
        bool Create(const WindowSpecification& _Specification) override;
        void Destroy() override;
    };
}