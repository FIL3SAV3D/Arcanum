#pragma once

#include "ArcEngine/Asset/Interface/IAsset.h"
#include "Mesh.h"

#include <vector>

struct Model : public IAsset
{
public:
    Model() : meshes{ } {}
    Model(const Model& _Model) : meshes{ _Model.meshes } {}
    Model(std::vector<Mesh> _Meshes) : meshes{ _Meshes } {}

    Model& operator=(const Model& _Model) { 
        this->meshes = _Model.meshes;
        return *this;
    }

    std::vector<Mesh> meshes;
};