#include "TextRenderComponent.h"
#include "Transform.h"
#include "TextObjectComponent.h"

dae::TextRenderComponent::TextRenderComponent(GameObject* pOwner) :
	Component(pOwner)
{
	m_transformPosition = GetOwner()->GetComponent<TransformComponent>().get();
	m_textTexture = GetOwner()->GetComponent<TextObjectComponent>().get();
}

dae::TextRenderComponent::~TextRenderComponent()
{

}


void dae::TextRenderComponent::Render() const
{
	auto transformPosition = m_transformPosition->GetWorldPosition();
	auto texture = m_textTexture->GetTexture();
	if (!texture)
		return;
	Renderer::GetInstance().RenderTexture(*texture, transformPosition.x, transformPosition.y);
}
