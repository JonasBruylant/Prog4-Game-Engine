#include "Transform.h"



dae::TransformComponent::TransformComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner), m_positionIsDirty{ false }
{
}

const glm::vec3& dae::TransformComponent::GetLocalPosition() const
{
	return m_LocalPosition;
}

const glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	if (m_positionIsDirty)
		UpdateWorldPosition();

	return m_WorldPosition;
}

void dae::TransformComponent::SetLocalPosition(const float x, const float y, const float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;

	SetPositionDirty();
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& position)
{
	SetLocalPosition(position.x, position.y, position.z);
}


void dae::TransformComponent::SetPositionDirty()
{
	m_positionIsDirty = true;

	for (auto& child : GetOwner().lock()->GetChildren())
	{
		child->GetTransform()->SetPositionDirty();
	}
}

void dae::TransformComponent::UpdateWorldPosition()
{
	auto componentOwner = GetOwner().lock();
	if (componentOwner->GetParent().expired())
		m_WorldPosition = m_LocalPosition;
	else
	{
		const auto parentTransform = componentOwner->GetParent().lock()->GetTransform();

		m_WorldPosition = parentTransform->GetWorldPosition() + m_LocalPosition;
	}
	m_positionIsDirty = false;
}

void dae::TransformComponent::AddToLocalPosition(const glm::vec3& position)
{
	AddToLocalPosition(position.x, position.y, position.z);
}

void dae::TransformComponent::AddToLocalPosition(const float x, const float y, const float z)
{
	m_LocalPosition.x += x;
	m_LocalPosition.y += y;
	m_LocalPosition.z += z;

	SetPositionDirty();
}
