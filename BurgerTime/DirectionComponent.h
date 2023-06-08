#pragma once
#include "Component.h"
#include "Structs.h"
#include "ResourceManager.h"
#include "Timer.h"

namespace dae {

	class TransformComponent;
	class ImageObjectComponent;
	class StateComponent;

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
		ImageObjectComponent* m_pImageObjectComponent = nullptr;
		TransformComponent* m_pTransformComponent = nullptr;
		StateComponent* m_pStateComponent = nullptr;
	
		std::weak_ptr<GameObject> m_pOwner;
		std::vector<std::shared_ptr<Texture2D>> m_DirectionTextures;

		int m_movementSpeed = 10;
		Timer& m_Timer = Timer::GetInstance();
	};

}
