#include "BlockingVolumeComponent.h"
#include "CollisionComponent.h"
#include "GameObject.h"

#include "StateComponent.h"
#include "EnemyComponent.h"
#include "ImageObjectComponent.h"

#include "Texture2D.h"

dae::BlockingVolumeComponent::BlockingVolumeComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner)
{
}

void dae::BlockingVolumeComponent::OnGameObjectHit(CollisionComponent* otherColComp, GameObject* otherCollision)
{
	auto transform = otherCollision->GetTransform();
	auto ownerTag = GetOwner().lock()->GetComponent<CollisionComponent>()->GetTag();
	auto collisionSize = GetOwner().lock()->GetComponent<CollisionComponent>()->GetMeasurements();

	m_textureSize = otherCollision->GetComponent<dae::ImageObjectComponent>()->GetTexture().get()->GetSize();
	if (otherColComp->GetTag() == "Player")
	{
		otherCollision->GetComponent<dae::StateComponent>()->SetCurrentState(State::Idle);
	}
	else if (otherColComp->GetTag() == "Enemy")
	{
		auto enemyComp = otherCollision->GetComponent<dae::EnemyComponent>();
		enemyComp->SetMovementSpeed(enemyComp->GetMovementSpeed() * -1.f);
	}

	if (ownerTag == "BlockingVolumeLeft")
		transform->SetLocalPosition(collisionSize.width, transform->GetWorldPosition().y, transform->GetWorldPosition().z);

	if(ownerTag == "BlockingVolumeRight")
		transform->SetLocalPosition(m_windowSize.x - collisionSize.width - m_textureSize.x, transform->GetWorldPosition().y, transform->GetWorldPosition().z);
}