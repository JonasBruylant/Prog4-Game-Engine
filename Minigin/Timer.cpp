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

float dae::Timer::GetTotalTime() const
{
	return m_TotalTime;
}

void dae::Timer::SetDeltaTime(float deltaTime)
{
	m_DeltaTime = deltaTime;
	m_TotalTime += deltaTime;
}

int dae::Timer::GetFrameTimeMS() const
{
	return m_FrameTimeMS;
}
