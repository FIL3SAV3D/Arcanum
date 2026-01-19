#pragma once

#include <cstdint>

class IState
{
public:
	IState() {};
	virtual ~IState() {};

	virtual void OnResize( const uint32_t _width, const uint32_t _height ) = 0;

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	virtual void update(const float _delta_time) = 0;
};