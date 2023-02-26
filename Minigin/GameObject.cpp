#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime)
{
	for (auto& component : m_pComponentList)
	{
		component.lock()->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (auto& component : m_pComponentList)
	{
		component.lock()->Render();
	}
}


template <typename T> T* dae::GameObject::AddComponent()
{
	m_pComponentList.push_back();
	return m_pComponentList.end();
}
template <typename T> T* dae::GameObject::GetComponent()
{
	//Code for getting component
}
template <typename T> void dae::GameObject::RemoveComponent()
{
	//m_ComponentList.
}
