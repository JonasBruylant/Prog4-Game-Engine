#include "ChangeDirectionCommand.h"
#include "GameObject.h"


dae::ChangeDirectionCommand::ChangeDirectionCommand(GameObject* pOwner) :
	Command()
{
	m_pDirectionComponent = pOwner->GetComponent<DirectionComponent>().get();
	m_pTransformComponent = pOwner->GetTransform();
}

void dae::ChangeDirectionCommand::Execute()
{   
	
	m_pDirectionComponent->SetCurrentDirection(m_CurrentDirection);
	m_pDirectionComponent->SetDirectionImage();
}
