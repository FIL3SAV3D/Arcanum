#pragma once

#include <vector>

#include "Asset/Interface/IAsset.h"

#include "Jolt/Jolt.h"
#include "Jolt/Math/Float3.h"
#include "Jolt/Math/Float2.h"

struct Mesh : public IAsset
{
public:
    struct VertexData
    {
        JPH::Float3 aPosition{};
        JPH::Float3 aNormal{};
        JPH::Float3 aTangent{};
        JPH::Float3 aBiTangent{};
        JPH::Float2 aTexCoords{};
    };

public:
    std::vector< VertexData > m_vertices{};
    std::vector<unsigned int> m_indices {};

public:
    unsigned int VAO{ 0 };
    unsigned int VBO{ 0 };
    unsigned int EBO{ 0 };

};