#pragma once

#include <cinttypes>
#include <string>

enum AssetType
{
    MODEL,
    TEXTURE,
    SHADER,
    NONE
};

typedef std::uint32_t AssetID;

struct IAsset
{
    AssetType type;
    std::string assetPath;
};