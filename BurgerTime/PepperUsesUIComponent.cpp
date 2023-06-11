#include "PepperUsesUIComponent.h"
#include "TextObjectComponent.h"
#include "AttackComponent.h"

dae::PepperUsesUIComponent::PepperUsesUIComponent(GameObject* pOwner):
	Component(pOwner)
{
	m_pTextObjectPepperComponent = GetOwner()->GetComponent<TextObjectComponent>().get();
	m_pTextObjectPepperComponent->SetText("Peppers: " + std::to_string(m_amountOfPepperUsage));
}

void dae::PepperUsesUIComponent::Notify(Event event)
{
	if (strcmp(event.eventName.c_str(), "Pepper Used") == 0)
		UpdatePepperUsage();
}

void dae::PepperUsesUIComponent::UpdatePepperUsage()
{
	--m_amountOfPepperUsage;
	m_pTextObjectPepperComponent->SetText("Peppers: " + std::to_string(m_amountOfPepperUsage));
}
