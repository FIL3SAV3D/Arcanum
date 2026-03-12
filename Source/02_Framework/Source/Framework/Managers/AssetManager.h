#pragma once

#include "ArcEngine/Asset/Interface/IAsset.h"

#include "ArcEngine/Util/Command/ICommand.h"

#include <filesystem>
#include <queue>
#include <unordered_map>
#include <string>
#include <memory>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


class AssetManager
{
public:
    AssetManager()  = default;
    ~AssetManager() = default;

    void Create();
    void Destroy();

    void CreateFactories();
    void DestroyFactories();


    void CreateAsset(const std::filesystem::path& _AssetPath);
    void DestroyAsset();

    void ScanAssetFolder();

    void DeserializeAssets();
    void SerializeAssets();

    std::shared_ptr<IAsset> LoadAsset(const char* _Name);

    std::shared_ptr<IAsset> GetAssetByName(const char* _Name);

private:
    AssetType GetTypeByExtension(const std::filesystem::path& _AssetPath);

    AssetID MakeNewAssetID();

    const aiScene* ImportObject(const std::filesystem::path& _AssetPath);

    void ProcessNode(aiNode* _Node, const aiScene* _Scene);
    void ProcessMesh(aiMesh* _mesh, const aiScene* _scene);

private:

    std::string m_AssetFolderFilePath{};
    std::string m_SerializedAssetFolderFilePath{};

    std::unordered_map<std::string, std::shared_ptr<IAsset>> m_AssetList{};

    std::vector<std::string> availableAssets;

    bool m_MeshShading = false;

private:
    Assimp::Importer importer;
};