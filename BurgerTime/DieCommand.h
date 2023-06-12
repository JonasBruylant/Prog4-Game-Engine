#pragma once
#include "Command.h"

namespace dae {
	class HealthComponent;

class DieCommand final : public Command
{
public:
	DieCommand(HealthComponent* pOwnerComponent);

	void Execute() override;
private:
	HealthComponent* m_pOwnerComponent{nullptr};
};
}

