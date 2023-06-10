#pragma once
#include "Component.h"
#include "Subject.h"
#include "SoundManager.h"


#include "SDLSoundSystem.h"
#include "Structs.h"


namespace dae
{
	struct SoundData;
	class ScoreComponent final : public Component
	{
	public:
		ScoreComponent(GameObject* pOwner);

		~ScoreComponent() = default;
		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;

		void GainScore(EnemyType enemyType) 
		{ 
			switch (enemyType)
			{
			case EnemyType::Egg:
				m_pActorGainedScoreEvent->NotifyObservers({ "Egg Died" });
				break;
			case EnemyType::Bean:
				m_pActorGainedScoreEvent->NotifyObservers({ "Bean Died" });
				break;
			case EnemyType::Pickle:
				m_pActorGainedScoreEvent->NotifyObservers({ "Pickle Died" });
				break;
			}

			SoundData soundToPlay{ 1, 0.1f,"Gain_score.mp3",  SoundData::SoundType::SoundEffect,false };
			SoundManager::GetInstance().GetSoundSystem()->NotifyQueue(soundToPlay);
		}

		Subject* GetScoreGainedSubject() const { return m_pActorGainedScoreEvent.get(); }

	private:
		std::unique_ptr<Subject> m_pActorGainedScoreEvent = std::make_unique<Subject>();
	};
}

