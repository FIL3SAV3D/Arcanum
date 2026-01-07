#pragma once

#include "ISystem.h"

class RenderSystem : ISystem
{
public:
	// Inherited via ISystem
	void OnCreate() override;
	void OnStart() override;
	void OnFixedUpdate(const float& _deltaTime) override;
	void OnUpdate(const float& _deltaTime) override;
	void OnRender() override;
	void OnRenderPost() override;
	void OnLateUpdate(const float& _deltaTime) override;
	void OnStop() override;
	void OnDestroy() override;

private:

};