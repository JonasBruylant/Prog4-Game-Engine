#include "CheckLevelFinishedComponent.h"
#include "SceneManager.h"
#include "ScoreUIComponent.h"
#include "HighscoreManager.h"
#include "HighscoreDisplayComponent.h"

dae::CheckLevelFinishedComponent::CheckLevelFinishedComponent(GameObject* pOwner) :
	Component(pOwner)
{
}

void dae::CheckLevelFinishedComponent::Notify(Event event)
{
	if (strcmp(event.eventName.c_str(), "Piece Finished") == 0)
		--m_uncompletedPiecesAmount;

	if (m_uncompletedPiecesAmount != 0)
		return;


	GoToNextScene();

}

void dae::CheckLevelFinishedComponent::GoToNextScene()
{
	int scoreThisLevel{ 0 };
	for (auto& currentUIComp : m_pPlayerUIScoreComps)
	{
		scoreThisLevel += currentUIComp->GetDisplayScore();
	}
	auto& sceneManager = SceneManager::GetInstance();
	auto& highScoreManager = HighscoreManager::GetInstance();
	highScoreManager.AddToTotalScore(scoreThisLevel);


	//std::cout << highScoreManager.GetTotalScore() << std::endl;
	if (sceneManager.GetActiveSceneName() == "Level 1")
		sceneManager.SetActiveSceneByName("Level 2");
	else if (sceneManager.GetActiveSceneName() == "Level 2")
		sceneManager.SetActiveSceneByName("Level 3");
	else if (sceneManager.GetActiveSceneName() == "Level 3")
	{
		auto pHighscoreDisplayComp = highScoreManager.GetHighScoreDisplayComp();
		pHighscoreDisplayComp->UpdateHighScore();
		sceneManager.SetActiveSceneByName("Highscore Scene");
	}
}

void dae::CheckLevelFinishedComponent::Update()
{
	//std::cout << m_uncompletedPiecesAmount << std::endl;
}

void dae::CheckLevelFinishedComponent::IncrementUncompletedPieces()
{
	++m_uncompletedPiecesAmount;
}

void dae::CheckLevelFinishedComponent::AddScoreUIComponent(ScoreUIComponent* pScoreUIComponent)
{
	m_pPlayerUIScoreComps.emplace_back(pScoreUIComponent);
}