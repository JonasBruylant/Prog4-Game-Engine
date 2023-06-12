#include "LevelSkipCommand.h"
#include "CheckLevelFinishedComponent.h"

dae::LevelSkipCommand::LevelSkipCommand(CheckLevelFinishedComponent* pCheckLevelFinishComponent)
{
	m_pCheckLevelFinishedComponent = pCheckLevelFinishComponent;
}

void dae::LevelSkipCommand::Execute()
{
	m_pCheckLevelFinishedComponent->GoToNextScene();
}
