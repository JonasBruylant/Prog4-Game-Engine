#include "TextRenderComponent.h"
#include "Transform.h"
#include "TextObjectComponent.h"

dae::TextRenderComponent::TextRenderComponent(std::weak_ptr<GameObject> pOwner) :
	Component(pOwner)
{

}

dae::TextRenderComponent::~TextRenderComponent()
{

}


void dae::TextRenderComponent::Render() const
{
	auto transformPosition = GetOwner().lock()->GetComponent<TransformComponent>()->GetLocalPosition();
	auto texture = GetOwner().lock()->GetComponent<TextObjectComponent>()->GetTexture();
	Renderer::GetInstance().RenderTexture(*texture, transformPosition.x, transformPosition.y);
}
