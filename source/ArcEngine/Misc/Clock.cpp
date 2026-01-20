#include "Clock.h"

#include <GLFW/glfw3.h>

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
	m_CurrentTime = static_cast<float>(glfwGetTime());
	m_DeltaTime = m_CurrentTime - m_LastTime;
	m_LastTime = m_CurrentTime;
}

float Clock::GetTime()
{
	return m_CurrentTime;
}

float Clock::GetDeltaTime()
{
	return m_DeltaTime;
}
