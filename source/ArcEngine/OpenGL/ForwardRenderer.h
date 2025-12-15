#pragma once

#include "IRenderer.h"

class ForwardRenderer : public IRenderer
{
	// Inherited via IRenderer
	void RenderSceneCB(const glm::mat4& projection, const glm::mat4x4& view) override;
};