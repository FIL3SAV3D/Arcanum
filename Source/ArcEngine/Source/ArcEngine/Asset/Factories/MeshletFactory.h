#pragma once

#include "ArcEngine/Asset/Interface/IAssimpFactory.h"
#include "ArcEngine/Core/VertexData.h"

struct ClusterModel;

namespace ArcEngine
{
    const size_t max_vertices = 64;
    const size_t max_triangles = 126; // note: in v0.25 or prior, max_triangles needs to be divisible by 4
    const float cone_weight = 0.0f;

    class MeshletFactory : public IAssimpFactory
    {
    public:
        std::shared_ptr<IAsset> CreateAsset(std::weak_ptr<Graphics> _GraphicsAPI, const aiScene* _Scene) override;

    private:
        void ProcessNode(aiNode* _Node, const aiScene* _Scene, std::shared_ptr<ClusterModel> _ClusterModel);
        void ProcessMesh(aiMesh* _mesh, const aiScene* _scene, std::shared_ptr<ClusterModel> _ClusterModel);

        std::vector<VertexData> GenerateRemapedVertexBuffer(const void* vertices, size_t vertex_count, size_t vertex_size, const unsigned int* remap);
        std::vector<unsigned int> GenerateRemapedIndexBuffer(const unsigned int* indices, size_t index_count, const unsigned int* remap);
    };
}