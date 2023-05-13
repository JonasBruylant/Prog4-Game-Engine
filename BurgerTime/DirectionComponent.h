#pragma once
#include "Component.h"
#include "Structs.h"
#include "ImageObjectComponent.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Timer.h"

namespace dae {

	class DirectionComponent : public Component
	{
	public:
		DirectionComponent(std::weak_ptr<GameObject> pOwner);
	
		~DirectionComponent() = default;
		DirectionComponent(const DirectionComponent& other) = delete;
		DirectionComponent(DirectionComponent&& other) = delete;
		DirectionComponent& operator=(const DirectionComponent& other) = delete;
		DirectionComponent& operator=(DirectionComponent&& other) = delete;
	
		void Update() override;

		void SetCurrentDirection(PlayerDirection direction) {m_CurrentDirection = direction;}
		PlayerDirection GetCurrentDirection() { return m_CurrentDirection; }
	
		void SetDirectionImage();
		void SetMovementSpeed(int speed) { m_movementSpeed = speed; }
	
	private:
		PlayerDirection m_CurrentDirection = PlayerDirection::None;
		ImageObjectComponent* m_ImageObjectComponent = nullptr;
		TransformComponent* m_TransformComponent = nullptr;
	
		std::weak_ptr<GameObject> m_pOwner;
		std::vector<std::shared_ptr<Texture2D>> m_DirectionTextures;

		int m_movementSpeed = 10;
		Timer& m_Timer = Timer::GetInstance();
	};

}
