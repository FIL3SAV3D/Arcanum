#pragma once

#include <vector>
#include <string>

#include "Mesh.h"
#include "Texture.h"

#include "assimp/scene.h"

class Shader;

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
	Model(const char* _path, bool gamma = false);

	void Draw(Shader& _shader);

	std::vector<Texture> texturesLoaded;

private:
	std::vector<Mesh> meshes{};
	std::string directory{};
	bool gammaCorrection = false;

	void LoadModel(std::string _path);
	void ProcessNode(aiNode* _node, const aiScene* _scene);
	Mesh ProcessMesh(aiMesh* _mesh, const aiScene* _scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* _material, aiTextureType _type, std::string _typeName);
};