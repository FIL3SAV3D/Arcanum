#include "window.h"

//std
#include <stdexcept>

namespace arc
{
	arcWindow::arcWindow(int w, int h, const std::string& name) : width{ w }, height{ h }, window_name{ name }
	{
		initWindow();
	}

	arcWindow::~arcWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void arcWindow::createWindowSurface(VkInstance _instance, VkSurfaceKHR* _surface)
	{
		if (glfwCreateWindowSurface(_instance, window, nullptr, _surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface");
		}
	}

	void arcWindow::framebufferResizeCallback(GLFWwindow* _window, int _width, int _height)
	{
		auto arc_window = reinterpret_cast<arcWindow*>(glfwGetWindowUserPointer(_window));
		arc_window->frame_buffer_resize = true;
		arc_window->width = _width;
		arc_window->height = _height;
	}

	void arcWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE , GLFW_TRUE);

		window = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetWindowSizeCallback(window, framebufferResizeCallback);
	}
}