#include "Window.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_vulkan.h>
#include <SDL3/SDL_opengl.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>



bool ArcEngine::Window::Create(const WindowSpecification& _Specification)
{
    const EngineGraphicsAPI& _graphicsAPI = _Specification.graphicsAPI;
    const std::string&       _windowName  = _Specification.windowName;
    const glm::uvec2&        _windowExtent  = _Specification.windowSize;
    
    // We initialize SDL and create a window with it.
    SDL_Init(SDL_INIT_VIDEO);

    SDL_WindowFlags window_flags{};

    switch (_graphicsAPI)
    {
    case VULKAN:
    {
        window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);
        break;
    }
    case OPENGL:
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

        SDL_GL_LoadLibrary(NULL);
        break;
    }
    default:
        break;
    }

    window = SDL_CreateWindow(
        _windowName.c_str(),
        _windowExtent.x,
        _windowExtent.y,
        window_flags);

    windowExtent = _windowExtent;

    return true;
}

void ArcEngine::Window::Destroy()
{

}