#pragma once

#include "IRenderer.h"

class ForwardRenderer : public IRenderer
{

	// Inherited via IRenderer
	void Initialize(const glm::vec2& _size) override;
	void RenderSceneCB(const glm::mat4& projection, const glm::mat4x4& view, const glm::vec3& cameraPosition, const bool& _DebugMode, const glm::vec2& _DepthRange) override;
	void Resize(const glm::vec2& _size) override;
};