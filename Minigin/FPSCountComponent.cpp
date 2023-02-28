#include "FPSCountComponent.h"
#include "TextObject.h"

dae::FPSCountComponent::FPSCountComponent(std::weak_ptr<GameObject> pOwner): Component(pOwner),
	m_FPS{0.f},
	m_UpdateTime{0.f}
{
}

dae::FPSCountComponent::~FPSCountComponent()
{
}

void dae::FPSCountComponent::Update(float deltaTime)
{
	m_UpdateTime += deltaTime;

	if (m_UpdateTime >= 1.f)
	{
		m_UpdateTime = 0.f;
		m_FPS = 1.f / deltaTime;
		m_pComponentOwner.lock()->GetComponent<TextObjectComponent>()->SetText(std::to_string(int(m_FPS)));
	}

}
