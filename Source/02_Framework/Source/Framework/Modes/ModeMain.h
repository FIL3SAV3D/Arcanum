#pragma once

#include <memory>

#include "Framework/Modes/IMode.h"

#include <ArcEngine/Platform/Window.h>

class ModeMain : public IMode
{
public:
	ModeMain(const char* _name, std::shared_ptr<ArcEngine::Window> _Window);
	~ModeMain();

	//void OnResize(const uint32_t _width, const uint32_t _height) override;

	//void OnCreate()  override;
	//void OnDestroy() override;

	//void OnEnter() override;
	//void OnExit()  override;

	//void Update(const float& _delta_time) override;

private:
	std::shared_ptr<ArcEngine::Window> window;
};