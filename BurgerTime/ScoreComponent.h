#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
	class ScoreComponent final : public Component
	{
	public:
		ScoreComponent(std::weak_ptr<GameObject> pOwner);

		~ScoreComponent() = default;
		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;

		void GainScore() 
		{ 
			m_pActorGainedScoreEvent->NotifyObservers({ "Score Gained" });
		}

		Subject* GetScoreGainedSubject() const { return m_pActorGainedScoreEvent.get(); }

	private:
		std::unique_ptr<Subject> m_pActorGainedScoreEvent = std::make_unique<Subject>();
	};
}

