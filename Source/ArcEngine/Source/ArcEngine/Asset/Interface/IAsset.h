#pragma once

enum AssetType
{
    MESH,
    MODEL,
    TEXTURE,
    NONE
};

struct IAsset
{
    AssetType type;
};