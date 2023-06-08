#include "AttackCommand.h"
#include "AttackComponent.h"


dae::AttackCommand::AttackCommand(AttackComponent* atkComp):
	Command()
{
	m_pAttackComponent = atkComp;
}

void dae::AttackCommand::Execute()
{
	//std::cout << "Attack Command triggered \n";
	m_pAttackComponent->Attack();
}
