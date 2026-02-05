#pragma once

#include "ArcEngine/ECS/Data/ECSTypeInfo.h"

#include "ArcEngine/ECS/Managers/SystemManager.h"
#include "ArcEngine/ECS/Managers/EntityManager.h"
#include "ArcEngine/ECS/Managers/ComponentManager.h"

#include "ArcEngine/Renderer/IRenderer.h"

class Coordinator : public std::enable_shared_from_this<Coordinator>
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
    std::shared_ptr<ISystem> RegisterSystem(Args ..._args)
    {
        return std::static_pointer_cast<ISystem>(m_SystemManager->RegisterSystem<T, priority, Args... >(shared_from_this(), _args...));
    }

    template<typename T>
    void SetSystemSignature(Signature signature)
    {
        m_SystemManager->SetSignature<T>(signature);
    }

public:
    const void OnCreate() const { m_SystemManager->OnCreate(); }
    const void OnStart() const { m_SystemManager->OnStart(); }
    const void OnInput(std::shared_ptr<Window> _Window) const { m_SystemManager->OnInput(_Window); }
    void OnUpdate(const float& _DeltaTime)
    {
        m_SystemManager->OnUpdate(_DeltaTime);
    }
    void OnLateUpdate(const float& _DeltaTime)
    {
        m_SystemManager->OnLateUpdate(_DeltaTime);
    }

    const void OnBeginRender(const RenderParams& _RenderParams) const {  };
    const void OnRender     () const { m_SystemManager->OnRender(); };
    const void OnEndRender  (const RenderParams& _RenderParams) const {  };

    void OnRenderUI(const RenderParams& _RenderParams)
    {
        m_SystemManager->OnRenderUI(_RenderParams);
    }

    void OnApplicationPause()
    {
        m_SystemManager->OnApplicationPause();
    }
    void OnCheckForDisabled()
    {
        m_SystemManager->OnApplicationPause();
    }

    void OnResize(const glm::uvec2& _Size)
    {
        m_SystemManager->OnResize(_Size);
    }

public:
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<SystemManager> m_SystemManager;
};