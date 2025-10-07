#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace arc
{
	class arcWindow
	{
	public:
		 arcWindow(int w, int h, const std::string& name);
		~arcWindow();

		arcWindow(const arcWindow&)            = delete;
		arcWindow& operator=(const arcWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose( window ); }
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		bool wasWindowResized() { return frame_buffer_resize; }
		void resetWindowResizedFlag() { frame_buffer_resize = false; }

		GLFWwindow* getGLFWWindow() const { return window; }

		void createWindowSurface(VkInstance _instance, VkSurfaceKHR* _surface);

	private:
		static void framebufferResizeCallback(GLFWwindow* _window, int _width, int _height);
		void initWindow();

		int width;
		int height;

		bool frame_buffer_resize = false;

		std::string window_name;
		GLFWwindow* window;
	};
}