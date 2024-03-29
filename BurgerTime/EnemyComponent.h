#pragma once
#include "Component.h"
#include "Timer.h"
#include <glm/glm.hpp>
#include "Structs.h"

#include "ResourceManager.h"

namespace dae {

	class TransformComponent;
	class StateComponent;
	class ImageObjectComponent;

class EnemyComponent final : public Component
{
public:
	EnemyComponent(GameObject* pOwner);

	~EnemyComponent() = default;
	EnemyComponent(const EnemyComponent& other) = delete;
	EnemyComponent(EnemyComponent&& other) = delete;
	EnemyComponent& operator=(const EnemyComponent& other) = delete;
	EnemyComponent& operator=(EnemyComponent&& other) = delete;

	void Update() override;
	void UpdateMovement();
	void UpdateStunnedBehavior();

	void SetMovementSpeed(float movementSpeed) { m_movementSpeed = movementSpeed; }
	float GetMovementSpeed() const { return m_movementSpeed; }

	void SetEnemyType(EnemyType type);
	EnemyType GetEnemyType() const { return m_Type;}

private:
	TransformComponent* m_pTransformComponent = nullptr;
	StateComponent* m_pStateComponent = nullptr;
	ImageObjectComponent* m_pImageObjectComponent = nullptr;

	std::vector<std::shared_ptr<Texture2D>> m_StateTextures;
	float m_movementSpeed{ 50.f };
	float m_TimeSinceLastStateChange{ 0.f };
	float m_TimeToBeStunned{ 3.f };
	float m_StunnedTime{};

	glm::vec2 m_direction{ 0.f,0.f };
	Timer& m_Timer = Timer::GetInstance();

	bool m_ChangedDebugColor{ false };
	bool m_ClimbLadder{ false };
	EnemyType m_Type{ EnemyType::Egg };

	ResourceManager& m_ResourceManager = ResourceManager::GetInstance();
	//std::shared_ptr<Texture2D> m_EnemyTexture;

};

}
