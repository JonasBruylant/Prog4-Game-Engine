#pragma once
#include "Command.h"

namespace dae {

	class ScoreComponent;
	class GainScoreCommand : public Command
	{
	public:
		GainScoreCommand(ScoreComponent* pOwnerComponent);
		virtual ~GainScoreCommand() = default;

		void Execute() override;
	private:
		ScoreComponent* m_pOwnerScoreComponent;
	};
}

