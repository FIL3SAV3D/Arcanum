#pragma once

#include <memory>

#include "ECS/Interfaces/ISystem.h"

#include "Core/ImGUIHandler.h"

#include "glm/vec4.hpp"

class Window;

class UIRenderSystem : public ISystem
{
public:
	UIRenderSystem(std::shared_ptr<Window> _window);
	~UIRenderSystem();

	void OnCreate() override;
	void OnRenderUI(const RenderParams& _RenderParams) override;
	void OnDestroy() override;

	glm::vec4 clear_color{ 0.1f, 0.1f, 0.1f, 1.0f };
private:
	std::unique_ptr<ImGUIHandler> m_ImGUIHandler;

	std::shared_ptr<Window> window;
};