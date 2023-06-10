#include "FPSCountComponent.h"
#include "TextObjectComponent.h"

dae::FPSCountComponent::FPSCountComponent(GameObject* pOwner): Component(pOwner),
	m_FPS{0.f}
{
	m_pTextObjectComponent = pOwner->GetComponent<TextObjectComponent>().get();
}

void dae::FPSCountComponent::Update()
{
	m_FPS = 1.f / m_Timer.GetDeltaTime();
	m_pTextObjectComponent->SetText(std::to_string(int(m_FPS)));

}
