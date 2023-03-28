#pragma once
#include "Command.h"
#include "Transform.h"

namespace dae {

	class MoveCommand : public Command
	{

	public:
		enum class Direction
		{
			Up,
			Down,
			Left,
			Right
		};
		MoveCommand(std::weak_ptr<GameObject> pOwner);
	
		virtual ~MoveCommand() = default;
	
		void Execute(float deltaTime) override;

		void SetDirection(Direction direction) { m_movementDirection = direction; }
		void SetMovementSpeed(int speed) { m_movementSpeed = speed; }
	private:
		Direction m_movementDirection;
		int m_movementSpeed;
		TransformComponent* m_pTransformComponent{nullptr};
	};

}
