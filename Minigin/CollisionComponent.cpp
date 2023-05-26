#include "CollisionComponent.h"

dae::CollisionComponent::CollisionComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner)
{
}

void dae::CollisionComponent::Update()
{
}
