#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Vertex
{
	glm::vec3 aPosition{};
	glm::vec3 aNormal{};
	glm::vec2 aTexCoords{};
	glm::vec3 aTangent{};
	glm::vec3 aBiTangent{};
};