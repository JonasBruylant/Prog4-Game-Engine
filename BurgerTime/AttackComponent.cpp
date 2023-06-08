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

dae::AttackComponent::AttackComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner)
{
	auto owner = pOwner.lock().get();
	m_pStateComponent = owner->GetComponent<StateComponent>().get();
	m_pCurrentScene = owner->GetScene();
}

void dae::AttackComponent::Attack()
{
	m_pStateComponent->SetCurrentState(State::Attacking);
	SpawnPepper();
}

void dae::AttackComponent::SpawnPepper()
{
	auto pPepper = std::make_shared<GameObject>();
	pPepper->Initialize();
	pPepper->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner().lock()->GetComponent<TransformComponent>()->GetWorldPosition());

	auto& resourceManager = ResourceManager::GetInstance();
	auto texture = resourceManager.LoadTexture("Pepper.png");
	pPepper->AddComponent<dae::ImageObjectComponent>()->SetTexture(texture);
	pPepper->AddComponent<dae::ImageRenderComponent>();
	auto pepperComponent = pPepper->AddComponent<dae::PepperComponent>();
	
	//Will stay indefinitely
	m_pCurrentScene->Add(pPepper);


	//Colision component set up
	auto collisionComp = pPepper->AddComponent<CollisionComponent>();
	auto worldPosition = pPepper->GetComponent<dae::TransformComponent>()->GetWorldPosition();
	auto Texture = pPepper->AddComponent<dae::ImageObjectComponent>()->GetTexture().get();
	auto textureSize = Texture->GetSize();
	dae::Box boxCollision{ static_cast<float>(textureSize.x), static_cast<float>(textureSize.y) };
	collisionComp->SetTag("Pepper");
	collisionComp->SetMeasurements(boxCollision);
	collisionComp->SetDebugColor({ 0,255,255 });

	//Bind the method with what variable will call it and an X amount of parameters.
	auto bindResult = std::bind(&dae::PepperComponent::OnEnemyHit, pepperComponent, std::placeholders::_1, std::placeholders::_2);
	collisionComp->SetOnTriggerEvent(bindResult);

	//std::cout << "Pepper made \n";
}
