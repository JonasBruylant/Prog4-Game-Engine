#pragma once
#include "Component.h"

namespace dae {
	class CollisionComponent;
	class GameObject;

class PlayerCollideCheckComponent final : public Component
{
public:
	PlayerCollideCheckComponent(std::weak_ptr<GameObject> pOwner);
	~PlayerCollideCheckComponent() = default;

	PlayerCollideCheckComponent(const PlayerCollideCheckComponent& other) = delete;
	PlayerCollideCheckComponent(PlayerCollideCheckComponent&& other) = delete;
	PlayerCollideCheckComponent& operator=(const PlayerCollideCheckComponent& other) = delete;
	PlayerCollideCheckComponent& operator=(PlayerCollideCheckComponent&& other) = delete;

	void OnGameObjectCollision(CollisionComponent* otherColComp, GameObject* otherCollision);

private:

};

}
