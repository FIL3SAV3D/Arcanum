#include "AssetManager.h"
#include <iostream>

#include "Asset/Types/Mesh.h"

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

    bool AssetManager::LoadAsset(const std::filesystem::path& _AssetPath)
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

        return false;
    }

    AssetType AssetManager::GetTypeByExtension(const std::filesystem::path& _AssetPath)
    {
        std::cout << _AssetPath.extension() << std::endl;
        return AssetType::MESH;
    }
}