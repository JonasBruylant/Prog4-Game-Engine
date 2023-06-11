#include "ChangeButtonIndexCommand.h"
#include "ButtonIndexManagerComponent.h"


dae::ChangeButtonIndexCommand::ChangeButtonIndexCommand(ButtonIndexManagerComponent* pButtonIdxComp, bool isNextButton)
{
	m_pButtonIdxManagerComp = pButtonIdxComp;
	m_isNextButton = isNextButton;
}

void dae::ChangeButtonIndexCommand::Execute()
{
	m_pButtonIdxManagerComp->UpdateIndex(m_isNextButton);
}
