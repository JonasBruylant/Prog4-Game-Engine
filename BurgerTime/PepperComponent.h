#pragma once
#include "Component.h"
#include "Timer.h"

namespace dae {

	class CollisionComponent;
	class GameObject;
class PepperComponent final : public Component
{
public:
	PepperComponent(GameObject* pOwner);

	~PepperComponent() = default;
	PepperComponent(const PepperComponent& other) = delete;
	PepperComponent(PepperComponent&& other) = delete;
	PepperComponent& operator=(const PepperComponent& other) = delete;
	PepperComponent& operator=(PepperComponent&& other) = delete;

	void OnEnemyHit(dae::CollisionComponent* otherCollision, GameObject* otherObject);

	void Update() override;
private:
	Timer& m_Timer = Timer::GetInstance();

	float m_TimeAlive{  };
	float m_MaxTimeAlive{ 3.f };
};


}