#pragma once

#include "ArcEngine/Asset/Interface/IAsset.h"
#include "ArcEngine/Core/VertexData.h"

#include <vector>

#define kMaxTriangles 124
#define kMaxVertices  64

struct Mesh : public IAsset
{
public:
    std::vector< VertexData > vertices{};
    std::vector< unsigned int > indices {};

    unsigned int VAO{ 0 };
    unsigned int VBO{ 0 };
    unsigned int EBO{ 0 };

    unsigned int MSMeshletAmount;

    unsigned int MSVerticesBuffer;
    unsigned int MSIndicesBuffer;

    unsigned int MSMeshletBuffer;
    unsigned int MSMeshletVerticiesBuffer;
    unsigned int MSMeshletTrianglesBuffer;

};