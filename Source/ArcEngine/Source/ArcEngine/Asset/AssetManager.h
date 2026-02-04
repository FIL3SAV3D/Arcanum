#pragma once

#include "Asset/Interface/IAsset.h"
#include "Asset/Interface/IAssetFactory.h"
#include "Util/Command/ICommand.h"

#include <filesystem>
#include <queue>
#include <unordered_map>

#include "Asset/Factories/MeshFactory.h"
#include "Asset/Factories/ModelFactory.h"

namespace ArcEngine
{
    class AssetManager
    {
    public:
        AssetManager();
        ~AssetManager();

        bool LoadAsset(const std::filesystem::path& _AssetPath);

    private:
        AssetType GetTypeByExtension(const std::filesystem::path& _AssetPath);

    private:
        std::unique_ptr<ModelFactory> m_ModelFactory;
    };



    class AssetLoadCommand : ICommand
    {
        void Execute() const
        {
        }

        void Undo() const
        {
        }

    };
}