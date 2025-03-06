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

		glm::vec3 rotate{ 0.0f };
		if (glfwGetKey(window, Keys.lookRight) == GLFW_PRESS)
			rotate.y += 1.0f;
		if (glfwGetKey(window, Keys.lookLeft) == GLFW_PRESS)
			rotate.y -= 1.0f;
		if (glfwGetKey(window, Keys.lookUp) == GLFW_PRESS)
			rotate.x += 1.0f;
		if (glfwGetKey(window, Keys.lookDown) == GLFW_PRESS)
			rotate.x -= 1.0f;

		float delta = glm::min(deltaTime, 1.0f);

		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
			transform.rotation += turn_speed * delta * glm::normalize(rotate);

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
			transform.position += move_speed * delta * glm::normalize(move_dir);
	}
}