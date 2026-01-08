#pragma once

#include "ECS/Data/ECSTypeInfo.h"

#include "ECS/Managers/SystemManager.h"
#include "ECS/Managers/EntityManager.h"
#include "ECS/Managers/ComponentManager.h"

class Coordinator
{
public:
    Coordinator()
    {
        mEntityManager    = std::make_unique<EntityManager>();
        mComponentManager = std::make_unique<ComponentManager>();
        mSystemManager    = std::make_unique<SystemManager>();
    }
    ~Coordinator()
    {
        mEntityManager.reset();
        mComponentManager.reset();
        mSystemManager.reset();
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

        mSystemManager->EntityDestroyed(entity);
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

        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    void RemoveComponent(Entity entity)
    {
        mComponentManager->RemoveComponent<T>(entity);

        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), false);
        mEntityManager->SetSignature(entity, signature);

        mSystemManager->EntitySignatureChanged(entity, signature);
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
    std::shared_ptr<T> RegisterSystem(Args ..._args)
    {
        return mSystemManager->RegisterSystem<T, priority, Args... >(_args...);
    }

    template<typename T>
    void SetSystemSignature(Signature signature)
    {
        mSystemManager->SetSignature<T>(signature);
    }

public:
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<SystemManager> mSystemManager;
};