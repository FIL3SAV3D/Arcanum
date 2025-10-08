#pragma once

#include <vector>

class OGLSystem
{
	struct Position
	{
		Position() {};
		Position(const float& _x, const float& _y, const float& _z) { x = _x; y = _y; z = _z; };
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	struct Vertex
	{
		Vertex() {};
		Vertex(const float& _x, const float& _y, const float& _z) { pos.x = _x; pos.y = _y; pos.z = _z; };
		Vertex(const Position& _pos) { pos.x = _pos.x; pos.y = _pos.y; pos.z = _pos.z;};
		Position pos{};
	};

	std::vector<Vertex> triangleBuffer = {
		Vertex(Position(-0.5f, -0.5f, 0.0f)),
		Vertex(Position(0.5f, -0.5f, 0.0f)),
		Vertex(Position(0.0f, 0.5f, 0.0f)),
	};

	float vertices[9] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

public:
	OGLSystem();
	~OGLSystem();

	void Run();

private:
	//void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	int screenWidth = 800;
	int screenHeight = 600;
	const char* windowName = "Arcanum";

	bool VSync = false;	


};