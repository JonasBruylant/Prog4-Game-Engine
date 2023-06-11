#pragma once
#include "Component.h"
#include "Observer.h"
#include "Event.h"
#include "Subject.h"

namespace dae
{
	class TextObjectComponent;
	class LivesRemainingUIComponent final : public Component, public Observer
	{
	public:
		LivesRemainingUIComponent(GameObject* pOwner);
		~LivesRemainingUIComponent() = default;

		LivesRemainingUIComponent(const LivesRemainingUIComponent& other) = delete;
		LivesRemainingUIComponent(LivesRemainingUIComponent&& other) = delete;
		LivesRemainingUIComponent& operator=(const LivesRemainingUIComponent& other) = delete;
		LivesRemainingUIComponent& operator=(LivesRemainingUIComponent&& other) = delete;

		void Notify(Event event) override;

	private:
		void UpdateText();
		TextObjectComponent* m_pTextObjectHealthComponent{nullptr};

		//TODO: Use to actually trrigger game over.
		std::unique_ptr<Subject> m_pGameOverEvent = std::make_unique<Subject>();
		int m_Lives{ 3 };
	};
}

