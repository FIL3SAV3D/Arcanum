#include "Application.h"

Application::Application():
	stateManger{std::make_unique<StateManger>()}
{
}

Application::~Application()
{
}

Application::Application(const Application& other)
{
}

Application::Application(Application&& other) noexcept
{
}

Application& Application::operator=(const Application& other)
{
	// TODO: insert return statement here
}

Application& Application::operator=(Application&& other) noexcept
{
	// TODO: insert return statement here
}
