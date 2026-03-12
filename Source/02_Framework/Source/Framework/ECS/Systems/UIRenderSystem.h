#pragma once

#include <memory>

#include "Framework/ECS/Interfaces/ISystem.h"

#include "ArcEngine/Core/ImGUIHandler.h"

#include "glm/vec4.hpp"

namespace ArcEngine
{
	class Window;
}

class UIRenderSystem : public ISystem
{
public:
	UIRenderSystem(std::shared_ptr<ArcEngine::Window> _window);
	~UIRenderSystem();

	void OnCreate();
	void OnRenderUI(const RenderParams& _RenderParams) override;
	void OnDestroy();

	glm::vec4 clear_color{ 0.1f, 0.1f, 0.1f, 1.0f };
private:
	std::unique_ptr<ImGUIHandler> m_ImGUIHandler;

	std::shared_ptr<ArcEngine::Window> window;
};