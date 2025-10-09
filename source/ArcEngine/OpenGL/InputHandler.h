#pragma once

#include <vector>
#include <memory>

struct GLFWwindow;
class iInputListener;

class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	void ProcessInput(GLFWwindow* window);

	void AddListener(const std::shared_ptr<iInputListener> _listener);
	void RemoveListener(const std::shared_ptr<iInputListener> _listener);

private:
	std::vector<std::shared_ptr<iInputListener>> listeners = {};
};