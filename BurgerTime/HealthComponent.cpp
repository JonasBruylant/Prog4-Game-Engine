#include "HealthComponent.h"

dae::HealthComponent::HealthComponent(std::weak_ptr<GameObject> pOwner):
Component(pOwner)
{
}

