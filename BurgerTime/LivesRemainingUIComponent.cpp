#include "LivesRemainingUIComponent.h"
#include "TextObjectComponent.h"
#include "Event.h"

dae::LivesRemainingUIComponent::LivesRemainingUIComponent(GameObject* pOwner):
	Component(pOwner)
{
	m_pTextObjectHealthComponent = GetOwner()->GetComponent<TextObjectComponent>().get();
	m_pTextObjectHealthComponent->SetText("Lives remaining: " + std::to_string(m_Lives));
}

void dae::LivesRemainingUIComponent::Notify(Event event)
{
	if (strcmp(event.eventName.c_str(), "Player Died") == 0)
		UpdateText();
}

void dae::LivesRemainingUIComponent::UpdateText()
{
	if(m_Lives > 0)
		--m_Lives;
	
	m_pTextObjectHealthComponent->SetText("Lives remaining: " + std::to_string(m_Lives));
}