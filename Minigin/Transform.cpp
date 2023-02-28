#include "Transform.h"



dae::TransformComponent::TransformComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner)
{
}

void dae::TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

}
