#pragma once
#include "Component.h"
#include "Subject.h"
#include "SoundManager.h"


#include "SDLSoundSystem.h"


namespace dae
{
	struct SoundData;
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

			SoundData soundToPlay{ 1, 0.1f,"Gain_score.mp3",  SoundData::SoundType::SoundEffect,false };
			SoundManager::GetInstance().GetSoundSystem()->NotifyQueue(soundToPlay);
		}

		Subject* GetScoreGainedSubject() const { return m_pActorGainedScoreEvent.get(); }

	private:
		std::unique_ptr<Subject> m_pActorGainedScoreEvent = std::make_unique<Subject>();
	};
}

