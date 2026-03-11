#pragma once

#include "Framework/ECS/Interfaces/ISystem.h"

class InputSystem : ISystem
{
    void OnInput(std::shared_ptr<ArcEngine::Window> _Window) override;
};