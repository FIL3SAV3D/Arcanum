#include "AssetManager.h"
#include <iostream>

#include "ArcEngine/Asset/Types/Mesh.h"
#include "ArcEngine/Util/Filepath.h"



#include "ArcEngine/Graphics/Graphics.h"

#include "Serializer.h"

namespace ArcEngine
{
    AssetManager::AssetManager()
    {
        
    }

    AssetManager::~AssetManager()
    {
        
    }

    void AssetManager::Create(std::weak_ptr<Graphics> _GraphicsAPI, bool _MeshShading)
    {
        graphicsAPI = _GraphicsAPI;
        m_MeshShading = _MeshShading;

        m_MeshletFactory = std::make_unique<MeshletFactory>();

        m_AssetFolderFilePath = Filepath::FindAssetFolder();
        m_SerializedAssetFolderFilePath = std::string(m_AssetFolderFilePath).append("\\SerializedAssets");
        //DeserializeAssets();
        ScanAssetFolder();
    }

    void AssetManager::Destroy()
    {
    }

    void AssetManager::CreateFactories()
    {
    }

    void AssetManager::DestroyFactories()
    {
    }
     
    const aiScene* AssetManager::ImportObject(const std::filesystem::path& _AssetPath)
    {
        const aiScene* scene = importer.ReadFile(_AssetPath.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            fmt::println("ERROR::ASSIMP::{}", importer.GetErrorString());
            return nullptr;
        }

        return scene;
    }

    void AssetManager::CreateAsset(const std::filesystem::path& _AssetPath)
    {

    }

    void AssetManager::DestroyAsset()
    {
    }

    void AssetManager::ScanAssetFolder()
    {
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(m_AssetFolderFilePath))
        {
            if (GetTypeByExtension(dirEntry) == NONE)
            {
                continue;
            }

            availableAssets.push_back(dirEntry.path().string());
            fmt::println("POSSIBLE ASSET FOUND: {}", dirEntry.path().string());
        }
    }

    void AssetManager::DeserializeAssets()
    {
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(m_SerializedAssetFolderFilePath))
        {
            if (!Serializer::HasSerializedExtension(dirEntry))
                continue;

            std::cout << dirEntry << std::endl;
        }
    }

    void AssetManager::SerializeAssets()
    {
    }

    std::shared_ptr<IAsset> AssetManager::LoadAsset(const char* _Name)
    {
        // Load Asset from list
        std::string assetName = { _Name };
        if (m_AssetList.contains(assetName))
        {
            return m_AssetList[_Name];
        }

        // If list didn't contain it then try finding path to file
        bool foundAsset = false;
        std::filesystem::path assetPath = {};
        for (const auto& path : availableAssets)
        {
            if (path.find(assetName, 0) != std::string::npos)
            {
                foundAsset = true;
                assetPath = path;
                break;
            }
        }

        // If asset is not in folder then return nullptr
        if (!foundAsset)
            return nullptr;

        switch (GetTypeByExtension(assetPath))
        {
        case MODEL:
        {
            const aiScene* scene = ImportObject(assetPath);
            auto asset = m_MeshletFactory->CreateAsset(graphicsAPI, scene);
            asset->assetPath = assetPath.string();
            return asset;
        }
        case TEXTURE:
        {
            assert(false && "Not yet implemented");
            break;
        }
        case NONE:
        {
            assert(false && "Not yet implemented");
            break;
        }
        default:
            break;
        }

        return nullptr;
    }

    std::shared_ptr<IAsset> AssetManager::GetAssetByName(const char* _Name)
    {
        return m_AssetList[_Name];
    }

    AssetType AssetManager::GetTypeByExtension(const std::filesystem::path& _AssetPath)
    {
        if (_AssetPath.extension() == ".fbx") { return AssetType::MODEL; }
        if (_AssetPath.extension() == ".obj") { return AssetType::MODEL; }
        if (_AssetPath.extension() == ".glb") { return AssetType::MODEL; }

        return AssetType::NONE;
    }


}