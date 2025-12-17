#pragma once

#include "glm/mat4x4.hpp"

class IRenderer
{
public:
	virtual void Initialize(const glm::vec2& _size) = 0;
	virtual void RenderSceneCB(const glm::mat4& projection, const glm::mat4x4& view, const glm::vec3& cameraPosition, const bool& _DebugMode) = 0;
	virtual void Resize(const glm::vec2& _size) = 0;
};