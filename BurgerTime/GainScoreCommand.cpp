#include "GainScoreCommand.h"
#include "ScoreComponent.h"

dae::GainScoreCommand::GainScoreCommand(ScoreComponent* pOwnerComponent)
{
	m_pOwnerScoreComponent = pOwnerComponent;
}

void dae::GainScoreCommand::Execute()
{
	m_pOwnerScoreComponent->GainScore();
}
