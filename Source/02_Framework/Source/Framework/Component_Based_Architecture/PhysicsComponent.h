#pragma once

#include "Framework/Component_Based_Architecture/Component.h"
#include "Framework/Component_Based_Architecture/EventListener.h"

class EventSystem;

class PhysicsComponent : public Component, public EventListener {
public:
    void OnInitialize() override;

    ~PhysicsComponent() override;

    void OnEvent(const Event& event) override;

private:
    EventSystem& GetEventSystem();
};