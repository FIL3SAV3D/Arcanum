#include "Window.h"

#include <glad/glad.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

bool ArcEngine::Window::Create(const WindowSpecification& _Specification)
{
    const EngineGraphicsAPI& _graphicsAPI = _Specification.graphicsAPI;
    const std::string&       _windowName  = _Specification.windowName;
    const glm::uvec2&        _windowSize  = _Specification.windowSize;
    
    glfwInit();

    // Switch For Hints
    switch (_graphicsAPI)
    {
    case ArcEngine::VULKAN:
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        break;
    } // Vulkan
    case ArcEngine::OPENGL:
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 1);

        break;
    } // OpenGL
    default:
    {
        assert(false && "API NOT IMPLEMENTED YET");
        break;
    } // Non-Supported API
    }

    window = glfwCreateWindow(_windowSize.x, _windowSize.y, _windowName.c_str(), nullptr, nullptr);

    assert(window != nullptr && "Failed to create GLFW window");

    switch (_graphicsAPI)
    {
    case ArcEngine::VULKAN:
    {
        break;
    } // Vulkan
    case ArcEngine::OPENGL:
    {
        glfwMakeContextCurrent(window);

        assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) && "Failed to initialize GLAD");

        //VSYNC
        glfwSwapInterval(false);

        break;
    } // OpenGL
    default:
    {
        assert(false && "API NOT IMPLEMENTED YET");
        break;
    } // Non-Supported API
    }

    windowSize = _windowSize;

    return true;
}

void ArcEngine::Window::Destroy()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}