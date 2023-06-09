#include "DirectionComponent.h"
#include <memory>
#include "ImageObjectComponent.h"
#include "Transform.h"
#include "StateComponent.h"

dae::DirectionComponent::DirectionComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner), m_pOwner{ pOwner }
{
	auto pLockedOwner = m_pOwner.lock().get();
	m_pImageObjectComponent = pLockedOwner->GetComponent<ImageObjectComponent>().get();
	m_pTransformComponent = pLockedOwner->GetComponent<TransformComponent>().get();

	auto& resourceManager = ResourceManager::GetInstance();
	m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MrPepperUp.png"));
	m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MrPepperRight.png"));
	m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MrPepperDown.png"));
	m_DirectionTextures.emplace_back(resourceManager.LoadTexture("MrPepperLeft.png"));
}

void dae::DirectionComponent::Update()
{
	glm::vec2 movement{0,0};

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
		if (m_pStateComponent->GetCurrentState() == State::CanClimb) // Can only move up or down when there is collision with a ladder
			movement.y = 1.f;
		break;
	case PlayerDirection::Left:
		movement.x = -1.f;
		break;
	default:
		break;
	}

	if (movement.x + movement.y == 0)
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

void dae::DirectionComponent::SetDirectionImage()
{
	if(m_CurrentDirection != PlayerDirection::None)
		m_pImageObjectComponent->SetTexture(m_DirectionTextures[static_cast<int>(m_CurrentDirection)]);
}
