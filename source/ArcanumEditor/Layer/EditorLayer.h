#pragma once

#include "Layer/ILayer.h"
#include <memory>

#include "OpenGL/Window.h"

class EditorLayer : public ILayer
{
public:
	EditorLayer(std::shared_ptr<Window> _Window);
	~EditorLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(const float& _DeltaTime) override;

	void OnImGuiRender();

private:
	std::shared_ptr<Window> window;
};