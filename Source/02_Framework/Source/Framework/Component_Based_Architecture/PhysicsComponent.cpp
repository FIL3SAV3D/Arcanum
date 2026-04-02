#include "PhysicsComponent.h"

#include "Framework/Component_Based_Architecture/CollisionEvent.h"
#include "Framework/Component_Based_Architecture/EventSystem.h"

void PhysicsComponent::Initialize() {
    // Register as event listener
    GetEventSystem().AddListener(this);
}

PhysicsComponent::~PhysicsComponent() {
    // Unregister as event listener
    GetEventSystem().RemoveListener(this);
}

void PhysicsComponent::OnEvent(const Event& event) {
    if (auto collisionEvent = dynamic_cast<const CollisionEvent*>(&event)) {
        // Handle collision event
    }
}

EventSystem& PhysicsComponent::GetEventSystem() {
    // Get event system from somewhere (e.g., service locator)
    static EventSystem eventSystem;
    return eventSystem;
}