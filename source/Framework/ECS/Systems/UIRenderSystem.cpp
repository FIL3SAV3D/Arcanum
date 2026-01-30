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
    m_ImGUIHandler = std::make_unique<ImGUIHandler>();
    m_ImGUIHandler->Initialize(window);
}

void UIRenderSystem::OnRenderUI(const RenderParams& _RenderParams)
{
    m_ImGUIHandler->Update();
    clear_color = m_ImGUIHandler->clear_color;
}

void UIRenderSystem::OnDestroy()
{
    m_ImGUIHandler->DeInitialize();
    m_ImGUIHandler.reset();
}