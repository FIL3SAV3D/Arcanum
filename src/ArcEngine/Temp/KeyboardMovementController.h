#pragma once

#include "GameObject.h"
#include "window/Window.h"
#include "FrameInfo.h"

namespace arc
{
	class KeyboardMovementController
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

		void moveInPlaneXZ(GLFWwindow* window, float delta, TransformComponent& transform);

		KeyMappings Keys{};

		float move_speed{ 3.0f };
		float turn_speed{ 1.5f };
	};
}