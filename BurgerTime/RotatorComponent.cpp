#include "RotatorComponent.h"
#include "Transform.h"


dae::RotatorComponent::RotatorComponent(std::weak_ptr<GameObject> pOwner) : Component(pOwner)
{
	m_pTransformComponent = GetOwner().lock()->GetTransform();
}

void dae::RotatorComponent::Update()
{
	m_UnitCircleValue += m_Timer.GetDeltaTime() * m_MovementSpeed;
	auto parent = GetOwner().lock()->GetParent();
	glm::vec3 rotationPosition = {cos(m_UnitCircleValue) * m_CircleSize, sin(m_UnitCircleValue) * m_CircleSize, 0};
	if (!parent.expired()) //If parent
		m_pTransformComponent->SetLocalPosition(rotationPosition);
	else //If no parent
		m_pTransformComponent->SetLocalPosition(rotationPosition + m_pPivotPoint);

}

void dae::RotatorComponent::SetPivotPoint(glm::vec3 pivot)
{
	SetPivotPoint(pivot.x, pivot.y, pivot.z);
}

void dae::RotatorComponent::SetPivotPoint(float x, float y, float z)
{

	m_pPivotPoint.x = x;
	m_pPivotPoint.y = y;
	m_pPivotPoint.z = z;
}

void dae::RotatorComponent::SetMovementSpeed(int speed)
{
	m_MovementSpeed = speed;
}
