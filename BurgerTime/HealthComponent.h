#pragma once
#include "Component.h"
#include "Subject.h"
#include "Timer.h"


namespace dae
{
	class CollisionComponent;

	class HealthComponent final : public Component
	{
	public:
		HealthComponent(GameObject* pOwner);

		~HealthComponent() = default;
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		int GetHealth() const { return m_Health; };
		void SetHealth(int health) { m_Health = health; }

		void OnEnemyHit(dae::CollisionComponent* otherCollision, dae::GameObject* otherGO);
		void TakeDamage(int damage);
		Subject* GetPlayerDiedEventSubject() const { return m_pGameObjectDiedEvent.get(); }
	private:
		std::unique_ptr<Subject> m_pGameObjectDiedEvent = std::make_unique<Subject>();
		int m_MaxHealth{ 1 };
		int m_Health{ 1 };
		float m_CooldownForNextHit{ 1.f };
		float m_HitTimer{ m_CooldownForNextHit };

		Timer& m_Timer = Timer::GetInstance();
	};
}

