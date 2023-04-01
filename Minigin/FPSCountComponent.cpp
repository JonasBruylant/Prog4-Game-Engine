#include "FPSCountComponent.h"
#include "TextObjectComponent.h"

dae::FPSCountComponent::FPSCountComponent(std::weak_ptr<GameObject> pOwner): Component(pOwner),
	m_FPS{0.f}
{
}

dae::FPSCountComponent::~FPSCountComponent()
{
}

void dae::FPSCountComponent::Update()
{
	m_FPS = 1.f / m_Timer.GetDeltaTime();
	GetOwner().lock()->GetComponent<TextObjectComponent>()->SetText(std::to_string(int(m_FPS)));

}
