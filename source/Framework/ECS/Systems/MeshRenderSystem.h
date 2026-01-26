#pragma once

#include <memory>
#include "ECS/Interfaces/ISystem.h"

class MeshRenderSystem : public ISystem
{
	void OnRender(std::shared_ptr<IRenderer> _Renderer) override;
};