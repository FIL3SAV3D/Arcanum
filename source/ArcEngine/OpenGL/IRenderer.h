#pragma once

#include "glm/mat4x4.hpp"

class IRenderer
{
public:
	virtual void RenderSceneCB(const glm::mat4& projection, const glm::mat4x4& view, const glm::vec3& cameraPosition) = 0;
};