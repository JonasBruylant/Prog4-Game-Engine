#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime)
{
	deltaTime;
}

void dae::GameObject::Render() const
{
}


void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

template <typename T> T* dae::GameObject::AddComponent()
{
	m_ComponentList.push_back();
	return m_ComponentList.end();
}
template <typename T> T* dae::GameObject::GetComponent()
{
	//Code for getting component
}
template <typename T> void dae::GameObject::RemoveComponent()
{
	//m_ComponentList.
}
