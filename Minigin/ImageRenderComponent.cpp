#include "ImageRenderComponent.h"
#include "ImageObjectComponent.h"
#include "Transform.h"
#include "Texture2D.h"

dae::ImageRenderComponent::ImageRenderComponent(GameObject* pOwner):
	Component(pOwner)
{

	m_pTransformPosition = GetOwner()->GetComponent<TransformComponent>().get();
	m_pImageTexture = GetOwner()->GetComponent<ImageObjectComponent>().get();
	m_pTexture = m_pImageTexture->GetTexture();
}

dae::ImageRenderComponent::~ImageRenderComponent()
{
}

void dae::ImageRenderComponent::Render() const
{

	auto& transformPosition = m_pTransformPosition->GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, transformPosition.x, transformPosition.y);
}
