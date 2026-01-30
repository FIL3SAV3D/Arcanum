#include "MeshFactory.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


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

    glBufferData(GL_ARRAY_BUFFER, _Mesh->m_vertices.size() * sizeof(Mesh::VertexData), &_Mesh->m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _Mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _Mesh->m_indices.size() * sizeof(unsigned int), &_Mesh->m_indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), (void*)offsetof(Mesh::VertexData, aNormal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), (void*)offsetof(Mesh::VertexData, aTangent));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), (void*)offsetof(Mesh::VertexData, aBiTangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), (void*)offsetof(Mesh::VertexData, aTexCoords));

    glBindVertexArray(0);

    return 0;
}
