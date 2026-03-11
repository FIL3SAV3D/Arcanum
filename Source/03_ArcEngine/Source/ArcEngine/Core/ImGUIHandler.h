#pragma once

#include <memory>
#include <ArcEngine/Platform/Window.h>

#include "glm/vec4.hpp"

#include "imgui.h"

class ImGUIHandler
{
public:
	ImGUIHandler();
	~ImGUIHandler();

	bool Initialize(std::shared_ptr<ArcEngine::Window> _Window);
	bool DeInitialize();

	void Update();

	void ShowExampleAppDockSpace(bool* p_open);

	glm::vec4 clear_color{0.1f, 0.1f, 0.1f, 1.0f};
private:
	bool show_demo_window = true;
	bool open = true;
	bool show_another_window = true;


	ImGuiIO* io;

	std::shared_ptr<ArcEngine::Window> window;
};