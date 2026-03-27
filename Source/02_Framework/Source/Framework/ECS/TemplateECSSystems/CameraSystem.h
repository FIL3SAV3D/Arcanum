#pragma once

#include "Framework/ECS/Interfaces/ISystem.h"

class CameraSystem : public ISystem
{
public:
    void GetSignature(SignatureParameters& _Parameters) override;

public:
    void OnInput(InputState& _InputState) override;
    void OnLateUpdate(GameState& _State, const float& _DeltaTime) override;

    void OnRenderStart(RenderState& _RenderState) override;

private:
    float yaw   = 0.0f;
    float pitch = 0.0f;

    const float m_CameraSpeed = 10.0f;
    const float m_CameraShiftSpeed = 25.0f;
    const float m_CameraSensitivity = 0.1f;
};