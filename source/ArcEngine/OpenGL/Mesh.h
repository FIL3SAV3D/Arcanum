#pragma once

#include <vector>

#include "Vertex.h"
#include "Material.h"

class Shader;

class Mesh
{
public:
	std::vector<Vertex>			vertices{};
	std::vector<unsigned int>	indices	{};
	std::vector<Material>		textures{};

	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices);
	void Draw(Shader& _shader);

	unsigned int GetVAO() { return VAO; };

private:
	unsigned int VAO{ 0 };
	unsigned int VBO{ 0 };
	unsigned int EBO{ 0 };

	void SetupMesh();
};