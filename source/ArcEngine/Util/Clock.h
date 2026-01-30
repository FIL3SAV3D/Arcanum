#pragma once

class Clock
{
public:
	Clock();

	~Clock();

	void Update();

	float GetTime();

	float GetDeltaTime();


private:
	float m_CurrentTime;
	float m_LastTime;
	float m_DeltaTime;
};