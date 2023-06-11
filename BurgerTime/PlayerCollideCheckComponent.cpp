#include "PlayerCollideCheckComponent.h"
#include "CollisionComponent.h"
#include "GameObject.h"

#include "HealthComponent.h"
#include "ImageObjectComponent.h"
#include "Texture2D.h"
#include "StateComponent.h"

dae::PlayerCollideCheckComponent::PlayerCollideCheckComponent(GameObject* pOwner):
	Component(pOwner)
{
	m_pOwner = pOwner;
	m_pStateComponent = m_pOwner->GetComponent<dae::StateComponent>().get();
	m_pCollisionComponent = m_pOwner->GetComponent<dae::CollisionComponent>().get();
	m_pHealthComponent = m_pOwner->GetComponent<dae::HealthComponent>().get();
	m_pTransformComponent = m_pOwner->GetTransform();
	m_pParentTransformComponent = m_pOwner->GetParent()->GetTransform();

}

void dae::PlayerCollideCheckComponent::OnGameObjectCollision(dae::CollisionComponent* otherColComp, dae::GameObject* otherCollision)
{
	//if (otherColComp->GetTag() == "Enemy")
	//{
	//	
	//}
	if (otherColComp->GetTag() == "Ladder")
	{
		m_pStateComponent->SetCurrentState(State::CanClimb);
	}
	if (otherColComp->GetTag() == "LadderTop")
	{
		m_pStateComponent->SetCurrentState(State::CanClimbDown);
	}
	else if (otherColComp->GetTag() == "LevelPlatform")
	{
		//std::cout << "Colliding with platform \n";
		auto localTransform = m_pParentTransformComponent->GetLocalPosition();
		auto otherWorldPos = otherCollision->GetTransform()->GetWorldPosition();

		auto textureSize = m_pImageObjComponent->GetTexture()->GetSize();

		m_pParentTransformComponent->SetLocalPosition(localTransform.x, otherWorldPos.y - textureSize.y - 1.f, localTransform.z);
	}
}
