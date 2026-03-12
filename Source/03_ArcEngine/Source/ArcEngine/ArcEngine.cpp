#include "ArcEngine.h"

void ArcEngine::ArcEngine::Create()
{
    WindowSpecification specs;
    specs.graphicsAPI = OPENGL;
    specs.windowName = "test";
    specs.windowSize = glm::uvec2(600, 400);
    m_Window.Create(specs);

    m_Graphics.Create(Graphics::OPENGL, m_Window);

    m_UIGraphics.Create(Graphics::OPENGL, m_Window);
}

void ArcEngine::ArcEngine::Destroy()
{
    m_UIGraphics.Destroy(m_Window);

    m_Graphics.Destroy();

    m_Window.Destroy();
}

void ArcEngine::ArcEngine::RenderMesh(const Model& _Model, const glm::mat4x4& _ObjectToWorld)
{
    m_Graphics.RenderMesh(_Model, _ObjectToWorld);
}

void ArcEngine::ArcEngine::RenderStart()
{
    m_Graphics.FrameStart(m_Window);
}

void ArcEngine::ArcEngine::RenderEnd()
{
    m_Graphics.FrameEnd(m_Window);
}

void ArcEngine::ArcEngine::RenderUIStart()
{
    m_UIGraphics.FrameStart(m_Window);
}

void ArcEngine::ArcEngine::RenderUIEnd()
{
    m_UIGraphics.FrameEnd(m_Window);
}

void ArcEngine::ArcEngine::SwapFrame()
{
}