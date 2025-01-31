#pragma once

// libs
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace arc
{
	class arcCamera
	{
	public:
		//arcCamera();
		//~arcCamera();

		void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);

		void setPerspectiveProjection(float fovy, float aspect, float near, float far);

		void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });
		void setViewTarget   (glm::vec3 position, glm::vec3 target   , glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });
		void setViewYXZ      (glm::vec3 position, glm::vec3 rotation);

		const glm::mat4& getProjectionMatrix() const { return projection_matrix; }
		const glm::mat4& getViewMatrix()       const { return view_matrix;       }

	private:
		glm::mat4 projection_matrix{ 1.0f };
		glm::mat4 view_matrix      { 1.0f };

	};
}