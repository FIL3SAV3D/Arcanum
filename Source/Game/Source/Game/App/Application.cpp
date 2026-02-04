#include "Application.h"

#include "Modes/GameMode.h"

ApplicationSpecification specs("App", glm::vec2(720, 480), false);

Application::Application() : IApplication{ specs }
{

}

void Application::Initialize()
{
    RegisterDefaultMode<GameMode>("Game");
}

void Application::Deinitialize()
{
}