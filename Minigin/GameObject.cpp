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
