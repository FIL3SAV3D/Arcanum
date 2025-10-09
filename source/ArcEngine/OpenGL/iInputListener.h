#pragma once

struct GLFWwindow;

class iInputListener
{
public:
	virtual void UpdateInput(GLFWwindow* window) = 0;
};