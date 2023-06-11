#pragma once
#include "Component.h"
#include "ResourceManager.h"
#include "Timer.h"

namespace dae {

	class TransformComponent;
	class ImageObjectComponent;
	class StateComponent;
	enum class PlayerDirection;
	enum class GameState;
	class DirectionComponent : public Component
	{
	public:
		DirectionComponent(GameObject* pOwner, bool isPlayerTwo, GameState gameState);
	
		~DirectionComponent() = default;
		DirectionComponent(const DirectionComponent& other) = delete;
		DirectionComponent(DirectionComponent&& other) = delete;
		DirectionComponent& operator=(const DirectionComponent& other) = delete;
		DirectionComponent& operator=(DirectionComponent&& other) = delete;
	
		void Update() override;

		void SetCurrentDirection(PlayerDirection direction);
		PlayerDirection GetCurrentDirection();
	
		void SetDirectionImage();
		void SetMovementSpeed(int speed) { m_movementSpeed = speed; }
		void SetDirectionStateComponent(StateComponent* pComponent) { m_pStateComponent = pComponent; }
	private:
		PlayerDirection m_CurrentDirection;
		ImageObjectComponent* m_pImageObjectComponent = nullptr;
		TransformComponent* m_pTransformComponent = nullptr;
		StateComponent* m_pStateComponent = nullptr;
	
		GameObject* m_pOwner;
		std::vector<std::shared_ptr<Texture2D>> m_DirectionTextures;

		int m_movementSpeed = 10;
		Timer& m_Timer = Timer::GetInstance();
		float m_timeStunned{};
		bool m_isPlayerTwo{ false };
	};

}
