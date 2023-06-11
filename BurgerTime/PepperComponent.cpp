#include "PepperComponent.h"
#include "CollisionComponent.h"
#include "Scene.h"
#include "GameObject.h"
#include "StateComponent.h"
#include "Structs.h"


dae::PepperComponent::PepperComponent(GameObject* pOwner):
	Component(pOwner)
{
}

void dae::PepperComponent::OnEnemyHit(dae::CollisionComponent* otherCollision, GameObject* otherObject)
{
	if (otherCollision->GetTag() == "Enemy")
	{
		GetOwner()->SetMarkedForDelete();
		if(dae::GameStateLookUp::g_CurrentGameState != GameState::Versus)
			otherObject->GetComponent<StateComponent>()->SetCurrentState(State::Stunned);
		else
			otherObject->GetChildren().back()->GetComponent<StateComponent>()->SetCurrentState(State::Stunned);
	}
			
}

void dae::PepperComponent::Update()
{
	m_TimeAlive += m_Timer.GetDeltaTime();

	if (m_TimeAlive < m_MaxTimeAlive)
		return;

	GetOwner()->SetMarkedForDelete();
}
