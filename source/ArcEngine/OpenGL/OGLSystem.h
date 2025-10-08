#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Vertex.h"
#include "ShaderManager.h"
#include "OGLWindow.h"

struct Mesh
{

	std::vector<Vertex> verticesBuffer = {
	Vertex(0.5f,  0.5f, 0.0f),
	Vertex(0.5f, -0.5f, 0.0f),
	Vertex(-0.5f, -0.5f, 0.0f),
	Vertex(-0.5f,  0.5f, 0.0f)
	};

	std::vector<unsigned int> indicesBuffer = // note that we start from 0!
	{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	constexpr size_t GetVerticesBufferSize() { return verticesBuffer.size() * sizeof(Vertex); }
	constexpr size_t GetIndiciesBufferSize() { return indicesBuffer.size() * sizeof(unsigned int); }
};

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

	Mesh Rect2D = {};

public:
	OGLSystem();
	~OGLSystem();

	void Run();

private:
	std::shared_ptr<OGLWindow>		sptr_OGLWindow		= nullptr;
	std::shared_ptr<ShaderManager>	sptr_shaderManager	= nullptr;

private:
	int screenWidth = 800;
	int screenHeight = 600;
	const char* windowName = "Arcanum";


	unsigned int VBO = 0;
	unsigned int VAO = 0;
	unsigned int EBO = 0;

};