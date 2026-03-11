#include "CameraSystem.h"

#include "Framework/ECS/Coordinator.h"
#include "Framework/ECS/Components/TransformComponent.h"
#include "Framework/ECS/Components/CameraComponent.h"
#include <Framework/ECS/Components/InputComponent.h>

void CameraSystem::OnInput(std::shared_ptr<ArcEngine::Window> _Window)
{
}

void CameraSystem::OnUpdate(const float& _DeltaTime)
{
	for (auto const& entity : mEntities)
	{
		auto& transformComponent = coordinator->GetComponent<TransformComponent>(entity);
		const auto& inputComponent = coordinator->GetComponent<InputComponent>(entity);

		//if (firstMouse) // initially set to true
		//{
		//	lastX = xpos;
		//	lastY = ypos;
		//	firstMouse = false;
		//}

		//float xoffset = xpos - lastX;
		//float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
		//lastX = xpos;
		//lastY = ypos;

		//const float sensitivity = 0.1f;
		//xoffset *= sensitivity;
		//yoffset *= sensitivity;

		//yaw += xoffset;
		//pitch += yoffset;

		//if (pitch > 89.0f)
		//	pitch = 89.0f;
		//if (pitch < -89.0f)
		//	pitch = -89.0f;

		//glm::vec3 direction;
		//direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		//direction.y = sin(glm::radians(pitch));
		//direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		//forward = glm::normalize(direction);


		//if (!(input.length() > 0.0f))
		//	return;

		//glm::vec3 calculatedForce{};
		//calculatedForce += forward * input.z;
		//calculatedForce += JPH::Vec3::Cross(up, forward) * input.x;
		//calculatedForce += up * input.y;
		////calculatedForce = glm::normalize(calculatedForce);

		//position += calculatedForce * _deltaTime * speed;
	}
}

void CameraSystem::OnLateUpdate(const float& _DeltaTime)
{
	for (auto const& entity : mEntities)
	{
		const auto& transformComponent = coordinator->GetComponent<TransformComponent>(entity);
		auto& cameraComponent = coordinator->GetComponent<CameraComponent>(entity);

		cameraComponent = CameraComponent{};

	}
}