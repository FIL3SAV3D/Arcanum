#pragma once

enum AssetType
{
    MESH,
    MODEL,
    TEXTURE,
    VERTEX_SHADER,
    FRAGMENT_SHADER,
    COMPUTE_SHADER,
    NONE
};

struct IAsset
{
    AssetType type;
};