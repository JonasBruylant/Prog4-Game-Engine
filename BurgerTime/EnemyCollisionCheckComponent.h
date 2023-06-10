#pragma once
#include "Component.h"

namespace dae
{
	class CollisionComponent;
class EnemyCollisionCheckComponent final : public Component
{
public:
	EnemyCollisionCheckComponent(GameObject* pOwner);

	~EnemyCollisionCheckComponent() = default;
	EnemyCollisionCheckComponent(const EnemyCollisionCheckComponent& other) = delete;
	EnemyCollisionCheckComponent(EnemyCollisionCheckComponent&& other) = delete;
	EnemyCollisionCheckComponent& operator=(const EnemyCollisionCheckComponent& other) = delete;
	EnemyCollisionCheckComponent& operator=(EnemyCollisionCheckComponent&& other) = delete;

	void OnGameObjectHit(CollisionComponent* otherColComp, GameObject* otherGOCol);
private:
};
}

