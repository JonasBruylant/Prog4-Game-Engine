#pragma once
#include "Component.h"
#include <glm/glm.hpp>
namespace dae {

	class CollisionComponent;
	class GameObject;
	class BlockingVolumeComponent final : public Component
	{
	public:
		BlockingVolumeComponent(GameObject* pOwner);
	
		~BlockingVolumeComponent() = default;
		BlockingVolumeComponent(const BlockingVolumeComponent& other) = delete;
		BlockingVolumeComponent(BlockingVolumeComponent&& other) = delete;
		BlockingVolumeComponent& operator=(const BlockingVolumeComponent& other) = delete;
		BlockingVolumeComponent& operator=(BlockingVolumeComponent&& other) = delete;
	
		void OnGameObjectHit(CollisionComponent* otherColComp, GameObject* otherCollision);
		void SetWindowSize(glm::vec2 size) { m_windowSize = size; }
	private:
		glm::vec2 m_windowSize{ 0.f,0.f };

	};

}
