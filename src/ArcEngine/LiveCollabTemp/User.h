#pragma once

#include <EntityComponentSystem/TempInfo.h>
#include <EntityComponentSystem/Components/TransformComponent.h>
#include <EntityComponentSystem/Coordinator.h>
#include "window/Window.h"
#include "Temp/FrameInfo.h"

namespace arc
{
	class User
	{
	public:
		struct KeyMappings {
			int moveLeft = GLFW_KEY_A;
			int moveRight = GLFW_KEY_D;
			int moveForward = GLFW_KEY_W;
			int moveBackward = GLFW_KEY_S;
			int moveUp = GLFW_KEY_E;
			int moveDown = GLFW_KEY_Q;
			int lookLeft = GLFW_KEY_LEFT;
			int lookRight = GLFW_KEY_RIGHT;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
		};

		KeyMappings Keys{};

		float move_speed{ 3.0f };
		float turn_speed{ 1.5f };

		User(Coordinator& coordinator, std::shared_ptr<arcModel> defaultModel);
		~User();


		void Update(const float& deltaTime, Coordinator& coordinator, GLFWwindow* window);

		Entity EntityID = 0;
		uint32_t UserID = 0;
	};
}
