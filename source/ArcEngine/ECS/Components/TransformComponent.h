#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/trigonometric.hpp>

struct TransformComponent
{
	glm::vec3 position{ 0.0f };
	glm::vec3 scale   { 1.0f };
	glm::quat rotation{  };

	glm::mat4 GetMat4x4() {
		glm::mat4 compoundMatrix = glm::mat4(1.0f);

		glm::mat4 matTranslate = glm::translate(glm::mat4(1.0), position);
		glm::mat4 matRotate = glm::mat4_cast(rotation);
		glm::mat4 matScale = glm::scale(glm::mat4(1.0), scale);

		return matTranslate * matRotate * matScale;
	}
	glm::mat3 GetNormalMat4x4()
	{
		glm::mat4 matTranslate = glm::translate(glm::mat4(1.0), position);
		glm::mat4 matRotate = glm::mat4_cast(rotation);

		const glm::vec3 invScale = 1.0f / scale;
		glm::mat4 matScale = glm::scale(glm::mat4(1.0), invScale);

		return matTranslate * matRotate * matScale;
	}
};