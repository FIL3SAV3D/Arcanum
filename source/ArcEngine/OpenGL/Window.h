#pragma once

struct GLFWwindow;

class Window
{
public:
	Window(const int& screenWidth, const int& screenHeight, const char* windowName);
	~Window();



	GLFWwindow* GetNativeWindow() const { return window; }

	int screenWidth;
	int screenHeight;
private:

	bool VSync = false;

	GLFWwindow* window = nullptr;
};