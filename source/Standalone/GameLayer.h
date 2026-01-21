#pragma once

#include "Layer/ILayer.h"

#include "memory"
#include "OpenGL/IRenderer.h"
#include <OpenGL/Window.h>
#include <OpenGL/Camera.h>

class GameLayer : public ILayer
{
public:
	GameLayer(std::shared_ptr<Window> _Window);
	~GameLayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(const float& _DeltaTime) override;

private:
	std::shared_ptr<Window> window;
	std::shared_ptr<IRenderer> renderer;

	std::shared_ptr<Camera> camera;
};