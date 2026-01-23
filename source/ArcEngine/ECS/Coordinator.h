#pragma once

#include "ECS/Data/ECSTypeInfo.h"

#include "ECS/Managers/SystemManager.h"
#include "ECS/Managers/EntityManager.h"
#include "ECS/Managers/ComponentManager.h"

#include "OpenGL/IRenderer.h"

class Coordinator
{
public:
    Coordinator()
    {
        mEntityManager    = std::make_unique<EntityManager>();
        mComponentManager = std::make_unique<ComponentManager>();
        m_SystemManager    = std::make_unique<SystemManager>();
    }
    ~Coordinator()
    {
        mEntityManager.reset();
        mComponentManager.reset();
        m_SystemManager.reset();
    }

    // Entity methods
    Entity CreateEntity()
    {
        return mEntityManager->CreateEntity();
    }

    void DestroyEntity(Entity entity)
    {
        mEntityManager->DestroyEntity(entity);

        mComponentManager->EntityDestroyed(entity);

        m_SystemManager->EntityDestroyed(entity);
    }

    // Component methods
    template<typename T>
    void RegisterComponent()
    {
        mComponentManager->RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        mComponentManager->AddComponent<T>(entity, component);

        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), true);
        mEntityManager->SetSignature(entity, signature);

        m_SystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    void RemoveComponent(Entity entity)
    {
        mComponentManager->RemoveComponent<T>(entity);

        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), false);
        mEntityManager->SetSignature(entity, signature);

        m_SystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    T& GetComponent(Entity entity)
    {
        return mComponentManager->GetComponent<T>(entity);
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        return mComponentManager->GetComponentType<T>();
    }

    template<typename T>
    bool HasComponent(Entity entity)
    {
        return mComponentManager->HasComponent<T>(entity);
    }

    // System methods
    template<typename T, const int priority, typename... Args >
    std::weak_ptr<T> RegisterSystem(Args ..._args)
    {
        return m_SystemManager->RegisterSystem<T, priority, Args... >(_args...);
    }

    template<typename T>
    void SetSystemSignature(Signature signature)
    {
        m_SystemManager->SetSignature<T>(signature);
    }

public:
    void OnCreate()
    {
        m_SystemManager->OnCreate();
    }

    void OnInput()
    {
        m_SystemManager->OnInput();
    }
    void OnUpdate(const float& _DeltaTime)
    {
        m_SystemManager->OnUpdate(_DeltaTime);
    }
    void OnLateUpdate(const float& _DeltaTime)
    {
        m_SystemManager->OnLateUpdate(_DeltaTime);
    }
    void OnRender(std::shared_ptr<IRenderer> _Renderer)
    {
        m_SystemManager->OnRender(_Renderer);
    }
    void OnRenderUI(std::shared_ptr<IRenderer> _Renderer)
    {
        m_SystemManager->OnRenderUI(_Renderer);
    }

    void OnApplicationPause()
    {
        m_SystemManager->OnApplicationPause();
    }
    void OnCheckForDisabled()
    {
        m_SystemManager->OnApplicationPause();
    }

public:
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<SystemManager> m_SystemManager;
};