#include "MeshletFactory.h"

#include "ArcEngine/Graphics/Graphics.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "ArcEngine/Asset/Types/ClusterModel.h"
#include "ModelFactory.h"

#include <ArcEngine/Thirdparty/MeshOptimizer/meshoptimizer.h>

namespace ArcEngine
{
    std::shared_ptr<IAsset> MeshletFactory::CreateAsset(std::weak_ptr<Graphics> _GraphicsAPI, const aiScene* _Scene)
    {
        std::shared_ptr<ClusterModel> clusterModel = std::make_shared<ClusterModel>();

        ProcessNode(_Scene->mRootNode, _Scene, clusterModel);

        return clusterModel;
    }

    void MeshletFactory::ProcessNode(aiNode* _Node, const aiScene* _Scene, std::shared_ptr<ClusterModel> _ClusterModel)
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < _Node->mNumMeshes; i++)
        {
            aiMesh* mesh = _Scene->mMeshes[_Node->mMeshes[i]];
            ProcessMesh(mesh, _Scene, _ClusterModel);
        }

        // then do the same for each of its children
        for (unsigned int i = 0; i < _Node->mNumChildren; i++)
        {
            ProcessNode(_Node->mChildren[i], _Scene, _ClusterModel);
        }
    }

    void MeshletFactory::ProcessMesh(aiMesh* _mesh, const aiScene* _scene, std::shared_ptr<ClusterModel> _ClusterModel)
    {
        std::vector<VertexData> vertices;
        std::vector<unsigned int> indices;

        VertexData vertex;
        glm::vec3 vector;

        // Vertex Processing
        for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
        {

            vector.x = _mesh->mVertices[i].x;
            vector.y = _mesh->mVertices[i].y;
            vector.z = _mesh->mVertices[i].z;
            vertex.aPosition = vector;

            vector.x = _mesh->mNormals[i].x;
            vector.y = _mesh->mNormals[i].y;
            vector.z = _mesh->mNormals[i].z;
            vertex.aNormal = vector;

            vector.x = _mesh->mTangents[i].x;
            vector.y = _mesh->mTangents[i].y;
            vector.z = _mesh->mTangents[i].z;
            vertex.aTangent = vector;

            vector.x = _mesh->mBitangents[i].x;
            vector.y = _mesh->mBitangents[i].y;
            vector.z = _mesh->mBitangents[i].z;
            vertex.aBiTangent = vector;

            if (_mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = _mesh->mTextureCoords[0][i].x;
                vec.y = _mesh->mTextureCoords[0][i].y;
                vertex.aTexCoords = vec;
            }
            else
                vertex.aTexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);

        }

        // Index Processing
        for (unsigned int i = 0; i < _mesh->mNumFaces; i++)
        {
            aiFace face = _mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        std::vector<VertexData> unindexed_vertices;

        // Generate remap table
        size_t face_count = _mesh->mNumFaces;
        size_t index_count = face_count * 3;
        size_t unindexed_vertex_count = face_count * 3;
        std::vector<unsigned int> remap(unindexed_vertex_count); // temporary remap table
        size_t vertex_count = meshopt_generateVertexRemap(&remap[0], NULL, index_count,
            &unindexed_vertices[0], unindexed_vertex_count, sizeof(VertexData));

        // Generate remaped buffers
        meshopt_remapIndexBuffer(&indices[0], nullptr, index_count, &remap[0]);
        meshopt_remapVertexBuffer(&vertices[0], &unindexed_vertices[0], unindexed_vertex_count, sizeof(VertexData), &remap[0]);

        // Optimize Vertex Cache
        meshopt_optimizeVertexCache(&indices[0], &indices[0], index_count, vertex_count);

        // Optimize Overdraw
        meshopt_optimizeOverdraw(&indices[0], &indices[0], index_count, &vertices[0].aPosition.x, vertex_count, sizeof(VertexData), 1.05f);

        // Optimize Vertex fetch
        meshopt_optimizeVertexFetch(&vertices[0], &indices[0], index_count, &vertices[0], vertex_count, sizeof(VertexData));

        // Build meshlet bounds
        size_t max_meshlets = meshopt_buildMeshletsBound(indices.size(), max_vertices, max_triangles);
        std::vector<meshopt_Meshlet> meshlets(max_meshlets);
        std::vector<unsigned int> meshlet_vertices(indices.size());
        std::vector<unsigned char> meshlet_triangles(indices.size()); // note: in v0.25 or prior, use indices.size() + max_meshlets * 3

        // Build meshlets
        size_t meshlet_count = meshopt_buildMeshlets(meshlets.data(), meshlet_vertices.data(), meshlet_triangles.data(), indices.data(),
            indices.size(), &vertices[0].aPosition.x, vertices.size(), sizeof(VertexData), max_vertices, max_triangles, cone_weight);

        // Resize last meshlet
        const meshopt_Meshlet& last = meshlets[meshlet_count - 1];

        meshlet_vertices.resize(last.vertex_offset + last.vertex_count);
        meshlet_triangles.resize(last.triangle_offset + last.triangle_count * 3);
        meshlets.resize(meshlet_count);

        // Optimized Meshlet
        for (size_t i = 0; i < meshlet_count; i++)
        {
            const meshopt_Meshlet& m = meshlets[i];
            meshopt_optimizeMeshlet(&meshlet_vertices[m.vertex_offset], &meshlet_triangles[m.triangle_offset], m.triangle_count, m.vertex_count);
        }

    }
}