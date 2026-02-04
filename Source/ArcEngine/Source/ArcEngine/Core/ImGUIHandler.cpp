#include "ImGUIHandler.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

ImGUIHandler::ImGUIHandler()
{

}

ImGUIHandler::~ImGUIHandler()
{

}

bool ImGUIHandler::Initialize(std::shared_ptr<Window> _Window)
{
	window = _Window;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	io = &ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(1.0f);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	style.FontScaleDpi = 1.0f;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 1.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(window->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	return true;
}

bool ImGUIHandler::DeInitialize()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ImGUIHandler::Update()
{
	// Imgui begin render
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport();

	//if (ImGui::BeginMainMenuBar())
	//{
	//	if (ImGui::BeginMenu("File"))
	//	{
	//		if (ImGui::MenuItem("Save", "CTRL+S")) {}
	//		if (ImGui::MenuItem("Build Release", ""))
	//		{
	//			system("MSBuild.exe Arcanum.sln /t:Standalone /p:Configuration=\"Release\" /p:Platform=\"x64\" /p:BuildProjectReferences=false");
	//		}
	//		if (ImGui::MenuItem("Build & Run Release", ""))
	//		{
	//			if (system("MSBuild.exe Arcanum.sln /t:Standalone /p:Configuration=\"Release\" /p:Platform=\"x64\" /p:BuildProjectReferences=false") == 0)
	//			{
	//				system("%cd%/bin/Release/Standalone.exe");
	//			}
	//		}
	//		if (ImGui::MenuItem("Exit", "ESCAPE")) { glfwSetWindowShouldClose(window->GetNativeWindow(), true); }
	//		ImGui::EndMenu();
	//	}
	//	if (ImGui::BeginMenu("Edit"))
	//	{
	//		if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
	//		if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {} // Disabled item
	//		ImGui::Separator();
	//		if (ImGui::MenuItem("Cut", "CTRL+X")) {}
	//		if (ImGui::MenuItem("Copy", "CTRL+C")) {}
	//		if (ImGui::MenuItem("Paste", "CTRL+V")) {}
	//		ImGui::EndMenu();
	//	}
	//	if (ImGui::BeginMenu("Debug"))
	//	{
	//		//if (ImGui::Checkbox("Debug Rendering", &debugMode)) {}
	//		//if (ImGui::SliderFloat2("Depth Range", glm::value_ptr(test), 0.0f, 1.0f)) {}

	//		ImGui::EndMenu();
	//	}
	//	if (ImGui::BeginMenu("Post Process"))
	//	{
	//		ImGui::EndMenu();
	//	}

	//	//ImGui::Text((std::string("FPS: ") + std::to_string(static_cast<int>(fps))).c_str());
	//	ImGui::EndMainMenuBar();
	//}

	ShowExampleAppDockSpace(&open);



	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// Imgui end render

	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGUIHandler::ShowExampleAppDockSpace(bool* p_open)
{
	// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
	// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
	// In this specific demo, we are not using DockSpaceOverViewport() because:
	// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
	// - we allow the host window to have padding (when opt_padding == true)
	// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
	// TL;DR; this demo is more complicated than what you would normally use.
	// If we removed all the options we are showcasing, this demo would become:
	//     void ShowExampleAppDockSpace()
	//     {
	//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	//     }

	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", p_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	else
	{
		//ShowDockingDisabledMessage();
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Save", "CTRL+S")) {}
					if (ImGui::MenuItem("Build Release", ""))
					{
						system("MSBuild.exe Arcanum.sln /t:Standalone /p:Configuration=\"Release\" /p:Platform=\"x64\" /p:BuildProjectReferences=false");
					}
					if (ImGui::MenuItem("Build & Run Release", ""))
					{
						if (system("MSBuild.exe Arcanum.sln /t:Standalone /p:Configuration=\"Release\" /p:Platform=\"x64\" /p:BuildProjectReferences=false") == 0)
						{
							system("%cd%/bin/Release/Standalone.exe");
						}
					}
					if (ImGui::MenuItem("Exit", "ESCAPE")) { glfwSetWindowShouldClose(window->GetNativeWindow(), true); }
					ImGui::EndMenu();
				}

		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			/*if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
				*p_open = false;*/
			ImGui::EndMenu();
		}
		/*HelpMarker(
			"When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!" "\n"
			"- Drag from window title bar or their tab to dock/undock." "\n"
			"- Drag from window menu button (upper-left button) to undock an entire node (all windows)." "\n"
			"- Hold SHIFT to disable docking (if io.ConfigDockingWithShift == false, default)" "\n"
			"- Hold SHIFT to enable docking (if io.ConfigDockingWithShift == true)" "\n"
			"This demo app has nothing to do with enabling docking!" "\n\n"
			"This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window." "\n\n"
			"Read comments in ShowExampleAppDockSpace() for more details.");*/

		ImGui::EndMainMenuBar();
	}

	ImGui::End();
}