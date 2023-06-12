#pragma once
#include "Command.h"


namespace dae {
	class CheckLevelFinishedComponent;

class LevelSkipCommand final : public Command
{
public:
	LevelSkipCommand(CheckLevelFinishedComponent* pCheckLevelFinishComponent);

	~LevelSkipCommand() = default;
	LevelSkipCommand(const LevelSkipCommand& other) = delete;
	LevelSkipCommand(LevelSkipCommand&& other) = delete;
	LevelSkipCommand& operator=(const LevelSkipCommand& other) = delete;
	LevelSkipCommand& operator=(LevelSkipCommand&& other) = delete;

	void Execute() override;
private:
	CheckLevelFinishedComponent* m_pCheckLevelFinishedComponent;
};

}
