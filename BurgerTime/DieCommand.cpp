#include "DieCommand.h"
#include "HealthComponent.h"

dae::DieCommand::DieCommand(HealthComponent* pOwnerComponent):
	Command(), m_pOwnerComponent{pOwnerComponent}
{
}

void dae::DieCommand::Execute()
{
	m_pOwnerComponent->DoDamage(1);
}
