#include "MoveCommand.h"
#include "GameObject.h"

namespace dae
{
	MoveCommand::MoveCommand(GameObject* pOwner) :
		Command()
	{
		m_pTransformComponent = pOwner->GetTransform();

	}

	void MoveCommand::Execute()
	{
		auto movement = m_movementDirection * (m_movementSpeed * m_Timer.GetDeltaTime());
		m_pTransformComponent->AddToLocalPosition({ movement,0});
	}
}
