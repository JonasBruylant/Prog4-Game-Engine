#include "PepperComponent.h"
#include "CollisionComponent.h"
#include "Scene.h"
#include "GameObject.h"
#include "StateComponent.h"


dae::PepperComponent::PepperComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner)
{
}

void dae::PepperComponent::OnEnemyHit(dae::CollisionComponent* otherCollision, GameObject* otherObject)
{
	if (otherCollision->GetTag() == "Enemy")
	{
		//TODO: Delete Owner / Set as marked for deletion
		GetOwner().lock().get()->SetMarkedForDelete();
		//TODO: Set Enemy state to stunned.
		otherObject->GetComponent<StateComponent>()->SetCurrentState(State::Stunned);
	}
			
}

void dae::PepperComponent::Update()
{
	m_TimeAlive += m_Timer.GetDeltaTime();

	if (m_TimeAlive < m_MaxTimeAlive)
		return;

	GetOwner().lock()->SetMarkedForDelete();
}
