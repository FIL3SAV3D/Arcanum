#pragma once

#include <set>
#include "Framework/ECS/Data/ECSTypeInfo.h"
#include <memory>
#include <glm/glm.hpp>
#include <Framework/States/States.h>

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



class ISystem
{
public:
    virtual void GetSignature(SignatureParameters& _Parameters) = 0;

public:
    virtual void OnCreate           (StartState& _State)                              {};
    virtual void OnDestroy          (EndState& _State)                              {};

    virtual void OnStart            (StartState& _State)                              {};
    virtual void OnEnd              (EndState& _State)                              {};

    virtual void OnInput            (InputState& _InputState) {};

    virtual void OnUpdate           (GameState& _State, const float& _DeltaTime)     {};
    virtual void OnLateUpdate       (GameState& _State, const float& _DeltaTime)     {};

    virtual void OnRenderStart      (RenderState& _RenderState) {};
    virtual void OnRender           (RenderState& _RenderState) {};
    virtual void OnRenderUI         (RenderState& _RenderState) {};
    virtual void OnRenderEnd        (RenderState& _RenderState) {};

    virtual void OnWindowResize(const glm::uvec2& _Size) {};

public:
    std::set<Entity> mEntities{};
};