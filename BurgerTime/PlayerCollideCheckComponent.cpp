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

void dae::PlayerCollideCheckComponent::OnGameObjectCollision(dae::CollisionComponent* otherColComp, dae::GameObject* )
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
}
