#pragma once

#include "ArcEngine/Asset/Interface/IAssetFactory.h"
#include "MeshFactory.h"

#include "ArcEngine/Asset/Types/Model.h"
#include "ArcEngine/Asset/Types/Mesh.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <string>

class ModelFactory : public IAssetFactory
{
public:
    std::shared_ptr<IAsset> CreateAsset(const std::filesystem::path& _AssetPath) override;

    std::shared_ptr<Model> LoadModel(std::string _path);
    void ProcessNode(aiNode* _Node, const aiScene* _Scene, std::shared_ptr<Model> _Model);
    Mesh ProcessMesh(aiMesh* _mesh, const aiScene* _scene);

    int SetUpMesh(Mesh& _Mesh);
    
private:
    MeshFactory m_MeshFactory;
};