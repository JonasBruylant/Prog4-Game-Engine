#include "ScoreUIComponent.h"

#include "TextObjectComponent.h"
#include "Event.h"

dae::ScoreUIComponent::ScoreUIComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner)
{
	m_pTextObjectHealthComponent = GetOwner().lock()->GetComponent<TextObjectComponent>().get();
	m_pTextObjectHealthComponent->SetText("Score: " + std::to_string(m_displayScore));
}

void dae::ScoreUIComponent::Notify(Event event)
{
	if (strcmp(event.eventName.c_str(), "Score Gained") == 0)
		UpdateScore();
}

void dae::ScoreUIComponent::SetAchievement(std::shared_ptr<SteamAchievements> achievement)
{
	m_pAchievement = achievement;
}

void dae::ScoreUIComponent::UpdateScore()
{
	m_displayScore += 100;
	//How do I link the value of the display score to the score of the component?
	m_pTextObjectHealthComponent->SetText("Score: " + std::to_string(m_displayScore));

	if (m_displayScore <= 500)
		return;

	m_pAchievement->SetAchievement("ACH_WIN_ONE_GAME");
}
