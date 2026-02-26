#pragma once

#include "ArcEngine/Asset/Interface/IAssimpFactory.h"

#include "ArcEngine/Asset/Types/Model.h"
#include "ArcEngine/Asset/Types/Mesh.h"

#include <string>

namespace ArcEngine
{
    class ModelFactory : public IAssimpFactory
    {
    public:
        std::shared_ptr<IAsset> CreateAsset(std::weak_ptr<Graphics> _GraphicsAPI, const aiScene* _Scene) override;

        std::shared_ptr<Model> LoadModel(std::string _path);
        void ProcessNode(aiNode* _Node, const aiScene* _Scene, std::shared_ptr<Model> _Model);
        
        Mesh ProcessMesh(aiMesh* _mesh, const aiScene* _scene);

        int SetUpMesh(Mesh& _Mesh);
    };
}