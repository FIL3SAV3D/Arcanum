#pragma once

#include <memory>

#include "ArcEngine/States/IState.h"
#include <ArcEngine/Layer/LayerStack.h>

class StateMain : public IState
{
public:
	StateMain();
	~StateMain();

	void OnResize(const uint32_t _width, const uint32_t _height) override;

	void OnCreate()  override;
	void OnDestroy() override;

	void OnEnter() override;
	void OnExit()  override;

	void update(const float _delta_time) override;

private:
	std::unique_ptr<LayerStack> m_LayerStack;
};