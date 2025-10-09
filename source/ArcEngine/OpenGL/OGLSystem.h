#pragma once



#include <vector>
#include <string>
#include <memory>

#include "Vertex.h"
#include "OGLWindow.h"

#include "InputHandler.h"

class Camera;
class iInputListener;

class OGLSystem
{
	struct Vector3
	{
		Vector3() {};
		Vector3(const float& _x, const float& _y, const float& _z) { x = _x; y = _y; z = _z; };
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	glm::vec3 cubePositions[10] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

public:
	OGLSystem();
	~OGLSystem();

	void Run();

private:
	std::shared_ptr<OGLWindow> sptr_OGLWindow = nullptr;
	std::shared_ptr<InputHandler> inputHandler = nullptr;

	std::shared_ptr<Camera> camera = nullptr;

private:


	int screenWidth = 800;
	int screenHeight = 600;

	const char* windowName = "Arcanum";

	float deltaTime = 0.0f;
	float currentFrameTime = 0.0f;
	float lastFrameTime = 0.0f;

	unsigned int lightVAO = 0;
	unsigned int cubeVAO = 0;

	unsigned int VBO = 0;
	unsigned int EBO = 0;

};