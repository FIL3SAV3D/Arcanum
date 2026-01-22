#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <format>
#include "Modes/ModeMain.h"

#include "Layers/ImGUILayer.h"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

Application::Application(const ApplicationSpecification& _Spec):
	m_LayerStack{std::make_unique<LayerStack>()},
	modeManger{std::make_unique<ModeManager>()},
	window{ std::make_shared<Window>(static_cast<int>(_Spec.windowSize.x), static_cast<int>(_Spec.windowSize.y), _Spec.name.c_str()) },
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
	m_LayerStack->PushLayer<ImGUILayer>(window);

	//modeManger->SwitchMode("Main");
}

void Application::Run()
{
	inputHandler->ProcessInput(window->GetNativeWindow());

	clock->Update();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_LayerStack->Update(clock->GetDeltaTime());

	glfwSwapBuffers(window->GetNativeWindow());
	glfwPollEvents();

	Render();

	// Check and call events and swap buffers
	

}

void Application::Render()
{
	//

	//// Imgui begin render
	

	//Render3D();
	//Render2D();
	//RenderUI();

	
}

void Application::Render3D()
{
}

void Application::Render2D()
{
}

void Application::RenderUI()
{
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window->GetNativeWindow(), &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	//glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// Imgui end render

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void Application::Destroy()
{

}

bool Application::IsQuitting()
{
	return glfwWindowShouldClose(window->GetNativeWindow());
}