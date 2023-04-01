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

		m_movementSpeed = 10;
		m_movementDirection = glm::vec2{0,0};
	}
	void MoveCommand::Execute()
	{
		auto& timer = Timer::GetInstance();
		auto movement = m_movementDirection * (m_movementSpeed * timer.GetDeltaTime());
		m_pTransformComponent->AddToLocalPosition({ movement,0});
	}
}
