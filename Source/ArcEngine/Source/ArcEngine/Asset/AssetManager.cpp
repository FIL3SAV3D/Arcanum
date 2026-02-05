#include "AssetManager.h"
#include <iostream>

#include "ArcEngine/Asset/Types/Mesh.h"

namespace ArcEngine
{
    AssetManager::AssetManager()
    {
        m_ModelFactory = std::make_unique<ModelFactory>();
    }

    AssetManager::~AssetManager()
    {
        m_ModelFactory.reset();
    }

    std::shared_ptr<IAsset> AssetManager::LoadAsset(const std::filesystem::path& _AssetPath)
    {
        std::shared_ptr<IAsset> asset;
        switch (GetTypeByExtension(_AssetPath))
        {
        case MESH:
            break;
        case MODEL:
        {
            asset = m_ModelFactory->CreateAsset(_AssetPath);
            break;
        }

        case TEXTURE:
            break;
        case NONE:
            break;
        default:
            break;
        }

        return asset;
    }

    AssetType AssetManager::GetTypeByExtension(const std::filesystem::path& _AssetPath)
    {
        std::cout << _AssetPath.extension() << std::endl;

        if (_AssetPath.extension() == ".fbx") { return AssetType::MODEL; }
        if (_AssetPath.extension() == ".obj") { return AssetType::MODEL; }
        if (_AssetPath.extension() == ".glb") { return AssetType::MODEL; }

        return AssetType::NONE;
    }
}