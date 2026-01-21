#include "EditorLayer.h"

#include "imgui.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "GLFW/glfw3.h"

EditorLayer::EditorLayer(std::shared_ptr<Window> _Window):
	ILayer{"Editor", DEFAULT_PRIORITIES::EDITOR},
	window{_Window}
{
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void EditorLayer::OnDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void EditorLayer::OnUpdate(const float& _DeltaTime)
{
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
		if (ImGui::BeginMenu("Debug"))
		{
			//if (ImGui::Checkbox("Debug Rendering", &debugMode)) {}
			//if (ImGui::SliderFloat2("Depth Range", glm::value_ptr(test), 0.0f, 1.0f)) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Post Process"))
		{
			ImGui::EndMenu();
		}

		//ImGui::Text((std::string("FPS: ") + std::to_string(static_cast<int>(fps))).c_str());
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

void EditorLayer::OnImGuiRender()
{

}
