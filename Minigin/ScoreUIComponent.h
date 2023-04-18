#pragma once
#include "Component.h"
#include "Observer.h"
#include "Event.h"

#include "Achievements.h"

namespace dae
{
	class TextObjectComponent;
	class ScoreUIComponent final : public Component, public Observer
	{
	public:
		ScoreUIComponent(std::weak_ptr<GameObject> pOwner);
		~ScoreUIComponent() = default;

		ScoreUIComponent(const ScoreUIComponent& other) = delete;
		ScoreUIComponent(ScoreUIComponent&& other) = delete;
		ScoreUIComponent& operator=(const ScoreUIComponent& other) = delete;
		ScoreUIComponent& operator=(ScoreUIComponent&& other) = delete;

		void Notify(Event event) override;
		void SetAchievement(std::shared_ptr<SteamAchievements> achievement);

	private:
		int m_displayScore{ 0 };
		TextObjectComponent* m_pTextObjectHealthComponent{ nullptr };
		void UpdateScore();

		std::shared_ptr<SteamAchievements> m_pAchievement;
	};
}
