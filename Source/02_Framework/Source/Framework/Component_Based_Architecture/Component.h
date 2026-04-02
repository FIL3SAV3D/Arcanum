#pragma once

#include "Framework/Component_Based_Architecture/ComponentTypeIDSystem.h"

class Entity;

class Component
{
public:
    enum class State {
        Uninitialized,
        Initializing,
        Active,
        Destroying,
        Destroyed
    };

private:
    State state = State::Uninitialized;
    Entity* owner = nullptr;

public:
    virtual ~Component() = default;

    template<typename T>
    static size_t GetTypeID() {
        return ComponentTypeIDSystem::GetTypeID<T>();
    }

    void Initialize();

    void Destroy();

    bool IsActive() const { return state == State::Active; }

    void SetOwner(Entity* entity);
    Entity* GetOwner() const;

protected:
    virtual void OnInitialize() {}
    virtual void OnDestroy() {}
    virtual void Update(float deltaTime) {}
    virtual void Render() {}

    friend class Entity; // Allow Entity to call protected methods
};