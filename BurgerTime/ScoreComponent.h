#pragma once
#include "Component.h"
#include "Subject.h"
#include "SoundManager.h"
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

			SoundData soundToPlay{ 1, 0.1f,"Gain_score.mp3",dae::SoundData::SoundType::SoundEffect };
			SoundManager::GetInstance().NotifyQueue(soundToPlay);
		}

		Subject* GetScoreGainedSubject() const { return m_pActorGainedScoreEvent.get(); }

	private:
		std::unique_ptr<Subject> m_pActorGainedScoreEvent = std::make_unique<Subject>();
	};
}

