#include "BlockingVolumeComponent.h"
#include "CollisionComponent.h"
#include "GameObject.h"

#include "StateComponent.h"
#include "EnemyComponent.h"
#include "ImageObjectComponent.h"

#include "Texture2D.h"

dae::BlockingVolumeComponent::BlockingVolumeComponent(GameObject* pOwner):
	Component(pOwner)
{
}

void dae::BlockingVolumeComponent::OnGameObjectHit(CollisionComponent* otherColComp, GameObject* otherCollision)
{
	const auto transform = otherCollision->GetTransform();
	const auto ownerTag = GetOwner()->GetComponent<CollisionComponent>()->GetTag();
	const auto collisionSize = GetOwner()->GetComponent<CollisionComponent>()->GetMeasurements();

	const auto textureSize = otherColComp->GetMeasurements();
	
	if (otherColComp->GetTag() == "Enemy")
	{
		auto enemyComp = otherCollision->GetComponent<dae::EnemyComponent>();
		enemyComp->SetMovementSpeed(enemyComp->GetMovementSpeed() * -1.f);
	}

	if (ownerTag == "BlockingVolumeLeft" && otherColComp->GetTag() == "Player")
		transform->SetLocalPosition(collisionSize.width, transform->GetWorldPosition().y, transform->GetWorldPosition().z);

	if (ownerTag == "BlockingVolumeRight" && otherColComp->GetTag() == "Player")
		transform->SetLocalPosition(m_windowSize.x - collisionSize.width - textureSize.width, transform->GetWorldPosition().y, transform->GetWorldPosition().z);
}
