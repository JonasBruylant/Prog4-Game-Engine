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
}

void dae::PlayerCollideCheckComponent::OnGameObjectCollision(dae::CollisionComponent* otherColComp, dae::GameObject* otherCollision)
{
	auto pOwner = GetOwner().lock();
	if (otherColComp->GetTag() == "Enemy")
	{
		auto healthComp = pOwner->GetComponent<dae::HealthComponent>();
		healthComp->TakeDamage(healthComp->GetHealth() - 1);
	}


	if (otherColComp->GetTag() == "Ladder")
	{
		pOwner->GetComponent<StateComponent>()->SetCurrentState(State::CanClimb);
		std::cout << "Player Colliding with Ladder \n";
	}
	else if (otherColComp->GetTag() == "LevelPlatform")
	{
		auto transform = pOwner->GetTransform();
		auto localTransform = transform->GetLocalPosition();
		auto otherWorldPos = otherCollision->GetTransform()->GetWorldPosition();

		auto textureSize = pOwner->GetComponent<dae::ImageObjectComponent>()->GetTexture()->GetSize();

		transform->SetLocalPosition(localTransform.x, otherWorldPos.y - textureSize.y - 1.f, localTransform.z);

		std::cout << "Player colliding with platform \n";
	}
}
