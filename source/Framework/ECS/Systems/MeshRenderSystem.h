#pragma once

#include <memory>
#include "ECS/Interfaces/ISystem.h"

class MeshRenderSystem : public ISystem
{
	MeshRenderSystem(std::shared_ptr<IRenderer> _Renderer);

	void OnCreate();
	void OnRender(std::shared_ptr<IRenderer> _Renderer) override;

private:
	std::shared_ptr<IRenderer> renderer;
};