#pragma once
#include "glm/glm.hpp"
#include "Component.h"
#include "Timer.h"
namespace dae 
{
	class TransformComponent;
	class GameObject;

	class RotatorComponent final : public Component
	{
	public:
		RotatorComponent(std::weak_ptr<GameObject> pOwner);
	
		virtual ~RotatorComponent() = default;
		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;
	
		void Update() override;
		void SetPivotPoint(glm::vec3 pivot);
		void SetPivotPoint(float x, float y, float z);
		void SetMovementSpeed(int speed);

	protected:
	
	private:
		std::shared_ptr<TransformComponent> m_pTransformComponent;
		glm::vec3 m_pPivotPoint{0,0,0};
		float m_UnitCircleValue{0.f};
		float m_CircleSize{30.f};
		int m_MovementSpeed{5};

		Timer& m_Timer = Timer::GetInstance();
	};
	
}
