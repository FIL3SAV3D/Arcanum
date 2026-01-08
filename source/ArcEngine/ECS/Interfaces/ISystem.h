#pragma once

#include <set>

#include "ECS/Data/ECSTypeInfo.h"

class ISystem
{
public:
    virtual void OnCreate() = 0;
    virtual void OnUpdate(const float& _deltaTime) = 0;
    virtual void OnDestroy() = 0;

public:
    std::set<Entity> mEntities{};
};