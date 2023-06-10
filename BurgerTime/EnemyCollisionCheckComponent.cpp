#include "EnemyCollisionCheckComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "StateComponent.h"


#include "GameObject.h"

dae::EnemyCollisionCheckComponent::EnemyCollisionCheckComponent(GameObject* pOwner):
	Component(pOwner)
{
}

void dae::EnemyCollisionCheckComponent::OnGameObjectHit(CollisionComponent* , GameObject* )
{
	//auto pOwner = GetOwner();
	//if (pOtherColComp->GetTag() == "Player")
	//{
	//	auto goHealthComp = pOtherGOCol->GetComponent<dae::HealthComponent>();
	//	goHealthComp->TakeDamage(1);
	//}
	//else if ((pOtherColComp->GetTag() == "Ladder") && (pOwner->GetComponent<dae::StateComponent>()->GetCurrentState() != State::CanClimb || pOwner->GetComponent<dae::StateComponent>()->GetCurrentState() != State::ClimbingDown))
	//{
	//	pOwner->GetComponent<dae::StateComponent>()->SetCurrentState(State::CanClimb);
	//}
	//else if (pOtherColComp->GetTag() == "LadderTop" && (pOwner->GetComponent<dae::StateComponent>()->GetCurrentState() != State::CanClimbDown || pOwner->GetComponent<dae::StateComponent>()->GetCurrentState() != State::ClimbingUp))
	//{
	//	pOwner->GetComponent<dae::StateComponent>()->SetCurrentState(State::CanClimbDown);
	//}
}
