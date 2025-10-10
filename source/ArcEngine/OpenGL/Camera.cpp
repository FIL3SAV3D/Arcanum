#include "Camera.h"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::Update(const float& _deltaTime)
{
	if (!(input.length() > 0.0f))
		return;

	glm::vec3 calculatedForce{};
	calculatedForce += forward * input.z;
	calculatedForce += glm::cross(up, forward) * input.x;
	calculatedForce += up * input.y;
	//calculatedForce = glm::normalize(calculatedForce);

	position += calculatedForce * _deltaTime * speed;
}

void Camera::SetPosition(const glm::vec3& _position)
{
	glm::mat4 translate = glm::translate(glm::mat4{}, _position);
	transform = translate;
}

void Camera::SetRotation(const glm::vec3& _rotation)
{
}

void Camera::LookAt(const glm::vec3& _target, const glm::vec3& _upAxis)
{
	up = glm::normalize(_upAxis);
	forward = glm::normalize(position - _target);
	right = glm::normalize(glm::cross(_upAxis, forward));

	viewMatrix = glm::lookAt(
		right,
		up,
		forward
	);

}

void Camera::UpdateInput(GLFWwindow* window)
{
	glm::vec3 temp{};

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			temp.z += 1;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			temp.z -= 1;

		// Right
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			temp.x += 1;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			temp.x -= 1;

		//Vertical
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			temp.y -= 1;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			temp.y += 1;

		double xpos, ypos;

		glfwGetCursorPos(window, &xpos, &ypos);

		if (firstMouse) // initially set to true
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
		lastX = xpos;
		lastY = ypos;

		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		forward = glm::normalize(direction);
	}
	else
	{
		firstMouse = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	input = temp;
}

glm::vec3 Camera::GetPosition() const
{
	return position;
}

glm::mat4x4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + forward, up);
}

glm::mat4x4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 1000.0f);;
}
