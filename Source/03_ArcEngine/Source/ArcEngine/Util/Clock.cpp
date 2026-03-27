#include "Clock.h"

#include <SDL3/SDL.h>

Clock::Clock():
m_CurrentTime{ 0 },
m_LastTime{ 0 },
m_DeltaTime{ 0 }
{
}

Clock::~Clock()
{
}

void Clock::Update()
{
	m_CurrentTime = SDL_GetTicks();
	m_DeltaTime = (m_CurrentTime - m_LastTime) / 1000.0;
	m_LastTime = m_CurrentTime;
}

double Clock::GetTime()
{
	return m_CurrentTime;
}

double Clock::GetDeltaTime()
{
	return m_DeltaTime;
}
