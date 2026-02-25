#pragma once

#include "ArcEngine/Asset/Interface/IAsset.h"
#include "ArcEngine/Asset/Interface/IAssetFactory.h"
#include "ArcEngine/Util/Command/ICommand.h"

#include <filesystem>
#include <queue>
#include <unordered_map>

#include "ArcEngine/Asset/Factories/MeshFactory.h"
#include "ArcEngine/Asset/Factories/ModelFactory.h"

#include "ArcEngine/Graphics/Graphics.h"

namespace ArcEngine
{
    class AssetManager
    {
    public:
        AssetManager();
        ~AssetManager();

        void Create();
        void Destroy();

        std::shared_ptr<IAsset> LoadAsset(const std::filesystem::path& _AssetPath);

    private:
        AssetType GetTypeByExtension(const std::filesystem::path& _AssetPath);

    private:
        std::filesystem::path m_AssetFolderFilePath;


        std::unique_ptr<ModelFactory> m_ModelFactory;

        Graphics graphics;


    };

    //class AssetLoadCommand : ICommand
    //{
    //    void Execute() const
    //    {
    //    }

    //    void Undo() const
    //    {
    //    }

    //};
}