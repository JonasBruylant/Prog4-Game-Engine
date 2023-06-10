#include "HealthComponent.h"
#include "CollisionComponent.h"
#include "Timer.h"

dae::HealthComponent::HealthComponent(GameObject* pOwner):
Component(pOwner)
{
}

void dae::HealthComponent::TakeDamage(int damage)
{																													
	m_Health -= damage;																								
																													
	if (m_Health >= 0)																								
	{																												
		m_pGameObjectDiedEvent->NotifyObservers({ "Player Died" });													
		m_Health = m_MaxHealth;																						
	}																												
																													
}																													
																													
void dae::HealthComponent::OnEnemyHit(dae::CollisionComponent * otherCollision)										
{																													
	if (otherCollision->GetTag() == "Enemy")																		
	{		
		m_HitTimer += m_Timer.GetDeltaTime();
		if (m_HitTimer < m_CooldownForNextHit)
			return;

		m_HitTimer = 0.f;
		//std::cout << "Player has been hit by enemy. \n";															
		TakeDamage(1);																								
	}																												
};																													
																													
																													
																													
																													