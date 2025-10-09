#include "InputHandler.h"

#include "iInputListener.h"

#include <GLFW/glfw3.h>

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
	size_t size = listeners.size();
	for (size_t i = 0; i < size; i++)
	{
		listeners.at(i).reset();
		RemoveListener(listeners.at(i));
	}
}

void InputHandler::ProcessInput(GLFWwindow* window)
{
	glfwPollEvents();

	for (auto listener : listeners)
	{
		listener->UpdateInput(window);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void InputHandler::AddListener(const std::shared_ptr<iInputListener> _listener)
{
	listeners.push_back(_listener);
}

void InputHandler::RemoveListener(const std::shared_ptr<iInputListener> _listener)
{
	std::vector<std::shared_ptr<iInputListener>>::iterator itr = std::find(listeners.begin(), listeners.end(), _listener);
	listeners.erase(itr);
}
