#pragma once

//#include <ArcEngine/Asset/Types/Model.h>

#include <memory>

struct Model;
struct ModelComponent
{
    std::shared_ptr<Model> model;
};