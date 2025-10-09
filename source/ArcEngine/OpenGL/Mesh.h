#pragma once

#include <vector>

#include "Vertex.h"
#include "Texture.h"

class Shader;

class Mesh
{
public:
	std::vector<Vertex>			vertices{};
	std::vector<unsigned int>	indices	{};
	std::vector<Texture>		textures{};

	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures);
	void Draw(Shader& _shader);

private:
	unsigned int VAO{ 0 };
	unsigned int VBO{ 0 };
	unsigned int EBO{ 0 };

	void SetupMesh();
};