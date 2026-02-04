#pragma once

#include <memory>
#include "ECS/Interfaces/ISystem.h"

#include "OpenGL/Camera.h"
#include <OpenGL/InputHandler.h>

class MeshRenderSystem : public ISystem
{
public:
	MeshRenderSystem(std::shared_ptr<InputHandler> _InputHandler);

	void OnCreate();
	void OnUpdate(const float& _DeltaTime) override;
	void OnRender(const RenderParams& _RenderParams) override;

private:
	std::shared_ptr<Camera> cam;
};