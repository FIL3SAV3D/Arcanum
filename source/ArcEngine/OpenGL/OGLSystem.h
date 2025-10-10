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