#include "ModelFactory.h"

#include "Asset/Types/Model.h"

std::shared_ptr<IAsset> ModelFactory::CreateAsset(const std::filesystem::path& _AssetPath)
{
    std::shared_ptr<Model> model;

    return std::shared_ptr<IAsset>();
}
