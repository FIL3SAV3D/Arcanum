#pragma once

#include <cstdint>

class Clock
{
public:
	Clock();

	~Clock();

	void Update();

	double GetTime();

	double GetDeltaTime();


private:
	uint64_t m_CurrentTime;
	uint64_t m_LastTime;
	double m_DeltaTime;
};