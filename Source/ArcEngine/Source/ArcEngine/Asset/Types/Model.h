#pragma once

#include "ArcEngine/Asset/Interface/IAsset.h"
#include "Mesh.h"

#include <vector>

struct Model : public IAsset
{
    std::vector<Mesh> meshes;
};