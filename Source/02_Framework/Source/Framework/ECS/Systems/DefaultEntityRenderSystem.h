#pragma once

#include "Framework/ECS/Interfaces/ISystem.h"

class DefaultEntityRenderSystem : public ISystem
{
    void GetSignature(SignatureParameters& _Parameters)       override;

    void OnRender    (State& _State) override;
};