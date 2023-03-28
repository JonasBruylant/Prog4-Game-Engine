#include "MoveCommand.h"
#include "GameObject.h"

namespace dae
{
	MoveCommand::MoveCommand(std::weak_ptr<GameObject> pOwner) :
		Command(pOwner)
	{
		auto pCommandOwner = GetOwner().lock().get();
		m_pTransformComponent = pCommandOwner->GetComponent<TransformComponent>().get();

		m_movementSpeed = 10;
		m_movementDirection = Direction::Up;
	}
	void MoveCommand::Execute(float deltaTime)
	{
		float x{}, y{};

		switch (m_movementDirection)
		{
		case dae::MoveCommand::Direction::Up:
			y = -(m_movementSpeed * deltaTime);
			break;
		case dae::MoveCommand::Direction::Down:
			y = m_movementSpeed * deltaTime;
			break;
		case dae::MoveCommand::Direction::Left:
			x = -(m_movementSpeed * deltaTime);
			break;
		case dae::MoveCommand::Direction::Right:
			x = m_movementSpeed * deltaTime;
			break;
		default:
			break;
		}

		auto goLocalPosition = m_pTransformComponent->GetLocalPosition();
		m_pTransformComponent->SetLocalPosition(goLocalPosition.x+ x, goLocalPosition.y+y, 0);
	}
}
