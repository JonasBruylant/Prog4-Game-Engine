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
		ChangeDirectionCommand(std::weak_ptr<GameObject> pOwner);
	
		~ChangeDirectionCommand() = default;
	
		void Execute() override;
		void SetDirection(PlayerDirection direction) { m_CurrentDirection = direction; }
	private:
		DirectionComponent* m_pDirectionComponent{ nullptr };
		TransformComponent* m_pTransformComponent{ nullptr };
		std::weak_ptr<GameObject> m_pOwner;

		PlayerDirection m_CurrentDirection;
	};

}
