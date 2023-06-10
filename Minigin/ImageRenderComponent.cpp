#include "ImageRenderComponent.h"
#include "ImageObjectComponent.h"
#include "Transform.h"
#include "Texture2D.h"

dae::ImageRenderComponent::ImageRenderComponent(GameObject* pOwner):
	Component(pOwner)
{

	m_pTransformPosition = GetOwner()->GetComponent<TransformComponent>().get();
	m_pImageTexture = GetOwner()->GetComponent<ImageObjectComponent>().get();
}

void dae::ImageRenderComponent::Render() const
{

	if (m_renderUsingRects)
	{
		Renderer::GetInstance().RenderTexture(*m_pImageTexture->GetTexture(), m_srcRect, m_destRect);
		return;
	}

	auto& transformPosition = m_pTransformPosition->GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_pImageTexture->GetTexture(), transformPosition.x, transformPosition.y);
}

void dae::ImageRenderComponent::Update()
{

	if (m_renderUsingRects)
	{
		auto& transformPosition = m_pTransformPosition->GetWorldPosition();
		m_destRect.x = static_cast<int>(transformPosition.x);
		m_destRect.y = static_cast<int>(transformPosition.y);
	}
}
