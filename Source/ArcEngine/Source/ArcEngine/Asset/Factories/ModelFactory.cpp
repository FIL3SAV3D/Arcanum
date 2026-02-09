#include "ModelFactory.h"

#include "ArcEngine/Thirdparty/MeshOptimizer/meshoptimizer.h"

#include <glad/glad.h>

#include <stb_image.h>
#include <iostream>

std::shared_ptr<IAsset> ModelFactory::CreateAsset(const std::filesystem::path& _AssetPath)
{
    return LoadModel(_AssetPath.string());
}

std::shared_ptr<Model> ModelFactory::LoadModel(std::string _path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	std::shared_ptr<Model> model = std::make_shared<Model>();

	ProcessNode(scene->mRootNode, scene, model);

	return model;
}

void ModelFactory::ProcessNode(aiNode* _Node, const aiScene* _Scene, std::shared_ptr<Model> _Model)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < _Node->mNumMeshes; i++)
	{
		aiMesh* mesh = _Scene->mMeshes[_Node->mMeshes[i]];
		_Model->meshes.push_back(ProcessMesh(mesh, _Scene));
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < _Node->mNumChildren; i++)
	{
		ProcessNode(_Node->mChildren[i], _Scene, _Model);
	}
}

Mesh ModelFactory::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
{
	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;

	std::vector<glm::vec3> vertPositions;

	//std::vector<Texture> textures;

	for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
	{
		VertexData vertex;
		glm::vec3 vector;

		vector.x = _mesh->mVertices[i].x;
		vector.y = _mesh->mVertices[i].y;
		vector.z = _mesh->mVertices[i].z;
		vertex.aPosition = vector;

		vertPositions.push_back(vector);


		vector.x = _mesh->mNormals[i].x;
		vector.y = _mesh->mNormals[i].y;
		vector.z = _mesh->mNormals[i].z;
		vertex.aNormal = vector;

		vector.x = _mesh->mTangents[i].x;
		vector.y = _mesh->mTangents[i].y;
		vector.z = _mesh->mTangents[i].z;
		vertex.aTangent = vector;

		vector.x = _mesh->mBitangents[i].x;
		vector.y = _mesh->mBitangents[i].y;
		vector.z = _mesh->mBitangents[i].z;
		vertex.aBiTangent = vector;

		if (_mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = _mesh->mTextureCoords[0][i].x;
			vec.y = _mesh->mTextureCoords[0][i].y;
			vertex.aTexCoords = vec;
		}
		else
			vertex.aTexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);

	}

	unsigned int itr = 0;
	// process indices
	for (unsigned int i = 0; i < _mesh->mNumFaces; i++)
	{
		aiFace face = _mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}



	//// process material
	//if (_mesh->mMaterialIndex >= 0)
	//{
	//	aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];

	//	int isMaterialPBR = 0;
	//	aiGetMaterialInteger(material, AI_MATKEY_SHADING_MODEL, &isMaterialPBR);

	//	if (isMaterialPBR == aiShadingMode_PBR_BRDF)
	//	{
	//		std::printf(std::string("Material Is PBR: ").append(_mesh->mName.C_Str()).c_str());
	//	}

	//	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material,
	//		aiTextureType_DIFFUSE, "texture_baseColor");
	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//	std::vector<Texture> specularMaps = LoadMaterialTextures(material,
	//		aiTextureType_SPECULAR, "texture_specular");
	//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	/*}*/

	std::vector<meshopt_Meshlet> meshlets;
	std::vector<unsigned int>    meshletVertices;
	std::vector<unsigned char>   meshletTriangles;

	const float  kConeWeight = 0.0f;

	const size_t maxMeshlets = meshopt_buildMeshletsBound(indices.size(), kMaxVertices, kMaxTriangles);

	meshlets.resize(maxMeshlets);
	meshletVertices.resize(maxMeshlets * kMaxVertices);
	meshletTriangles.resize(maxMeshlets * kMaxTriangles * 3);

	size_t meshletCount = meshopt_buildMeshlets(&meshlets[0],
		&meshletVertices[0],
		&meshletTriangles[0],
		&indices[0],
		indices.size(),
		&vertPositions[0].x,
		vertPositions.size(),
		sizeof(glm::vec3),
		kMaxVertices,
		kMaxTriangles,
		kConeWeight);

	auto& last = meshlets[meshletCount - 1];
	meshletVertices.resize(last.vertex_offset + last.vertex_count);
	meshletTriangles.resize(last.triangle_offset + ((last.triangle_count * 3 + 3) & ~3));
	meshlets.resize(meshletCount);

	//std::vector<uint32_t> meshletTrianglesU32;
	//for (auto& m : meshlets)
	//{
	//	// Save triangle offset for current meshlet
	//	uint32_t triangleOffset = static_cast<uint32_t>(meshletTrianglesU32.size());

	//	// Repack to uint32_t
	//	for (uint32_t i = 0; i < m.triangle_count; ++i)
	//	{
	//		uint32_t i0 = 3 * i + 0 + m.triangle_offset;
	//		uint32_t i1 = 3 * i + 1 + m.triangle_offset;
	//		uint32_t i2 = 3 * i + 2 + m.triangle_offset;

	//		uint8_t  vIdx0 = meshletTriangles[i0];
	//		uint8_t  vIdx1 = meshletTriangles[i1];
	//		uint8_t  vIdx2 = meshletTriangles[i2];
	//		uint32_t packed = ((static_cast<uint32_t>(vIdx0) & 0xFF) << 0) |
	//			((static_cast<uint32_t>(vIdx1) & 0xFF) << 8) |
	//			((static_cast<uint32_t>(vIdx2) & 0xFF) << 16);
	//		meshletTrianglesU32.push_back(packed);
	//	}

	//	// Update triangle offset for current meshlet
	//	m.triangle_offset = triangleOffset;
	//}

	Mesh mesh;
	mesh.vertices = vertices;
	mesh.indices = indices;

	mesh.MSMeshletAmount = meshlets.size();

	glGenBuffers(1, &mesh.MSVerticesBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mesh.MSVerticesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, vertPositions.size() * sizeof(glm::vec3), &vertPositions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &mesh.MSIndicesBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mesh.MSIndicesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &mesh.MSMeshletBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mesh.MSMeshletBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, meshlets.size() * sizeof(meshopt_Meshlet), &meshlets[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &mesh.MSMeshletTrianglesBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mesh.MSMeshletTrianglesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, meshletTriangles.size() * sizeof(uint8_t), &meshletTriangles[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	SetUpMesh(mesh);

	return mesh;
}

//std::vector<Texture> ModelFactory::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
//{
//	std::vector<Texture> textures;
//	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//	{
//		aiString str;
//		mat->GetTexture(type, i, &str);
//		bool skip = false;
//		for (unsigned int j = 0; j < texturesLoaded.size(); j++)
//		{
//			if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0)
//			{
//				textures.push_back(texturesLoaded[j]);
//				skip = true;
//				break;
//			}
//		}
//		if (!skip)
//		{   // if texture hasn't been loaded already, load it
//			Texture texture;
//			texture.id = TextureFromFile(str.C_Str(), directory, gammaCorrection);
//			texture.type = typeName;
//			texture.path = str.C_Str();
//			textures.push_back(texture);
//			texturesLoaded.push_back(texture); // add to loaded textures
//		}
//	}
//	return textures;
//}

//unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
//{
//	std::string filename = std::string(path);
//	filename = directory + '\\' + filename;
//
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//
//	stbi_set_flip_vertically_on_load(false);
//	int width, height, nrComponents;
//	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//	if (data)
//	{
//		GLenum format;
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//		std::cout << "Texture failed to load at path: " << path << std::endl;
//		stbi_image_free(data);
//	}
//
//	return textureID;
//}

int ModelFactory::SetUpMesh(Mesh& _Mesh)
{
	glGenVertexArrays(1, &_Mesh.VAO);
	glGenBuffers(1, &_Mesh.VBO);
	glGenBuffers(1, &_Mesh.EBO);

	glBindVertexArray(_Mesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _Mesh.VBO);

	glBufferData(GL_ARRAY_BUFFER, _Mesh.vertices.size() * sizeof(VertexData), &_Mesh.vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _Mesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _Mesh.indices.size() * sizeof(unsigned int), &_Mesh.indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, aNormal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, aTangent));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, aBiTangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, aTexCoords));

	glBindVertexArray(0);

	return 0;
}