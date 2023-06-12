#include "ReturnToMainMenuCommand.h"
#include "SceneManager.h"
#include "HighscoreManager.h"

void dae::ReturnToMainMenuCommand::Execute()
{
	HighscoreManager::GetInstance().ResetTotalScore();
	
	auto& sceneManager = SceneManager::GetInstance();
	sceneManager.SetActiveSceneByName("Main Menu");
	

	sceneManager.RemoveSceneByName("Level 1");
	sceneManager.RemoveSceneByName("Level 2");
	sceneManager.RemoveSceneByName("Level 3");
}
