#pragma once

#include "ArcEngine/Asset/Interface/IAsset.h"

#include <memory>
#include <filesystem>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace ArcEngine
{
    class Graphics;

    class IAssimpFactory
    {
    public:
        virtual std::shared_ptr<IAsset> CreateAsset(std::weak_ptr<Graphics> _GraphicsAPI, const aiScene* _Scene) = 0;
        virtual ~IAssimpFactory() {};
    };
}