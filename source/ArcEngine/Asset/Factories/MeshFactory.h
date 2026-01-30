#pragma once

#include "Asset/Interface/IAssetFactory.h"

#include <Asset/Types/Mesh.h>

class MeshFactory : public IAssetFactory
{
public:
    std::shared_ptr<IAsset> CreateAsset(const std::filesystem::path& _AssetPath) override;

    int SetUpMesh(std::shared_ptr<Mesh> _Mesh);
};