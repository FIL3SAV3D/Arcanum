#pragma once

#include "glm/glm.hpp"
#include <string>
#include "ArcEngine/Util/ArcEngineTypes.h"

struct SDL_Window;

namespace ArcEngine
{
    struct WindowSpecification
    {
        glm::uvec2 windowSize;
        std::string windowName;
        EngineGraphicsAPI graphicsAPI;
    };

    class IWindow
    {
    public:
        virtual bool Create (const WindowSpecification& _Specification) = 0;
        virtual void Destroy() = 0;

        SDL_Window* GetNativeWindow() const { return window; }
        glm::uvec2 GetScreenSize()    const { return windowExtent; }
        void SetScreenSize(const glm::uvec2 _Size) { windowExtent = _Size; }

    protected:
        struct SDL_Window* window{ nullptr };
        glm::uvec2 windowExtent{};
    };
}