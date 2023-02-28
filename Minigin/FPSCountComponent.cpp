#include "FPSCountComponent.h"

dae::FPSCountComponent::FPSCountComponent(): 
	m_FPS{0}, 
	m_pOwner{}
{
}

dae::FPSCountComponent::FPSCountComponent(std::weak_ptr<GameObject> pOwner):
	m_FPS{0},
	m_pOwner{pOwner}
{
}

dae::FPSCountComponent::~FPSCountComponent()
{
}

void dae::FPSCountComponent::Update(float deltaTime)
{
	m_FPS = 1 / deltaTime;
}
