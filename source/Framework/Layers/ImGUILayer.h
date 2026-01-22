#pragma once

#include "Layer/ILayer.h"

#include <memory>

#include "OpenGL/Window.h"

#include "Core/ImGUIHandler.h"

class ImGUILayer : public ILayer
{
public:
	ImGUILayer(std::shared_ptr<Window> _Window);
	~ImGUILayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(const float& _DeltaTime) override;

private:
	std::shared_ptr<Window> window;
	std::unique_ptr<ImGUIHandler> imGUIHandler;
};