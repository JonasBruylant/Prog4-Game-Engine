#include "TextRenderComponent.h"
#include "Transform.h"
#include "TextObjectComponent.h"

dae::TextRenderComponent::TextRenderComponent(std::weak_ptr<GameObject> pOwner) :
	Component(pOwner)
{
	m_transformPosition = GetOwner().lock()->GetComponent<TransformComponent>();
	m_textTexture = GetOwner().lock()->GetComponent<TextObjectComponent>();
}

dae::TextRenderComponent::~TextRenderComponent()
{

}


void dae::TextRenderComponent::Render() const
{
	auto transformPosition = m_transformPosition.lock()->GetWorldPosition();
	auto texture = m_textTexture.lock()->GetTexture();
	Renderer::GetInstance().RenderTexture(*texture, transformPosition.x, transformPosition.y);
}
