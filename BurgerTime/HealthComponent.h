#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{

	class HealthComponent final : public Component
	{
	public:
		HealthComponent(std::weak_ptr<GameObject> pOwner);

		~HealthComponent() = default;
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		int GetHealth() const { return m_Health; };
		void SetHealth(int health) { m_Health = health; }
		void DoDamage(int damage)
		{
			m_Health -= damage;

			if (m_Health >= 0)
			{
				m_pGameObjectDiedEvent->NotifyObservers({ "Player Died" });
				m_Health = m_MaxHealth;
			}

		}
		Subject* GetPlayerDiedEventSubject() const { return m_pGameObjectDiedEvent.get(); }
	private:
		std::unique_ptr<Subject> m_pGameObjectDiedEvent = std::make_unique<Subject>();
		int m_MaxHealth{ 1 };
		int m_Health{ 1 };
	};
}

