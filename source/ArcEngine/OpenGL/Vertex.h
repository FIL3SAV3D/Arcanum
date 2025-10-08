#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Vertex
{
public:
	Vertex() {};
	Vertex(const float& _x, const float& _y, const float& _z) { pos.x = _x; pos.y = _y; pos.z = _z; };
	//Vertex(const glm::vec3& _pos) { pos.x = _pos.x; pos.y = _pos.y; pos.z = _pos.z; };

	glm::vec3 pos{};
	glm::vec3 color{};
	glm::tvec2<float> texCoords{};

	constexpr size_t DataSize() { return sizeof(Vertex); }
};