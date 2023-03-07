#include "ImageRenderComponent.h"
#include "Transform.h"
#include "ImageObjectComponent.h"

dae::ImageRenderComponent::ImageRenderComponent(std::weak_ptr<GameObject> pOwner):
	Component(pOwner)
{

	m_pTransformPosition = GetOwner().lock()->GetComponent<TransformComponent>();
	m_pImageTexture = GetOwner().lock()->GetComponent<ImageObjectComponent>();
}

dae::ImageRenderComponent::~ImageRenderComponent()
{
}

void dae::ImageRenderComponent::Render() const
{

	auto transformPosition = m_pTransformPosition.lock()->GetWorldPosition();
	auto texture = m_pImageTexture.lock()->GetTexture();
	Renderer::GetInstance().RenderTexture(*texture, transformPosition.x, transformPosition.y);
}
