#pragma once

struct GLFWwindow;

class iInputListener
{
public:
	virtual void UpdateInput(const bool* _KeyStates) = 0;
};