#include "RenderComponent.h"

dae::RenderComponent::RenderComponent() :
	m_Renderer{ Renderer::GetInstance() }, 
	m_pOwner{}
{
}

dae::RenderComponent::RenderComponent(std::weak_ptr<GameObject> pOwner) : 
	m_Renderer{ Renderer::GetInstance() }, 
	m_pOwner{pOwner}
{

}

dae::RenderComponent::~RenderComponent()
{

}

void dae::RenderComponent::Render() const
{
	m_Renderer.Render();
}
