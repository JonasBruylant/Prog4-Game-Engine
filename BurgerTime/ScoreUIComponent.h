#pragma once
#include "Component.h"
#include "Observer.h"
#include "Event.h"


namespace dae
{
	class TextObjectComponent;
	class ScoreUIComponent final : public Component, public Observer
	{
	public:
		ScoreUIComponent(GameObject* pOwner);
		~ScoreUIComponent() = default;

		ScoreUIComponent(const ScoreUIComponent& other) = delete;
		ScoreUIComponent(ScoreUIComponent&& other) = delete;
		ScoreUIComponent& operator=(const ScoreUIComponent& other) = delete;
		ScoreUIComponent& operator=(ScoreUIComponent&& other) = delete;

		void Notify(Event event) override;
		int GetDisplayScore() const { return m_displayScore; }
	private:
		int m_displayScore{ 0 };
		TextObjectComponent* m_pTextObjectHealthComponent{ nullptr };
		void UpdateScore(int score);

	};
}
