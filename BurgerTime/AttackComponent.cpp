#include "AttackComponent.h"
#include "StateComponent.h"
#include <memory>
#include "Scene.h"

#include "ResourceManager.h"

#include "ImageObjectComponent.h"
#include "ImageRenderComponent.h"
#include "CollisionComponent.h"
#include "Texture2D.h"
#include "Peppercomponent.h"

dae::AttackComponent::AttackComponent(GameObject* pOwner):
	Component(pOwner)
{
	m_pStateComponent = pOwner->GetComponent<StateComponent>().get();
	m_pCurrentScene = pOwner->GetScene();
}

void dae::AttackComponent::Attack()
{
	if (m_attacksLeft > 0)
	{
		m_pStateComponent->SetCurrentState(State::Attacking);
		SpawnPepper();
		m_pPepperAttackUsedEvent->NotifyObservers({ "Pepper Used" });
		--m_attacksLeft;
	}
}

void dae::AttackComponent::SpawnPepper()
{
	auto pPepper = std::make_shared<GameObject>();
	pPepper->Initialize();
	pPepper->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition());

	auto& resourceManager = ResourceManager::GetInstance();
	auto texture = resourceManager.LoadTexture("Pepper.png");
	pPepper->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
	pPepper->AddComponent<dae::ImageRenderComponent>();
	auto pepperComponent = pPepper->AddComponent<dae::PepperComponent>();
	
	
	m_pCurrentScene->Add(pPepper);


	//Colision component set up
	auto collisionComp = pPepper->AddComponent<CollisionComponent>();
	auto Texture = pPepper->GetComponent<dae::ImageObjectComponent>()->GetTexture().get();
	auto textureSize = Texture->GetSize();
	//dae::Box boxCollision{ static_cast<float>(textureSize.x), static_cast<float>(textureSize.y) };
	collisionComp->SetTag("Pepper");
	collisionComp->SetMeasurements(static_cast<float>(textureSize.x), static_cast<float>(textureSize.y));
	collisionComp->SetDebugColor({ 0,255,255 });

	//Bind the method with what variable will call it and an X amount of parameters.
	auto bindResult = std::bind(&dae::PepperComponent::OnEnemyHit, pepperComponent, std::placeholders::_1, std::placeholders::_2);
	collisionComp->SetOnTriggerEvent(bindResult);

	//std::cout << "Pepper made \n";
}
