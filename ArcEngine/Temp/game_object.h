#pragma once

#include "model.h"

// std
#include <memory>

// libs
#include "glm/gtc/matrix_transform.hpp"

namespace arc
{
	struct TransformComponent
	{
		glm::vec3 translation{ 0.0f }; // position offset
		glm::vec3 scale      { 1.0f };
		glm::vec3 rotation   { 0.0f };

		// Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
		// Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
		// https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};

	class arcGameObject
	{
	public:
		using id_t = unsigned int;

		static arcGameObject createGameObject()
		{
			static id_t current_id = 0;
			return arcGameObject{ current_id++ };
		}

		arcGameObject(const arcGameObject&) = delete;
		arcGameObject& operator=(const arcGameObject&) = delete;

		arcGameObject(arcGameObject&&) = default;
		arcGameObject& operator=(arcGameObject&&) = default;

		const id_t getId() { return id; }

		std::shared_ptr < arcModel > model{};
		glm::vec3 color;
		TransformComponent transform{};

	private:
		arcGameObject(id_t _object_id) : id{ _object_id } {}

		id_t id;
	};
}