#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"
#include "Transform.h"


dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime)
{
	for (auto& component : m_pComponentList)
	{
		component.get()->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (auto& component : m_pComponentList)
	{
		component.get()->Render();
	}
}

void dae::GameObject::Initialize()
{
	m_pPosition = AddComponent<TransformComponent>();
}

void dae::GameObject::SetParent(std::weak_ptr<GameObject> pParent, bool keepWorldPosition)
{
	if (m_pParent.expired())
		m_pPosition.get()->SetLocalPosition(m_pPosition.get()->GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			m_pPosition.get()->SetLocalPosition(m_pPosition.get()->GetLocalPosition() - pParent.lock()->m_pPosition.get()->GetWorldPosition());
		m_pPosition.get()->SetPositionDirty();
	}
	if (!m_pParent.expired())
		m_pParent.lock()->RemoveChild(shared_from_this());
	m_pParent = pParent;
	if (!m_pParent.expired())
		m_pParent.lock()->AddChild(shared_from_this());
}

void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> pChild)
{
	m_pChildrenList.erase(std::remove(m_pChildrenList.begin(), m_pChildrenList.end(), pChild));
}

void dae::GameObject::AddChild(std::shared_ptr<GameObject> pChild)
{
	m_pChildrenList.emplace_back(pChild);
}