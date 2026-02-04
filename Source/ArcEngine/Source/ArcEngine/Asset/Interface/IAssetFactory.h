#pragma once

#include "ArcEngine/Asset/Interface/IAsset.h"

#include <memory>
#include <filesystem>

class IAssetFactory
{
public:
    virtual std::shared_ptr<IAsset> CreateAsset(const std::filesystem::path& _AssetPath) = 0;
    virtual ~IAssetFactory() {};
};