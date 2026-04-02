#include "EventSystem.h"

#include "Framework/Component_Based_Architecture/Event.h"
#include "Framework/Component_Based_Architecture/EventListener.h"

void EventSystem::AddListener(EventListener* listener) {
    listeners.push_back(listener);
}

void EventSystem::RemoveListener(EventListener* listener) {
    auto it = std::find(listeners.begin(), listeners.end(), listener);
    if (it != listeners.end()) {
        listeners.erase(it);
    }
}

void EventSystem::DispatchEvent(const Event& event) {
    for (auto listener : listeners) {
        listener->OnEvent(event);
    }
}