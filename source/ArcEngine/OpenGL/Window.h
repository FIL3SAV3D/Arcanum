#pragma once

#include "glm/vec2.hpp"

struct GLFWwindow;

class Window
{
public:
	Window(const glm::uvec2& _WindowSize, const char* windowName);
	~Window();

	GLFWwindow* GetNativeWindow() const { return window; }

	const glm::uvec2& GetScreenSize() const { return windowSize; }

private:
	GLFWwindow* window = nullptr;

	glm::uvec2 windowSize{};

	bool VSync = false;

};