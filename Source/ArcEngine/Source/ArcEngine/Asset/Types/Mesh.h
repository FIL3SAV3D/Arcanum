#pragma once

#include "Asset/Interface/IAsset.h"
#include "Core/VertexData.h"

#include <vector>


struct Mesh : public IAsset
{
public:
    Mesh() : vertices{}, indices{} {}
    Mesh(const std::vector< VertexData >&   _Vertices, const std::vector< unsigned int >& _Indices) : vertices{_Vertices}, indices{_Indices} {}

public:

    std::vector< VertexData > vertices{};
    std::vector< unsigned int > indices {};

    unsigned int VAO{ 0 };
    unsigned int VBO{ 0 };
    unsigned int EBO{ 0 };

};