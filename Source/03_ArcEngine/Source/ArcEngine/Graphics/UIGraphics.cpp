#include "UIGraphics.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_vulkan.h>

#include <SDL3/SDL_video.h>
#include <backends/imgui_impl_sdl3.h>

void ArcEngine::UIGraphics::Create(const Graphics::GraphicsAPI _GraphicsAPI, const Window& _Window)
{
    m_graphicsAPI = _GraphicsAPI;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    switch (m_graphicsAPI)
    {
    case(Graphics::OPENGL):
    {
        // Setup Platform/Renderer backends
        ImGui_ImplSDL3_InitForOpenGL(_Window.GetNativeWindow(), SDL_GL_GetCurrentContext());
        ImGui_ImplOpenGL3_Init("#version 460");
        break;
    }
    case(Graphics::VULKAN):
    {
        assert(false && "not yet implemented");
        break;
    }
    default:
    {
        assert(false && "not yet implemented or invalid");
        break;
    }
    }
}

void ArcEngine::UIGraphics::Destroy(const Window& _Window)
{
    switch (m_graphicsAPI)
    {
    case(Graphics::OPENGL):
    {
        ImGui_ImplOpenGL3_Shutdown();
        break;
    }
    case(Graphics::VULKAN):
    {
        assert(false && "not yet implemented");
        break;
    }
    default:
    {
        assert(false && "not yet implemented or invalid");
        break;
    }
    }

    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void ArcEngine::UIGraphics::FrameStart(const Window& _Window)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void ArcEngine::UIGraphics::FrameEnd(const Window& _Window)
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}