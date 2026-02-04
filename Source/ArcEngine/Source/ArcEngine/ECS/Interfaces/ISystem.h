#pragma once

#include <set>

#include "ArcEngine/ECS/Data/ECSTypeInfo.h"
#include <ArcEngine/Renderer/IRenderer.h>

#include <memory>

#include "ArcEngine/OpenGL/Window.h"

class Coordinator;

class ISystem
{
public:
    virtual void OnCreate          ()                                     {};
    virtual void OnStart() {};
    virtual void OnInput           (std::shared_ptr<Window> _Window)      {};
    virtual void OnUpdate          (const float& _DeltaTime)              {};
    virtual void OnLateUpdate      (const float& _DeltaTime)              {};

    virtual void OnBeginRender     (const RenderParams& _RenderParams) {};
    virtual void OnRender          (const RenderParams& _RenderParams) {};
    virtual void OnEndRender       (const RenderParams& _RenderParams) {};

    virtual void OnBeginRenderUI   (const RenderParams& _RenderParams) {};
    virtual void OnRenderUI        (const RenderParams& _RenderParams) {};
    virtual void OnEndRenderUI   (const RenderParams& _RenderParams) {};

    virtual void OnApplicationPause()                                     {};
    virtual void OnCheckForDisabled()                                     {};
    virtual void OnQuit() {};
    virtual void OnDestroy         ()                                     {};

public:
    std::set<Entity> mEntities{};

    std::shared_ptr<Coordinator> coordinator;
};