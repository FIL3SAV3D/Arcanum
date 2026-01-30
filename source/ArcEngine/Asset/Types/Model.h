#pragma once

#include "Asset/Interface/IAsset.h"
#include "Mesh.h"

#include <vector>

struct Model : public IAsset
{
    std::vector<Mesh*> m_Meshes;
};