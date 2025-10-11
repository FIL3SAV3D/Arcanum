#pragma once

// GLM
#include <glm/geometric.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "iInputListener.h"

struct GLFWwindow;

class Camera : public iInputListener
{
public:
	Camera();
	~Camera();

	void Update(const float& _deltaTime);

	// Inherited via iInputListener
	void UpdateInput(GLFWwindow* window) override;

	glm::vec3 GetPosition() const;

	glm::mat4x4 GetViewMatrix() const;
	glm::mat4x4 GetProjectionMatrix() const;


	void SetPosition(const glm::vec3& _position);
	void SetRotation(const glm::vec3& _rotation);

	void LookAt(const glm::vec3& _target, const glm::vec3& _upAxis);

	int screenWidth;
	int screenHeight;

private:
	bool firstMouse = true;

	float lastX = 400;
	float lastY = 300;

	float yaw = 0;
	float pitch = 0;

	const float speed = 10.0f;

	glm::mat4 transform{};

	glm::vec3 input{};

	glm::mat4 viewMatrix{};

	glm::vec3 position{};

	glm::vec3 up{ 0.0f, 1.0f,  0.0f };
	glm::vec3 right{};
	glm::vec3 forward{ 0.0f, 0.0f, -1.0f };
};