#pragma once

#include "glm/glm.hpp"
#include <string>
#include "ArcEngine/Util/ArcEngineTypes.h"

struct GLFWwindow;

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

        GLFWwindow* GetNativeWindow() const { return window; }
        glm::uvec2 GetScreenSize()    const { return windowSize; }
        void SetScreenSize(const glm::uvec2 _Size) {  windowSize = _Size; }

    protected:
        GLFWwindow* window = nullptr;
        glm::uvec2 windowSize{};
    };
}