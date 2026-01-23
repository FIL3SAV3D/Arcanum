#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Application.h"
#include <format>
#include "Modes/ModeMain.h"

//#include "Layers/ImGUILayer.h"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "ECS/UIRenderSystem.h"

Application::Application(const ApplicationSpecification& _Spec):
	m_LayerStack{std::make_unique<LayerStack>()},
	modeManger{std::make_unique<ModeManager>()},
	window{ std::make_shared<Window>(static_cast<int>(_Spec.windowSize.x), static_cast<int>(_Spec.windowSize.y), _Spec.name.c_str()) },
	inputHandler{std::make_unique<InputHandler>()},
	clock{std::make_unique<Clock>()},
	coordinator{ std::make_unique<Coordinator>() }

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

void Application::Run()
{
	OnInput();

	clock->Update();
	const float deltaTime = clock->GetDeltaTime();

	OnUpdate(deltaTime);

	OnLateUpdate(deltaTime);
	auto color = UI.lock()->clear_color;

	glClearColor(color.x, color.y, color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	OnRender();

	OnRenderUI();

	glfwSwapBuffers(window->GetNativeWindow());

	OnApplicationPause();

	OnCheckForDisabled();
}

#pragma region  ApplicationStart

// Start
void Application::OnCreate()
{
	UI = coordinator->RegisterSystem<UIRenderSystem, 10>(window);

	coordinator->m_SystemManager->RecalculateUpdateOrder();

	coordinator->OnCreate();
}

void Application::OnEnable()
{
}

void Application::OnStart()
{
}
// Start

#pragma endregion

#pragma region ApplicationRun

// Update
void Application::OnInput()
{
	inputHandler->ProcessInput(window->GetNativeWindow());

	coordinator->OnInput();
}

void Application::OnUpdate(const float& _DeltaTime)
{
	coordinator->OnUpdate(_DeltaTime);
}

void Application::OnLateUpdate(const float& _DeltaTime)
{
	coordinator->OnLateUpdate(_DeltaTime);
}

void Application::OnRender()
{
	coordinator->OnRender(renderer);
}

void Application::OnRenderUI()
{
	coordinator->OnRenderUI(renderer);
	//ImGui::Render();
	//int display_w, display_h;
	//glfwGetFramebufferSize(window->GetNativeWindow(), &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	////glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//// Imgui end render

	//if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
	//	GLFWwindow* backup_current_context = glfwGetCurrentContext();
	//	ImGui::UpdatePlatformWindows();
	//	ImGui::RenderPlatformWindowsDefault();
	//	glfwMakeContextCurrent(backup_current_context);
	//}

}

void Application::OnApplicationPause()
{
}

void Application::OnCheckForDisabled()
{
}
// Update

#pragma endregion

#pragma region ApplicationEnd

// Quit
void Application::OnQuit()
{
}

void Application::OnDisable()
{
}

void Application::OnDestroy()
{

}
// Quit

#pragma endregion

bool Application::IsQuitting()
{
	return glfwWindowShouldClose(window->GetNativeWindow());
}