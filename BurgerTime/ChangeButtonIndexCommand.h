#pragma once
#include "Command.h"


namespace dae {
	//class GameObject;
	class ButtonIndexManagerComponent;
class ChangeButtonIndexCommand final : public Command
{
public:
	ChangeButtonIndexCommand(ButtonIndexManagerComponent* pButtonIdxComp, bool isNextButton);

	void Execute() override;
private:
	ButtonIndexManagerComponent* m_pButtonIdxManagerComp;
	bool m_isNextButton{ false };
};

}
