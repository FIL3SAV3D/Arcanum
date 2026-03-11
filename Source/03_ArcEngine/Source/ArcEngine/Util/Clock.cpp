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
	m_CurrentTime = SDL_GetPerformanceCounter();
	m_DeltaTime = (double)((m_CurrentTime - m_LastTime) * 1000 / (double)SDL_GetPerformanceFrequency());
	m_DeltaTime = m_DeltaTime * 0.001;
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
