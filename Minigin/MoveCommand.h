#pragma once
#include "Command.h"
#include "Transform.h"
#include "Timer.h"

namespace dae {

	class MoveCommand final : public Command
	{

	public:
		MoveCommand(GameObject* pOwner);
	
		~MoveCommand() = default;
	
		void Execute() override;

		void SetDirection(float x, float y) { SetDirection({ x,y }); }
		void SetDirection(glm::vec2 direction) { m_movementDirection = direction; }
		void SetMovementSpeed(int speed) { m_movementSpeed = speed; }
	private:
		glm::vec2 m_movementDirection = glm::vec2{ 0,0 };
		int m_movementSpeed = 10;
		TransformComponent* m_pTransformComponent{nullptr};


		Timer& m_Timer = Timer::GetInstance();
	};

}
