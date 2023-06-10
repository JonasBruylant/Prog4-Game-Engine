#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"
#include "Transform.h"


dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	if (m_isMarkedForDelete)
		return;

	for (auto& component : m_pComponentList)
	{
		component.get()->Update();
	}
	for (auto& child : m_pChildrenList)
	{
		child->Update();
	}
}

void dae::GameObject::Render() const
{
	if (m_isMarkedForDelete)
		return;

	for (auto& component : m_pComponentList)
	{
		component.get()->Render();
		component.get()->RenderImGUI();
	}

	for (auto& child : m_pChildrenList)
	{
		child->Render();
	}
}

void dae::GameObject::Initialize()
{
	m_pPosition = AddComponent<TransformComponent>().get();
}

void dae::GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
{
	if (m_isMarkedForDelete)
		return;

	if (!m_pParent)
		m_pPosition->SetLocalPosition(m_pPosition->GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			m_pPosition->SetLocalPosition(m_pPosition->GetLocalPosition() - pParent->m_pPosition->GetWorldPosition());
		m_pPosition->SetPositionDirty();
	}
	if (m_pParent)
		m_pParent->RemoveChild(shared_from_this());
	m_pParent = pParent;
	if (m_pParent)
		m_pParent->AddChild(shared_from_this());

	
}

void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> pChild)
{

	m_pChildrenList.erase(std::remove(m_pChildrenList.begin(), m_pChildrenList.end(), pChild));
}

void dae::GameObject::AddChild(std::shared_ptr<GameObject> pChild)
{
	pChild->SetScene(m_pCurrentScene);
	m_pChildrenList.emplace_back(pChild);
}