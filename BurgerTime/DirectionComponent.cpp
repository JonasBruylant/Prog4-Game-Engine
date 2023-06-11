#include "DirectionComponent.h"
#include <memory>
#include "ImageObjectComponent.h"
#include "Transform.h"
#include "StateComponent.h"
#include "Structs.h"

dae::DirectionComponent::DirectionComponent(GameObject* pOwner, bool isPlayerTwo, GameState gameState):
	Component(pOwner), m_pOwner{ pOwner }
{
	//m_CurrentDirection{};
	m_pImageObjectComponent = pOwner->GetComponent<ImageObjectComponent>().get();
	m_pTransformComponent = pOwner->GetComponent<TransformComponent>().get();
	m_isPlayerTwo = isPlayerTwo;
	auto& resourceManager = ResourceManager::GetInstance();
	if (!isPlayerTwo)
	{
		m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MrPepperUp.png"));
		m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MrPepperRight.png"));
		m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MrPepperDown.png"));
		m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MrPepperLeft.png"));
	}
	else
	{
		if (gameState == GameState::Coop)
		{
			m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MsSaltUp.png"));
			m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MsSaltRight.png"));
			m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MsSaltDown.png"));
			m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MsSaltLeft.png"));
		}
		else if (gameState == GameState::Versus)
		{
			m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MrBeanUp.png"));
			m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MrBeanRight.png"));
			m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MrBeanDown.png"));
			m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MrBeanLeft.png"));
		}
	}
}

void dae::DirectionComponent::Update()
{
	glm::vec2 movement{0,0};

	if (m_pStateComponent->GetCurrentState() == State::Stunned)
	{
		m_timeStunned += m_Timer.GetDeltaTime();

		if(m_timeStunned > 3.f)
		{
			m_timeStunned = 0.f;
			m_pStateComponent->SetCurrentState(State::Idle);
		}
		return;
	}

	switch (m_CurrentDirection)
	{
	case PlayerDirection::Up:
		if(m_pStateComponent->GetCurrentState() == State::CanClimb) // Can only move up or down when there is collision with a ladder
			movement.y = -1.f;
		break;
	case PlayerDirection::Right:
		movement.x = 1.f;
		break;
	case PlayerDirection::Down:
		if (m_pStateComponent->GetCurrentState() == State::CanClimb || m_pStateComponent->GetCurrentState() == State::CanClimbDown) // Can only move up or down when there is collision with a ladder
			movement.y = 1.f;
		break;
	case PlayerDirection::Left:
		movement.x = -1.f;
		break;
	default:
		break;
	}

	if ((movement.x + movement.y == 0))
	{
		if (m_pStateComponent->GetCurrentState() != State::Idle)
			m_pStateComponent->SetCurrentState(State::Idle);
		return;
	}


	movement *= m_Timer.GetDeltaTime() * m_movementSpeed;
	m_pTransformComponent->AddToLocalPosition(movement.x, movement.y ,0);

	if (m_pStateComponent->GetCurrentState() != State::Walking)
		m_pStateComponent->SetCurrentState(State::Walking);
}

void dae::DirectionComponent::SetCurrentDirection(PlayerDirection direction)
{
	m_CurrentDirection = direction;
}
dae::PlayerDirection dae::DirectionComponent::GetCurrentDirection()
{
	return m_CurrentDirection;
}

void dae::DirectionComponent::SetDirectionImage()
{
		if(m_CurrentDirection != PlayerDirection::None)
		{ 
			m_pImageObjectComponent->SetTexture(m_DirectionTextures[static_cast<int>(m_CurrentDirection)]);
		}
	
}
