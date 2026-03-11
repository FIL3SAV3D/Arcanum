#include "InputSystem.h"

#include "Framework/ECS/Coordinator.h"
#include "Framework/ECS/Components/InputComponent.h"

void InputSystem::OnInput(std::shared_ptr<ArcEngine::Window> _Window)
{
	//glfwPollEvents();

	//if (glfwGetKey(_Window->GetNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	//{
	//	glfwSetWindowShouldClose(_Window->GetNativeWindow(), true);
	//}

	//for (auto const& entity : mEntities)
	//{
	//	auto& inputComponent = coordinator->GetComponent<InputComponent>(entity);

	//	JPH::Vec3& direction = inputComponent.direction;

	//	double& xPos = inputComponent.xPos;
	//	double& yPos = inputComponent.yPos;

	//	bool& mouseOne = inputComponent.mouseOne;
	//	bool& mouseTwo = inputComponent.mouseTwo;

	//	auto nativeWindow = _Window->GetNativeWindow();

	//	float xDir = 0.0f;
	//	float yDir = 0.0f;
	//	float zDir = 0.0f;

	//	zDir += (glfwGetKey(nativeWindow, GLFW_KEY_W) == GLFW_PRESS) * 1.0f;
	//	zDir += (glfwGetKey(nativeWindow, GLFW_KEY_S) == GLFW_PRESS) * -1.0f;

	//	xDir += (glfwGetKey(nativeWindow, GLFW_KEY_A) == GLFW_PRESS) * 1.0f;
	//	xDir += (glfwGetKey(nativeWindow, GLFW_KEY_D) == GLFW_PRESS) * -1.0f;

	//	yDir += (glfwGetKey(nativeWindow, GLFW_KEY_Q) == GLFW_PRESS) * 1.0f;
	//	yDir += (glfwGetKey(nativeWindow, GLFW_KEY_E) == GLFW_PRESS) * -1.0f;

	//	direction.SetX(xDir);
	//	direction.SetY(yDir);
	//	direction.SetZ(zDir);

	//	mouseOne = (glfwGetMouseButton(nativeWindow, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS);
	//	mouseTwo = (glfwGetMouseButton(nativeWindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS);

	//	glfwGetCursorPos(nativeWindow, &xPos, &yPos);
	//}
}