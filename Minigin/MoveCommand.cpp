#include "MoveCommand.h"
#include "GameObject.h"
#include "Timer.h"

namespace dae
{
	MoveCommand::MoveCommand(std::weak_ptr<GameObject> pOwner) :
		Command(), m_pOwner{pOwner}
	{
		auto pCommandOwner = m_pOwner.lock().get();
		m_pTransformComponent = pCommandOwner->GetComponent<TransformComponent>().get();

	}

	void MoveCommand::Execute()
	{
		auto movement = m_movementDirection * (m_movementSpeed * m_Timer.GetDeltaTime());
		m_pTransformComponent->AddToLocalPosition({ movement,0});
	}
}
