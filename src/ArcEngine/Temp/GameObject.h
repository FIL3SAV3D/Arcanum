#pragma once

#include "model.h"

// std
#include <memory>
#include <unordered_map>

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

	struct PointLightComponent
	{
		float light_intensity = 1.0f;
	};

	class arcGameObject
	{
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, arcGameObject>;

		static arcGameObject createGameObject()
		{
			static id_t current_id = 0;
			return arcGameObject{ current_id++ };
		}

		static arcGameObject makePointLight(
			float _intensity = 1.0f, 
			float _radius = 0.5f, 
			glm::vec3 _color = glm::vec3{ 1.0f }
		);

		arcGameObject(const arcGameObject&) = delete;
		arcGameObject& operator=(const arcGameObject&) = delete;

		arcGameObject(arcGameObject&&) = default;
		arcGameObject& operator=(arcGameObject&&) = default;

		const id_t getId() { return id; }

		glm::vec3 color;
		TransformComponent transform{};

		// Optional pointer components
		std::shared_ptr < arcModel > model               = nullptr;
		std::unique_ptr<PointLightComponent> point_light = nullptr;

	private:
		arcGameObject(id_t _object_id) : id{ _object_id } {}

		id_t id;
	};
}