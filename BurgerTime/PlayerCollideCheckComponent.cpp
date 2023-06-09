#include "PlayerCollideCheckComponent.h"
#include "CollisionComponent.h"
#include "GameObject.h"

#include "HealthComponent.h"
#include "ImageObjectComponent.h"
#include "Texture2D.h"
#include "StateComponent.h"

dae::PlayerCollideCheckComponent::PlayerCollideCheckComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner)
{
	m_pOwner = pOwner.lock().get();
	m_pStateComponent = m_pOwner->GetComponent<dae::StateComponent>().get();
	m_pCollisionComponent = m_pOwner->GetComponent<dae::CollisionComponent>().get();
	m_pHealthComponent = m_pOwner->GetComponent<dae::HealthComponent>().get();
	m_pTransformComponent = m_pOwner->GetTransform().get();
	m_pParentTransformComponent = m_pOwner->GetParent().lock()->GetTransform().get();

}

void dae::PlayerCollideCheckComponent::OnGameObjectCollision(dae::CollisionComponent* otherColComp, dae::GameObject* otherCollision)
{
	
	if (otherColComp->GetTag() == "Enemy")
	{
		m_pHealthComponent->TakeDamage(m_pHealthComponent->GetHealth() - 1);
	}


	if (otherColComp->GetTag() == "Ladder")
	{
		m_pStateComponent->SetCurrentState(State::CanClimb);
		std::cout << "Player Colliding with Ladder \n";
	}
	else if (otherColComp->GetTag() == "LevelPlatform")
	{
		
		auto localTransform = m_pParentTransformComponent->GetLocalPosition();
		auto otherWorldPos = otherCollision->GetTransform()->GetWorldPosition();

		auto textureSize = m_pImageObjComponent->GetTexture()->GetSize();

		m_pParentTransformComponent->SetLocalPosition(localTransform.x, otherWorldPos.y - textureSize.y - 1.f, localTransform.z);

		std::cout << "Player colliding with platform \n";
	}
}
