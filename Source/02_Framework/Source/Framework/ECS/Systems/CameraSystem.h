#pragma once

#include "Framework/ECS/Interfaces/ISystem.h"

class CameraSystem : ISystem
{
    void OnInput(std::shared_ptr<ArcEngine::Window> _Window) override;
    void OnLateUpdate(State& _State, const float& _DeltaTime) override;

private:
    bool firstMouse = true;
};