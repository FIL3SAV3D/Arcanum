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

		glfwGetCursorPos(window, &current_mouse_position_X, &current_mouse_position_Y);

		if (firstmouse)
		{
			last_mouse_position_X = current_mouse_position_X;
			last_mouse_position_Y = current_mouse_position_Y;
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

		float xoffset = static_cast<float>(current_mouse_position_X - last_mouse_position_X);
		float yoffset = static_cast<float>(last_mouse_position_Y - current_mouse_position_Y);

		last_mouse_position_X = current_mouse_position_X;
		last_mouse_position_Y = current_mouse_position_Y;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		float capped_delta_time = glm::min(deltaTime, 1.0f);

		yaw   += xoffset * capped_delta_time;
		pitch += yoffset * capped_delta_time;

		if (pitch >= 1.5f)
		{
			pitch = 1.49f;

		}

		if (pitch <= -1.5f)
		{
			pitch = -1.49f;
		}

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

	void User::DebugPrint(const bool& enabled)
	{

	}
}