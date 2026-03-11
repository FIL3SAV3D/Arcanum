#include "ArcEngine.h"

void ArcEngine::ArcEngine::Create()
{
    WindowSpecification specs;
    specs.graphicsAPI = OPENGL;
    specs.windowName = "test";
    specs.windowSize = glm::uvec2(600, 400);
    m_Window.Create(specs);
}

void ArcEngine::ArcEngine::Destroy()
{
    m_Window.Destroy();
}