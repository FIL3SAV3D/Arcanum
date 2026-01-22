#pragma once

#include <memory>
#include <OpenGL/Window.h>

#include "glm/vec4.hpp"

#include "imgui.h"

class ImGUIHandler
{
public:
	ImGUIHandler();
	~ImGUIHandler();

	bool Initialize(std::shared_ptr<Window> _Window);
	bool DeInitialize();

	void Update();

	void ShowExampleAppDockSpace(bool* p_open);

private:
	bool show_demo_window = true;
	bool open = true;
	bool show_another_window = true;

	glm::vec4 clear_color{0.1f, 0.1f, 0.1f, 1.0f};

	ImGuiIO* io;

	std::shared_ptr<Window> window;
};