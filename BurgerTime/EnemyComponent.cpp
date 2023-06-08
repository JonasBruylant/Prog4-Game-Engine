#include "EnemyComponent.h"
#include "Transform.h"
#include "StateComponent.h"
#include "CollisionComponent.h"
#include "ImageObjectComponent.h"

dae::EnemyComponent::EnemyComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner)
{
	auto owner = pOwner.lock();
	m_pTransformComponent = owner->GetComponent<TransformComponent>().get();
	m_pStateComponent = owner->GetComponent<StateComponent>().get();
}

void dae::EnemyComponent::Update()
{

	//TODO: Upgrade movement logic to detect collision with the level and ladders.
	UpdateMovement();

	if (m_pStateComponent->GetCurrentState() != State::Stunned)
		return;


	if (!m_ChangedDebugColor)
	{
		GetOwner().lock().get()->GetComponent<CollisionComponent>()->SetDebugColor({ 255.f,255.f,255.f });
		m_ChangedDebugColor = true;
	}

	m_StunnedTime += m_Timer.GetDeltaTime();

	if (m_StunnedTime > m_TimeToBeStunned)
	{
		m_pStateComponent->SetCurrentState(State::Walking);
		m_ChangedDebugColor = false;
		m_StunnedTime = 0.f;
	}


}

void dae::EnemyComponent::UpdateMovement()
{
	if (m_pStateComponent->GetCurrentState() != State::Walking)
		return;

	if (!m_ChangedDebugColor)
		GetOwner().lock().get()->GetComponent<CollisionComponent>()->SetDebugColor({ 255.f,255.f,0.f });

	m_direction.x = m_Timer.GetDeltaTime() * m_movementSpeed;
	m_pTransformComponent->AddToLocalPosition(m_direction.x, m_direction.y, 0);

}

void dae::EnemyComponent::SetEnemyType(EnemyType type)
{
	if (m_Type == type)
		m_Type = type;

	switch (m_Type)
	{
	case EnemyType::Egg:
		m_EnemyTexture = m_ResourceManager.LoadTexture("MrEggLeft.png");
		break;
	case EnemyType::Bean:
		m_EnemyTexture = m_ResourceManager.LoadTexture("MrBeanLeft.png");
		break;
	case EnemyType::Pickle:
		m_EnemyTexture = m_ResourceManager.LoadTexture("MrPickleLeft.png");
		break;
	}

	GetOwner().lock()->GetComponent<ImageObjectComponent>()->SetTexture(m_EnemyTexture);
}
