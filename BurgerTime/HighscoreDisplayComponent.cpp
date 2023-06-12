#include "HighscoreDisplayComponent.h"
#include "HighscoreManager.h"
#include "TextObjectComponent.h"
#include "TextRenderComponent.h"

#include "Transform.h"
#include "ResourceManager.h"

dae::HighscoreDisplayComponent::HighscoreDisplayComponent(GameObject* pOwner):
	Component(pOwner)
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	for (int i = 0; i < 10; ++i)
	{
		auto go = std::make_shared<GameObject>();
		go->Initialize();
		go->SetParent(pOwner, true);
		go->GetTransform()->SetLocalPosition({300.f, 100.f + (30.f * i),0});

		auto pTextObjComp = go->AddComponent<dae::TextObjectComponent>();
		pTextObjComp->SetColor(255,255,255);
		pTextObjComp->SetFont(font);
		m_pTextObjectComp.emplace_back(pTextObjComp.get());
		go->AddComponent<dae::TextRenderComponent>();

	}
}

void dae::HighscoreDisplayComponent::UpdateHighScore()
{
	auto& highscoreManager = HighscoreManager::GetInstance();
	highscoreManager.InsertFinalScore();
	auto& highScores = highscoreManager.GetHighScores();

	for (int i = 0; i < static_cast<int>(m_pTextObjectComp.size()); ++i)
	{
		if (i >= static_cast<int>(highScores.size()))
			m_pTextObjectComp[i]->SetText(std::to_string(0));
		else
			m_pTextObjectComp[i]->SetText(std::to_string(highScores[i]));
	}
}
