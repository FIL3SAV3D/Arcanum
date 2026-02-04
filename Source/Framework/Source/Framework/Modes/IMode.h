#pragma once

#include <cstdint>
#include <memory>
#include <string>

class IMode
{
public:
	IMode() = default;
	virtual ~IMode() = default;

	virtual void OnResize( const uint32_t _width, const uint32_t _height ) = 0;

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Update(const float& _delta_time) = 0;

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
private:
};