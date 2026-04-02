#pragma once

#include "Framework/Component_Based_Architecture/Event.h"

class Entity;

class CollisionEvent : public Event
{
private:
    Entity* entity1;
    Entity* entity2;

public:
    CollisionEvent(Entity* e1, Entity* e2) : entity1(e1), entity2(e2) {}

    Entity* GetEntity1() const { return entity1; }
    Entity* GetEntity2() const { return entity2; }
};