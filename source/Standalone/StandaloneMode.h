#pragma once

#include "Modes/IMode.h"

class StandaloneMode : public IMode
{
	// Inherited via IMode
	void OnResize(const uint32_t _width, const uint32_t _height) override;
	void OnCreate() override;
	void OnDestroy() override;
	void OnEnter() override;
	void OnExit() override;
	void Update(const float& _delta_time) override;
};