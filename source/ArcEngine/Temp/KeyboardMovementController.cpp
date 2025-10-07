#include "KeyboardMovementController.h"

namespace arc
{
	void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float delta, TransformComponent& transform)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		if (firstmouse)
		{
			lPosX = xpos;
			lPosY = ypos;
			firstmouse = false;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			move_speed = sprint;
		}
		else
		{
			move_speed = 3.0f;
		}

		float xoffset = xpos - lPosX;
		float yoffset = lPosY - ypos;
		lPosX = xpos;
		lPosY = ypos;

		float sensitivity = 1.0f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		float Delta = glm::min(delta, 1.0f);

		yaw += xoffset * Delta;
		pitch += yoffset * Delta;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		transform.rotation.x = pitch;
		transform.rotation.y = yaw;

		glm::vec3 front;
		front.x = cos(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
		front.y = sin(glm::radians(transform.rotation.x));
		front.z = sin(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
		glm::normalize(front);

		transform.rotation.x = glm::clamp(transform.rotation.x, -1.5f, 1.5f);
		transform.rotation.y = glm::mod(transform.rotation.y, glm::two_pi<float>());

		//const glm::vec3 forward_dir = front;
		//const glm::vec3 right_dir{ front.z, 0.0f, -front.x };
		//const glm::vec3 up_dir{ 0.0f, -1.0f, 0.0f };

		float yaw = transform.rotation.y;
		const glm::vec3 forward_dir{ glm::sin(yaw), 0.0f, glm::cos(yaw) };
		const glm::vec3 right_dir{ forward_dir.z, 0.0f, -forward_dir.x };
		const glm::vec3 up_dir{ 0.0f, -1.0f, 0.0f };

		glm::vec3 move_dir{ 0.0f };

		if (glfwGetKey(window, Keys.moveForward) == GLFW_PRESS)
			move_dir += forward_dir;
		if (glfwGetKey(window, Keys.moveBackward) == GLFW_PRESS)
			move_dir -= forward_dir;
		if (glfwGetKey(window, Keys.moveRight) == GLFW_PRESS)
			move_dir += right_dir;
		if (glfwGetKey(window, Keys.moveLeft) == GLFW_PRESS)
			move_dir -= right_dir;
		if (glfwGetKey(window, Keys.moveUp) == GLFW_PRESS)
			move_dir += up_dir;
		if (glfwGetKey(window, Keys.moveDown) == GLFW_PRESS)
			move_dir -= up_dir;

		if (glm::dot(move_dir, move_dir) > std::numeric_limits<float>::epsilon())
			transform.position += move_speed * Delta * glm::normalize(move_dir);

		printf("pos: %f %f %f\n", move_dir.x, move_dir.y, move_dir.z);

	}
}