#pragma once

#include <memory>

class Coordinator;

class IScene
{
public:
    virtual void Create (std::shared_ptr<Coordinator> _coordinator)  {};
    virtual void Destroy(std::shared_ptr<Coordinator> _coordinator) {};
};