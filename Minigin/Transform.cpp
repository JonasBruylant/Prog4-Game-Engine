#include "Transform.h"



dae::TransformComponent::TransformComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner)
{
}

const glm::vec3 dae::TransformComponent::GetWorldPosition() const
{
	if (!m_pParent.expired())
	{
		return m_pParent.lock()->GetComponent<TransformComponent>()->GetWorldPosition() + GetLocalPosition();
	}
	return GetLocalPosition();
}

void dae::TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

}
