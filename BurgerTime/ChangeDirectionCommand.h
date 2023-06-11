#pragma once
#include "Command.h"
#include "DirectionComponent.h"
#include "Transform.h"
#include "Structs.h"

namespace dae
{

	class ChangeDirectionCommand final : public Command
	{
	public:
		ChangeDirectionCommand(GameObject* pOwner);
	
		void Execute() override;
		void SetDirection(PlayerDirection direction) { m_CurrentDirection = direction; }
	private:
		DirectionComponent* m_pDirectionComponent{ nullptr };
		TransformComponent* m_pTransformComponent{ nullptr };
		//GameObject* m_pOwner;

		PlayerDirection m_CurrentDirection;
	};

}
