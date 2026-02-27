#include "MeshletFactory.h"

#include "ArcEngine/Graphics/Graphics.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "ArcEngine/Asset/Types/ClusterModel.h"
#include "ModelFactory.h"

#include <ArcEngine/Thirdparty/MeshOptimizer/meshoptimizer.h>

#include <glad/glad.h>

namespace ArcEngine
{
    std::shared_ptr<IAsset> MeshletFactory::CreateAsset(std::weak_ptr<Graphics> _GraphicsAPI, const aiScene* _Scene)
    {
        std::shared_ptr<ClusterModel> clusterModel = std::make_shared<ClusterModel>();
        clusterModel->type = MODEL;
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
        std::vector<VertexData> rawMeshDatavertices;
        std::vector<unsigned int> rawMeshDataIndices;

        VertexData vertex;
        glm::vec3 vector;

        // Vertex Processing
        for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
        {

            vector.x = _mesh->mVertices[i].x;
            vector.y = _mesh->mVertices[i].y;
            vector.z = _mesh->mVertices[i].z;
            vertex.aPosition = vector;

            //vector.x = _mesh->mNormals[i].x;
            //vector.y = _mesh->mNormals[i].y;
            //vector.z = _mesh->mNormals[i].z;
            //vertex.aNormal = vector;

            //vector.x = _mesh->mTangents[i].x;
            //vector.y = _mesh->mTangents[i].y;
            //vector.z = _mesh->mTangents[i].z;
            //vertex.aTangent = vector;

            //vector.x = _mesh->mBitangents[i].x;
            //vector.y = _mesh->mBitangents[i].y;
            //vector.z = _mesh->mBitangents[i].z;
            //vertex.aBiTangent = vector;

            //if (_mesh->mTextureCoords[0])
            //{
            //    glm::vec2 vec;
            //    vec.x = _mesh->mTextureCoords[0][i].x;
            //    vec.y = _mesh->mTextureCoords[0][i].y;
            //    vertex.aTexCoords = vec;
            //}
            //else
            //    vertex.aTexCoords = glm::vec2(0.0f, 0.0f);

            rawMeshDatavertices.push_back(vertex);

        }

        // Index Processing
        for (unsigned int i = 0; i < _mesh->mNumFaces; i++)
        {
            aiFace face = _mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                rawMeshDataIndices.push_back(face.mIndices[j]);
            }
        }

        //std::vector<VertexData> unindexed_vertices;

        // Generate remap table
        size_t vertex_size = sizeof(VertexData);
        size_t face_count = _mesh->mNumFaces;
        size_t index_count = face_count * 3;
        size_t unindexed_vertex_count = face_count * 3;
        std::vector<unsigned int> remap(unindexed_vertex_count); // temporary remap table
        size_t vertex_count = meshopt_generateVertexRemap(&remap[0], &rawMeshDataIndices[0], index_count,
            &rawMeshDatavertices[0], _mesh->mNumVertices, sizeof(VertexData));

        std::vector<VertexData> targetVertexBuffer;
        std::vector<unsigned int> targetIndexBuffer;

        targetVertexBuffer.resize(vertex_count);
        targetIndexBuffer.resize(index_count);

        // Generate remaped buffers
        meshopt_remapIndexBuffer(&targetIndexBuffer[0], &rawMeshDataIndices[0], index_count, &remap[0]);
        meshopt_remapVertexBuffer(&targetVertexBuffer[0], &rawMeshDatavertices[0], vertex_count, vertex_size, &remap[0]);

        // Optimize Vertex Cache
        meshopt_optimizeVertexCache(&targetIndexBuffer[0], &targetIndexBuffer[0], index_count, vertex_count);

        // Optimize Overdraw
        meshopt_optimizeOverdraw(&targetIndexBuffer[0], &targetIndexBuffer[0], index_count, &targetVertexBuffer[0].aPosition.x, vertex_count, sizeof(VertexData), 1.05f);

        // Optimize Vertex fetch
        meshopt_optimizeVertexFetch(&targetVertexBuffer[0], &targetIndexBuffer[0], index_count, &targetVertexBuffer[0], vertex_count, sizeof(VertexData));

        std::vector<VertexData>& ProcessedVertexData = targetVertexBuffer;
        std::vector<unsigned int>& ProcessedIndexData = targetIndexBuffer;

        // Build meshlet bounds
        size_t max_meshlets = meshopt_buildMeshletsBound(ProcessedIndexData.size(), max_vertices, max_triangles);
        std::vector<meshopt_Meshlet> meshlets(max_meshlets);
        std::vector<unsigned int> meshlet_vertices(ProcessedIndexData.size());
        std::vector<unsigned char> meshlet_triangles(ProcessedIndexData.size()); // note: in v0.25 or prior, use indices.size() + max_meshlets * 3

        // Build meshlets
        size_t meshlet_count = meshopt_buildMeshlets(meshlets.data(), meshlet_vertices.data(), meshlet_triangles.data(), ProcessedIndexData.data(),
            ProcessedIndexData.size(), &ProcessedVertexData[0].aPosition.x, ProcessedVertexData.size(), sizeof(VertexData), max_vertices, max_triangles, cone_weight);

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

        // I need to upload:
        // VertexData
        // Meshlets
        // Meshlet_Vertices
        // Meshlet_Triangles

        _ClusterModel->meshletCount = meshlet_count;

        unsigned int& VBO  = _ClusterModel->VBO;
        unsigned int& MBO  = _ClusterModel->MBO;
        unsigned int& MVBO = _ClusterModel->MVBO;
        unsigned int& MTBO = _ClusterModel->MTBO;

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, ProcessedVertexData.size() * sizeof(VertexData), &ProcessedVertexData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        glGenBuffers(1, &MBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, MBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, meshlets.size() * sizeof(meshopt_Meshlet), &meshlets[0], GL_STATIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        glGenBuffers(1, &MVBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, MVBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, meshlet_vertices.size() * sizeof(unsigned int), &meshlet_vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        glGenBuffers(1, &MTBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, MTBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, meshlet_triangles.size() * sizeof(uint8_t), &meshlet_triangles[0], GL_STATIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    std::vector<VertexData> MeshletFactory::GenerateRemapedVertexBuffer(const void* vertices, size_t vertex_count, size_t vertex_size, const unsigned int* remap)
    {
        std::vector<VertexData> temp;
        temp.resize(vertex_count);
        meshopt_remapVertexBuffer(&temp[0], vertices, vertex_count, vertex_size, &remap[0]);
        return temp;
    }
    std::vector<unsigned int> MeshletFactory::GenerateRemapedIndexBuffer(const unsigned int* indices, size_t index_count, const unsigned int* remap)
    {
        std::vector<unsigned int> temp;
        temp.resize(index_count);
        meshopt_remapIndexBuffer(&temp[0], indices, index_count, &remap[0]);
        return temp;
    }
}