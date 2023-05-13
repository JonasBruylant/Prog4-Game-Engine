#include "ChangeDirectionCommand.h"
#include "GameObject.h"


dae::ChangeDirectionCommand::ChangeDirectionCommand(std::weak_ptr<GameObject> pOwner) :
	Command(), m_pOwner{ pOwner }
{
	auto pCommandOwner = m_pOwner.lock().get();
	m_pDirectionComponent = pCommandOwner->GetComponent<DirectionComponent>().get();
	m_pTransformComponent = pCommandOwner->GetComponent<TransformComponent>().get();
}

void dae::ChangeDirectionCommand::Execute()
{   
	m_pDirectionComponent->SetCurrentDirection(m_CurrentDirection);
	m_pDirectionComponent->SetDirectionImage();
}
