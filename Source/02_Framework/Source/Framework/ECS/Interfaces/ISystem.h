#pragma once

#include <set>

#include "Framework/ECS/Data/ECSTypeInfo.h"
#include <ArcEngine/Renderer/IRenderer.h>

#include <memory>

#include "ArcEngine/Platform/Window.h"

class Coordinator;
class ComponentManager;

namespace ArcEngine
{
    class ArcEngine;
}

enum UpdatePriorities
{
    BEGIN_UPDATE = 0,
    UPDATE = 100,
    LATE_UPDATE = 200,
    END_UPDATE = 300,
};

struct SignatureParameters
{
    Signature signature{};
    ComponentManager& componentManager;
};

struct State
{
    Coordinator& coordinator;
    ArcEngine::ArcEngine& engine;
};

class ISystem
{
public:
    virtual void GetSignature(SignatureParameters& _Parameters) = 0;

public:
    virtual void OnCreate       (State& _State)                              {};
    virtual void OnDestroy      (State& _State)                              {};

    virtual void OnStart        (State& _State)                              {};
    virtual void OnEnd          (State& _State)                              {};

    virtual void OnInput           (std::shared_ptr<ArcEngine::Window> _Window) {};

    virtual void OnUpdate          (State& _State, const float& _DeltaTime)     {};
    virtual void OnLateUpdate      (State& _State, const float& _DeltaTime)     {};

    virtual void OnRender          (State& _State) {};
    virtual void OnRenderUI        (const RenderParams& _RenderParams) {};


    virtual void OnResize(const glm::uvec2& _Size) {};

public:
    std::set<Entity> mEntities{};
};