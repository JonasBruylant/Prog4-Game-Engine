#pragma once
#include "Component.h"

namespace dae {
	class GameObject;
	class StateComponent;
	class CollisionComponent;
	class ImageObjectComponent;
	class HealthComponent;
	class TransformComponent;

	class PlayerCollideCheckComponent final : public Component
	{
	public:
		PlayerCollideCheckComponent(GameObject* pOwner);
		~PlayerCollideCheckComponent() = default;

		PlayerCollideCheckComponent(const PlayerCollideCheckComponent& other) = delete;
		PlayerCollideCheckComponent(PlayerCollideCheckComponent&& other) = delete;
		PlayerCollideCheckComponent& operator=(const PlayerCollideCheckComponent& other) = delete;
		PlayerCollideCheckComponent& operator=(PlayerCollideCheckComponent&& other) = delete;

		void OnGameObjectCollision(CollisionComponent* otherColComp, GameObject* otherCollision);

		void SetParentImageObjectComponent(ImageObjectComponent* pComponent) { m_pImageObjComponent = pComponent; }
private:
	StateComponent* m_pStateComponent{ nullptr };
	CollisionComponent* m_pCollisionComponent{ nullptr };
	ImageObjectComponent* m_pImageObjComponent{ nullptr };
	HealthComponent* m_pHealthComponent{ nullptr };
	TransformComponent* m_pTransformComponent{ nullptr };
	TransformComponent* m_pParentTransformComponent{ nullptr };

	GameObject* m_pOwner{ nullptr };
};

}
