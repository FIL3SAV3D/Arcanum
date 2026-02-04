#pragma once

#include "ECS/Interfaces/ISystem.h"

class InputSystem : ISystem
{
    void OnInput(std::shared_ptr<Window> _Window) override;
};