#pragma once
#include "Command.h"
#include "Transform.h"

namespace dae {

	class MoveCommand : public Command
	{

	public:
		MoveCommand(std::weak_ptr<GameObject> pOwner);
	
		virtual ~MoveCommand() = default;
	
		void Execute(float deltaTime) override;

		void SetDirection(float x, float y) { SetDirection({ x,y }); }
		void SetDirection(glm::vec2 direction) { m_movementDirection = direction; }
		void SetMovementSpeed(int speed) { m_movementSpeed = speed; }
	private:
		glm::vec2 m_movementDirection;
		int m_movementSpeed;
		TransformComponent* m_pTransformComponent{nullptr};

		std::weak_ptr<GameObject> m_pOwner;
	};

}
