#include "ArcEngine.h"

void Engine::Engine::Create()
{
    ArcEngine::WindowSpecification specs;
    specs.graphicsAPI = ArcEngine::OPENGL;
    specs.windowName = "test";
    specs.windowSize = glm::uvec2(1920, 1080);
    m_Window.Create(specs);

    m_Graphics.Create(ArcEngine::Graphics::OPENGL, m_Window);

    m_UIGraphics.Create(ArcEngine::Graphics::OPENGL, m_Window);
}

void Engine::Engine::Destroy()
{
    m_UIGraphics.Destroy(m_Window);

    m_Graphics.Destroy();

    m_Window.Destroy();
}

void Engine::UpdateCameraData(const glm::vec4& _Position, const glm::mat4& _View, const glm::mat4& _Projection)
{
    m_Graphics.UpdateCameraData(_Position, _View, _Projection);
}

void Engine::Engine::RenderMesh(std::shared_ptr<Model> _Model, const glm::mat4x4& _ObjectToWorld)
{

    m_Graphics.RenderMesh(_Model, _ObjectToWorld);
}

void Engine::Engine::RenderStart()
{
    m_Graphics.FrameStart(m_Window);
}

void Engine::Engine::RenderEnd()
{
    m_Graphics.FrameEnd(m_Window);
}

void Engine::Engine::RenderUIStart()
{
    m_UIGraphics.FrameStart(m_Window);
}

void Engine::Engine::RenderUIEnd()
{
    m_UIGraphics.FrameEnd(m_Window);
}

const glm::uvec2 Engine::GetWindowSize()
{
    return m_Window.GetScreenSize();
}

void Engine::Engine::ResizeSwapchain(const glm::uvec2& _Size)
{
    m_Graphics.Resize(_Size);
    m_Window.SetScreenSize(_Size);
}

void Engine::Engine::SwapFrame()
{
}