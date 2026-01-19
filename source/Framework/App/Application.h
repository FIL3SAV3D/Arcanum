#pragma once

#include <memory>

#include "States/StateManager.h"

class Application
{
public:
	Application();
	~Application();

	Application(const Application& other) = delete; // copy constructor
	Application(Application&& other) noexcept = delete; // move constructor
	Application& operator=(const Application& other) = delete; // copy assignment
	Application& operator=(Application&& other) noexcept = delete; // move assignment

	void Create();

	void Run();

	void Destroy();

	bool IsQuitting();

private:
	std::unique_ptr<StateManger> stateManger;
};