#pragma once
#include "Command.h"

namespace dae {

class ReturnToMainMenuCommand final : public Command
{
public:
	ReturnToMainMenuCommand() = default;
	void Execute() override;
private:
};

}
