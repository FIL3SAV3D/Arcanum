#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Vertex.h"
#include "OGLWindow.h"

#include "InputHandler.h"

class Camera;
class iInputListener;
class FrameBuffer;

struct GLFWwindow;

class OGLSystem
{
public:
	OGLSystem();
	~OGLSystem();

	void Run();

	static void FrameBufferSizeCallback(GLFWwindow* _window, int _width, int _height);

	// Callbacks
	static void CursorCallback	(GLFWwindow* _window, double _xpos, double _ypos);
	static void ScrollCallback	(GLFWwindow* _window, double xoffset, double yoffset);
	static void KeyCallback		(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);


private:
	std::shared_ptr<OGLWindow> sptr_OGLWindow = nullptr;
	std::shared_ptr<InputHandler> inputHandler = nullptr;

	std::shared_ptr<Camera> camera = nullptr;

	std::shared_ptr<FrameBuffer> frameBuffer;
private:
	int screenWidth;
	int screenHeight;

	const char* windowName = "Arcanum";

	float deltaTime = 0.0f;
	float currentFrameTime = 0.0f;
	float lastFrameTime = 0.0f;

	unsigned int lightVAO = 0;
	unsigned int cubeVAO = 0;

	unsigned int skyboxVAO = 0;

	unsigned int VBO = 0;
	unsigned int EBO = 0;
};