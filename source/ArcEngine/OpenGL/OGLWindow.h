#pragma once

struct GLFWwindow;

class OGLWindow
{
public:
	OGLWindow(const int& screenWidth, const int& screenHeight, const char* windowName);
	~OGLWindow();

	constexpr GLFWwindow* GetWindow() const { return window; }

private:
	bool VSync = false;

	GLFWwindow* window = nullptr;
};