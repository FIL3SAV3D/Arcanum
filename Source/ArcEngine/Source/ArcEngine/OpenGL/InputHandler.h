#pragma once

#include <vector>
#include <memory>
#include <iostream>

struct GLFWwindow;
class iInputListener;

class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	static void CursorCallBackImpl(GLFWwindow* window, double _xpos, double _ypos)		{ /*std::cout << "CURSOR MOVEMENT" << std::endl;*/ };
	static void ScrollCallBackImpl(GLFWwindow* window, double xoffset, double yoffset) { /*std::cout << "SCROLLING" << std::endl;*/ };
	static void KeyCallBackImpl(GLFWwindow* window, int key, int scancode, int action, int mods)		{ /*std::cout << "KEY INPUT" << std::endl;*/ };

	void ProcessInput(GLFWwindow* window);

	void AddListener(const std::shared_ptr<iInputListener> _listener);
	void RemoveListener(const std::shared_ptr<iInputListener> _listener);

private:
	std::vector<std::shared_ptr<iInputListener>> listeners = {};
};