#pragma once

#include "IState.h"

class StateMain : public IState
{
	// Inherited via IState
	void OnResize(const uint32_t _width, const uint32_t _height) override;
	void OnCreate() override;
	void OnDestroy() override;
	void OnEnter() override;
	void OnExit() override;
	void RenderPrePass() override;
	void Render3D() override;
	void RenderPostPass() override;
};