#include "HealthComponent.h"
#include "CollisionComponent.h"
#include "Timer.h"
#include "StateComponent.h"
#include "Structs.h"
#include "SoundSystem.h"
#include "SoundManager.h"

dae::HealthComponent::HealthComponent(GameObject* pOwner):
Component(pOwner)
{
}

void dae::HealthComponent::TakeDamage(int damage)
{						

	m_Health -= damage;																								
																													
	if (m_Health <= 0)																								
	{																												
		m_pGameObjectDiedEvent->NotifyObservers({ "Player Died" });													
		m_Health = m_MaxHealth;																						
	}																												
																													
}																													
																													
void dae::HealthComponent::OnEnemyHit(dae::CollisionComponent * otherCollision, dae::GameObject* otherGO)
{
	if (GameStateLookUp::g_CurrentGameState != GameState::Versus)
	{
		if (otherCollision->GetTag() == "Enemy" && otherGO->GetComponent<StateComponent>()->GetCurrentState() != State::Stunned)																		
		{		
			m_HitTimer += m_Timer.GetDeltaTime();
			if (m_HitTimer < m_CooldownForNextHit)
				return;

			m_HitTimer = 0.f;
			dae::SoundData soundEffect = SoundData{ 2, 0.2f,"03_Player_Hurt.mp3",  SoundData::SoundType::SoundEffect,false };
			SoundManager::GetInstance().GetSoundSystem()->NotifyQueue(soundEffect);
			//std::cout << "Player has been hit by enemy. \n";															
			TakeDamage(1);																								
		}																												
	}
	else
	{
		if (otherCollision->GetTag() == "Enemy" && otherGO->GetChildren().front()->GetComponent<StateComponent>()->GetCurrentState() != State::Stunned)
		{
			m_HitTimer += m_Timer.GetDeltaTime();
			if (m_HitTimer < m_CooldownForNextHit)
				return;

			m_HitTimer = 0.f;

			dae::SoundData soundEffect = SoundData{ 2, 0.2f,"03_Player_Hurt.mp3",  SoundData::SoundType::SoundEffect,false };
			SoundManager::GetInstance().GetSoundSystem()->NotifyQueue(soundEffect);
			//std::cout << "Player has been hit by enemy. \n";															
			TakeDamage(1);
		}
	}
};																													
																													
																													
																													
																													