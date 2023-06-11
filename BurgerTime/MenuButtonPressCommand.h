#pragma once
#include "Command.h"


namespace dae {
	class ButtonIndexManagerComponent;
class MenuButtonPressCommand final : public Command
{
public:
	MenuButtonPressCommand(ButtonIndexManagerComponent* pButtonIdxManComp);

	void Execute() override;
private:
	ButtonIndexManagerComponent* m_pButtonIdxManComp;
};
}

