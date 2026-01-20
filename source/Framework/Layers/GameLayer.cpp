#include "GameLayer.h"

#include "OpenGL/DeferredRenderer.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

GameLayer::GameLayer(std::shared_ptr<Window> _Window):
	ILayer("Game"),
	window{ _Window }
{
	OnAttach();

	camera = std::make_shared<Camera>();
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
	renderer = std::make_shared<DeferredRenderer>();
	renderer->Initialize(glm::vec2(window->screenWidth, window->screenHeight));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(const float& _DeltaTime)
{
	//camera->SetPosition(glm::vec3(0.0f, 0.0f, -1000.0f));
	camera->UpdateInput(window->GetNativeWindow());
	camera->Update(_DeltaTime);


	renderer->RenderSceneCB(camera->GetProjectionMatrix(), camera->GetViewMatrix(), camera->GetPosition(), false, glm::vec2(0.0f, 1.0f));

	// Imgui begin render
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save", "CTRL+S")) {}
			if (ImGui::MenuItem("Exit", "ESCAPE")) { glfwSetWindowShouldClose(window->GetNativeWindow(), true); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {} // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		//if (ImGui::BeginMenu("Debug"))
		//{
		//	if (ImGui::Checkbox("Debug Rendering", &debugMode)) {}
		//	if (ImGui::SliderFloat2("Depth Range", glm::value_ptr(test), 0.0f, 1.0f)) {}

		//	ImGui::EndMenu();
		//}
		//if (ImGui::BeginMenu("Post Process"))
		//{
		//	ImGui::EndMenu();
		//}

		/*ImGui::Text((std::string("FPS: ") + std::to_string(static_cast<int>(fps))).c_str());*/
		ImGui::EndMainMenuBar();
	}

	//ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// Imgui end render


	// Check and call events and swap buffers
	glfwSwapBuffers(window->GetNativeWindow());
	glfwPollEvents();
}
