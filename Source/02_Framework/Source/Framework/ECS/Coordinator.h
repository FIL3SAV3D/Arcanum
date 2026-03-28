#pragma once

#include "Framework/ECS/Data/ECSTypeInfo.h"

#include "Framework/ECS/Managers/SystemManager.h"
#include "Framework/ECS/Managers/EntityManager.h"
#include "Framework/ECS/Managers/ComponentManager.h"

class Coordinator
{
public:
    Coordinator()  = default;
    ~Coordinator() = default;

    void Create()
    {
        m_EntityManager.Create();
    }
    void Destroy()
    {
        m_EntityManager.Destroy();
    }

    // Entity methods
    Entity CreateEntity()
    {
        return m_EntityManager.CreateEntity();
    }

    void DestroyEntity(Entity entity)
    {
        m_EntityManager.DestroyEntity(entity);

        m_ComponentManager.EntityDestroyed(entity);

        m_SystemManager.EntityDestroyed(entity);
    }

    // Component methods
    template<typename T>
    void RegisterComponent()
    {
        m_ComponentManager.RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        m_ComponentManager.AddComponent<T>(entity, component);

        auto signature = m_EntityManager.GetSignature(entity);
        signature.set(m_ComponentManager.GetComponentType<T>(), true);
        m_EntityManager.SetSignature(entity, signature);

        m_SystemManager.EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    void RemoveComponent(Entity entity)
    {
        m_ComponentManager.RemoveComponent<T>(entity);

        auto signature = m_EntityManager.GetSignature(entity);
        signature.set(m_ComponentManager.GetComponentType<T>(), false);
        m_EntityManager.SetSignature(entity, signature);

        m_SystemManager.EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    T& GetComponent(Entity entity)
    {
        return m_ComponentManager.GetComponent<T>(entity);
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        return m_ComponentManager.GetComponentType<T>();
    }

    template<typename T>
    bool HasComponent(Entity entity)
    {
        return m_ComponentManager.HasComponent<T>(entity);
    }

    // System methods
    template<typename T, const int priority, typename... Args >
    std::shared_ptr<ISystem> RegisterSystem(Args ..._args)
    {
        return std::static_pointer_cast<ISystem>(m_SystemManager.RegisterSystem<T, priority, Args... >(m_ComponentManager, _args...));
    }

    template<typename T>
    void UnregisterSystem()
    {
        return m_SystemManager.unreg<T>();
    }

    template<typename T>
    void SetSystemSignature(Signature signature)
    {
        m_SystemManager.SetSignature<T>(signature);
    }

public:
    void OnCreate(StartState& _State) { m_SystemManager.OnCreate(_State); }
    void OnStart(StartState& _State) { m_SystemManager.OnStart(_State); }
    void OnInput(InputState& _InputState) { m_SystemManager.OnInput(_InputState); }

    void OnUpdate(GameState& _State, const float& _DeltaTime) { m_SystemManager.OnUpdate(_State, _DeltaTime); };

    void OnLateUpdate(GameState& _State, const float& _DeltaTime)
    {
        m_SystemManager.OnLateUpdate(_State, _DeltaTime);
    }

    void OnRenderStart  (RenderState& _State) { m_SystemManager.OnRenderStart(_State); }
    void OnRender       (RenderState& _State) { m_SystemManager.OnRender(_State); }
    void OnRenderUI     (RenderState& _State) { m_SystemManager.OnRenderUI(_State); }
    void OnRenderEnd    (RenderState& _State) { m_SystemManager.OnRenderEnd(_State); }

    void OnApplicationPause()
    {
        m_SystemManager.OnApplicationPause();
    }
    void OnCheckForDisabled()
    {
        m_SystemManager.OnApplicationPause();
    }

    void OnResize(const glm::uvec2& _Size)
    {
        m_SystemManager.OnResize(_Size);
    }

public:
    ComponentManager m_ComponentManager;
    EntityManager    m_EntityManager;
    SystemManager    m_SystemManager;
};