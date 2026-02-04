#pragma once

#include "ArcEngine/Asset/Interface/IAsset.h"
#include "Mesh.h"

#include <vector>

struct Model : public IAsset
{
public:
    Model() : meshes{ } {}
    Model(std::vector<Mesh> _Meshes) : meshes{ _Meshes } {}

    std::vector<Mesh> meshes;
};