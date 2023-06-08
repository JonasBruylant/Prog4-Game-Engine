#pragma once
#include "Command.h"

namespace dae
{
	class AttackComponent;
	class AttackCommand final : public Command
	{
	public:
		AttackCommand(AttackComponent* atkComp);
		virtual ~AttackCommand() = default;

		void Execute() override;
	private:
		AttackComponent* m_pAttackComponent;
	};

}
