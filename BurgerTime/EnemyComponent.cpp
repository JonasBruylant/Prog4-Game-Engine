#include "EnemyComponent.h"
#include "Transform.h"
#include "StateComponent.h"
#include "CollisionComponent.h"
#include "ImageObjectComponent.h"

dae::EnemyComponent::EnemyComponent(GameObject* pOwner):
	Component(pOwner)
{
	srand(static_cast<unsigned int>(time(nullptr)));


	auto owner = pOwner;
	m_pTransformComponent = owner->GetComponent<TransformComponent>().get();
	m_pStateComponent = owner->GetComponent<StateComponent>().get();

	m_pImageObjectComponent = pOwner->GetComponent<ImageObjectComponent>().get();
	m_pTransformComponent = pOwner->GetComponent<TransformComponent>().get();


}

void dae::EnemyComponent::Update()
{

	//TODO: Upgrade movement logic to detect collision with the level and ladders.
	UpdateMovement();
	UpdateStunnedBehavior();


}

void dae::EnemyComponent::UpdateMovement()
{
	if (m_pStateComponent->GetCurrentState() == State::Walking)
	{

		if (!m_ChangedDebugColor)
			GetOwner()->GetComponent<CollisionComponent>()->SetDebugColor({ 255.f,255.f,0.f });

		m_direction.x = m_Timer.GetDeltaTime() * m_movementSpeed;
		m_pTransformComponent->AddToLocalPosition(m_direction.x, m_direction.y, 0);

		if(m_movementSpeed > 0)
			GetOwner()->GetComponent<ImageObjectComponent>()->SetTexture(m_StateTextures[1]);
		else if(m_movementSpeed < 0)
			GetOwner()->GetComponent<ImageObjectComponent>()->SetTexture(m_StateTextures[0]);
	}
	// else if (m_pStateComponent->GetCurrentState() == State::CanClimb && m_TimeSinceLastStateChange > 1.f)
	// {
	// 	m_ClimbLadder = (rand() % 2 == 0); // This will cause it to check every frame untill collision stops so enemy is almost GUARENTEED to climb up or down
	// 	if (m_ClimbLadder)
	// 		m_pStateComponent->SetCurrentState(State::ClimbingUp);
	// 	else
	// 		m_pStateComponent->SetCurrentState(State::Walking);
	// 	m_TimeSinceLastStateChange = 0.f;
	// }
	// else if (m_pStateComponent->GetCurrentState() == State::CanClimbDown && m_TimeSinceLastStateChange > 1.f)
	// {
	// 	m_ClimbLadder = (rand() % 2 == 0); // This will cause it to check every frame untill collision stops so enemy is almost GUARENTEED to climb up or down
	// 	if (m_ClimbLadder)			
	// 		m_pStateComponent->SetCurrentState(State::ClimbingDown);
	// 	else
	// 		m_pStateComponent->SetCurrentState(State::Walking);
	// 	m_TimeSinceLastStateChange = 0.f;
	// }
	// 
	// if (m_pStateComponent->GetCurrentState() == State::ClimbingDown)
	// {
	// 	std::cout << "Climbing down \n";
	// 	m_direction.y = -(m_Timer.GetDeltaTime() * m_movementSpeed);
	// 	m_pTransformComponent->AddToLocalPosition(0, m_direction.y, 0);
	// }
	// else if (m_pStateComponent->GetCurrentState() == State::ClimbingUp)
	// {
	// 	std::cout << "Climbing up \n";
	// 	m_direction.y = m_Timer.GetDeltaTime() * m_movementSpeed;
	// 	m_pTransformComponent->AddToLocalPosition(0, m_direction.y, 0);
	// }
	m_TimeSinceLastStateChange += m_Timer.GetDeltaTime();
	//std::cout << m_TimeSinceLastStateChange << std::endl;
}

void dae::EnemyComponent::UpdateStunnedBehavior()
{
	if (m_pStateComponent->GetCurrentState() != State::Stunned)
		return;


	if (!m_ChangedDebugColor)
	{
		GetOwner()->GetComponent<CollisionComponent>()->SetDebugColor({ 255.f,255.f,255.f });
		m_ChangedDebugColor = true;

		GetOwner()->GetComponent<ImageObjectComponent>()->SetTexture(m_StateTextures[2]);
	}

	m_StunnedTime += m_Timer.GetDeltaTime();

	if (m_StunnedTime > m_TimeToBeStunned)
	{
		m_pStateComponent->SetCurrentState(State::Walking);
		m_ChangedDebugColor = false;
		m_StunnedTime = 0.f;
	}

}

void dae::EnemyComponent::SetEnemyType(EnemyType type)
{
	if (m_Type != type)
		m_Type = type;

	switch (m_Type)
	{
	case EnemyType::Egg:
		m_StateTextures.emplace_back(m_ResourceManager.LoadTexture("MrEggLeft.png"));
		m_StateTextures.emplace_back(m_ResourceManager.LoadTexture("MrEggRight.png"));
		m_StateTextures.emplace_back(m_ResourceManager.LoadTexture("MrEggStunned.png"));
		break;
	case EnemyType::Bean:
		m_StateTextures.emplace_back(m_ResourceManager.LoadTexture("MrBeanLeft.png"));
		m_StateTextures.emplace_back(m_ResourceManager.LoadTexture("MrBeanRight.png"));
		m_StateTextures.emplace_back(m_ResourceManager.LoadTexture("MrBeanStunned.png"));
		break;
	case EnemyType::Pickle:
		m_StateTextures.emplace_back(m_ResourceManager.LoadTexture("MrPickleLeft.png"));
		m_StateTextures.emplace_back(m_ResourceManager.LoadTexture("MrPickleRight.png"));
		m_StateTextures.emplace_back(m_ResourceManager.LoadTexture("MrPickleStunned.png"));
		break;
	}

	GetOwner()->GetComponent<ImageObjectComponent>()->SetTexture(m_StateTextures[0]);
}
