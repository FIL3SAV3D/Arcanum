#pragma once

#include "Framework/ECS/Interfaces/ISystem.h"

class UIRenderSystem : public ISystem
{
public:
	void GetSignature(SignatureParameters& _Parameters) override;

	void OnRenderUI(RenderState& _RenderState) override;

private:
	std::string adress{};
};