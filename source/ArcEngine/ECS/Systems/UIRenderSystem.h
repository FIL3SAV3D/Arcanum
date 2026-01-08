#pragma once

#include <memory>

#include "ECS/Interfaces/ISystem.h"


class Window;

class UIRenderSystem : public ISystem
{
public:
	UIRenderSystem(std::shared_ptr<Window> _window);
	~UIRenderSystem();

	void OnCreate() override;
	void OnUpdate(const float& _deltaTime) override;
	void OnDestroy() override;

private:
	std::shared_ptr<Window> window;
};