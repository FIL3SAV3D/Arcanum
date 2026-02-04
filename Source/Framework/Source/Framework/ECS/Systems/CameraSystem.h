#pragma once

#include "ArcEngine/ECS/Interfaces/ISystem.h"

class CameraSystem : ISystem
{
    void OnInput(std::shared_ptr<Window> _Window) override;
    void OnUpdate(const float& _DeltaTime) override;
    void OnLateUpdate(const float& _DeltaTime) override;

private:
    bool firstMouse = true;
};