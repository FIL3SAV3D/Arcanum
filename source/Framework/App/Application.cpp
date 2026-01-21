#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <format>
#include "Modes/ModeMain.h"

Application::Application():
	m_LayerStack{std::make_unique<LayerStack>()},
	modeManger{std::make_unique<ModeManager>()},
	window      {std::make_shared<Window>      (800,600, "Arcanum")},
	inputHandler{std::make_unique<InputHandler>()},
	clock{std::make_unique<Clock>()}
{
	glfwSetWindowUserPointer(window->GetNativeWindow(), this);
	//glfwSetFramebufferSizeCallback(window->GetNativeWindow(), FrameBufferSizeCallback);

	glfwSetCursorPosCallback(window->GetNativeWindow(), InputHandler::CursorCallBackImpl);
	glfwSetScrollCallback(window->GetNativeWindow(), InputHandler::ScrollCallBackImpl);
	glfwSetKeyCallback(window->GetNativeWindow(), InputHandler::KeyCallBackImpl);

}

Application::~Application()
{
}

void Application::Create()
{
	modeManger->PushMode<ModeMain>("Main", window);

	//modeManger->SwitchMode("Main");
}

void Application::Run()
{
	inputHandler->ProcessInput(window->GetNativeWindow());

	clock->Update();

	m_LayerStack->Update(clock->GetDeltaTime());
}

void Application::Destroy()
{

}

bool Application::IsQuitting()
{
	return glfwWindowShouldClose(window->GetNativeWindow());
}