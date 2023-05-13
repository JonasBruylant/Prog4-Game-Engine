#include "DirectionComponent.h"
#include <memory>

dae::DirectionComponent::DirectionComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner), m_pOwner{ pOwner }
{
	auto pCommandOwner = m_pOwner.lock().get();
	m_ImageObjectComponent = pCommandOwner->GetComponent<ImageObjectComponent>().get();
	m_TransformComponent = pCommandOwner->GetComponent<TransformComponent>().get();


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
	case Up:
		movement.y = -1.f;
		break;
	case Right:
		movement.x = 1.f;
		break;
	case Down:
		movement.y = 1.f;
		break;
	case Left:
		movement.x = -1.f;
		break;
	default:
		break;
	}

	if (movement.x + movement.y != 0)
	{
		movement *= m_Timer.GetDeltaTime() * m_movementSpeed;
		m_TransformComponent->AddToLocalPosition(movement.x, movement.y ,0);
	}
}

void dae::DirectionComponent::SetDirectionImage()
{
	if(m_CurrentDirection != None)
		m_ImageObjectComponent->SetTexture(m_DirectionTextures[static_cast<int>(m_CurrentDirection)]);
}
