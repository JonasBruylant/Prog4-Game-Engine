#include "RenderComponent.h"
#include "Transform.h"
#include "TextObject.h"

dae::RenderComponent::RenderComponent(std::weak_ptr<GameObject> pOwner) : 
	Component(pOwner)
{

}

dae::RenderComponent::~RenderComponent()
{

}

void dae::RenderComponent::Render() const
{
	auto transformPosition = m_pComponentOwner.lock()->GetComponent<TransformComponent>()->GetPosition();
	auto texture = m_pComponentOwner.lock()->GetComponent<TextObjectComponent>()->GetTexture();
	Renderer::GetInstance().RenderTexture(*texture, transformPosition.x, transformPosition.y);
}
