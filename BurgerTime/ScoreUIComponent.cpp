#include "ScoreUIComponent.h"

#include "TextObjectComponent.h"
#include "Event.h"

dae::ScoreUIComponent::ScoreUIComponent(GameObject* pOwner):
	Component(pOwner)
{
	m_pTextObjectHealthComponent = GetOwner()->GetComponent<TextObjectComponent>().get();
	m_pTextObjectHealthComponent->SetText("Score: " + std::to_string(m_displayScore));
}

void dae::ScoreUIComponent::Notify(Event event)
{															   
	if (strcmp(event.eventName.c_str(), "Bean Died") == 0)	   
		UpdateScore(100);									   
	if (strcmp(event.eventName.c_str(), "Pickle Died") == 0)   
		UpdateScore(200);									   
	if (strcmp(event.eventName.c_str(), "Egg Died") == 0)	   
		UpdateScore(300);
}



void dae::ScoreUIComponent::UpdateScore(int score)
{
	m_displayScore += score;
	m_pTextObjectHealthComponent->SetText("Score: " + std::to_string(m_displayScore));

}
