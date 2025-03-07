#include "User.h"

namespace arc
{
	User::User(Coordinator& coordinator, std::shared_ptr<arcModel> defaultModel) :
		EntityID(coordinator.CreateEntity())
	{
		coordinator.AddComponent(EntityID, TransformComponent());
		coordinator.AddComponent(EntityID, ModelComponent(defaultModel));

	}

	User::~User()
	{
	}

	void User::Update(const float& deltaTime, Coordinator& coordinator, GLFWwindow* window)
	{
		TransformComponent& transform = coordinator.GetComponent<TransformComponent>(EntityID);

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

		float Delta = glm::min(deltaTime, 1.0f);

		yaw += xoffset * Delta;
		pitch += yoffset * Delta;

		if (pitch > 89.0f)
			pitch = 88.5f;
		if (pitch < -89.0f)
			pitch = -88.5f;

		transform.rotation.x = pitch;
		transform.rotation.y = yaw;

		glm::vec3 front;
		front.x = cos(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
		front.y = sin(glm::radians(transform.rotation.x));
		front.z = sin(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
		glm::normalize(front);

		transform.rotation.x = glm::clamp(transform.rotation.x, -1.5f, 1.5f);
		transform.rotation.y = glm::mod(transform.rotation.y, glm::two_pi<float>());

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
	}
}