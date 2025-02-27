#pragma once

#include <glm/vec4.hpp>

struct PointLightComponent
{
	float intensity = 1.0f;
	glm::vec4 color{ 1.0f };
};