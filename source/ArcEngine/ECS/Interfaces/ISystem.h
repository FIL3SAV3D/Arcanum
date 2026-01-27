#pragma once

#include <set>

#include "ECS/Data/ECSTypeInfo.h"
#include <OpenGL/IRenderer.h>

#include <memory>

class Coordinator;

class ISystem
{
public:
    virtual void OnCreate          ()                                     {};
    virtual void OnStart() {};
    virtual void OnInput           ()                                     {};
    virtual void OnUpdate          (const float& _DeltaTime)              {};
    virtual void OnLateUpdate      (const float& _DeltaTime)              {};
    virtual void OnRender          (std::shared_ptr<IRenderer> _Renderer) {};
    virtual void OnRenderUI        (std::shared_ptr<IRenderer> _Renderer) {};
    virtual void OnApplicationPause()                                     {};
    virtual void OnCheckForDisabled()                                     {};
    virtual void OnQuit() {};
    virtual void OnDestroy         ()                                     {};

public:
    std::set<Entity> mEntities{};

    std::shared_ptr<Coordinator> coordinator;
};