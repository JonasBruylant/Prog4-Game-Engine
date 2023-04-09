#include "Timer.h"

dae::Timer::Timer(): m_DeltaTime{0.f}
{

}

dae::Timer::~Timer()
{

}

void dae::Timer::Update()
{
}

float dae::Timer::GetDeltaTime() const
{
	return m_DeltaTime;
}

void dae::Timer::SetDeltaTime(float deltaTime)
{
	m_DeltaTime = deltaTime;
}

int dae::Timer::GetFrameTimeMS() const
{
	return m_FrameTimeMS;
}
