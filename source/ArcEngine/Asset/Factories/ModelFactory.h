#pragma once

#include "Asset/Interface/IAssetFactory.h"

class ModelFactory : public IAssetFactory
{
public:
    std::shared_ptr<IAsset> CreateAsset(const std::filesystem::path& _AssetPath) override;
};