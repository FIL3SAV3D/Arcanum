#include "MeshFactory.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <typeinfo>

std::shared_ptr<IAsset> MeshFactory::CreateAsset(const std::filesystem::path& _AssetPath)
{
    auto mesh = std::make_shared<Mesh>();
    SetUpMesh(mesh);

    return std::shared_ptr<IAsset>();
}

int MeshFactory::SetUpMesh(std::shared_ptr<Mesh> _Mesh)
{
    glGenVertexArrays(1, &_Mesh->VAO);
    glGenBuffers(1, &_Mesh->VBO);
    glGenBuffers(1, &_Mesh->EBO);

    glBindVertexArray(_Mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _Mesh->VBO);

    glBufferData(GL_ARRAY_BUFFER, _Mesh->vertices.size() * sizeof(VertexData), &_Mesh->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _Mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _Mesh->indices.size() * sizeof(unsigned int), &_Mesh->indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, aNormal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, aTangent));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, aBiTangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, aTexCoords));

    glBindVertexArray(0);

    return 0;
}
