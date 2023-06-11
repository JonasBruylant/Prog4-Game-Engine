#include "MenuButtonPressCommand.h"
#include "ButtonIndexManagerComponent.h"
#include "MenuButtonComponent.h"

dae::MenuButtonPressCommand::MenuButtonPressCommand(ButtonIndexManagerComponent* pButtonIdxManComp):
	m_pButtonIdxManComp{pButtonIdxManComp}
{
}

void dae::MenuButtonPressCommand::Execute()
{
	m_pButtonIdxManComp->ExecuteActiveButton();
}
