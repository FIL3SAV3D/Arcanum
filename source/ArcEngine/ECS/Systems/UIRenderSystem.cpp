#include "UIRenderSystem.h"

#include "OpenGL/Window.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

UIRenderSystem::UIRenderSystem(std::shared_ptr<Window> _window)
{
    window = _window;
}

UIRenderSystem::~UIRenderSystem()
{
    window = nullptr;
}

void UIRenderSystem::OnCreate()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void UIRenderSystem::OnUpdate(const float& _deltaTime)
{
    // Imgui begin render
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // Imgui end render
}

void UIRenderSystem::OnDestroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}